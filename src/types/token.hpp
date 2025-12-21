/**
 * \file token.hpp
 * \author Gnomeball
 * \brief A file outlining the implementation of the Token class
 * \version 0.1
 * \date 2024-10-21
 */

#ifndef TOKEN
#define TOKEN

#include <string>

#include "../debug.hpp"
#include "../enums/token-type.hpp"

/**
 * \brief A class to outline the Token type
 */
class Token {

    private:

        /**
         * \brief What Type of Token this is; one of TokenType
         */
        TokenType type;

        /**
         * \brief The value this Token has
         */
        std::string value;

        /**
         * \brief If this Token is an error, this will be the reason
         */
        std::string reason;

        /**
         * \brief The line this Token was found on
         *
         * This value is used when returning errors back to stderr;
         * so that the location of the token can more easily be recovered
         */
        int line;

        /**
         * \brief The position in that line this Token was found
         *
         * This value is used when returning errors back to stderr;
         * so that the location of the token can more easily be recovered
         */
        int position_on_line;

    public:

        // Constructors

        /**
         * \brief Default constructor for a new Token object
         */
        Token(void) {} // default

        /**
         * \brief Construct a new Token object with a type
         *
         * \param type What Type of Token this is
         */
        Token(TokenType type)
        : type{ type } {}

        /**
         * \brief Construct a new Token object with a type, and a value
         *
         * \param type What Type of Token this is
         * \param value The value this Token has
         */
        Token(TokenType type, std::string value)
        : type{ type }, value{ value } {}

        /**
         * \brief Construct a new Token object with a type, a value, and a reason
         *
         * //! This should only be used for error tokens
         *
         * \param type What Type of Token this is
         * \param value The value this Token has
         * \param reason The reason for the error
         */
        Token(TokenType type, std::string value, std::string reason)
        : type{ type }, value{ value }, reason{ reason } {}

        /**
         * \brief Construct a new Token object with a type, a value, and a line / position pair
         *
         * \param type What Type of Token this is
         * \param value The value this Token has
         * \param line The line number this Token was found on
         * \param position The position of this Token within it's line
         */
        Token(TokenType type, std::string value, const int line, const int position)
        : type{ type }, value{ value }, line{ line }, position_on_line{ position } {}

        // Accessors

        /**
         * \brief Get the type of this Token
         *
         * \return The type of this Token
         */
        TokenType get_type(void) {
            return this->type;
        }

        /**
         * \brief Get the value of this Token
         *
         * \return The value of this Token
         */
        const std::string get_value(void) {
            return this->value;
        }

        const std::string get_reason(void) {
            return this->reason;
        }

        void set_reason(std::string reason) {
            this->reason = reason;
        }

        /**
         * \brief Get the line number for this Token
         *
         * \return The line number this Token was found on
         */
        int get_line(void) {
            return this->line;
        }

        /**
         * \brief Get the position on line for this Token
         *
         * \return The position of this Token within it's line
         */
        int get_position(void) {
            return this->position_on_line;
        }

