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
#include <utility>

#include "../debug.hpp"
#include "../enums/error-type.hpp"
#include "../enums/token-type.hpp"

/**
 * \brief A token with a type, value, diagnostic reason and source span.
 *
 * Tokeniser spans normally use one-based lines and zero-based byte offsets
 * within a line; lengths count source bytes, not tab-expanded display columns.
 * Positions and length can be zero when no span has been assigned.
 */
class Token {

    private:

        /**
         * \brief What Type of Token this is; one of TokenType
         */
        TokenType type{};

        /**
         * \brief The value this Token has
         */
        std::string value;

        /**
         * \brief If this Token is an error, this will be the reason
         */
        ErrorType reason{ErrorType::ET_NONE};

        /**
         * \brief The line this Token was found on
         *
         * This value is used when returning errors back to stderr;
         * so that the location of the token can more easily be recovered
         */
        int line{};

        /**
         * \brief The position in that line this Token was found
         *
         * This value is used when returning errors back to stderr;
         * so that the location of the token can more easily be recovered
         */
        int position_on_line{};

        /**
         * \brief The length of this Token
         *
         * This value is used when returning errors back to stderr;
         * so that the location of the token can more easily be marked
         */
        int length{};

    public:

        // Constructors

        /**
         * \brief Construct a Token with a value-initialised type, empty value,
         * ET_NONE reason and zero line, position and length.
         */
        Token() = default; // default

        /**
         * \brief Construct a new Token object with a type
         *
         * The value is empty, the reason is ET_NONE and the span is zero.
         *
         * \param type What Type of Token this is
         */
        explicit Token(const TokenType type)
        : type{ type } {}

        /**
         * \brief Construct a new Token object with a type, a value, and position data
         *
         * \param type What Type of Token this is
         * \param value The value this Token has
         * \param line The line number this Token was found on
         * \param position The position of this Token within it's line
         * \param length The length of this Token
         */
        Token(const TokenType type, std::string value, const int line, const int position, const int length)
        : type{ type }, value{std::move( value )}, line{ line }, position_on_line{ position }, length{ length } {}

        /**
         * \brief Construct a new Token object with a type, a value, a reason, and position data
         *
         * Intended for error tokens; the type is not checked.
         *
         * \param type What Type of Token this is
         * \param value The value this Token has
         * \param reason The reason for the error
         * \param line The line number this Token was found on
         * \param position The starting position of this Token within it's line
         * \param length The length of this Token
         */
        Token(const TokenType type, std::string value, const ErrorType reason, const int line, const int position, const int length)
        : type{ type }, value{std::move( value )}, reason{ reason }, line{ line }, position_on_line{ position }, length{ length } {}

        // Accessors

        /**
         * \brief Get the type of this Token
         *
         * \return The type of this Token
         */
        TokenType get_type() const {
            return this->type;
        }

        /**
         * \brief Get the value of this Token
         *
         * \return The value of this Token
         */
        std::string get_value() const {
            return this->value;
        }

        /**
         * \brief Get the reason for this error
         *
         * @return The Token's diagnostic reason
         */
        ErrorType get_reason() const {
            return this->reason;
        }

        /**
         * \brief Set the reason for this error
         *
         * @param error_reason The diagnostic reason to store
         */
        void set_reason(const ErrorType error_reason) {
            this->reason = error_reason;
        }

        /**
         * \brief Get the line number for this Token
         *
         * \return The line number this Token was found on
         */
        int get_line() const {
            return this->line;
        }

        /**
         * \brief Get the position on line for this Token
         *
         * \return The position of this Token within it's line
         */
        int get_position() const {
            return this->position_on_line;
        }

        /**
         * \brief Return the source-byte length of this Token.
         *
         * \return The source-byte length of this Token
         */
        int get_length() const {
            return this->length;
        }

        /**
         * \brief Used to set the position of a Token
         *
         * \param line_number The line this Token was found on
         * \param position The position within that line this Token was found
         */
        void set_token_position(const int line_number, const int position) {
            this->line = line_number;
            this->position_on_line = position;
        }

        /**
         * \brief Set the source length of this Token
         *
         * \param source_length The number of source bytes occupied by this Token
         */
        void set_token_length(const int source_length) {
            this->length = source_length;
        }

        // Helpers

        /**
         * \brief Return a selective debug representation of this Token.
         *
         * Includes the type, the value for identifiers, constants and errors,
         * and the reason for errors. Line and position are included only when
         * DEBUG_PRINT_TOKEN_LINE_POSITIONS is defined; length is not included.
         * An unmapped type or error reason can throw std::out_of_range.
         *
         * \return A string representational of this Token
         */
        std::string to_string() const {
            std::string out = "Token [Type: " + token_string_values.at(this->type);

            switch (this->type) {
                case TokenType::TK_IDENTIFIER:
                case TokenType::TK_CONSTANT: {
                    out += ", Value: " + this->value;
                    break;
                }
                case TokenType::TK_ERROR: {
                    out += ", Value: " + this->value;
                    out += ", Reason: " + error_type_string(this->reason);
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
