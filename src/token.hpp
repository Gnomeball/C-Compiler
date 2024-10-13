/*
 * In this file we define; the Token class, the internal function of the
 * Token class, as well as a list of all currently supported Token types
 */

#ifndef TOKEN
#define TOKEN

#include <ostream>
#include <string>

// List of all token types currently supported
enum class TokenType {
    // Error
    TK_ERROR,                //* Any Error

    // Single-character
    TK_EOF,                  //* EOF

    TK_SEMI_COLON,           //* ;

    TK_OPEN_PARENTHESIS,     //* \(
    TK_CLOSE_PARENTHESIS,    //* \)
    TK_OPEN_BRACE,           //* {
    TK_CLOSE_BRACE,          //* }

    // Values
    TK_CONSTANT,             //* [0-9]+\b

    // Keywords
    TK_KEYWORD_INT,          //* int\b
    TK_KEYWORD_VOID,         //* void\b
    TK_KEYWORD_RETURN,       //* return\b

    // Identifier
    TK_IDENTIFIER,           //* [a-zA-Z_]\w*\b
};

// Class to define a Token, as well as it's function
class Token {

    private:

        TokenType type;

        int int_value;
        std::string string_value;

        // Probably a better way to do this, but at least it's fast
        std::string type_string(void) {
            switch (this->type) {
                case TokenType::TK_ERROR:               return "ERROR";
                case TokenType::TK_EOF:                 return "EOF";
                case TokenType::TK_SEMI_COLON:          return "SEMI_COLON";
                case TokenType::TK_OPEN_PARENTHESIS:    return "OPEN_PARENTHESIS";
                case TokenType::TK_CLOSE_PARENTHESIS:   return "CLOSE_PARENTHESIS";
                case TokenType::TK_OPEN_BRACE:          return "OPEN_BRACE";
                case TokenType::TK_CLOSE_BRACE:         return "CLOSE_BRACE";
                case TokenType::TK_CONSTANT:            return "CONSTANT";
                case TokenType::TK_KEYWORD_INT:         return "KW_INT";
                case TokenType::TK_KEYWORD_VOID:        return "KW_VOID";
                case TokenType::TK_KEYWORD_RETURN:      return "KW_RETURN";
                case TokenType::TK_IDENTIFIER:          return "IDENTIFIER";
                default: return "UNKNOWN"; break;
            }
        }

    public:

        // Constructors

        Token(void) {} // default

        Token(TokenType type) :type{type} {}

        Token(TokenType type, int value) :type{type}, int_value{value} {}

        Token(TokenType type, std::string value) :type{type}, string_value{value} {}

        // Accessors

        TokenType getType(void) { return this->type; }

        int getIntValue(void) { return this->int_value; }

        std::string getStringValue(void) { return this->string_value; }

        // Helpers

        std::string to_string(void) {
            std::string out = "TOKEN [Type: " + this->type_string();

            if (this->type == TokenType::TK_IDENTIFIER) {
                out += ", Value: "+ this->string_value;
            } else if (this->type == TokenType::TK_CONSTANT) {
                out += ", Value: "+ std::to_string(this->int_value);
            }

            return out + "]";
        }

        // Overrides

        bool operator==(Token &other) {
            return (this->type == other.type) && (this->int_value == other.int_value) && (this->string_value == other.string_value);
        }

        std::ostream& operator<<(std::ostream &os) {
            return os << this->to_string();
        }

};

#endif
