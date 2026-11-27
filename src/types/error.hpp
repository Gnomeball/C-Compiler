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
 * \brief A class to outline the Assembly type
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
         * \brief Construct a new Error object from a Token
         *
         * \param token The Token object to be converted into an Error
         */
        explicit Error(const Token& token)
        : reason{ token.get_reason() }, line{ token.get_line() }, start_pos{ token.get_position() }, length{ token.get_length() } {}

        /**
         * \brief Construct a new Error object from a Byte
         *
         * \param byte The Byte object to be converted into an Error
         */
        explicit Error(const Byte& byte)
        : reason{ byte.get_reason() }, line{ byte.get_line() }, start_pos{ byte.get_position() }, length{ 1 } {}

        // Accessors

        ErrorType get_reason() const {
            return this->reason;
        }

        int get_line() const {
            return this->line;
        }

        int get_start_pos() const {
            return this->start_pos;
        }

        int get_length() const {
            return this->length;
        }

        // Helpers

        // Overrides
};

#endif // ERROR
