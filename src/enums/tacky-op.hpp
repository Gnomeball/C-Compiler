/**
 * \file tacky-op.hpp
 * \author Gnomeball
 * \brief A file listing all currently supported TackyOps
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef TACKY_OP
#define TACKY_OP

#include <map>
#include <string>

/**
 * \brief An enumeration of all currently supported TackyOp
 */
enum class TackyOp : int {
    // Operators
    TACKY_COMPLEMENT, //!< OP_COMPLEMENT
    TACKY_NEGATE,     //!< OP_NEGATE

    TACKY_NOT, //!< OP_NOT

    TACKY_ADD,      //!< OP_ADD
    TACKY_SUBTRACT, //!< OP_SUBTRACT
    TACKY_MULTIPLY, //!< OP_MULTIPLY
    TACKY_DIVIDE,   //!< OP_DIVIDE
    TACKY_MODULO,   //!< OP_MODULO

    TACKY_BW_AND,      //!< OP_BW_AND
    TACKY_BW_OR,       //!< OP_BW_OR
    TACKY_BW_XOR,      //!< OP_BW_XOR
    TACKY_LEFT_SHIFT,  //!< OP_LEFT_SHIFT
    TACKY_RIGHT_SHIFT, //!< OP_RIGHT_SHIFT

    TACKY_JUMP_IF_ZERO,     //!< OP_JUMP_IF_ZERO
    TACKY_JUMP_IF_NOT_ZERO, //!< OP_JUMP_IF_NOT_ZERO
    TACKY_JUMP,             //!< OP_JUMP
    TACKY_LABEL,            //!< OP_LABEL
    TACKY_COPY,             //!< OP_COPY

    TACKY_EQUAL,              //!< OP_EQUAL
    TACKY_NOT_EQUAL,          //!< OP_NOT_EQUAL
    TACKY_LESS_THAN,          //!< OP_LESS_THAN
    TACKY_GREATER_THAN,       //!< OP_GREATER_THAN
    TACKY_LESS_THAN_EQUAL,    //!< OP_LESS_THAN_EQUAL
    TACKY_GREATER_THAN_EQUAL, //!< OP_GREATER_THAN_EQUAL

    // Values
    TACKY_VALUE, //!< Identifier to a temporary variable

    // Keywords
    TACKY_RETURN, //!< OP_RETURN

    // Function
    TACKY_FUNCTION, //!< OP_FUNCTION

    // Error
    TACKY_ERROR, //!< Any error
};

/**
 * \brief A map of all TackyOps to their string representations
 *
 * This is used when printing out Tacky information to stdout
 */
const std::map<TackyOp, std::string> tacky_op_string = {
    // Operators
    { TackyOp::TACKY_COMPLEMENT, "COMPLEMENT" },
    { TackyOp::TACKY_NEGATE, "NEGATE" },

    { TackyOp::TACKY_NOT, "NOT" },

    { TackyOp::TACKY_ADD, "ADD" },
    { TackyOp::TACKY_SUBTRACT, "SUBTRACT" },
    { TackyOp::TACKY_MULTIPLY, "MULTIPLY" },
    { TackyOp::TACKY_DIVIDE, "DIVIDE" },
    { TackyOp::TACKY_MODULO, "MODULO" },

    {TackyOp::TACKY_BW_AND, "BW_AND"},
    {TackyOp::TACKY_BW_OR, "BW_OR"},
    {TackyOp::TACKY_BW_XOR, "BW_XOR"},
    {TackyOp::TACKY_LEFT_SHIFT, "LEFT_SHIFT"},
    {TackyOp::TACKY_RIGHT_SHIFT, "RIGHT_SHIFT"},

    { TackyOp::TACKY_JUMP_IF_ZERO, "JUMP_IF_ZERO" },
    { TackyOp::TACKY_JUMP_IF_NOT_ZERO, "JUMP_IF_NOT_ZERO" },
    { TackyOp::TACKY_JUMP, "JUMP"},
    { TackyOp::TACKY_LABEL, "LABEL"},
    { TackyOp::TACKY_COPY, "COPY"},

    { TackyOp::TACKY_EQUAL, "EQUAL" },
    { TackyOp::TACKY_NOT_EQUAL, "NOT_EQUAL" },
    { TackyOp::TACKY_LESS_THAN, "LESS_THAN" },
    { TackyOp::TACKY_GREATER_THAN, "GREATER_THAN" },
    { TackyOp::TACKY_LESS_THAN_EQUAL, "LESS_THAN_EQUAL" },
    { TackyOp::TACKY_GREATER_THAN_EQUAL, "GREATER_THAN_EQUAL" },

    // Values
    { TackyOp::TACKY_VALUE, "VALUE" },

    // Keywords
    { TackyOp::TACKY_RETURN, "RETURN" },

    // Function
    { TackyOp::TACKY_FUNCTION, "FUNCTION" },

    // Error
    { TackyOp::TACKY_ERROR, "OP_ERROR" },
};

#endif // TACKY_OP
