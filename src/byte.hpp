/*
 * In this file we define the Byte class, as well as it's internal function
 */

#ifndef BYTE
#define BYTE

#include <string>

enum class OpCode {
    // Values
    OP_CONSTANT, //* [0-9]+\b

    // Keywords
    OP_RETURN, //* return\b

    // Identifier
    OP_IDENTIFIER, //* [a-zA-Z_]\w*\b
};

class Byte {

    private:

        OpCode op;

        int int_value;
        std::string string_value;

        // Probably a better way to do this, but at least it's fast
        std::string op_string(void) {
            switch (this->op) {
                case OpCode::OP_CONSTANT: return "CONSTANT";
                case OpCode::OP_RETURN: return "RETURN";
                case OpCode::OP_IDENTIFIER: return "IDENTIFIER";
                default: return "UNKNOWN"; break;
            }
        }

    public:

        // Constructors

        Byte(void) {} // default

        Byte(OpCode op)
        : op{ op } {}

        Byte(OpCode op, int value)
        : op{ op }, int_value{ value } {}

        Byte(OpCode op, std::string value)
        : op{ op }, string_value{ value } {}

        // Accessors

        OpCode get_op(void) {
            return this->op;
        }

        int get_int_value(void) {
            return this->int_value;
        }

        std::string get_string_value(void) {
            return this->string_value;
        }

        // Helpers

        std::string to_string(void) {
            std::string out = "BYTE [OpCode: " + this->op_string();

            if (this->op == OpCode::OP_IDENTIFIER) {
                out += ", Value: " + this->string_value;
            } else if (this->op == OpCode::OP_CONSTANT) {
                out += ", Value: " + std::to_string(this->int_value);
            }

            return out + "]";
        }

        // Overrides
};

#endif