        /**
         * \brief Returns the character length of this Token
         *
         * \return The character length of this Token
         */
        int get_length(void) const {
            switch (this->type) {
                case TokenType::TK_OPEN_PARENTHESIS:
                case TokenType::TK_CLOSE_PARENTHESIS:
                case TokenType::TK_OPEN_BRACE:
                case TokenType::TK_CLOSE_BRACE:
                case TokenType::TK_QUESTION:
                case TokenType::TK_COLON:
                case TokenType::TK_COMMA:
                case TokenType::TK_SEMI_COLON:
                case TokenType::TK_PLUS:
                case TokenType::TK_MINUS:
                case TokenType::TK_STAR:
                case TokenType::TK_SLASH:
                case TokenType::TK_PERCENTAGE:
                case TokenType::TK_CARET:
                case TokenType::TK_AMPERSAND:
                case TokenType::TK_PIPE:
                case TokenType::TK_TILDE:
                case TokenType::TK_BANG:
                case TokenType::TK_EQUAL:
                case TokenType::TK_GREATER:
                case TokenType::TK_LESS: {
                    return 1;
                }

                case TokenType::TK_PLUS_PLUS:
                case TokenType::TK_MINUS_MINUS:
                case TokenType::TK_AMPE_AMPE:
                case TokenType::TK_PIPE_PIPE:
                case TokenType::TK_BANG_EQUAL:
                case TokenType::TK_EQUAL_EQUAL:
                case TokenType::TK_GREATER_EQUAL:
                case TokenType::TK_LESS_EQUAL:
                case TokenType::TK_PLUS_EQUAL:
                case TokenType::TK_MINUS_EQUAL:
                case TokenType::TK_STAR_EQUAL:
                case TokenType::TK_SLASH_EQUAL:
                case TokenType::TK_PERCENTAGE_EQUAL:
                case TokenType::TK_AMPERSAND_EQUAL:
                case TokenType::TK_PIPE_EQUAL:
                case TokenType::TK_CARET_EQUAL:
                case TokenType::TK_LEFT_CHEVRONS:
                case TokenType::TK_RIGHT_CHEVRONS: {
                    return 2;
                }

                case TokenType::TK_LEFT_CHEVRONS_EQUAL:
                case TokenType::TK_RIGHT_CHEVRONS_EQUAL: {
                    return 3;
                }

                case TokenType::TK_KEYWORD_IF: return 2;
                case TokenType::TK_KEYWORD_ELSE: return 4;
                case TokenType::TK_KEYWORD_SWITCH: return 6;
                case TokenType::TK_KEYWORD_CASE: return 4;
                case TokenType::TK_KEYWORD_DEFAULT: return 7;
                case TokenType::TK_KEYWORD_GOTO: return 4;
                case TokenType::TK_KEYWORD_DO: return 2;
                case TokenType::TK_KEYWORD_FOR: return 3;
                case TokenType::TK_KEYWORD_WHILE: return 5;
                case TokenType::TK_KEYWORD_BREAK: return 5;
                case TokenType::TK_KEYWORD_CONTINUE: return 8;
                case TokenType::TK_KEYWORD_INT: return 3;
                case TokenType::TK_KEYWORD_VOID: return 4;
                case TokenType::TK_KEYWORD_STATIC: return 6;
                case TokenType::TK_KEYWORD_EXTERN: return 6;
                case TokenType::TK_KEYWORD_RETURN: return 6;

                case TokenType::TK_CONSTANT:
                case TokenType::TK_IDENTIFIER: {
                    return this->value.length();
                }

                case TokenType::TK_ERROR: return this->value.size();
                case TokenType::TK_EOF: return 0;

                default: return 0;
            }
        }

        /**
         * \brief Used to set the position of a Token
         *
         * \param line The line this Token was found on
         * \param position The position within that line this Token was found
         */
        void set_token_position(const int line, const int position) {
            this->line = line;
            this->position_on_line = position;
        }

        // Helpers

        /**
         * \brief Returns a string containing the information related to this Token
         *
         * \return A string represententation of this Token
         */
        const std::string to_string(void) {
            std::string out = "Token [Type: " + token_string_values.at(this->type);

            switch (this->type) {
                case TokenType::TK_IDENTIFIER:
                case TokenType::TK_CONSTANT: {
                    out += ", Value: " + this->value;
                    break;
                }
                case TokenType::TK_ERROR: {
                    out += ", Value: " + this->value;
                    out += ", Reason: " + this->reason;
                    break;
                }
                default: break;
            }

#ifdef DEBUG_PRINT_TOKEN_LINE_POSITIONS
            out += ", line: " + std::to_string(this->line);
            out += ", pos: " + std::to_string(this->position_on_line);
#endif

            return out + "]";
        }

        // Overrides

        // bool operator==(Token &other) {
        //     return (this->type == other.type) && (this->value == other.value);
        // }

        // bool operator!=(Token &other) {
        //     return (this->type != other.type) || (this->value != other.value);
        // }
};

#endif // TOKEN
