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

#include "token-type.hpp"

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
         * \brief The value this Token has; used for TK_IDENTIFIER names, or TK_CONSTANT values
         */
        std::string value;

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

        // ########## Constructors ##########

        /**
         * \brief Default constructor for a new Token object
         */
        Token(void) {}

        /**
         * \brief Construct a new Token object with a type
         *
         * \param type What Type of Token this is; one of TokenType
         */
        Token(TokenType type)
        : type{ type } {}

        /**
         * \brief Construct a new Token object with a type, and a value
         *
         * \param type What Type of Token this is; one of TokenType
         * \param value The value this Token has; used for TK_IDENTIFIER names, or TK_CONSTANT values
         */
        Token(TokenType type, std::string value)
        : type{ type }, value{ value } {}

        // ########## Accessors ##########

        /**
         * \brief Get the type of this Token
         *
         * \return The type of this Token; one of TokenType
         */
        TokenType get_type(void) {
            return this->type;
        }

        /**
         * \brief Get the value of this Token
         *
         * \return The value of this Token; used to retrieve TK_IDENTIFIER names, or TK_CONSTANT values
         */
        const std::string get_value(void) {
            return this->value;
        }

        // ########## Helpers ##########

        /**
         * \brief Returns a string contrining the information related to this Token
         *
         * \return A string represententation of this Token
         */
        const std::string to_string(void) {
            std::string out = "TOKEN [Type: " + token_string_values.at(this->type);

            if (this->type == TokenType::TK_IDENTIFIER) {
                out += ", Value: " + this->value;
            } else if (this->type == TokenType::TK_CONSTANT) {
                out += ", Value: " + this->value;
            }

            return out + "]";
        }

        // ########## Overrides ##########

        // bool operator==(Token &other) {
        //     return (this->type == other.type) && (this->value == other.value);
        // }

        // bool operator!=(Token &other) {
        //     return (this->type != other.type) || (this->value != other.value);
        // }
};

#endif // TOKEN
