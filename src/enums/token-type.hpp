/**
 * \file token-type.hpp
 * \author Gnomeball
 * \brief A file listing listing all currently supported Token types
 * \version 0.1
 * \date 2024-10-21
 */

#ifndef TOKEN_TYPE
#define TOKEN_TYPE

#include <map>
#include <string>

/**
 * \brief An enumeration of all currently supported TokenType
 */
enum class TokenType : int {
    // Single-character tokens
    TK_OPEN_PARENTHESIS,  //!< (
    TK_CLOSE_PARENTHESIS, //!< )

    TK_OPEN_BRACE,  //!< {
    TK_CLOSE_BRACE, //!< }

    TK_QUESTION, //!< ?
    TK_COLON,    //!< :

    TK_COMMA, //!< ,

    TK_SEMI_COLON, //!< ;

    // Operators
    TK_PLUS,  //!< \+
    TK_MINUS, //!< \-
    TK_STAR,  //!< Asterisk
    TK_SLASH, //!< /

    TK_PERCENTAGE, //!< %
    TK_CARET,      //!< ^
    TK_AMPERSAND,  //!< &
    TK_PIPE,       //!< |

    TK_TILDE, //!< ~

    TK_BANG,    //!< !
    TK_EQUAL,   //!< =
    TK_GREATER, //!< \>
    TK_LESS,    //!< <

    // Multiple-character tokens
    TK_PLUS_PLUS,   //!< ++
    TK_MINUS_MINUS, //!< \--

    TK_AMPE_AMPE, //!< &&
    TK_PIPE_PIPE, //!< ||

    TK_BANG_EQUAL,    //!< !=
    TK_EQUAL_EQUAL,   //!< ==
    TK_GREATER_EQUAL, //!< >=
    TK_LESS_EQUAL,    //!< <=

    TK_PLUS_EQUAL,  //!< +=
    TK_MINUS_EQUAL, //!< -=
    TK_STAR_EQUAL,  //!< *=
    TK_SLASH_EQUAL, //!< /=

    TK_PERCENTAGE_EQUAL, //!< %=
    TK_AMPERSAND_EQUAL,  //!< &=
    TK_PIPE_EQUAL,       //!< |=
    TK_CARET_EQUAL,      //!< ^=

    TK_LEFT_CHEVRONS,  //!< <<
    TK_RIGHT_CHEVRONS, //!< \>>

    TK_LEFT_CHEVRONS_EQUAL,  //!< <<=
    TK_RIGHT_CHEVRONS_EQUAL, //!< \>>=

    // Keywords
    TK_KEYWORD_IF,   //!< if
    TK_KEYWORD_ELSE, //!< else

    TK_KEYWORD_SWITCH,  //!< switch
    TK_KEYWORD_CASE,    //!< case
    TK_KEYWORD_DEFAULT, //!< default

    TK_KEYWORD_GOTO, //!< goto

    TK_KEYWORD_DO,    //!< do
    TK_KEYWORD_FOR,   //!< for
    TK_KEYWORD_WHILE, //!< while

    TK_KEYWORD_BREAK,    //!< break
    TK_KEYWORD_CONTINUE, //!< continue

    TK_KEYWORD_INT,  //!< int
    TK_KEYWORD_VOID, //!< void

    TK_KEYWORD_STATIC, //!< static
    TK_KEYWORD_EXTERN, //!< extern

    TK_KEYWORD_RETURN, //!< return

    // Values
    TK_CONSTANT, //!< [0-9]+

    // Identifier
    TK_IDENTIFIER, //!< [a-zA-Z_]\\w*

    // Error
    TK_ERROR, //!< Any Error

    // EOF
    TK_EOF, //!< EOF
};

/**
 * \brief A map of all TokenTypes to their string representations
 *
 * This is used when printing out Token information to stdout
 */
