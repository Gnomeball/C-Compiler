/**
 * \file error.hpp
 * \author Gnomeball
 * \brief A file outlining the implementation of the Error class
 * \version 0.1
 * \date 2025-12-21
 */

#ifndef ERROR
#define ERROR

#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "token.hpp"

/**
 * \brief A class to outline the Assembly type
 */
class Error {

    private:

        /**
         * \brief The value this Error has
         */
        std::string value;

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

        /**
         * \brief The raw text of the line this Error was found on
         */
        std::string line_text;

        // not sure if this is really the best way
        std::string red = "\x1b[1;31m";
        std::string purple = "\x1b[1;32m";
        std::string blue = "\x1b[1;36m";
        std::string reset = "\x1b[1;0m";

        // Helpers

        std::string text_red(std::string text) {
            return red + text + reset;
        }

        std::string text_purple(std::string text) {
            return purple + text + reset;
        }

        std::string text_blue(std::string text) {
            return blue + text + reset;
        }

    public:

        // Constructors

        /**
         * \brief The default constructor for an Error object
         */
        Error(void) {} // default

        Error(Token error)
        : value{ error.get_value() }, reason{ error.get_reason() },
          line{ error.get_line() }, pos{ error.get_position() } {}

        /**
         * \brief Construct a new Error object with all it's data
         *
         * \param value The value that caused the error
         * \param reason The reason for the Error
         * \param line The line the Error was found on
         * \param pos The position in that line the Error was found
         */
        Error(std::string value, std::string reason, int line, int pos)
        : value{ value }, reason{ reason }, line{ line }, pos{ pos } {}

        // Accessors

        int get_line(void) {
            return this->line;
        }

        /**
         * \brief Set the raw line text for this Error
         *
         * //! not the best way to do this, to_string needs moving out to the error handler
         *
         * \param text The raw line text
         */
        void set_line_text(std::string text) {
            this->line_text = text;
        }

        // Helpers

        /**
         * \brief Returns a string containing the information related to this Error
         *
         * \return A string represententation of this Error
         */
        const std::string to_string(void) {
            std::stringstream out;

            out << text_red("Error") << " | " << text_blue(this->reason) << std::endl;
            out << std::setw(7) << "|" << std::endl;
            out << std::setw(5) << this->line << " | " << this->line_text << std::endl;
            out << "      | " << std::setw(this->pos) << " " << text_purple("^") << std::endl;

            return out.str();
        }

        // Overrides
};

#endif // ERROR
