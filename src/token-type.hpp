#include <map>
#include <string>

// List of all token types currently supported
enum class TokenType {
    // Error
    TK_ERROR, // Any Error

    // Single-character
    TK_EOF, // EOF

    TK_SEMI_COLON, // ;

    TK_OPEN_PARENTHESIS,  // \(
    TK_CLOSE_PARENTHESIS, // \)
    TK_OPEN_BRACE,        // {
    TK_CLOSE_BRACE,       // }

    // Operators
    TK_TILDE, // ~

    TK_MINUS, // - (is used for both OP_MINUS and OP_NEGATE)

    TK_MINUS_MINUS, // --

    // Values
    TK_CONSTANT, // [0-9]+

    // Keywords
    TK_KEYWORD_INT,    // int
    TK_KEYWORD_VOID,   // void
    TK_KEYWORD_RETURN, // return

    // Identifier
    TK_IDENTIFIER, // [a-zA-Z_]\w*
};

const std::map<TokenType, std::string> type_string = {
    // Error
    { TokenType::TK_ERROR, "ERROR" },

    // Single-character
    { TokenType::TK_EOF, "EOF" },

    { TokenType::TK_SEMI_COLON, "SEMI_COLON" },

    { TokenType::TK_OPEN_PARENTHESIS, "OPEN_PARENTHESIS" },
    { TokenType::TK_CLOSE_PARENTHESIS, "CLOSE_PARENTHESIS" },
    { TokenType::TK_OPEN_BRACE, "OPEN_BRACE" },
    { TokenType::TK_CLOSE_BRACE, "CLOSE_BRACE" },

    // Operators
    { TokenType::TK_TILDE, "TILDE" },

    { TokenType::TK_MINUS, "MINUS" },

    { TokenType::TK_MINUS_MINUS, "MINUS_MINUS" },

    // Values
    { TokenType::TK_CONSTANT, "CONSTANT" },

    // Keywords
    { TokenType::TK_KEYWORD_INT, "KW_INT" },
    { TokenType::TK_KEYWORD_VOID, "KW_VOID" },
    { TokenType::TK_KEYWORD_RETURN, "KW_RETURN" },

    // Identifier
    { TokenType::TK_IDENTIFIER, "IDENTIFIER" },
};
