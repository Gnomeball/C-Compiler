/*
 * In this file we define our parser, which takes in a list
 * of tokens, and from those builds a chunk of bytecode
 */

#include <iostream>

#include "byte.hpp"
#include "chunk.hpp"
#include "token.hpp"
#include "data.hpp"
#include "debug.hpp"

int error = 0;

void consume_token(Token *current_token, TokenType expected) {
#ifdef DEBUG_PARSER
    std::cout << "Entered consume_token - " << current_token->to_string() << std::endl;
#endif
    if (current_token->getType() != expected) {
        // throw error
        std::cout << "Error found near " << current_token->to_string() << std::endl;
        error = 1;
    } else {
        // pop the token from the front of the list, and re-point our pointer
        tokens.pop_front();
        *current_token = tokens.front();
    }
}

void parse_constant(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_int - " << current_token->to_string() << std::endl;
#endif
    // we expect : a constant token
    Byte constant = Byte(OpCode::OP_CONSTANT, current_token->getIntValue());
    memory_chunk.add_byte(constant);
    consume_token(current_token, TokenType::TK_CONSTANT);
}

void parse_identifier(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_identifier - " << current_token->to_string() << std::endl;
#endif
    // we expect : an identifier token
    Byte ident = Byte(OpCode::OP_IDENTIFIER, current_token->getStringValue());
    memory_chunk.add_byte(ident);
    consume_token(current_token, TokenType::TK_IDENTIFIER);
}

void parse_expression(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_expression" << std::endl;
#endif
    // we expect : <int>
    parse_constant(current_token);
}

void parse_statement(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_statement" << std::endl;
#endif
    // we expect : "return" <expression> ";"
    Byte kw_return = Byte(OpCode::OP_RETURN);
    memory_chunk.add_byte(kw_return);
    consume_token(current_token, TokenType::TK_KEYWORD_RETURN);
    parse_expression(current_token);
    consume_token(current_token, TokenType::TK_SEMI_COLON);
}

void parse_function(Token *current_token) {
#ifdef DEBUG_PARSER
    std::cout << "Entered parse_function" << std::endl;
#endif
    // we expect : "int" <identifier> "(" "void" ")" "{" <statement> "}"
    consume_token(current_token, TokenType::TK_KEYWORD_INT);
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
        error = 1;
    }
    return error;
}