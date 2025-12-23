/**
 * \file tokeniser.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implementation of the Tokeniser class
 * \version 0.1
 * \date 2024-10-22
 */

#ifndef TOKENISER
#define TOKENISER

#include <fstream>
#include <list>
#include <string>

#include "../debug.hpp"
#include "../types/error.hpp"
#include "../types/token.hpp"

/**
 * \brief A class outlining the Tokeniser class, which is used to tokenise a C source file.
 *
 * The aim of this class is to take in a single string, a file name;
 * scan and lex the file to produce a list of tokens; and return that.
 *
 * The primary interface will be a single public .run() method, which will
 * hand off to several private internal helper methods that encapsulate
 * the entire functionality of the class.
 *
 * This should keep things simple.
 */
class Tokeniser {

    private:

        /**
         * \brief The input stream
         */
        std::ifstream input;

        /**
         * \brief A vector of Tokens found by this Tokeniser
         */
        std::list<Token> tokens;

        /**
         * \brief Set to true upon object initialisation if the stream opens correctly
         */
        bool is_open = false;

        /**
         * \brief Set to true up on producing an error Token
         */
        bool found_error = false;

        std::list<Error> errors;

        /**
         * \brief Tracks current line number within the input
         *
         * Value is updated every time we hit a '\n'
         */
        int current_line_number;

        /**
         * \brief Tracks current position on line within the input
         *
         * Value is zeroed every '\n' and incremented every character in between
         */
        int current_char_position;

        /**
         * \brief Returns the next character from the input.
         *
         * If the next character found is a \\n then this method also incremnts
         * the current line number, and resets the current position; otherwise
         * it simply increments the current position.
         *
         * \return The next character from the input.
         */
        char get_next_character() {
            // Get the next character from input
            char next = this->input.get();

            // If we've hit the end of a line;
            if ('\n' == next) {
                // Increment line counter and reset position
                this->current_line_number++;
                // std::cout << "Incrementing line to " << this->current_line_number << std::endl;
                this->current_char_position = 0;
            } else {
                // Otherwise, we increment the current position
                this->current_char_position++;
            }

            return next;
        };

        /**
         * \brief Used to put the previous character back onto the input stream,
         * typically because we don't need it yet.
         */
        void put_back_unwanted_char() {
            this->current_char_position--;
            this->input.unget();
        }

        /**
         * \brief Used to peek at the next character without consuming it.
         *
         * \return The next character in the input, but does not remove it.
         */
        char peek_at_next_character() {
            return this->input.peek();
        }

        /**
         * \brief Skips characters in input until a useful one is found; we define
         * a useful character as one that is not a form of whitespace.
         *
         * \return
         */
        char skip_until_useful() {
            char next = this->get_next_character();

            // Whilst this character is not one that we want, skip it
            while (' ' == next || '\t' == next || '\n' == next || '\r' == next || '\f' == next) {
                next = this->get_next_character();
            }

            // Return first useful character
            return next;
        }

        char skip_until_end_of_comment(std::string type) {
            char next = this->get_next_character();

            if ("//" == type) {
                // If the comment type is // then we skip until to the end of the line
                while ('\n' != next) {
                    next = this->get_next_character();
                }
            } else if ("/*" == type) {
                // If the comment type is /* then we skip until the closing */ of the comment
                char previous;
                do {
                    previous = next;
                    // std::cout << "Comment!" << std::endl;
                    next = get_next_character();
                } while (!(next == '/' && previous == '*'));
            }

            return next;
        }

        /**
         * \brief Helper method used to find the position of a character within a string.
         *
         * \param c A character to search for.
         * \param str The string to search within.
         *
         * \return If the character if found then this method returns the index of that
         * character, otherwise it wll return npos.
         */
        int position_of(char c, std::string str) {
            return str.find(c);
        }

        /**
         * \brief Helper method used to set the position of a Token
         *
         * \param token The Token to set the location for
         */
        void set_position_of(Token *token) {
            token->set_token_position(this->current_line_number, this->current_char_position - token->get_length());
        }

        /**
         * \brief Scans the input for a constant.
         *
         * \param c The first digit of a constant, found within find_next_token().
         *
         * \return A Token representing the constant.
         */
        Token scan_for_constant(char c) {
            // Variable to store the value of the found constant
            int value = 0;

            // Used to store the found index of any digit
            int index;

            // Whilst we still find numbers
            while (0 <= (index = position_of(c, "0123456789"))) {
                // note : this may look strange, but it's simplicity will allow easy
                //        parsing of any base; we just need to know the input base and
                //        can convert the result back after collection
                value = value * 10 + index;
                c = this->get_next_character();
            }

            // Next character isn't one we want, so we put it back
            this->put_back_unwanted_char();

            // Value is now equal to the value of the constant, but because we will only
            // ever use the string representation of it, we use that form.
            return Token(TokenType::TK_CONSTANT, std::to_string(value));
        }

