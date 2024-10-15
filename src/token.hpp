/*
 * In this file we define; the Token class, the internal function of the
 * Token class, as well as a list of all currently supported Token types
 */

#ifndef TOKEN
#define TOKEN

#include <string>

#include "token-type.hpp"

// Class to define a Token, as well as it's function
class Token {

    private:

        // TODO: Store location of the token within the token, line and position, so that errors during parsing can use this information

        TokenType type;

        int int_value;
        std::string string_value;


    public:

        // Constructors

        Token(void) {} // default

        Token(TokenType type)
        : type{ type } {}

        Token(TokenType type, int value)
        : type{ type }, int_value{ value } {}

        Token(TokenType type, std::string value)
        : type{ type }, string_value{ value } {}

        // Accessors

        TokenType get_type(void) {
            return this->type;
        }

        int get_int_value(void) {
            return this->int_value;
        }

        const std::string get_string_value(void) {
            return this->string_value;
        }

        // Helpers

        const std::string to_string(void) {
            std::string out = "TOKEN [Type: " + type_string.at(this->type);

            if (this->type == TokenType::TK_IDENTIFIER) {
                out += ", Value: " + this->string_value;
            } else if (this->type == TokenType::TK_CONSTANT) {
                out += ", Value: " + std::to_string(this->int_value);
            }

            return out + "]";
        }

        // Overrides

        bool operator==(Token &other) {
            return (this->type == other.type) && (this->int_value == other.int_value) &&
            (this->string_value == other.string_value);
        }
};

#endif
