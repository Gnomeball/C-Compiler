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

        std::string value;


    public:

        // Constructors

        Token(void) {} // default

        Token(TokenType type)
        : type{ type } {}

        Token(TokenType type, std::string value)
        : type{ type }, value{ value } {}

        // Accessors

        TokenType get_type(void) {
            return this->type;
        }

        const std::string get_value(void) {
            return this->value;
        }

        // Helpers

        const std::string to_string(void) {
            std::string out = "TOKEN [Type: " + type_string.at(this->type);

            if (this->type == TokenType::TK_IDENTIFIER) {
                out += ", Value: " + this->value;
            } else if (this->type == TokenType::TK_CONSTANT) {
                out += ", Value: " + this->value;
            }

            return out + "]";
        }

        // Overrides

        bool operator==(Token &other) {
            return (this->type == other.type) && (this->value == other.value);
        }
};

#endif
