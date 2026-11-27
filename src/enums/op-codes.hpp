/**
 * \file op-codes.hpp
 * \author Gnomeball
 * \brief Opcode vocabulary for the Byte representation
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef OP_CODE
#define OP_CODE

#include <map>
#include <string>

/**
 * \brief Operations and markers used in Byte records.
 *
 * Expressions use postfix order: operands precede their operators, with the
 * left operand before the right. Logical begin markers appear between the
 * operands so the tackifier can generate short-circuit control flow. See
 * grammar/byte.md for the full representation and field conventions.
 */
enum class OpCode : int {
    // Unary Operators
    OP_COMPLEMENT, //!< Bitwise complement of the preceding operand.
    OP_NEGATE,     //!< Arithmetic negation of the preceding operand.
    OP_NOT,        //!< Logical negation; produces 1 for zero, otherwise 0.

    OP_DECREMENT, //!< Declared but not emitted by the parser or handled by the tackifier.

    // Binary operators
    OP_ADD,      //!< Add the operands.
    OP_SUBTRACT, //!< Subtract the right operand from the left.
    OP_MULTIPLY, //!< Multiply the operands.
    OP_DIVIDE,   //!< Divide the left operand by the right.
    OP_MODULO,   //!< Remainder of the left operand divided by the right.

    OP_BW_AND,      //!< Bitwise AND of the operands.
    OP_BW_OR,       //!< Bitwise OR of the operands.
    OP_BW_XOR,      //!< Bitwise XOR of the operands.
    OP_LEFT_SHIFT,  //!< Shift the left operand left by the right operand.
    OP_RIGHT_SHIFT, //!< Shift the left operand right by the right operand.

    OP_LOGIC_AND,          //!< Finish a matching AND expression; result is 0 or 1.
    OP_LOGIC_OR,           //!< Finish a matching OR expression; result is 0 or 1.
    OP_EQUAL,              //!< Compare operands for equality; result is 0 or 1.
    OP_NOT_EQUAL,          //!< Compare operands for inequality; result is 0 or 1.
    OP_LESS_THAN,          //!< Test whether left is less than right; result is 0 or 1.
    OP_GREATER_THAN,       //!< Test whether left is greater than right; result is 0 or 1.
    OP_LESS_THAN_EQUAL,    //!< Test whether left is less than or equal to right; result is 0 or 1.
    OP_GREATER_THAN_EQUAL, //!< Test whether left is greater than or equal to right; result is 0 or 1.

    // Values
    OP_CONSTANT, //!< Supply an integer operand as a decimal string in value.

    // Keywords
    OP_RETURN, //!< Return the preceding expression result; corresponds to TK_KEYWORD_RETURN.

    // Function
    OP_FUNCTION, //!< Begin a function; value holds its identifier.

    // Short-circuit markers
    OP_AND_BEGIN, //!< After the left operand of &&; skip the right operand if left is zero.
    OP_OR_BEGIN,  //!< After the left operand of ||; skip the right operand if left is nonzero.

    // Error
    OP_ERROR, //!< Parser error record rather than an executable operation.
};

/**
 * \brief A map of OpCodes to their debug labels.
 *
 * This is used when printing out Byte information to stdout
 */
const std::map<OpCode, std::string> op_code_string = {
    // Operators
    { OpCode::OP_COMPLEMENT, "COMPLEMENT" },
    { OpCode::OP_NEGATE, "NEGATE" },
    { OpCode::OP_NOT, "NOT" },

    { OpCode::OP_DECREMENT, "DECREMENT" },

    // Binary operators
    { OpCode::OP_ADD, "ADD" },
    { OpCode::OP_SUBTRACT, "SUBTRACT" },
    { OpCode::OP_MULTIPLY, "MULTIPLY" },
    { OpCode::OP_DIVIDE, "DIVIDE" },
    { OpCode::OP_MODULO, "MODULO" },

    { OpCode::OP_BW_AND, "BW_AND" },
    { OpCode::OP_BW_OR, "BW_OR" },
    { OpCode::OP_BW_XOR, "BW_XOR" },
    { OpCode::OP_LEFT_SHIFT, "LEFT_SHIFT" },
    { OpCode::OP_RIGHT_SHIFT, "RIGHT_SHIFT" },

    { OpCode::OP_LOGIC_AND, "LOGIC_AND" },
    { OpCode::OP_LOGIC_OR, "LOGIC_OR" },
    { OpCode::OP_EQUAL, "EQUAL" },
    { OpCode::OP_NOT_EQUAL, "NOT_EQUAL" },
    { OpCode::OP_LESS_THAN, "LESS_THAN" },
    { OpCode::OP_GREATER_THAN, "GREATER_THAN" },
    { OpCode::OP_LESS_THAN_EQUAL, "LESS_THAN_EQUAL" },
    { OpCode::OP_GREATER_THAN_EQUAL, "GREATER_THAN_EQUAL" },

    // Values
    { OpCode::OP_CONSTANT, "CONSTANT" },

    // Keywords
    { OpCode::OP_RETURN, "RETURN" },

    // Function
    { OpCode::OP_FUNCTION, "FUNCTION" },

    // Short-circuit markers
    { OpCode::OP_AND_BEGIN, "AND_BEGIN" },
    { OpCode::OP_OR_BEGIN, "OR_BEGIN" },

    // Error
    { OpCode::OP_ERROR, "ERROR" },
};

#endif // OP_CODE
