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

        // Helpers

        /**
         * \brief Returns a string containing the information related to this Byte
         *
         * \return A string representational of this Token
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
