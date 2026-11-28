/**
 * \file tokeniser.hpp
 * \author Gnomeball
 * \brief Scan a source file into tokens and lexical diagnostics.
 * \version 0.1
 * \date 2024-10-22
 */

#ifndef TOKENISER
#define TOKENISER

#include <cctype>
#include <cstdio>
#include <fstream>
#include <list>
#include <string>

#include "../debug.hpp"
#include "../types/error.hpp"
#include "../types/token.hpp"

/**
 * \brief Scan an input file into tokens with source spans.
 *
 * run() filters comments, retains error tokens and collects diagnostics.
 */
class Tokeniser {

    private:

        /**
         * \brief The input stream
         */
        std::ifstream input;

        /**
         * \brief A list of Tokens found by this Tokeniser
         */
        std::list<Token> tokens;

        /**
         * \brief Set to true upon object initialisation if the stream opens correctly
         */
        bool is_open = false;

        /**
         * \brief Set when a lexical error or unterminated block comment is found.
         */
        bool found_error = false;

        /**
         * \brief A list of any Errors found
         */
        std::list<Error> errors;

        /**
         * \brief Tracks current line number within the input
         *
         * Value is updated every time we hit a '\n'
         */
        int current_line_number{};

        /**
         * \brief Tracks current position on line within the input
         *
         * Value is zeroed every '\n' and incremented every character in between
         */
        int current_char_position{};