        /**
         * \brief Scans the input for an identifier.
         *
         * \param c The first letter of the identifier, found within find_next_token().
         *
         * \return A Token representing that identifier.
         */
        Token scan_for_identifier(char c) {
            // Variable to store the found value of the identifier
            std::string value = "";

            // Used to store the index of any found letter
            int index;

            // Whilst we still find letters
            while (0 <= (index = position_of(c, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"))) {
                value.push_back(c);
                c = this->get_next_character();
            }

            // Next character isn't one we want, so we put it back
            this->put_back_unwanted_char();

            // Check if this identifier is actually a keyword
            if (identifiers_to_token.count(value) != 0) {
                // Return the keyword Token
                return Token(identifiers_to_token.at(value));
            }

            // Otherwise, return it as an identifier
            return Token(TokenType::TK_IDENTIFIER, value);
        }

        /**
         * \brief Helper method used for matching 2-character operators
         *
         * Matches the next character against one option that can possibly form an
         * operator with the current character; for example '*', which can become
         * either one of '*', or '*='.
         *
         * \param possible A possible match to form a 2-character operator
         * \param type_possible The TokenType that should be returned if a match is found
         * \param otherwise The TokenType that should be returned if no match is found
         *
         * \return A Token with the necessary type
         */
        Token match_next(char possible, TokenType type_possible, TokenType otherwise) {
            if (possible == this->peek_at_next_character()) {
                // we still need to consume the character
                this->get_next_character();
                return Token(type_possible);
            }
            // Otherwise
            return Token(otherwise);
        }

        /**
         * \brief Helper method used for matching 2-character operators
         *
         * Matches the next character against one of two options that can both form an
         * operator with the current character; for example '+', which can become any
         * one of '+', '++', or '+='.
         *
         * \param a A possible match to form a 2-character operator
         * \param type_a The TokenType that should be returned if a match is found with a
         * \param b Another possible match to form a 2-character operator
         * \param type_b The TokenType that should be returned if a match is found with b
         * \param otherwise The TokenType that should be returned if no match is found
         *
         * \return A Token with the necessary type
         */
        Token attempt_to_match(char a, TokenType type_a, char b, TokenType type_b, TokenType otherwise) {
            if (a == this->peek_at_next_character()) {
                // we still need to consume the character
                this->get_next_character();
                return Token(type_a);
            } else if (b == this->peek_at_next_character()) {
                // we still need to consume the character
                this->get_next_character();
                return Token(type_b);
            }
            // Otherwise
            return Token(otherwise);
        }

        /**
         * \brief Scans through the input, trying to find the next Token.
         *
         * Most of the processing happens within this method, as it is called repeatedly
         * until the entire file has been converted into Tokens
         *
         * \return The next Token.
         */
        Token find_next_token() {
            // Start by getting the first useful character
            char next = this->skip_until_useful();

            // And try to match it against all our single-character Tokens
            switch (next) {
                // If we are at the end of the file, we return the EOF Token
                case EOF: return Token(TokenType::TK_EOF);

                // Otherwise, we test each of the tokens one by one
                case '(': return Token(TokenType::TK_OPEN_PARENTHESIS);
                case ')': return Token(TokenType::TK_CLOSE_PARENTHESIS);
                case '{': return Token(TokenType::TK_OPEN_BRACE);
                case '}': return Token(TokenType::TK_CLOSE_BRACE);
                case '?': return Token(TokenType::TK_QUESTION);
                case ':': return Token(TokenType::TK_COLON);
                case ',': return Token(TokenType::TK_COMMA);
                case ';': return Token(TokenType::TK_SEMI_COLON);

                case '+': return attempt_to_match('+', TokenType::TK_PLUS_PLUS, '=', TokenType::TK_PLUS_EQUAL, TokenType::TK_PLUS);
                case '-': return attempt_to_match('-', TokenType::TK_MINUS_MINUS, '=', TokenType::TK_MINUS_EQUAL, TokenType::TK_MINUS);

                case '*': return match_next('=', TokenType::TK_STAR_EQUAL, TokenType::TK_STAR);

                case '/': {
                    if ('=' == this->peek_at_next_character()) {
                        // Token is /=
                        next = get_next_character();
                        return Token(TokenType::TK_SLASH_EQUAL);
                    } else if ('/' == this->peek_at_next_character()) {
                        // Token is //, skip to end of line
                        Token temp = Token(TokenType::TK_SLASH_SLASH);
                        this->set_position_of(&temp);
                        next = this->skip_until_end_of_comment("//");
                        return temp;
                    } else if ('*' == this->peek_at_next_character()) {
                        // Token is /*, skip to closing */
                        Token temp = Token(TokenType::TK_SLASH_STAR);
                        this->set_position_of(&temp);
                        next = this->skip_until_end_of_comment("/*");
                        return temp;
                    }
                }
                case '%': return match_next('=', TokenType::TK_PERCENTAGE_EQUAL, TokenType::TK_PERCENTAGE);
                case '^': return match_next('=', TokenType::TK_CARET_EQUAL, TokenType::TK_CARET);

                case '&': return attempt_to_match('&', TokenType::TK_AMPE_AMPE, '=', TokenType::TK_AMPERSAND_EQUAL, TokenType::TK_AMPERSAND);
                case '|': return attempt_to_match('|', TokenType::TK_PIPE_PIPE, '=', TokenType::TK_PIPE_EQUAL, TokenType::TK_PIPE);

                case '~': return Token(TokenType::TK_TILDE);

                case '!': return match_next('=', TokenType::TK_BANG_EQUAL, TokenType::TK_BANG);
                case '=': return match_next('=', TokenType::TK_EQUAL_EQUAL, TokenType::TK_EQUAL);

                case '>': {
                    if ('>' == this->peek_at_next_character()) {
                        // Token is >> or >>=
                        next = this->get_next_character();
                        return match_next('=', TokenType::TK_RIGHT_CHEVRONS_EQUAL, TokenType::TK_RIGHT_CHEVRONS);
                    } else if ('=' == this->peek_at_next_character()) {
                        // Token is >=
                        next = this->get_next_character();
                        return Token(TokenType::TK_GREATER_EQUAL);
                    }
                    // Otherwise
                    return Token(TokenType::TK_GREATER);
                };
                case '<': {
                    if ('<' == this->peek_at_next_character()) {
                        // Token is << or <<=
                        next = this->get_next_character();
                        return match_next('=', TokenType::TK_LEFT_CHEVRONS_EQUAL, TokenType::TK_LEFT_CHEVRONS);
                    } else if ('=' == this->peek_at_next_character()) {
                        // Token is <=
                        next = this->get_next_character();
                        return Token(TokenType::TK_LESS_EQUAL);
                    }
                    // Otherwise
                    return Token(TokenType::TK_LESS);
                };

                // After exhausting all of those options;
                //     we check for constants, and then identifers
                default:
                    // If it's a digit
                    if (std::isdigit(next)) {
                        Token constant = this->scan_for_constant(next);
                        // Ensure constant is correctly formed
                        if (std::isalpha(this->peek_at_next_character())) {
                            // Malformed constant
                            this->found_error = true;
                            return Token(TokenType::TK_ERROR, std::string(1, this->peek_at_next_character()), "Malformed Constant");
                        }
                        return constant;
                    }
                    // If it's a letter, or underscore
                    if ('-' == next || std::isalpha(next)) {
                        return this->scan_for_identifier(next);
                    }
            }

            // If nothing matched, return the error Token
            std::string error = "";
            error.push_back(next);
            this->found_error = true;
            return Token(TokenType::TK_ERROR, error, "Unrecognised Character");
        }

    public:

        /**
         * \brief Default constructor for a Tokeniser
         */
        Tokeniser() {} // default

        /**
         * \brief Construct a new Tokeniser object with an input file
         *
         * \param file The input file this Tokeniser should convert into Tokens
         */
        Tokeniser(std::string file) {
            // Attempt to open the stream, and set is_open
            this->input = std::ifstream(file);
            this->is_open = this->input.is_open();

            // Set up default values for tokenising
            this->current_line_number = 1;
            this->current_char_position = 0;
        }

        /**
         * \brief Used to check if the input file opened correctly.
         */
        bool opened() {
            return this->is_open;
        }

        /**
         * \brief Used to check if an error was found.
         *
         * \return True if an error Token was produced, otherwise false.
         */
        bool had_error() {
            return this->found_error;
        }

        /**
         * \brief Get the list of Errors
         *
         * \return The list of Errors, if any were found
         */
        std::list<Error> get_errors(void) {
            return this->errors;
        }

        /**
         * \brief Scans the input file and returns a list of found Tokens.
         *
         * \return A list of Tokens found within the input file.
         */
        std::list<Token> run() {
            // Start by reading one Token
            Token temp = this->find_next_token();

#ifdef DEBUG_TOKENISER
            std::cout << std::endl;
            std::cout << " === Beginning Tokenisation === " << std::endl;
            std::cout << std::endl;
#endif

            // Move through the file, scanning for Tokens
            while (temp.get_type() != TokenType::TK_EOF) {

                // If this Token isn't a comment,
                if (temp.get_type() != TokenType::TK_SLASH_SLASH && temp.get_type() != TokenType::TK_SLASH_STAR) {
                    // Set the position of the Token
                    this->set_position_of(&temp);

                    // add it to the list
                    tokens.push_back(temp);

                    // If this Token was an error, add it to that list also
                    if (temp.get_type() == TokenType::TK_ERROR) {
                        this->errors.push_back(Error(temp));
                    }
                }

#ifdef DEBUG_TOKENISER
                std::cout << "Found : " << token_string_values.at(temp.get_type())
                          << " at line: " << this->current_line_number
                          << ", pos: " << this->current_char_position << std::endl;
#endif

                // Otherwise, get the next one
                temp = this->find_next_token();
            }

            // Because we don't stop at the first error, the final EOF token gets ignored
            if (temp.get_type() == TokenType::TK_EOF) {
                // So we need to make sure we add it
                this->tokens.push_back(temp);
            }

#ifdef DEBUG_TOKENISER
            std::cout << std::endl;
            std::cout << " === Finishing Tokenisation === " << std::endl;
            std::cout << std::endl;
#endif

            return this->tokens;
        }
};

#endif // TOKENISER
