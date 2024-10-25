/**
 * \file byte-ops.hpp
 * \author Gnomeball
 * \brief A file listing all currently supported Byte OpCode's
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef OPCODE
#define OPCODE

#include <map>
#include <string>

enum class OpCode : int {
    // Operators
    OP_COMPLEMENT, // TK_TILDE
    OP_NEGATE,     // TK_MINUS

    // OP_MINUS, // TK_MINUS

    OP_DECREMENT, // TK_MINUS_MINUS

    // Values
    OP_CONSTANT, // TK_CONSTANT

    // Keywords
    OP_RETURN, // TK_RETURN

    // Function
    OP_FUNCTION, // TK_IDENTIFIER () {}

    // Error
    OP_ERROR, // Any error
};

const std::map<OpCode, std::string> op_code_string = {
    // Operators
    { OpCode::OP_COMPLEMENT, "COMPLEMENT" },
    { OpCode::OP_NEGATE, "NEGATE" },

    // { OpCode::OP_MINUS, "MINUS" }.

    { OpCode::OP_DECREMENT, "DECREMENT" },

    // Values
    { OpCode::OP_CONSTANT, "CONSTANT" },

    // Keywords
    { OpCode::OP_RETURN, "RETURN" },

    // Function
    { OpCode::OP_FUNCTION, "OP_FUNCTION" },

    // Error
    { OpCode::OP_ERROR, "OP_ERROR" },
};

#endif
