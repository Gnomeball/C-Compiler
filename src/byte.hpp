/*
 * In this file we define the Byte class, as well as it's internal function
 */

#ifndef BYTE
#define BYTE

#include <string>

enum class OpCode {
    // Error
    // OP_ERROR,                //* Any Error

    // Single-character
    // OP_EOF,                  //* EOF

    // OP_SEMI_COLON,           //* ;

    // OP_OPEN_PARENTHESIS,     //* \(
    // OP_CLOSE_PARENTHESIS,    //* \)
    // OP_OPEN_BRACE,           //* {
    // OP_CLOSE_BRACE,          //* }

    // Values
    OP_CONSTANT,             //* [0-9]+\b

    // Keywords
    // OP_INT,                  //* int\b
    // OP_VOID,                 //* void\b
    OP_RETURN,               //* return\b

    // Identifier
    OP_IDENTIFIER,           //* [a-zA-Z_]\w*\b
};

class Byte {

    private:

        OpCode op;

        int int_value;
        std::string string_value;

        // Probably a better way to do this, but at least it's fast
        std::string op_string(void) {
            switch (this->op) {
                // case OpCode::OP_ERROR:              return "ERROR";
                // case OpCode::OP_EOF:                return "EOF";
                // case OpCode::OP_SEMI_COLON:         return "SEMI_COLON";
                // case OpCode::OP_OPEN_PARENTHESIS:   return "OPEN_PARENTHESIS";
                // case OpCode::OP_CLOSE_PARENTHESIS:  return "CLOSE_PARENTHESIS";
                // case OpCode::OP_OPEN_BRACE:         return "OPEN_BRACE";
                // case OpCode::OP_CLOSE_BRACE:        return "CLOSE_BRACE";
                case OpCode::OP_CONSTANT:           return "CONSTANT";
                // case OpCode::OP_INT:                return "INT";
                // case OpCode::OP_VOID:               return "VOID";
                case OpCode::OP_RETURN:             return "RETURN";
                case OpCode::OP_IDENTIFIER:         return "IDENTIFIER";
                default: return "UNKNOWN"; break;
            }
        }

    public:

        // Constructors

        Byte(void) {} // default

        Byte(OpCode op) :op{op} {}

        Byte(OpCode op, int value) :op{op}, int_value{value} {}

        Byte(OpCode op, std::string value) :op{op}, string_value{value} {}

        // Accessors

        OpCode getOp(void) { return this->op; }

        int getIntValue(void) { return this->int_value; }

        std::string getStringValue(void) { return this->string_value; }

        // Helpers

        std::string to_string(void) {
            std::string out = "BYTE [OpCode: " + this->op_string();

            if (this->op == OpCode::OP_IDENTIFIER) {
                out += ", Value: "+ this->string_value;
            } else if (this->op == OpCode::OP_CONSTANT) {
                out += ", Value: "+ std::to_string(this->int_value);
            }

            return out + "]";
        }

        // Overrides

        // std::ostream& operator<<(std::ostream &os) {
        //     return os << this->to_string();
        // }

};

#endif
