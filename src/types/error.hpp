/**
 * \file error.hpp
 * \author Gnomeball
 * \brief A file outlining the implementation of the Error class
 * \version 0.1
 * \date 2025-12-21
 */

#ifndef ERROR
#define ERROR

#include <string>

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
        std::string reason;

        /**
         * \brief The line this Error was found on
         */
        int line;

        /**
         * \brief The position in that line this Error was found
         */
        int pos;

    public:

        // Constructors

        /**
         * \brief Construct a new Error object from a Token
         *
         * \param token The Token object to be converted into an Error
         */
        explicit Error(Token token)
        : reason{ token.get_reason() }, line{ token.get_line() }, pos{ token.get_position() } {}

        /**
         * \brief Construct a new Error object from a Byte
         *
         * \param byte The Byte object to be converted into an Error
         */
        explicit Error(Byte byte)
        : reason{ byte.get_reason() }, line{ byte.get_line() }, pos{ byte.get_position() } {}

        // Accessors

        std::string get_reason() {
            return this->reason;
        }

        int get_line() const {
            return this->line;
        }

        int get_pos() const {
            return this->pos;
        }

        // Helpers

        // Overrides
};

#endif // ERROR
