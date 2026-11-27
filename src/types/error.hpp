/**
 * \file error.hpp
 * \author Gnomeball
 * \brief A file outlining the implementation of the Error class
 * \version 0.1
 * \date 2025-12-21
 */

#ifndef ERROR
#define ERROR

#include "../enums/error-type.hpp"
#include "byte.hpp"
#include "token.hpp"

/**
 * \brief A diagnostic reason and source span copied from a Token or Byte.
 *
 * Source lines are normally one-based; start positions are zero-based byte
 * offsets within the line and lengths are in source bytes, not display columns.
 * A source object without position data may have zero-valued positions.
 */
class Error {

    private:

        /**
         * \brief The reason for this being an Error
         */
        ErrorType reason{ErrorType::ET_NONE};

        /**
         * \brief The line this Error was found on
         */
        const int line;

        /**
         * \brief The position in that line this Error starts on
         */
        const int start_pos;

        /**
         * \brief The length of the discovered Error
         */
        const int length;

    public:

        // Constructors

        /**
         * \brief Copy a Token's reason, line, position and length.
         *
         * \param token The Token object to be converted into an Error
         */
        explicit Error(const Token& token)
        : reason{ token.get_reason() }, line{ token.get_line() }, start_pos{ token.get_position() }, length{ token.get_length() } {}

        /**
         * \brief Copy a Byte's reason, line and position.
         *
         * The length is currently fixed at one byte because Byte does not
         * store a span width. This placeholder will need to change when error
         * handling is expanded to cover Byte spans.
         *
         * \param byte The Byte object to be converted into an Error
         */
        explicit Error(const Byte& byte)
        : reason{ byte.get_reason() }, line{ byte.get_line() }, start_pos{ byte.get_position() }, length{ 1 } {}

        // Accessors

        /**
         * \brief Get the reason for this Error
         *
         * @return ErrorType the diagnostic reason for this Error
         */
        ErrorType get_reason() const {
            return this->reason;
        }

        /**
         * \brief Get the line number where this Error was discovered
         *
         * @return int the line number where this Error was discovered
         */
        int get_line() const {
            return this->line;
        }

        /**
         * \brief Get the zero-based source-byte offset on the line.
         *
         * @return int the starting position of this Error
         */
        int get_start_pos() const {
            return this->start_pos;
        }

        /**
         * \brief Get the span length in source bytes.
         *
         * @return int the length of this Error
         */
        int get_length() const {
            return this->length;
        }

        // Helpers

        // Overrides
};

#endif // ERROR
