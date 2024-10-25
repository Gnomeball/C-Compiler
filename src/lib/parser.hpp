/**
 * \file parser.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implmentation of the Parser class
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef PARSER
#define PARSER

#include <list>
#include <vector>

#include "../types/byte.hpp"
#include "../types/token.hpp"

/*
 * The aim of this class is to take in a list of Tokens;
 * and parse them to produce a vector of Bytes.
 *
 * The primary interface will be a single public .run() method, which will
 * hand off to several private internal helper methods that encapsulate
 * the entire functionality of the class.
 *
 * This should keep things simple.
 */

class Parser {

        std::list<Token> *tokens;

        std::vector<Byte> bytes;

        bool found_error = false;

    private:

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

        void parse_identifier() {
            // identifier ::= alpha+
            // Right now this only supports function names, in time it needs to support variables
            this->bytes.push_back(Byte(OpCode::OP_FUNCTION, this->tokens->front().get_value()));
            consume_token(TokenType::TK_IDENTIFIER);
        }

        void parse_constant() {
            // integer ::= digit+
            this->bytes.push_back(Byte(OpCode::OP_CONSTANT, this->tokens->front().get_value()));
            consume_token(TokenType::TK_CONSTANT);
        }

        void parse_primary() {
            // primary ::= integer
            //           | "(" expression ")"
            switch (this->tokens->front().get_type()) {
                case TokenType::TK_OPEN_PARENTHESIS: {
                    consume_token(TokenType::TK_OPEN_PARENTHESIS);
                    parse_expression();
                    consume_token(TokenType::TK_CLOSE_PARENTHESIS, "Expected ')'");
                    break;
                }
                case TokenType::TK_CONSTANT: {
                    parse_constant();
                    break;
                }
                default: return; // Unreachable
            }
        }

        void parse_unary() {
            // unary ::= unary_op primary
            //         | primary
            switch (this->tokens->front().get_type()) {
                case TokenType::TK_TILDE: {
                    consume_token(TokenType::TK_TILDE);
                    parse_primary();
                    this->bytes.push_back(Byte(OpCode::OP_COMPLEMENT));
                    break;
                }
                case TokenType::TK_MINUS: {
                    consume_token(TokenType::TK_MINUS);
                    parse_primary();
                    this->bytes.push_back(Byte(OpCode::OP_NEGATE));
                    break;
                }
                case TokenType::TK_CONSTANT: {
                    parse_primary();
                    break;
                }
                default: return; // Unreachable
            }
        }

        void parse_expression() {
            // expression ::= unary
            parse_unary();
        }

        void parse_return() {
            // return ::= "return" expression ";"
            consume_token(TokenType::TK_KEYWORD_RETURN, "Expected return keyword");
            parse_expression();
            this->bytes.push_back(Byte(OpCode::OP_RETURN));
            consume_token(TokenType::TK_SEMI_COLON, "Expected ';'");
        }

        void parse_block() {
            // block ::= "{" return "}"
            consume_token(TokenType::TK_OPEN_BRACE, "Expected '{'");
            parse_return();
            consume_token(TokenType::TK_CLOSE_BRACE, "Expected '}'");
        }

        void parse_function() {
            // function ::= "int" identifier "(" "void" ")" block
            consume_token(TokenType::TK_KEYWORD_INT, "Expected int keyword");
            parse_identifier();
            consume_token(TokenType::TK_OPEN_PARENTHESIS, "Expected '('");
            consume_token(TokenType::TK_KEYWORD_VOID, "Expected void keyword");
            consume_token(TokenType::TK_CLOSE_PARENTHESIS, "Expected ')'");
            parse_block();
        }

        void parse_program() {
            // program ::= function
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
         * \brief Parses the list of Tokens and returns a vector of found Bytes.
         *
         * \return A vector of Bytes produced from the list of Tokens
         */
        std::vector<Byte> run() {
            parse_program();

            // If we still have Tokens left over
            if (TokenType::TK_EOF != this->tokens->front().get_type()) {
                this->found_error = true;
            }

            return this->bytes;
        }
};

#endif
