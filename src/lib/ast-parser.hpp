/**
 * \file ast-parser.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implementation of the AST Parser class
 * \version 0.1
 * \date 2025-12-15
 */

#ifndef AST_PARSER
#define AST_PARSER

#include <cstddef>
#include <list>

#include "../types/node.hpp"
#include "../types/token.hpp"

#ifdef DEBUG_PARSER
    #include <iostream>
#endif

/**
 * \brief A class outlining the AST Parser class, which is used to turn Tokens into a AST containing Nodes.
 *
 * The aim of this class is to take in a list of Tokens;
 * and parse them to produce an Abstract Syntax Tree contrinong Nodes.
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
class AST_Parser {

        /**
         * \brief The vector of Tokens this Parser uses to build its Byte vector
         */
        std::list<Token> *tokens;

        /**
         * \brief The AST of Nodes built by this Parser
         */
        Node tree;

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
                //! this->bytes.push_back(Byte(OpCode::OP_ERROR, message));
                this->found_error = true;
            } else {
                // consume the token
                this->tokens->pop_front();
            }
        }

        //         /**
        //          * \brief Adds a Byte to the list of found Bytes
        //          *
        //          * This function also facilitates debug output for the Parser
        //          *
        //          * \param byte The Byte
        //          */
        //         void add_byte(Byte byte) {
        // #ifdef DEBUG_PARSER
        //             std::cout << "Found : " << op_code_string.at(byte.get_op()) << std::endl;
        // #endif
        //             this->bytes.push_back(byte);
        //         }

        /**
         * \brief Attempts to Parse an Identifier
         *
         * Grammar:
         *
         * identifier ::= alpha+
         */
        std::string parse_identifier() {
            // Right now this only supports function names, in time it needs to support variables
            std::string value = this->tokens->front().get_value();
            // add_byte(Byte(OpCode::OP_FUNCTION, this->tokens->front().get_value()));
            consume_token(TokenType::TK_IDENTIFIER);
            return value;
        }

        /**
         * \brief Attempts to Parse a Constant (currently only integers)
         *
         * Grammar:
         *
         * integer ::= digit+
         * constant ::= Constant ( value: integer )
         *
         * \param negative If the expected constant is negative
         */
        std::string parse_constant(bool negative = false) {
            std::string value;
            if (negative) {
                value += "-";
            }
            value += this->tokens->front().get_value();
            consume_token(TokenType::TK_CONSTANT);
            return value;
        }

        // /**
        //  * \brief Attempts to Parse a Primary
        //  *
        //  * Grammar:
        //  *
        //  * primary ::= integer
        //  *           | expression
        //  *           | "(" expression ")"
        //  *
        //  * \param negative If the preceeding Token is TK_MINUS, and the constant we find is therefore negative
        //  */
        // void parse_primary(bool negative = false) {
        //     switch (this->tokens->front().get_type()) {
        //         // integer
        //         case TokenType::TK_CONSTANT: {
        //             parse_constant(negative);
        //             break;
        //         }
        //         // expression
        //         case TokenType::TK_MINUS: {
        //             consume_token(TokenType::TK_MINUS);
        //             parse_constant(true);
        //             break;
        //         }
        //         case TokenType::TK_TILDE: {
        //             consume_token(TokenType::TK_TILDE);
        //             parse_constant();
        //             add_byte(Byte(OpCode::OP_COMPLEMENT));
        //             break;
        //         }
        //         // "(" expression ")"
        //         case TokenType::TK_OPEN_PARENTHESIS: {
        //             consume_token(TokenType::TK_OPEN_PARENTHESIS);
        //             parse_expression();
        //             consume_token(TokenType::TK_CLOSE_PARENTHESIS, "Expected ')'");
        //             break;
        //         }
        //         default: {
        //             // error, missing constant
        //             consume_token(TokenType::TK_CONSTANT, "Missing constant");
        //         }
        //     }
        // }

        /**
         * \brief Attempts to Parse a Unary
         *
         * Grammar:
         *
         * unary ::= unary_op primary
         *         | primary
         * unary ::= Complement ( children: expression )
         *         | Negate     ( children: expression )
         */
        Node parse_unary() {
            switch (this->tokens->front().get_type()) {
                case TokenType::TK_TILDE: {
                    consume_token(TokenType::TK_TILDE);
                    // make complement node
                    // Node complement = Node(NodeType::NT_COMPLEMENT);
                    // // find children and add them
                    // Node expression = parse_expression();
                    // complement.add_child(expression);
                    // return node
                    return parse_expression();
                }
                case TokenType::TK_MINUS: {
                    consume_token(TokenType::TK_MINUS);
                    // make negate node
                    // Node negate = Node(NodeType::NT_NEGATE);
                    // // find children and add them
                    // Node expression = parse_expression();
                    // negate.add_child(expression);
                    // return node
                    return parse_expression();
                }
                default: break;
            }

            this->found_error = true;
            return Node(NodeType::NT_ERROR);
        }

        /**
         * \brief Attempts to Parse an Expression
         *
         * Grammar:
         *
         * expression ::= constant
         *              | unary
         */
        Node parse_expression() {

            switch (this->tokens->front().get_type()) {
                case TokenType::TK_CONSTANT: {
                    // find and set value
                    std::string value = parse_constant();
                    Node constant = Node(NodeType::NT_CONSTANT);
                    constant.set_value(value);
                    // return node
                    return constant;
                }
                case TokenType::TK_TILDE: {
                    // make expression node
                    Node expression = Node(NodeType::NT_COMPLEMENT);
                    // find children and add them
                    Node complement = parse_unary();
                    expression.add_child(complement);
                    // return node
                    return expression;
                }
                case TokenType::TK_MINUS: {
                    // make expression node
                    Node expression = Node(NodeType::NT_NEGATE);
                    // find children and add them
                    Node negate = parse_unary();
                    expression.add_child(negate);
                    // return node
                    return expression;
                }
                default: break;
            }

            this->found_error = true;
            return Node(NodeType::NT_ERROR);
        }

        /**
         * \brief Attempts to Parse a Statement
         *
         * Grammar:
         *
         * return ::= "return" expression ";"
         * statement ::= Return ( value: expression )
         */
        Node parse_statement() {
            consume_token(TokenType::TK_KEYWORD_RETURN, "Expected return keyword");

            // make statement node
            Node statement = Node(NodeType::NT_RETURN);
            // find children and add them
            Node expression = parse_expression();
            statement.add_child(expression);

            consume_token(TokenType::TK_SEMI_COLON, "Expected ';'");

            // return node
            return statement;
        }

        // /**
        //  * \brief Attemps to Parse a Block
        //  *
        //  * Grammar:
        //  *
        //  * block ::= "{" return "}"
        //  */
        // void parse_block() {
        //     consume_token(TokenType::TK_OPEN_BRACE, "Expected '{'");
        //     parse_return();
        //     consume_token(TokenType::TK_CLOSE_BRACE, "Expected '}'");
        // }

        /**
         * \brief Attempts to Parse a Function
         *
         * Grammar:
         *
         * function ::= "int" identifier "(" "void"* ")" block
         * function ::= Function ( identifier: name, children: statement )
         */
        Node parse_function() {
            consume_token(TokenType::TK_KEYWORD_INT, "Expected int keyword");
            std::string identifier = parse_identifier();
            consume_token(TokenType::TK_OPEN_PARENTHESIS, "Expected '('");
            // void keyword should be optional
            if (this->tokens->front().get_type() == TokenType::TK_KEYWORD_VOID) {
                consume_token(TokenType::TK_KEYWORD_VOID);
            }
            consume_token(TokenType::TK_CLOSE_PARENTHESIS, "Expected ')'");

            // taken from parse_block
            consume_token(TokenType::TK_OPEN_BRACE, "Expected '{'");

            // make function node
            Node function = Node(NodeType::NT_FUNCTION);
            function.set_value(identifier);
            // find children and add them
            Node statement = parse_statement();
            function.add_child(statement);

            // taken from parse_block
            consume_token(TokenType::TK_CLOSE_BRACE, "Expected '}'");

            // return node
            return function;
        }

        /**
         * \brief Attempts to Parse a Program
         *
         * Grammar:
         *
         * program ::= function
         * program ::= Program ( children: function )
         */
        Node parse_program() {
            // make program node
            Node program = Node(NodeType::NT_PROGRAM);
            // find children and add them
            Node function = parse_function();
            program.add_child(function);

            // return node
            return program;
        }

    public:

        /**
         * \brief Default constructor for an AST Parser
         */
        AST_Parser() {} // Default

        /**
         * \brief Construct a new AST Parser object with a list of Tokens
         *
         * \param tokens The list of Tokens this AST Parser should convert into Nodes
         */
        AST_Parser(std::list<Token> *tokens)
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
         * \brief Parses the list of Tokens and returns an AST containing Nodes
         *
         * \return The AST produced from the Tokens
         */
        Node run() {

// #ifdef DEBUG_PARSER
//             std::cout << std::endl;
//             std::cout << " === Beginning Parsing === " << std::endl;
//             std::cout << std::endl;
// #endif

            this->tree = parse_program();

// #ifdef DEBUG_PARSER
//             std::cout << std::endl;
//             std::cout << " === Finishing Parsing === " << std::endl;
//             std::cout << std::endl;
// #endif

            // If we still have Tokens left over
            if (TokenType::TK_EOF != this->tokens->front().get_type()) {
                this->found_error = true;
            }

            return this->tree;
        }
};

#endif // AST_PARSER
