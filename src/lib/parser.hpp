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
#include "../types/token.hpp"

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
         * \brief The vector of Tokens this Parser uses to build its Byte vector
         */
        std::list<Token> *tokens;

        /**
         * \brief A vector of Bytes built by this Parser
         */
        std::list<Byte> bytes;

        /**
         * \brief Set to true upon finding an error
         */
        bool found_error = false;

    private:

        /**
         * \brief Attempts to consume a Token of the expected TokenType
         *
         * \param expected The expected TokenType
         * \param message A potential error message to pass through to error Tokens
         */
        void consume_token(TokenType expected, std::string message = "") {
            if (tokens->front().get_type() != expected) {
                // error
                this->bytes.push_back(Byte(OpCode::OP_ERROR, message));
                this->found_error = true;
            } else {
                // consume the token
                this->tokens->pop_front();
            }
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
            this->bytes.push_back(Byte(OpCode::OP_FUNCTION, this->tokens->front().get_value()));
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
        void parse_constant(bool negative = false) {
            std::string value;
            if (negative) {
                value += "-";
            }
            this->bytes.push_back(Byte(OpCode::OP_CONSTANT, value += this->tokens->front().get_value()));
            consume_token(TokenType::TK_CONSTANT);
        }

        /**
         * \brief Attempts to Parse a Primary
         *
         * Grammar:
         *
         * primary ::= integer
         *           | expression
         *           | "(" expression ")"
         *
         * \param negative If the preceeding Token is TK_MINUS, and the constant we find is therefore negative
         */
        void parse_primary(bool negative = false) {
            switch (this->tokens->front().get_type()) {
                // integer
                case TokenType::TK_CONSTANT: {
                    parse_constant(negative);
                    break;
                }
                // expression
                case TokenType::TK_MINUS: {
                    consume_token(TokenType::TK_MINUS);
                    parse_constant(true);
                    break;
                }
                case TokenType::TK_TILDE: {
                    consume_token(TokenType::TK_TILDE);
                    parse_constant();
                    this->bytes.push_back(Byte(OpCode::OP_COMPLEMENT));
                    break;
                }
                // "(" expression ")"
                case TokenType::TK_OPEN_PARENTHESIS: {
                    consume_token(TokenType::TK_OPEN_PARENTHESIS);
                    parse_expression();
                    consume_token(TokenType::TK_CLOSE_PARENTHESIS, "Expected ')'");
                    break;
                }
                default: {
                    // error, missing constant
                    consume_token(TokenType::TK_CONSTANT, "Missing constant");
                }
            }
        }

        /**
         * \brief Attempts to Parse a Unary
         *
         * Grammar:
         *
         * unary ::= unary_op primary
         *         | primary
         */
        void parse_unary() {
            switch (this->tokens->front().get_type()) {
                // unary_op primary
                case TokenType::TK_TILDE: {
                    consume_token(TokenType::TK_TILDE);
                    parse_primary();
                    this->bytes.push_back(Byte(OpCode::OP_COMPLEMENT));
                    break;
                }
                case TokenType::TK_MINUS: {
                    consume_token(TokenType::TK_MINUS);
                    if (this->tokens->front().get_type() == TokenType::TK_CONSTANT) {
                        parse_primary(true);
                    } else {
                        parse_primary();
                        this->bytes.push_back(Byte(OpCode::OP_NEGATE));
                    }
                    break;
                }
                // primary
                default: {
                    parse_primary();
                    break;
                };
            }
        }

        /**
         * \brief Attempts to Parse an Expression
         *
         * Grammar:
         *
         * expression ::= unary
         */
        void parse_expression() {
            // Currently only supports unary, in time we will have other operators
            parse_unary();
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
            this->bytes.push_back(Byte(OpCode::OP_RETURN));
            consume_token(TokenType::TK_SEMI_COLON, "Expected ';'");
        }

        /**
         * \brief Attemps to Parse a Block
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
        Parser() {} // Default

        /**
         * \brief Construct a new Parser object with a list of Tokens
         *
         * \param tokens The list of Tokens this Parser should convert into Bytes
         */
        Parser(std::list<Token> *tokens)
        : tokens{ tokens } {}

        /**
         * \brief Used to check if an error was found.
         *
         * \return True if an error Token was produced, otherwise false.
         */
        bool had_error() {
            return this->found_error;
        }

        /**
         * \brief Parses the list of Tokens and returns a list of found Bytes.
         *
         * \return A list of Bytes produced from the list of Tokens
         */
        std::list<Byte> run() {
            parse_program();

            // If we still have Tokens left over
            if (TokenType::TK_EOF != this->tokens->front().get_type()) {
                this->found_error = true;
            }

            return this->bytes;
        }
};

#endif // PARSER