const std::map<TokenType, std::string> token_string_values = {
    // Single-character tokens
    { TokenType::TK_OPEN_PARENTHESIS, "TK_OPEN_PARENTHESIS" },   //!< (
    { TokenType::TK_CLOSE_PARENTHESIS, "TK_CLOSE_PARENTHESIS" }, //!< )

    { TokenType::TK_OPEN_BRACE, "TK_OPEN_BRACE" },   //!< {
    { TokenType::TK_CLOSE_BRACE, "TK_CLOSE_BRACE" }, //!< }

    { TokenType::TK_QUESTION, "TK_QUESTION" }, //!< ?
    { TokenType::TK_COLON, "TK_COLON" },       //!< :

    { TokenType::TK_COMMA, "TK_SEMI_COMMA" }, //!< ,

    { TokenType::TK_SEMI_COLON, "TK_SEMI_COLON" }, //!< ;

    // Operators
    { TokenType::TK_PLUS, "TK_PLUS" },   //!< \+
    { TokenType::TK_MINUS, "TK_MINUS" }, //!< \-
    { TokenType::TK_STAR, "TK_STAR" },   //!< Asterisk
    { TokenType::TK_SLASH, "TK_SLASH" }, //!< /

    { TokenType::TK_PERCENTAGE, "TK_PERCENTAGE" }, //!< %
    { TokenType::TK_CARET, "TK_CARET" },           //!< ^
    { TokenType::TK_AMPERSAND, "TK_AMPERSAND" },   //!< &
    { TokenType::TK_PIPE, "TK_PIPE" },             //!< |

    { TokenType::TK_TILDE, "TK_TILDE" }, //!< ~

    { TokenType::TK_EQUAL, "TK_EQUAL" },     //!< =
    { TokenType::TK_BANG, "TK_BANG" },       //!< !
    { TokenType::TK_GREATER, "TK_GREATER" }, //!< \>
    { TokenType::TK_LESS, "TK_LESS" },       //!< <

    // Multiple-character tokens
    { TokenType::TK_PLUS_PLUS, "TK_PLUS_PLUS" },     //!< ++
    { TokenType::TK_MINUS_MINUS, "TK_MINUS_MINUS" }, //!< \--

    { TokenType::TK_AMPE_AMPE, "TK_AMPE_AMPE" }, //!< &&
    { TokenType::TK_PIPE_PIPE, "TK_PIPE_PIPE" }, //!< ||

    { TokenType::TK_BANG_EQUAL, "TK_BANG_EQUAL" },       //!< !=
    { TokenType::TK_EQUAL_EQUAL, "TK_EQUAL_EQUAL" },     //!< ==
    { TokenType::TK_GREATER_EQUAL, "TK_GREATER_EQUAL" }, //!< >=
    { TokenType::TK_LESS_EQUAL, "TK_LESS_EQUAL" },       //!< <=

    { TokenType::TK_PLUS_EQUAL, "TK_PLUS_EQUAL" },   //!< +=
    { TokenType::TK_MINUS_EQUAL, "TK_MINUS_EQUAL" }, //!< -=
    { TokenType::TK_STAR_EQUAL, "TK_STAR_EQUAL" },   //!< *=
    { TokenType::TK_SLASH_EQUAL, "TK_SLASH_EQUAL" }, //!< /=

    { TokenType::TK_PERCENTAGE_EQUAL, "TK_PERCENTAGE_EQUAL" }, //!< %=
    { TokenType::TK_AMPERSAND_EQUAL, "TK_AMPERSAND_EQUAL" },   //!< &=
    { TokenType::TK_PIPE_EQUAL, "TK_PIPE_EQUAL" },             //!< |=
    { TokenType::TK_CARET_EQUAL, "TK_CARET_EQUAL" },           //!< ^=

    { TokenType::TK_LEFT_CHEVRONS, "TK_LEFT_CHEVRONS" },   //!< <<
    { TokenType::TK_RIGHT_CHEVRONS, "TK_RIGHT_CHEVRONS" }, //!< \>>

    { TokenType::TK_LEFT_CHEVRONS_EQUAL, "TK_LEFT_CHEVRONS_EQUAL" },   //!< <<=
    { TokenType::TK_RIGHT_CHEVRONS_EQUAL, "TK_RIGHT_CHEVRONS_EQUAL" }, //!< \>>=

    // Keywords
    { TokenType::TK_KEYWORD_IF, "TK_KEYWORD_IF" },     //!< if
    { TokenType::TK_KEYWORD_ELSE, "TK_KEYWORD_ELSE" }, //!< else

    { TokenType::TK_KEYWORD_SWITCH, "TK_KEYWORD_SWITCH" },   //!< switch
    { TokenType::TK_KEYWORD_CASE, "TK_KEYWORD_CASE" },       //!< case
    { TokenType::TK_KEYWORD_DEFAULT, "TK_KEYWORD_DEFAULT" }, //!< default

    { TokenType::TK_KEYWORD_GOTO, "TK_KEYWORD_GOTO" }, //!< goto

    { TokenType::TK_KEYWORD_DO, "TK_KEYWORD_DO" },       //!< do
    { TokenType::TK_KEYWORD_FOR, "TK_KEYWORD_FOR" },     //!< for
    { TokenType::TK_KEYWORD_WHILE, "TK_KEYWORD_WHILE" }, //!< while

    { TokenType::TK_KEYWORD_BREAK, "TK_KEYWORD_BREAK" },       //!< break
    { TokenType::TK_KEYWORD_CONTINUE, "TK_KEYWORD_CONTINUE" }, //!< continue

    { TokenType::TK_KEYWORD_INT, "TK_KEYWORD_INT" },   //!< int
    { TokenType::TK_KEYWORD_VOID, "TK_KEYWORD_VOID" }, //!< void

    { TokenType::TK_KEYWORD_STATIC, "TK_KEYWORD_STATIC" }, //!< static
    { TokenType::TK_KEYWORD_EXTERN, "TK_KEYWORD_EXTERN" }, //!< extern

    { TokenType::TK_KEYWORD_RETURN, "TK_KEYWORD_RETURN" }, //!< return

    // Values
    { TokenType::TK_CONSTANT, "TK_CONSTANT" }, //!< [0-9]+

    // Identifier
    { TokenType::TK_IDENTIFIER, "TK_IDENTIFIER" }, //!< [a-zA-Z_]\\w*

    // Error
    { TokenType::TK_ERROR, "TK_ERROR" }, //!< Any Error

    // EOF
    { TokenType::TK_EOF, "TK_EOF" }, //!< EOF
};