        /**
         * \brief Returns the next character from the input.
         *
         * If the next character found is a '\\n' then this method also increments
         * the current line number, and resets the current position; otherwise
         * it simply increments the current position.
         *
         * \return The next character from the input.
         */
        int get_next_character() {
            // Get the next character from input
            const int next = this->input.get();

            // EOF is not a source character and must not advance the position.
            if (EOF == next) {
                return EOF;
            }

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
         * \brief Used to peek at the next character without consuming it.
         *
         * \return The next character without consuming it, or EOF.
         */
        int peek_at_next_character() {
            return this->input.peek();
        }

        /**
         * \brief Consume spaces, tabs, newlines, carriage returns and form feeds.
         *
         * \return The first consumed non-whitespace character, or EOF.
         */
        int skip_until_useful() {
            int next = this->get_next_character();

            // Whilst this character is not one that we want, skip it
            while (' ' == next || '\t' == next || '\n' == next || '\r' == next || '\f' == next) {
                next = this->get_next_character();
            }

            // Return first useful character
            return next;
        }

        /**
         * \brief Consume a line or block comment after its opening slash.
         *
         * \param type Either double-slash or slash-star; the second opening character is pending.
         *
         * A line comment ends at a consumed newline or EOF. A block comment ends
         * at the closing star-slash; if unterminated, an Error spanning its opening line is recorded
         * and the error flag is set, without adding an error token to the output.
         *
         * \return The last consumed character, including the comment delimiter.
         */
        char skip_until_end_of_comment(const std::string& type) {
            // The opening / of either type has already been consumed
            const int opening_line = this->current_line_number;
            const int opening_pos = this->current_char_position - 1;
            char next = this->get_next_character(); // consume either the second / or opening *

            if ("//" == type) {
                while (this->input.peek() != EOF) {
                    next = this->get_next_character();
                    // If the comment type is // then we skip until to the end of the line
                    if ('\n' == next) {
                        break;
                    }
                }
            } else if ("/*" == type) {
                int first_line_length = 2; // Opening /*
                bool on_opening_line = true; // True until first line break
                char previous = '\0'; // Opening * can't be re-used as closing *

                while (this->input.peek() != EOF) {
                    next = this->get_next_character();
                    // Because these comments can span multiple lines, we need to track that
                    if (on_opening_line) {
                        if ('\n' == next) {
                            on_opening_line = false;
                        } else {
                            ++first_line_length;
                        }
                    }
                    // If the comment type is /* then we skip until the closing */ of the comment
                    if ('*' == previous && '/' == next) {
                        return next;
                    }
                    previous = next;
                }

                // If no proper closing */ was found, create an error and flip the flag
                Token error(TokenType::TK_ERROR, "/*", ErrorType::ET_UNTERMINATED_BLOCK_COMMENT,
                            opening_line, opening_pos, first_line_length);
                this->errors.emplace_back(error);
                this->found_error = true;
            }

            return next;
        }

        /**
         * \brief Helper method used to find the position of a character within a string.
         *
         * \param c A character to search for.
         * \param str The string to search within.
         *
         * \return The result of string::find converted to int, including its
         * npos result when the character is absent.
         */
        static int position_of(char c, const std::string& str) {
            return static_cast<int>(str.find(c));
        }

        /**
         * \brief Set a token's source position and fill in a missing length.
         *
         * A non-EOF token with zero length receives the current position minus
         * start; an existing length and the EOF length are left unchanged.
         *
         * \param token Token copied and updated.
         * \param line One-based source line.
         * \param start Zero-based byte offset on that line.
         * \return The updated copy.
         */
        Token record_token_span(Token token, const int line, const int start) const {
            token.set_token_position(line, start);
            if (token.get_type() != TokenType::TK_EOF && token.get_length() == 0) {
                token.set_token_length(this->current_char_position - start);
            }
            return token;
        }

        /**
         * \brief Scan a numeric candidate starting with an already consumed digit.
         *
         * Accepts decimal, leading-zero octal and 0x/0X hexadecimal integers.
         * Alphanumeric characters, underscores, dots and exponent signs are
         * collected as part of the candidate so unsupported forms produce one
         * error token. Octal digits 8 or 9 produce an invalid-octal error;
         * malformed hexadecimal and other unsupported forms use a separate error.
         * Integer range checking is left to the parser.
         *
         * \param c The first digit of a constant, found within find_next_token().
         *
         * \return A constant or error token with its source span; errors set the flag.
         */
        Token scan_for_constant(char c) {
            // The first digit has already been consumed
            const int line = this->current_line_number;
            const int start = this->current_char_position - 1;
            std::string constant(1, c);

            char next = this->input.peek();
            while (next != EOF) {
                const bool number_character = std::isalnum(next) || next == '_' || next == '.';
                const bool hexadecimal = constant.length() > 1 && constant[0] == '0' && (constant[1] == 'x' || constant[1] == 'X');
                const bool exponent_marker = hexadecimal
                    ? (constant.back() == 'p' || constant.back() == 'P')
                    : (constant.back() == 'e' || constant.back() == 'E');
                const bool exponent_sign = (next == '+' || next == '-') && exponent_marker;
                if (!number_character && !exponent_sign) {
                    break;
                }
                constant.push_back(this->get_next_character());
                next = this->input.peek();
            }

            // Check for hexadecimal literals
            if (constant.length() > 1 && constant[0] == '0' && (constant[1] == 'x' || constant[1] == 'X')) {
                // Filter out currently unsupported hexadecimal literals
                if (constant.length() == 2 ||
                    constant.find_first_not_of("0123456789abcdefABCDEF", 2) != std::string::npos) {
                    this->found_error = true;
                    return {TokenType::TK_ERROR, constant, ErrorType::ET_UNSUPPORTED_NUMERIC_LITERAL,
                            line, start, static_cast<int>(constant.length())};
                }

                return {TokenType::TK_CONSTANT, constant, line, start, static_cast<int>(constant.length())};
            }

            // Filter out currently unsupported numeric literals
            if (constant.find_first_not_of("0123456789") != std::string::npos) {
                this->found_error = true;
                return {TokenType::TK_ERROR, constant, ErrorType::ET_UNSUPPORTED_NUMERIC_LITERAL,
                        line, start, static_cast<int>(constant.length())};
            }

            // Check for invalid octal literals
            if (constant.length() > 1 && constant[0] == '0') {
                if (constant.find_first_of("89") != std::string::npos) {
                    this->found_error = true;
                    return {TokenType::TK_ERROR, constant, ErrorType::ET_INVALID_OCTAL_LITERAL,
                            line, start, static_cast<int>(constant.length())};
                }
            }

            return {TokenType::TK_CONSTANT, constant, line, start, static_cast<int>(constant.length())};
        }

        /**
         * \brief Scan an identifier or keyword after its first character.
         *
         * Subsequent characters may be alphanumeric or underscores. Recognised
         * keywords have an empty value; other identifiers retain their spelling.
         *
         * \param c The first letter of the identifier, found within find_next_token().
         *
         * \return A Token representing that identifier.
         */
        Token scan_for_identifier(char c) {
            // Capture the starting position of this identifier
            const int line = this->current_line_number;
            const int start = this->current_char_position - 1;

            // Variable to store the found value of the identifier
            std::string value(1, c);

            // Whilst we still find valid characters
            char next = this->input.peek();
            while (next != EOF && (std::isalnum(next) || next == '_')) {
                value.push_back(this->get_next_character());
                next = this->input.peek();
            }

            const int length = static_cast<int>(value.length());
            // If this is a defined keyword
            if (identifiers_to_token.count(value) != 0) {
                return {Token(identifiers_to_token.at(value), "", line, start, length)};
            }

            // Otherwise, return it as an identifier
            return {Token(TokenType::TK_IDENTIFIER, value, line, start, length)};
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
                return {Token(type_possible)};
            }
            // Otherwise
            return {Token(otherwise)};
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
                return {Token(type_a)};
            } else if (b == this->peek_at_next_character()) {
                // we still need to consume the character
                this->get_next_character();
                return {Token(type_b)};
            }
            // Otherwise
            return {Token(otherwise)};
        }

        /**
         * \brief Consume and classify the next token after whitespace.
         *
         * Comment markers are returned after consuming their comments and are
         * filtered by run(). An unterminated block comment records a separate
         * diagnostic; unrecognised characters produce error tokens. EOF carries
         * the current position and zero length.
         *
         * \return The next Token.
         */
        Token find_next_token() {
            // Start by getting the first useful character
            int next = this->skip_until_useful();

            if (EOF == next) {
                return {Token(TokenType::TK_EOF, "", this->current_line_number, this->current_char_position, 0)};
            }

            const int token_line = this->current_line_number;
            const int token_start = this->current_char_position - 1;

            // And try to match it against all our single-character Tokens
            switch (next) {
                // Otherwise, we test each of the tokens one by one
                case '(': return this->record_token_span(Token(TokenType::TK_OPEN_PARENTHESIS), token_line, token_start);
                case ')': return this->record_token_span(Token(TokenType::TK_CLOSE_PARENTHESIS), token_line, token_start);
                case '{': return this->record_token_span(Token(TokenType::TK_OPEN_BRACE), token_line, token_start);
                case '}': return this->record_token_span(Token(TokenType::TK_CLOSE_BRACE), token_line, token_start);
                case '?': return this->record_token_span(Token(TokenType::TK_QUESTION), token_line, token_start);
                case ':': return this->record_token_span(Token(TokenType::TK_COLON), token_line, token_start);
                case ',': return this->record_token_span(Token(TokenType::TK_COMMA), token_line, token_start);
                case ';': return this->record_token_span(Token(TokenType::TK_SEMI_COLON), token_line, token_start);

                case '+': return this->record_token_span(attempt_to_match('+', TokenType::TK_PLUS_PLUS, '=', TokenType::TK_PLUS_EQUAL, TokenType::TK_PLUS), token_line, token_start);
                case '-': return this->record_token_span(attempt_to_match('-', TokenType::TK_MINUS_MINUS, '=', TokenType::TK_MINUS_EQUAL, TokenType::TK_MINUS), token_line, token_start);

                case '*': return this->record_token_span(match_next('=', TokenType::TK_STAR_EQUAL, TokenType::TK_STAR), token_line, token_start);

                case '/': {
                    if ('=' == this->peek_at_next_character()) {
                        // Token is /=
                        this->get_next_character();
                        return this->record_token_span(Token(TokenType::TK_SLASH_EQUAL), token_line, token_start);
                    } else if ('/' == this->peek_at_next_character()) {
                        // Token is //, skip to end of line
                        auto temp = Token(TokenType::TK_SLASH_SLASH, "", token_line, token_start, 2);
                        this->skip_until_end_of_comment("//");
                        return temp;
                    } else if ('*' == this->peek_at_next_character()) {
                        // Token is /*, skip to closing */
                        auto temp = Token(TokenType::TK_SLASH_STAR, "", token_line, token_start, 2);
                        this->skip_until_end_of_comment("/*");
                        return temp;
                    }
                    // Otherwise
                    return this->record_token_span(Token(TokenType::TK_SLASH), token_line, token_start);
                }
                case '%': return this->record_token_span(match_next('=', TokenType::TK_PERCENTAGE_EQUAL, TokenType::TK_PERCENTAGE), token_line, token_start);
                case '^': return this->record_token_span(match_next('=', TokenType::TK_CARET_EQUAL, TokenType::TK_CARET), token_line, token_start);

                case '&': return this->record_token_span(attempt_to_match('&', TokenType::TK_AMPE_AMPE, '=', TokenType::TK_AMPERSAND_EQUAL, TokenType::TK_AMPERSAND), token_line, token_start);
                case '|': return this->record_token_span(attempt_to_match('|', TokenType::TK_PIPE_PIPE, '=', TokenType::TK_PIPE_EQUAL, TokenType::TK_PIPE), token_line, token_start);

                case '~': return this->record_token_span(Token(TokenType::TK_TILDE), token_line, token_start);

                case '!': return this->record_token_span(match_next('=', TokenType::TK_BANG_EQUAL, TokenType::TK_BANG), token_line, token_start);
                case '=': return this->record_token_span(match_next('=', TokenType::TK_EQUAL_EQUAL, TokenType::TK_EQUAL), token_line, token_start);

                case '>': {
                    if ('>' == this->peek_at_next_character()) {
                        // Token is >> or >>=
                        this->get_next_character();
                        return this->record_token_span(match_next('=', TokenType::TK_RIGHT_CHEVRONS_EQUAL, TokenType::TK_RIGHT_CHEVRONS), token_line, token_start);
                    } else if ('=' == this->peek_at_next_character()) {
                        // Token is >=
                        this->get_next_character();
                        return this->record_token_span(Token(TokenType::TK_GREATER_EQUAL), token_line, token_start);
                    }
                    // Otherwise
                    return this->record_token_span(Token(TokenType::TK_GREATER), token_line, token_start);
                };
                case '<': {
                    if ('<' == this->peek_at_next_character()) {
                        // Token is << or <<=
                        this->get_next_character();
                        return this->record_token_span(match_next('=', TokenType::TK_LEFT_CHEVRONS_EQUAL, TokenType::TK_LEFT_CHEVRONS), token_line, token_start);
                    } else if ('=' == this->peek_at_next_character()) {
                        // Token is <=
                        this->get_next_character();
                        return this->record_token_span(Token(TokenType::TK_LESS_EQUAL), token_line, token_start);
                    }
                    // Otherwise
                    return this->record_token_span(Token(TokenType::TK_LESS), token_line, token_start);
                };

                // After exhausting all of those options;
                //     we check for constants, and then identifiers
                default:
                    // If it's a digit
                    if (std::isdigit(next)) {
                        return this->record_token_span(this->scan_for_constant(static_cast<char>(next)), token_line, token_start);
                    }
                    // If it's a letter, or underscore
                    if ('_' == next || std::isalpha(next)) {
                        return this->record_token_span(this->scan_for_identifier(static_cast<char>(next)), token_line, token_start);
                    }
            }

            // If nothing matched, return the error Token
            std::string error;
            error.push_back(static_cast<char>(next));
            this->found_error = true;
            return {TokenType::TK_ERROR, error, ErrorType::ET_UNRECOGNISED_CHARACTER, token_line, token_start, 1};
        }

    public:

        /**
         * \brief Construct without an open input file; positions default to zero.
         */
        Tokeniser() = default; // default

        /**
         * \brief Attempt to open an input file and start at line one, column zero.
         *
         * \param file The input file this Tokeniser should convert into Tokens
         */
        explicit Tokeniser(const std::string& file)
        : current_line_number { 1 }, current_char_position { 0 } {
            // Attempt to open the stream, and set is_open
            this->input = std::ifstream(file);
            this->is_open = this->input.is_open();

            // // Set up default values for tokenising
            // this->current_line_number = 1;
            // this->current_char_position = 0;
        }

        /**
         * \brief Check whether the input file opened during construction.
         * \return True if the file opened, otherwise false.
         */
        bool opened() const {
            return this->is_open;
        }

        /**
         * \brief Used to check if an error was found.
         *
         * \return True for an error token or an unterminated block comment.
         */
        bool had_error() const {
            return this->found_error;
        }

        /**
         * \brief Get the list of Errors
         *
         * \return A copy of the collected lexical diagnostics.
         */
        std::list<Error> get_errors() {
            return this->errors;
        }

        /**
         * \brief Scan the remaining input and return the collected tokens.
         *
         * Comment markers are discarded. Error tokens are retained and copied
         * into the diagnostics list; scanning continues after errors and appends
         * an EOF token. Unterminated block comments add a diagnostic separately.
         *
         * \return A copy of the token list, including EOF.
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
                    // add it to the list
                    tokens.push_back(temp);

                    // If this Token was an error, add it to that list also
                    if (temp.get_type() == TokenType::TK_ERROR) {
                        this->errors.emplace_back(temp);
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
