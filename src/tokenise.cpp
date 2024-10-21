/*
 * In this file we define our Scanner, which reads through
 * the input file, and from it creates Tokens
 */

#ifndef TOKENISE
#define TOKENISE

#include <cctype>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "tokenise.hpp"
#include "enums/token.hpp"

#include "debug.hpp"

#include "data.hpp"

//>> Begin variable declarations

// // Pointer to our input file
// std::ifstream input_file;

// // A container for the Tokens we find
// std::vector<Token> tokens;

// Track current line, and position within it
int current_line_number = 1;
int current_character_position = 0;

// Stores a character which we may need to put back
int put_back = 0;

//<< End variable declarations

// Gets the next character from the input file
char get_next_char(void) {
    char current_character;

    // If we need to put back the previous character
    if (put_back) {
        current_character = put_back;
        put_back = 0;
        return current_character;
    }

    // Read the next charater from the input file
    current_character = input_file.get();
    // Increment position on line tracker
    current_character_position++;

    // If we hit the end of a line
    if ('\n' == current_character) {
        // Increment line count
        current_line_number++;
        // Reset position on line tracker
        current_character_position = 0;
    }

    return current_character;
}

static char peek_next_char(void) {
    // used if we need to check what the next charater is, without consuming it
    return input_file.peek();
}

// Skips over whitespace and other characters we don't need to worry about,
// returning the first character that isn't whitespace
static char skip_until_useful(void) {
    char current_character = get_next_char();

    // Whilst the current character isn't one we want, keep skipping
    while (' ' == current_character || '\t' == current_character || '\n' == current_character || '\r' == current_character || '\f' == current_character) {
        current_character = get_next_char();
    }

    // Return the first character that we don't skip over
    return current_character;
}

// Stores a character we want to put back because we don't need it yet
static void put_back_unwanted_char(char c) {
    put_back = c;
}

// Helper function for finding position of a caracter in a set string
static int position_of(char c, std::string str) {
    return str.find(c);
}

// Function to scan in a constant value
static const std::string scan_for_constant(char c) {
    // Variable to store the value of the found constant
    int value = 0;

    // Temporary variable
    int temp;

    // Whilst we still find numbers
    while ((temp = position_of(c, "0123456789")) >= 0) {
        value = value * 10 + temp;
        c = get_next_char();
    }

    // Next character isn't an integer, put it back
    put_back_unwanted_char(c);

#ifdef DEBUG_SCANNER
    std::cout << "Found constant: " << value << std::endl;
#endif

    return std::to_string(value);
}

// Function to scan for an identifier / keyword
static std::string scan_for_identifier_or_keyword(char c) {
    // Variable to store the value of the found identifier
    std::string value = "";

    // Temporary value
    char temp;

    // Whilst we still find useful characters
    //! must be a better way, but for now this'll do
    while ((temp = position_of(c, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_")) >= 0) {
        value += c;
        c = get_next_char();
    }

    // Next character isn't alpha or underscore, put it back
    put_back_unwanted_char(c);

#ifdef DEBUG_SCANNER
    std::cout << "Found identifier or keyword: " << value << std::endl;
#endif

    return value;
}

// Scan through our input file, character by character, attempting to match a token
static Token find_next_token(void) {
    // Start by getting the first useful character
    char current_character = skip_until_useful();

    // Try to match it with any of our single-character Tokens
    switch (current_character) {
        case EOF: return Token(TokenType::TK_EOF);
        case ';': return Token(TokenType::TK_SEMI_COLON);
        case '(': return Token(TokenType::TK_OPEN_PARENTHESIS);
        case ')': return Token(TokenType::TK_CLOSE_PARENTHESIS);
        case '{': return Token(TokenType::TK_OPEN_BRACE);
        case '}': return Token(TokenType::TK_CLOSE_BRACE);
        case '~': return Token(TokenType::TK_TILDE);
        case '-': {
            if ('-' == peek_next_char()) {
                // Token is a --, consume the next character and return
                current_character = get_next_char();
                return Token(TokenType::TK_MINUS_MINUS);
            } else {
                // Token is simply a minus
                return Token(TokenType::TK_MINUS);
            }
        };
        default:
            // Check if it's a constant
            if (isdigit(current_character)) {
                const std::string value = scan_for_constant(current_character);
                if (std::isalpha(put_back)) {
                    // Malformed constant
                    std::string value = "";
                    value.push_back(put_back);
                    return Token(TokenType::TK_ERROR, value);
                }
                return Token(TokenType::TK_CONSTANT, value);
            }
            // Check if it's a letter or underscore
            if (isalpha(current_character) || '_' == current_character) {
                const std::string value = scan_for_identifier_or_keyword(current_character);
                // std::cout << value << std::endl;
                // Try to match the possible identifier against a Token
                if ("int" == value) {
                    return Token(TokenType::TK_KEYWORD_INT);
                } else if ("void" == value) {
                    return Token(TokenType::TK_KEYWORD_VOID);
                } else if ("return" == value) {
                    return Token(TokenType::TK_KEYWORD_RETURN);
                } else {
                    return Token(TokenType::TK_IDENTIFIER, value);
                }
            };
    }

    // If nothing matches we return an error Token
    std::string error = "";
    error.push_back(current_character);
    return Token(TokenType::TK_ERROR, error);
}

// Scan the input, building the Token array
// returns 0 if no Errors are found, 1 otherwise
int scan_for_tokens() {
    // input_file = std::ifstream(file);

    // Start by getting the first token
    Token temp = find_next_token();

    // Move through the file, finding tokens
    while (temp.get_type() != TokenType::TK_ERROR) {

        // If we find one, add it to our list
        tokens.insert(tokens.end(), temp);

        // If this token is EOF
        if (temp.get_type() == TokenType::TK_EOF) {
            return 0;
        }

        temp = find_next_token();
    }

    // Implicit else, we've hit an error
    std::cout << "Error found near \'" << temp.get_value() << "\' on line "
              << current_line_number << " at position " << current_character_position << std::endl;

    return 1;
}

#endif