/**
 * \brief A map of all keywords to their Token representation
 *
 * This is used when checking if a found identifier is actually a keyword
 */
const std::map<std::string, TokenType> identifiers_to_token = {
    // Keywords
    { "if", TokenType::TK_KEYWORD_IF },     //!< if
    { "else", TokenType::TK_KEYWORD_ELSE }, //!< else

    { "switch", TokenType::TK_KEYWORD_SWITCH },   //!< switch
    { "case", TokenType::TK_KEYWORD_CASE },       //!< case
    { "default", TokenType::TK_KEYWORD_DEFAULT }, //!< default

    { "goto", TokenType::TK_KEYWORD_GOTO }, //!< goto

    { "do", TokenType::TK_KEYWORD_DO },       //!< do
    { "for", TokenType::TK_KEYWORD_FOR },     //!< for
    { "while", TokenType::TK_KEYWORD_WHILE }, //!< while

    { "break", TokenType::TK_KEYWORD_BREAK },       //!< break
    { "continue", TokenType::TK_KEYWORD_CONTINUE }, //!< continue

    { "int", TokenType::TK_KEYWORD_INT },   //!< int
    { "void", TokenType::TK_KEYWORD_VOID }, //!< void

    { "static", TokenType::TK_KEYWORD_STATIC }, //!< static
    { "extern", TokenType::TK_KEYWORD_EXTERN }, //!< extern

    { "return", TokenType::TK_KEYWORD_RETURN }, //!< return
};

#endif // TOKEN_TYPE
