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

#include "../enums/error-type.hpp"
#include "../enums/op-codes.hpp"

/**
 * \brief A parser bytecode record containing an opcode and optional value.
 *
 * See grammar/byte.md for operation and value conventions. Position data
 * normally uses a one-based line and zero-based source-byte offset, not a
 * tab-expanded display column; zero may mean no position was assigned.
 * Byte does not currently store a span width. Errors constructed from it use
 * a one-byte placeholder, which will need to change when Byte error handling
 * is expanded to cover spans.
 */
class Byte {

    private:

        /**
         * \brief The OpCode this Byte carries
         */
        const OpCode op;

        /**
         * \brief The value this Byte carries
         */
        const std::string value;

        /**
         * \brief If this Byte is an error, this will be the reason
         */
        ErrorType reason{ ErrorType::ET_NONE };

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
         * \brief Default construction is disabled; an opcode is required.
         */
        Byte() = delete; // default

        /**
         * \brief Construct a Byte with an opcode and an empty value.
         *
         * The reason is ET_NONE and the line and position are zero.
         *
         * \param op Which OpCode this Byte carries
         */
        Byte(const OpCode op)
        : op{ op } {}

        /**
         * \brief Construct a new Byte object with an OpCode, and a Value
         *
         * The reason is ET_NONE and the line and position are zero.
         *
         * \param op Which OpCode this Byte carries
         * \param value The value this Byte carries
         */
        Byte(const OpCode op, std::string value)
        : op{ op }, value{std::move( value )} {}

        /**
         * \brief Construct a new Byte object with an OpCode, a Value, and a Reason
         *
         * Intended for error bytes; the opcode is not checked. The line and
         * position initially default to zero.
         *
         * \param op Which OpCode this Byte carries
         * \param value The value this Byte carries
         * \param reason The reason this Byte is an error
         */
        Byte(const OpCode op, std::string value, const ErrorType reason)
        : op{ op }, value{std::move( value )}, reason{ reason } {}

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
        std::string get_value() const {
            return this->value;
        }

        /**
         * \brief Get the reason for this error
         *
         * @return The reason this Byte is an error
         */
        ErrorType get_reason() const {
            return this->reason;
        }

        /**
         * \brief Set the reason for this error
         *
         * @param error_reason Set the reason this Byte is an error
         */
        void set_reason(const ErrorType error_reason) {
            this->reason = error_reason;
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
         * \brief Get the zero-based source-byte offset on the line.
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
         * \brief Return a selective debug representation of this Byte.
         *
         * Includes the opcode and, for function, constant and error opcodes,
         * the value. It omits the reason and position. An unmapped opcode can
         * throw std::out_of_range.
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
