/**
 * \file parser.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implementation of the Parser class
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef PARSER
#define PARSER

#include <list>

#include "../types/byte.hpp"
#include "../types/error.hpp"
#include "../types/token.hpp"

#ifdef DEBUG_PARSER
    #include <iostream>
#endif

/**
 * \brief A class outlining the Parser class, which is used to turn Tokens into Bytes.
 *
 * The aim of this class is to take in a list of Tokens;
 * and parse them to produce a list of Bytes.
 *
 * The primary interface will be a single public .run() method, which will
 * hand off to several private internal helper methods that encapsulate
 * the entire functionality of the class.
 *
 * This should keep things simple.
 *
 * Note: As the Grammar supported by the Compiler increases in complexity,
 * this file should see the most significant changes,
 * implementing many changes in control flow as required.
 */
class Parser {

        /**
         * \brief The list of Tokens this Parser uses to build its Byte list
         */
        std::list<Token> *tokens{};

        /**
         * \brief A list of Bytes built by this Parser
         */
        std::list<Byte> bytes;

        /**
         * \brief Set to true upon finding an error
         */
        bool found_error = false;

        /**
         * \brief A list of any Errors found
         */
         std::list<Error> errors;

    private:

        /**
         * \brief Helper method used to set the position of a Byte
         *
         * \param byte The Byte to set the location for
         */
        void set_position_of(Byte *byte) const {
            byte->set_byte_position(this->tokens->front().get_line(), this->tokens->front().get_position() - this->tokens->front().get_length());
        }

        /**
         * \brief Attempts to consume a Token of the expected TokenType
         *
         * \param expected The expected TokenType
         * \param message A potential error message to pass through to error Tokens
         */
        void consume_token(TokenType expected, const std::string& message = "") {
            if (tokens->front().get_type() != expected) {
                // error
                this->bytes.emplace_back(OpCode::OP_ERROR, message);
                auto temp = Byte(OpCode::OP_ERROR, "", message);
                set_position_of(&temp);
                this->errors.emplace_back(temp);
                this->found_error = true;
            } else {
                // consume the token
                this->tokens->pop_front();
            }
        }

        /**
         * \brief Adds a Byte to the list of found Bytes
         *
         * This function also facilitates debug output for the Parser
         *
         * \param byte The Byte
         */
        void add_byte(const Byte& byte) {
#ifdef DEBUG_PARSER
            std::cout << "Found : " << op_code_string.at(byte.get_op()) << std::endl;
#endif
            this->bytes.push_back(byte);
        }

        /**
         * \brief Attempts to Parse an Identifier
         *
         * Grammar:
         *
         * identifier ::= alpha+
         */
        void parse_identifier() {
            // Right now this only supports function names, in time it needs to support variables
            add_byte(Byte(OpCode::OP_FUNCTION, this->tokens->front().get_value()));
            consume_token(TokenType::TK_IDENTIFIER);
        }

        /**
         * \brief Attempts to Parse a Constant (currently only integers)
         *
         * Grammar:
         *
         * integer ::= digit+
         *
         * \param negative If the expected constant is negative
         */
        void parse_constant(const bool negative = false) {
            std::string value;
            if (negative) {
                value += "-";
            }
            add_byte(Byte(OpCode::OP_CONSTANT, value += this->tokens->front().get_value()));
            consume_token(TokenType::TK_CONSTANT);
        }

        /**
         * \brief Attempts to Parse a Factor
         *
         * A factor is the operand of a binary expression: a unary operator
         * applied to another factor, a parenthesised expression, or an integer.
         *
         * Grammar:
         *
         * factor ::= unary_op factor
         *          | "(" expression ")"
         *          | integer
         */
        void parse_factor() {
            switch (this->tokens->front().get_type()) {
                // unary_op factor
                case TokenType::TK_TILDE: {
                    consume_token(TokenType::TK_TILDE);
                    parse_factor();
                    add_byte(Byte(OpCode::OP_COMPLEMENT));
                    break;
                }
                case TokenType::TK_MINUS: {
                    consume_token(TokenType::TK_MINUS);
                    // A '-' immediately followed by an integer is folded into a
                    // single negative constant; otherwise it negates the factor.
                    if (this->tokens->front().get_type() == TokenType::TK_CONSTANT) {
                        parse_constant(true);
                    } else {
                        parse_factor();
                        add_byte(Byte(OpCode::OP_NEGATE));
                    }
                    break;
                }
                // "(" expression ")"
                case TokenType::TK_OPEN_PARENTHESIS: {
                    consume_token(TokenType::TK_OPEN_PARENTHESIS);
                    parse_expression();
                    consume_token(TokenType::TK_CLOSE_PARENTHESIS, "Expected ')'");
                    break;
                }
                // integer
                case TokenType::TK_CONSTANT: {
                    parse_constant();
                    break;
                }
                default: {
                    // error, expected a factor
                    consume_token(TokenType::TK_CONSTANT, "Expected Expression");
                }
            }
        }

        /**
         * \brief Returns the precedence of a binary operator Token
         *
         * A higher value binds more tightly. Any Token that is not a binary_op
         * returns -1, which is below every valid minimum precedence and so ends
         * the precedence-climbing loop in parse_expression.
         *
         * \param type The TokenType to check
         * \return The precedence of the operator, or -1 if it is not a binary_op
         */
        static int precedence_of(const TokenType type) {
            switch (type) {
                case TokenType::TK_STAR:
                case TokenType::TK_SLASH:
                case TokenType::TK_PERCENTAGE:
                    return 50;
                case TokenType::TK_PLUS:
                case TokenType::TK_MINUS:
                    return 45;
                default:
                    return -1;
            }
        }

