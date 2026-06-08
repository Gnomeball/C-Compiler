/**
 * \file byte.hpp
 * \author Gnomeball
 * \brief A file outlining the implementation of the Byte class
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef BYTE
#define BYTE

#include <string>
#include <utility>

#include "../enums/op-codes.hpp"

/**
 * \brief A class to outline the Byte type
 */
class Byte {

    private:

        /**
         * \brief The OpCode this Byte carries
         */
        OpCode op;

        /**
         * \brief The value this Byte carries
         */
        std::string value;

        /**
         * \brief If this Byte is an error, this will be the reason
         */
        std::string reason;

        /**
         * \brief The line this Byte was found on
         *
         * This value is used when returning errors back to stderr;
         * so that the location of the byte can more easily be recovered
         */
        int line{};

        /**
         * \brief The position in that line this Byte was found
         *
         * This value is used when returning errors back to stderr;
         * so that the location of the byte can more easily be recovered
         */
        int position_on_line{};

    public:

        // Constructors

        /**
         * \brief Default constructor for a new Byte object
         */
        Byte() = default; // default

        /**
         * \brief Construct a new Bype object with an OpCode
         *
         * \param op Which OpCode this Byte carries
         */
        Byte(const OpCode op)
        : op{ op } {}

        /**
         * \brief Construct a new Byte object with an OpCode, and a Value
         *
         * \param op Which OpCode this Byte carries
         * \param value The value this Byte carries
         */
        Byte(OpCode op, std::string value)
        : op{ op }, value{std::move( value )} {}

        /**
         * \brief Construct a new Byte object with an OpCode, a Value, and a Reason
         *
         * //! This should only be used for error bytes
         *
         * \param op Which OpCode this Byte carries
         * \param value The value this Byte carries
         * \param reason The reason this Byte is an error
         */
        Byte(OpCode op, std::string value, std::string reason)
        : op{ op }, value{std::move( value )}, reason {std::move( reason )} {}

        // Accessors

        /**
         * \brief Get the OpCode of this Byte
         *
         * \return The OpCode of the Byte
         */
        OpCode get_op() const {
            return this->op;
        }

        /**
         * \brief Get the value of this Byte
         *
         * \return The value of the Byte
         */
        std::string get_value() {
            return this->value;
        }

        /**
         * \brief Get the reason for this error
         *
         * @return The reason this Byte is an error
         */
        std::string get_reason() {
            return this->reason;
        }

        /**
         * \brief Set the reason for this error
         *
         * @param error_reason Set the reason this Byte is an error
         */
        void set_reason(std::string error_reason) {
            this->reason = std::move(error_reason);
        }

        /**
         * \brief Get the line number for this Byte
         *
         * \return The line number this Byte was found on
         */
        int get_line() const {
            return this->line;
        }

        /**
         * \brief Get the position on line for this Byte
         *
         * \return The position of this Byte within it's line
         */
        int get_position() const {
            return this->position_on_line;
        }

        /**
         * \brief Used to set the position of a Byte
         *
         * \param line The line this Byte was found on
         * \param position The position within that line this Byte was found
         */
        void set_byte_position(const int line, const int position) {
            this->line = line;
            this->position_on_line = position;
        }

        // Helpers

        /**
         * \brief Returns a string containing the information related to this Byte
         *
         * \return A string representational of this Byte
         */
        std::string to_string() const {
            std::string out = "Byte [Op: " + op_code_string.at(this->op);

            if (this->op == OpCode::OP_FUNCTION) {
                out += ", Identifier: " + this->value;
            } else if (this->op == OpCode::OP_CONSTANT) {
                out += ", Value: " + this->value;
            } else if (this->op == OpCode::OP_ERROR) {
                out += ", Error: " + this->value;
            }

            return out + "]";
        }

        // Overrides
};

#endif // BYTE
