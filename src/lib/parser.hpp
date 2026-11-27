/**
 * \file parser.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implementation of the Parser class
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef PARSER
#define PARSER

#include <cstdint>
#include <list>
#include <string>

#include "../enums/error-type.hpp"
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
         * \brief Temporary function, maybe?, to facilitate the conversion of constants from octal or hex into decimal
         *
         * @param constant The constant as found
         * @param negative Whether it is negative
         * @param value The decimal value after conversion
         *
         * @return Whether the conversion was successful
         */
        static bool convert_integer_literal(const std::string& constant, const bool negative, std::string& value) {
            size_t offset = 0;

            // Capture the original base
            int base = 10;
            if (constant.length() > 1 && constant[0] == '0' &&
                (constant[1] == 'x' || constant[1] == 'X')) {
                base = 16;
                offset = 2;
            } else if (constant.length() > 1 && constant[0] == '0') {
                base = 8;
            }

            const std::uint64_t maximum = negative ? 2147483648ULL : 2147483647ULL;
            std::uint64_t magnitude = 0;
            for (size_t index = offset; index < constant.length(); ++index) {
                const char digit = constant[index];
                int digit_value = -1;
                if (digit >= '0' && digit <= '9') {
                    digit_value = digit - '0';
                } else if (digit >= 'a' && digit <= 'f') {
                    digit_value = digit - 'a' + 10;
                } else if (digit >= 'A' && digit <= 'F') {
                    digit_value = digit - 'A' + 10;
                }

                if (digit_value < 0 || digit_value >= base ||
                    magnitude > (maximum - static_cast<std::uint64_t>(digit_value)) /
                        static_cast<std::uint64_t>(base)) {
                    return false;
                }
                magnitude = magnitude * static_cast<std::uint64_t>(base) +
                    static_cast<std::uint64_t>(digit_value);
            }

            if (negative) {
                value = magnitude == 2147483648ULL
                    ? "-2147483648"
                    : std::to_string(-static_cast<std::int64_t>(magnitude));
            } else {
                value = std::to_string(magnitude);
            }
            return true;
        }

        /**
         * \brief Helper method used to set the position of a Byte
         *
         * \param byte The Byte to set the location for
         */
        void set_position_of(Byte *byte) const {
            byte->set_byte_position(this->tokens->front().get_line(), this->tokens->front().get_position());
        }

        /**
         * \brief Attempts to consume a Token of the expected TokenType
         *
         * \param expected The expected TokenType
         * \param reason A potential reason to pass through to error Bytes
         */
        void consume_token(const TokenType expected, const ErrorType reason = ErrorType::ET_NONE) {
            if (tokens->front().get_type() != expected) {
                // error
                this->bytes.emplace_back(OpCode::OP_ERROR, error_type_string(reason));
                auto temp = Byte(OpCode::OP_ERROR, "", reason);
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
         * Integer spellings are defined in grammar/grammar.bnf. The parser
         * converts decimal, octal, and hexadecimal values within the supported
         * signed 32-bit range.
         *
         * \param negative If the expected constant is negative
         */
        void parse_constant(const bool negative = false) {
            Token& token = this->tokens->front();
            std::string value;
            if (!convert_integer_literal(token.get_value(), negative, value)) {
                constexpr ErrorType reason = ErrorType::ET_UNSUPPORTED_NUMERIC_LITERAL;
                Token error(TokenType::TK_ERROR, token.get_value(), reason, token.get_line(), token.get_position(), token.get_length());
                this->errors.emplace_back(error);
                this->found_error = true;

                Byte error_byte(OpCode::OP_ERROR, error_type_string(reason), reason);
                error_byte.set_byte_position(token.get_line(), token.get_position());
                add_byte(error_byte);
            } else {
                if (negative && value != "-2147483648" && value[0] != '-') {
                    value.insert(value.begin(), '-');
                }
                add_byte(Byte(OpCode::OP_CONSTANT, value));
            }
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
                case TokenType::TK_BANG: {
                    consume_token(TokenType::TK_BANG);
                    parse_factor();
                    add_byte(Byte(OpCode::OP_NOT));
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
                    consume_token(TokenType::TK_CLOSE_PARENTHESIS, ErrorType::ET_EXPECTED_CLOSE_PARENTHESIS);
                    break;
                }
                // integer
                case TokenType::TK_CONSTANT: {
                    parse_constant();
                    break;
                }
                default: {
                    // error, expected a factor
                    consume_token(TokenType::TK_CONSTANT, ErrorType::ET_EXPECTED_EXPRESSION);
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
                case TokenType::TK_LEFT_CHEVRONS:
                case TokenType::TK_RIGHT_CHEVRONS:
                    return 40;
                case TokenType::TK_LESS:
                case TokenType::TK_LESS_EQUAL:
                case TokenType::TK_GREATER:
                case TokenType::TK_GREATER_EQUAL:
                    return 39;
                case TokenType::TK_EQUAL_EQUAL:
                case TokenType::TK_BANG_EQUAL:
                    return 38;
                case TokenType::TK_AMPERSAND:
                    return 35;
                case TokenType::TK_CARET:
                    return 30;
                case TokenType::TK_PIPE:
                    return 25;
                case TokenType::TK_AMPE_AMPE:
                    return 20;
                case TokenType::TK_PIPE_PIPE:
                    return 15;
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
                case TokenType::TK_PLUS:           return OpCode::OP_ADD;
                case TokenType::TK_MINUS:          return OpCode::OP_SUBTRACT;
                case TokenType::TK_STAR:           return OpCode::OP_MULTIPLY;
                case TokenType::TK_SLASH:          return OpCode::OP_DIVIDE;
                case TokenType::TK_PERCENTAGE:     return OpCode::OP_MODULO;
                case TokenType::TK_AMPERSAND:      return OpCode::OP_BW_AND;
                case TokenType::TK_PIPE:           return OpCode::OP_BW_OR;
                case TokenType::TK_CARET:          return OpCode::OP_BW_XOR;
                case TokenType::TK_LEFT_CHEVRONS:  return OpCode::OP_LEFT_SHIFT;
                case TokenType::TK_RIGHT_CHEVRONS: return OpCode::OP_RIGHT_SHIFT;
                case TokenType::TK_AMPE_AMPE:      return OpCode::OP_LOGIC_AND;
                case TokenType::TK_PIPE_PIPE:      return OpCode::OP_LOGIC_OR;
                case TokenType::TK_EQUAL_EQUAL:    return OpCode::OP_EQUAL;
                case TokenType::TK_BANG_EQUAL:     return OpCode::OP_NOT_EQUAL;
                case TokenType::TK_LESS:           return OpCode::OP_LESS_THAN;
                case TokenType::TK_GREATER:        return OpCode::OP_GREATER_THAN;
                case TokenType::TK_LESS_EQUAL:     return OpCode::OP_LESS_THAN_EQUAL;
                case TokenType::TK_GREATER_EQUAL:  return OpCode::OP_GREATER_THAN_EQUAL;
                default:                           return OpCode::OP_ERROR;
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
                this->bytes.emplace_back(OpCode::OP_ERROR,"",ErrorType::ET_EXPECTED_EXPRESSION);
                this->found_error = true;
                return;
            }

            // Left operand
            parse_factor();

            // { binary_op factor }
            while (precedence_of(this->tokens->front().get_type()) >= min_precedence) {
                const TokenType op = this->tokens->front().get_type();
                const int precedence = precedence_of(op);

                // Consume the next token
                consume_token(op);

                // Insert label bytes for conditional logic
                if (op == TokenType::TK_AMPE_AMPE) {
                    add_byte(Byte(OpCode::OP_AND_BEGIN));
                } else if (op == TokenType::TK_PIPE_PIPE) {
                    add_byte(Byte(OpCode::OP_OR_BEGIN));
                }

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
            consume_token(TokenType::TK_KEYWORD_RETURN, ErrorType::ET_EXPECTED_RETURN_KEYWORD);
            parse_expression();
            add_byte(Byte(OpCode::OP_RETURN));
            consume_token(TokenType::TK_SEMI_COLON, ErrorType::ET_EXPECTED_SEMICOLON);
        }

        /**
         * \brief Attempts to Parse a Block
         *
         * Grammar:
         *
         * block ::= "{" return "}"
         */
        void parse_block() {
            consume_token(TokenType::TK_OPEN_BRACE, ErrorType::ET_EXPECTED_OPEN_BRACE);
            parse_return();
            consume_token(TokenType::TK_CLOSE_BRACE, ErrorType::ET_EXPECTED_CLOSE_BRACE);
        }

        /**
         * \brief Attempts to Parse a Function
         *
         * Grammar:
         *
         * function ::= "int" identifier "(" "void"* ")" block
         */
        void parse_function() {
            consume_token(TokenType::TK_KEYWORD_INT, ErrorType::ET_EXPECTED_INT_KEYWORD);
            parse_identifier();
            consume_token(TokenType::TK_OPEN_PARENTHESIS, ErrorType::ET_EXPECTED_OPEN_PARENTHESIS);
            // void keyword should be optional
            if (this->tokens->front().get_type() == TokenType::TK_KEYWORD_VOID) {
                consume_token(TokenType::TK_KEYWORD_VOID);
            }
            consume_token(TokenType::TK_CLOSE_PARENTHESIS, ErrorType::ET_EXPECTED_CLOSE_PARENTHESIS);
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
