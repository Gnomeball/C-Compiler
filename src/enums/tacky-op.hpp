/**
 * \file tacky-op.hpp
 * \author Gnomeball
 * \brief Operation vocabulary for the Tacky representation
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef TACKY_OP
#define TACKY_OP

#include <map>
#include <string>

/**
 * \brief Operations and markers used in Tacky records.
 *
 * Unary operations use src_a and dest; binary operations use src_a as the
 * left operand, src_b as the right operand, and dest for the result.
 * See grammar/tacky.md for field types and the full operand conventions.
 */
enum class TackyOp : int {
    // Operators
    TACKY_COMPLEMENT, //!< Bitwise complement of src_a.
    TACKY_NEGATE,     //!< Arithmetic negation of src_a.

    TACKY_NOT, //!< Logical negation of src_a; produces 1 for zero, otherwise 0.

    TACKY_ADD,      //!< Add the operands.
    TACKY_SUBTRACT, //!< Subtract the right operand from the left.
    TACKY_MULTIPLY, //!< Multiply the operands.
    TACKY_DIVIDE,   //!< Divide the left operand by the right.
    TACKY_MODULO,   //!< Remainder of the left operand divided by the right.

    TACKY_BW_AND,      //!< Bitwise AND of the operands.
    TACKY_BW_OR,       //!< Bitwise OR of the operands.
    TACKY_BW_XOR,      //!< Bitwise XOR of the operands.
    TACKY_LEFT_SHIFT,  //!< Shift the left operand left by the right operand.
    TACKY_RIGHT_SHIFT, //!< Shift the left operand right by the right operand.

    TACKY_JUMP_IF_ZERO,     //!< Jump to dest if src_a is zero.
    TACKY_JUMP_IF_NOT_ZERO, //!< Jump to dest if src_a is nonzero.
    TACKY_JUMP,             //!< Jump unconditionally to the label in src_a.
    TACKY_LABEL,            //!< Define the label in src_a.
    TACKY_COPY,             //!< Copy src_a to dest.

    TACKY_EQUAL,              //!< Compare operands for equality; result is 0 or 1.
    TACKY_NOT_EQUAL,          //!< Compare operands for inequality; result is 0 or 1.
    TACKY_LESS_THAN,          //!< Test whether left is less than right; result is 0 or 1.
    TACKY_GREATER_THAN,       //!< Test whether left is greater than right; result is 0 or 1.
    TACKY_LESS_THAN_EQUAL,    //!< Test whether left is less than or equal to right; result is 0 or 1.
    TACKY_GREATER_THAN_EQUAL, //!< Test whether left is greater than or equal to right; result is 0 or 1.

    // Values
    TACKY_VALUE, //!< Declared but not emitted by the current tackifier; constants use its operand stack.

    // Keywords
    TACKY_RETURN, //!< Return the value in src_a; dest is unused.

    // Function
    TACKY_FUNCTION, //!< Begin a function named in src_a.

    // Error
    TACKY_ERROR, //!< Tackifier error record with its message in dest.
};

/**
 * \brief A map of TackyOps to their debug labels.
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
