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

#include "../enums/byte-ops.hpp"

class Byte {

    private:

        OpCode op;

        std::string value;

    public:

        // Constructors

        Byte(void) {} // default

        Byte(OpCode op)
        : op{ op } {}

        Byte(OpCode op, std::string value)
        : op{ op }, value{ value } {}

        // Accessors

        OpCode get_op(void) {
            return this->op;
        }

        std::string get_value(void) {
            return this->value;
        }

        // Helpers

        const std::string to_string(void) {
            std::string out = "BYTE [Op: " + op_code_string.at(this->op);

            if (this->op == OpCode::OP_FUNCTION) {
                out += ", Value: " + this->value;
            } else if (this->op == OpCode::OP_CONSTANT) {
                out += ", Value: " + this->value;
            } else if (this->op == OpCode::OP_ERROR) {
                out += ", Error: " + this->value;
            }

            return out + "]";
        }

        // Overrides
};

#endif
