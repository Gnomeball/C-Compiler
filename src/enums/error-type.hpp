/**
* \file error-type.hpp
 * \author Gnomeball
 * \brief A file listing all currently considered error types
 * \version 0.1
 * \date 2026-11-26
 */

#ifndef ERROR_TYPE
#define ERROR_TYPE

#include <map>
#include <string>

/**
 * \brief An enumeration of the diagnostic reasons used by the compiler
 */
enum class ErrorType : int {
    ET_NONE,

    ET_UNRECOGNISED_CHARACTER,

    ET_UNSUPPORTED_NUMERIC_LITERAL,
    ET_INVALID_OCTAL_LITERAL,

    ET_UNTERMINATED_BLOCK_COMMENT,

    ET_EXPECTED_INT_KEYWORD,
    ET_EXPECTED_RETURN_KEYWORD,

    ET_EXPECTED_EXPRESSION,

    ET_EXPECTED_SEMICOLON,
    ET_EXPECTED_OPEN_PARENTHESIS,
    ET_EXPECTED_CLOSE_PARENTHESIS,
    ET_EXPECTED_OPEN_BRACE,
    ET_EXPECTED_CLOSE_BRACE,
};

/**
 * \brief A map of all ErrorTypes to their diagnostic reasons
 */
const std::map<ErrorType, std::string> error_string_values = {
    { ErrorType::ET_NONE, "" },

    { ErrorType::ET_UNRECOGNISED_CHARACTER, "Unrecognised Character" },

    { ErrorType::ET_UNSUPPORTED_NUMERIC_LITERAL, "Unsupported Numeric Literal" },
    { ErrorType::ET_INVALID_OCTAL_LITERAL, "Invalid Octal Literal" },

    { ErrorType::ET_UNTERMINATED_BLOCK_COMMENT, "Unterminated Block Comment" },

    { ErrorType::ET_EXPECTED_INT_KEYWORD, "Expected int keyword" },
    { ErrorType::ET_EXPECTED_RETURN_KEYWORD, "Expected return keyword" },

    { ErrorType::ET_EXPECTED_EXPRESSION, "Expected Expression" },

    { ErrorType::ET_EXPECTED_SEMICOLON, "Expected ';'" },
    { ErrorType::ET_EXPECTED_OPEN_PARENTHESIS, "Expected '('" },
    { ErrorType::ET_EXPECTED_CLOSE_PARENTHESIS, "Expected ')'" },
    { ErrorType::ET_EXPECTED_OPEN_BRACE, "Expected '{'" },
    { ErrorType::ET_EXPECTED_CLOSE_BRACE, "Expected '}'" },
};

inline const std::string& error_type_string(const ErrorType error_type) {
    return error_string_values.at(error_type);
}



#endif // ERROR_TYPE
