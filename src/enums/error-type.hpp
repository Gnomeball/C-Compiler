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
    ET_NONE, ///< No diagnostic reason; maps to an empty string.

    ET_UNRECOGNISED_CHARACTER, ///< A character cannot be recognised as a token.

    ET_UNSUPPORTED_NUMERIC_LITERAL, ///< A numeric literal has an unsupported form or value.
    ET_INVALID_OCTAL_LITERAL, ///< An octal literal contains a digit outside 0-7.

    ET_UNTERMINATED_BLOCK_COMMENT, ///< A block comment reaches EOF without a closing */.

    ET_EXPECTED_IDENTIFIER, ///< Expected an identifier.
    ET_EXPECTED_INT_KEYWORD, ///< Expected the int keyword at a function declaration.
    ET_EXPECTED_RETURN_KEYWORD, ///< Expected the return keyword in a function body.

    ET_EXPECTED_EXPRESSION, ///< Expected an expression or expression operand.

    ET_EXPECTED_SEMICOLON, ///< Expected ';' after a return expression.
    ET_EXPECTED_OPEN_PARENTHESIS, ///< Expected '(' in a function declaration.
    ET_EXPECTED_CLOSE_PARENTHESIS, ///< Expected ')' after parameters or an expression.
    ET_EXPECTED_OPEN_BRACE, ///< Expected '{' at the start of a function body.
    ET_EXPECTED_CLOSE_BRACE, ///< Expected '}' at the end of a function body.

    ET_UNEXPECTED_TOKEN_AFTER_FUNCTION, ///< Unexpected token after function declaration
};

/**
 * \brief A map of all ErrorTypes to their diagnostic reasons
 *
 * This is used when printing out error information to stderr
 */
const std::map<ErrorType, std::string> error_string_values = {
    { ErrorType::ET_NONE, "" },

    { ErrorType::ET_UNRECOGNISED_CHARACTER, "Unrecognised Character" },

    { ErrorType::ET_UNSUPPORTED_NUMERIC_LITERAL, "Unsupported Numeric Literal" },
    { ErrorType::ET_INVALID_OCTAL_LITERAL, "Invalid Octal Literal" },

    { ErrorType::ET_UNTERMINATED_BLOCK_COMMENT, "Unterminated Block Comment" },

    { ErrorType::ET_EXPECTED_IDENTIFIER, "Expected Identifier" },
    { ErrorType::ET_EXPECTED_INT_KEYWORD, "Expected int keyword" },
    { ErrorType::ET_EXPECTED_RETURN_KEYWORD, "Expected return keyword" },

    { ErrorType::ET_EXPECTED_EXPRESSION, "Expected Expression" },

    { ErrorType::ET_EXPECTED_SEMICOLON, "Expected ';'" },
    { ErrorType::ET_EXPECTED_OPEN_PARENTHESIS, "Expected '('" },
    { ErrorType::ET_EXPECTED_CLOSE_PARENTHESIS, "Expected ')'" },
    { ErrorType::ET_EXPECTED_OPEN_BRACE, "Expected '{'" },
    { ErrorType::ET_EXPECTED_CLOSE_BRACE, "Expected '}'" },

    { ErrorType::ET_UNEXPECTED_TOKEN_AFTER_FUNCTION, "Unexpected token after function declaration" }
};

/**
 * \brief Get the diagnostic reason for the given ErrorType
 *
 * \param error_type The diagnostic reason to look up
 *
 * \return A reference to the corresponding string in error_string_values
 *
 * \throws std::out_of_range if the value is not in the map
 */
inline const std::string& error_type_string(const ErrorType error_type) {
    return error_string_values.at(error_type);
}

#endif // ERROR_TYPE
