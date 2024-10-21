/*
 * In this file we define our parser, which takes in a list
 * of tokens, and from those builds a list of Bytes
 */

#include <iostream>

#include "globals.hpp"
#include "debug.hpp"

#include "enums/byte.hpp"
#include "enums/token.hpp"

// TODO: Add tracking to Tokens so errors found here can report that information, and probably move errors to a seperate file

int parse_error = 0;

void consume_token(Token *current_token, TokenType expected) {
#ifdef DEBUG_PARSER
    std::cout << "Entered consume_token - " << current_token->to_string() << std::endl;
#endif
    if (current_token->get_type() != expected) {
        // throw error
        std::cout << "Error found near " << current_token->to_string() << std::endl;
        parse_error = 1;
    } else {
        // pop the token from the front of the list, and re-point our pointer
        tokens.pop_front();
        if (!tokens.empty()) {
            *current_token = tokens.front();
        }
    }
}

void parse_constant(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_int - " << current_token->to_string() << std::endl;
#endif
    // we expect : a constant token
    Byte constant = Byte(OpCode::OP_CONSTANT, current_token->get_value());
    bytes.push_back(constant);
    consume_token(current_token, TokenType::TK_CONSTANT);
}

void parse_identifier(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_identifier - " << current_token->to_string() << std::endl;
#endif
    // we expect : an identifier token
    // Byte ident = Byte(OpCode::OP_IDENTIFIER, current_token->get_value());
    // bytes.push_back(ident);
    consume_token(current_token, TokenType::TK_IDENTIFIER);
}

void parse_unary(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_unary" << std::endl;
#endif
    // we expect : "-" (NEGATE) | "~"
    switch (current_token->get_type()) {
        case TokenType::TK_TILDE: {
            // expect : ~
            bytes.push_back(OpCode::OP_COMPLEMENT);
            consume_token(current_token, TokenType::TK_TILDE);
            break;
        }
        case TokenType::TK_MINUS: {
            // expect : -
            bytes.push_back(OpCode::OP_NEGATE);
            consume_token(current_token, TokenType::TK_MINUS);
            break;
        }
        default: break;
    }
}

void parse_expression(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_expression" << std::endl;
#endif

    // HACK: This will need to be smarter when we hit operator precedence,
    //       will need an order of oprators to scan through, attempting to match
    //       the highest ones first so that we aren't doing things a wonky order

    // we expect : <int> | <unary> <expression> | "(" <expression> ")"
    switch (current_token->get_type()) {
        case TokenType::TK_OPEN_PARENTHESIS: {
            // expect : "(" <expression> ")"
            consume_token(current_token, TokenType::TK_OPEN_PARENTHESIS);
            parse_expression(current_token);
            consume_token(current_token, TokenType::TK_CLOSE_PARENTHESIS);
            break;
        }
        case TokenType::TK_TILDE: {
            // expect : <unary> <expression>
            // return : <expression> <unary>
            Byte unary = Byte(OpCode::OP_COMPLEMENT);
            consume_token(current_token, TokenType::TK_TILDE);
            parse_expression(current_token);
            bytes.push_back(unary);
            break;
        }
        case TokenType::TK_MINUS: {
            // expect : <unary> <expression>
            // return : <expression> <unary>
            Byte unary = Byte(OpCode::OP_NEGATE);
            consume_token(current_token, TokenType::TK_MINUS);
            parse_expression(current_token);
            bytes.push_back(unary);
            break;
        }
        case TokenType::TK_CONSTANT: {
            // expect : <int>
            parse_constant(current_token);
            break;
        }
        default:
            // found something we did not expect, log the error
            std::cout << "Error found near " << current_token->to_string() << std::endl;
            parse_error = 1;
            break;
    }
}

void parse_statement(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_statement" << std::endl;
#endif
    // we expect : "return" <expression> ";"
    // we return : <expression> "return" ";"
    consume_token(current_token, TokenType::TK_KEYWORD_RETURN);
    parse_expression(current_token);
    Byte kw_return = Byte(OpCode::OP_RETURN);
    bytes.push_back(kw_return);
    consume_token(current_token, TokenType::TK_SEMI_COLON);
}

void parse_function(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_function" << std::endl;
#endif
    // we expect : "int" <identifier> "(" "void" ")" "{" <statement> "}"
    // we return : <function> <identifier>
    consume_token(current_token, TokenType::TK_KEYWORD_INT);
    Byte function = Byte(OpCode::OP_FUNCTION, current_token->get_value());
    bytes.push_back(function);
    parse_identifier(current_token);
    consume_token(current_token, TokenType::TK_OPEN_PARENTHESIS);
    consume_token(current_token, TokenType::TK_KEYWORD_VOID);
    consume_token(current_token, TokenType::TK_CLOSE_PARENTHESIS);
    consume_token(current_token, TokenType::TK_OPEN_BRACE);
    parse_statement(current_token);
    consume_token(current_token, TokenType::TK_CLOSE_BRACE);
}

int parse_program(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_program" << std::endl;
#endif
    // we expect : <function> EOF
    parse_function(current_token);
    consume_token(current_token, TokenType::TK_EOF);
    if (tokens.size() != 0) {
        // we still have tokens left
        parse_error = 1;
    }
    return parse_error;
}