        /**
         * \brief Helper function to map a binary operator Token to its OpCode
         *
         * \param type The TokenType of the operator
         * \return The matching OpCode
         */
        static OpCode op_code_for(const TokenType type) {
            switch (type) {
                case TokenType::TK_PLUS:       return OpCode::OP_ADD;
                case TokenType::TK_MINUS:      return OpCode::OP_SUBTRACT;
                case TokenType::TK_STAR:       return OpCode::OP_MULTIPLY;
                case TokenType::TK_SLASH:      return OpCode::OP_DIVIDE;
                case TokenType::TK_PERCENTAGE: return OpCode::OP_MODULO;
                default:                       return OpCode::OP_ERROR;
            }
        }

        /**
         * \brief Attempts to Parse an Expression
         *
         * Uses precedence climbing so that binary operators chain with the
         * correct precedence and left-associativity. The operator Byte is
         * emitted last (postfix), after both of its operands, keeping the flat
         * Byte stream consumed downstream valid.
         *
         * Grammar:
         *
         * expression ::= factor { binary_op factor }
         *
         * \param min_precedence The minimum operator precedence this call may consume
         */
        void parse_expression(const int min_precedence = 0) {
            //! Currently maybe a bodge?, but does it enables us to pass tests?
            // I'm asking this because at one point this function was starting a process to pop an empty list
            if (this->tokens->empty()) {
                this->bytes.emplace_back(OpCode::OP_ERROR, "Expected Expression");
                this->found_error = true;
                return;
            }

            // Left operand
            parse_factor();

            // { binary_op factor }
            while (precedence_of(this->tokens->front().get_type()) >= min_precedence) {
                const TokenType op = this->tokens->front().get_type();
                const int precedence = precedence_of(op);

                consume_token(op);
                // Parse the right operand at a higher minimum precedence so that
                // operators of equal precedence are left-associative.
                parse_expression(precedence + 1);
                // Emit the operator Byte last (postfix).
                add_byte(Byte(op_code_for(op)));
            }
        }

        /**
         * \brief Attempts to Parse a Return Statement
         *
         * Grammar:
         *
         * return ::= "return" expression ";"
         */
        void parse_return() {
            consume_token(TokenType::TK_KEYWORD_RETURN, "Expected return keyword");
            parse_expression();
            add_byte(Byte(OpCode::OP_RETURN));
            consume_token(TokenType::TK_SEMI_COLON, "Expected ';'");
        }

        /**
         * \brief Attempts to Parse a Block
         *
         * Grammar:
         *
         * block ::= "{" return "}"
         */
        void parse_block() {
            consume_token(TokenType::TK_OPEN_BRACE, "Expected '{'");
            parse_return();
            consume_token(TokenType::TK_CLOSE_BRACE, "Expected '}'");
        }

        /**
         * \brief Attempts to Parse a Function
         *
         * Grammar:
         *
         * function ::= "int" identifier "(" "void"* ")" block
         */
        void parse_function() {
            consume_token(TokenType::TK_KEYWORD_INT, "Expected int keyword");
            parse_identifier();
            consume_token(TokenType::TK_OPEN_PARENTHESIS, "Expected '('");
            // void keyword should be optional
            if (this->tokens->front().get_type() == TokenType::TK_KEYWORD_VOID) {
                consume_token(TokenType::TK_KEYWORD_VOID);
            }
            consume_token(TokenType::TK_CLOSE_PARENTHESIS, "Expected ')'");
            parse_block();
        }

        /**
         * \brief Attempts to Parse a Program
         *
         * Grammar:
         *
         * program ::= function
         */
        void parse_program() {
            parse_function();
        }

    public:

        /**
         * \brief Default constructor for a Parser
         */
        Parser() = default; // Default

        /**
         * \brief Construct a new Parser object with a list of Tokens
         *
         * \param tokens The list of Tokens this Parser should convert into Bytes
         */
        explicit Parser(std::list<Token> *tokens)
        : tokens{ tokens } {}

        /**
         * \brief Used to check if an error was found.
         *
         * \return True if an error Token was produced, otherwise false.
         */
        bool had_error() const {
            return this->found_error;
        }

        /**
         * \brief Get the list of Errors
         *
         * \return The list of Errors, if any were found
         */
        std::list<Error> get_errors() {
            return this->errors;
        }

        /**
         * \brief Parses the list of Tokens and returns a list of found Bytes.
         *
         * \return A list of Bytes produced from the list of Tokens
         */
        std::list<Byte> run() {

#ifdef DEBUG_PARSER
            std::cout << std::endl;
            std::cout << " === Beginning Parsing === " << std::endl;
            std::cout << std::endl;
#endif

            parse_program();

#ifdef DEBUG_PARSER
            std::cout << std::endl;
            std::cout << " === Finishing Parsing === " << std::endl;
            std::cout << std::endl;
#endif

            // If we still have Tokens left over
            if (TokenType::TK_EOF != this->tokens->front().get_type()) {
                this->found_error = true;
            }

            return this->bytes;
        }
};

#endif // PARSER
