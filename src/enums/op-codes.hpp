/**
 * \file op-codes.hpp
 * \author Gnomeball
 * \brief A file listing all currently supported Byte OpCode's
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef OP_CODE
#define OP_CODE

#include <map>
#include <string>

/**
 * \brief An enumeration of all currently supported OpCodes
 */
enum class OpCode : int {
    // Unary Operators
    OP_COMPLEMENT, //!< TK_TILDE
    OP_NEGATE,     //!< TK_MINUS
    // OP_MINUS,   //!< TK_MINUS
    OP_NOT,        //!< TK_BANG

    OP_DECREMENT, //!< TK_MINUS_MINUS

    // Binary operators
    OP_ADD,      //!< TK_PLUS
    OP_SUBTRACT, //!< TK_MINUS
    OP_MULTIPLY, //!< TK_STAR
    OP_DIVIDE,   //!< TK_SLASH
    OP_MODULO,   //!< TK_PERCENTAGE

    OP_BW_AND,      //!< TK_AMPERSAND
    OP_BW_OR,       //!< TK_PIPE
    OP_BW_XOR,      //!< TK_CARET
    OP_LEFT_SHIFT,  //!< TK_LEFT_CHEVRONS
    OP_RIGHT_SHIFT, //!< TK_RIGHT_CHEVRONS

    OP_LOGIC_AND,           //!< TK_AMPE_AMPE
    OP_LOGIC_OR,            //!< TK_PIPE_PIPE
    OP_EQUAL,               //!< TK_EQUAL_EQUAL
    OP_NOT_EQUAL,           //!< TK_BANG_EQUAL
    OP_LESS_THAN,           //!< TK_LESS
    OP_GREATER_THAN,        //!< TK_GREATER
    OP_LESS_THAN_EQUAL,     //!< TK_LESS_EQUAL
    OP_GREATER_THAN_EQUAL,  //!< TK_GREATER_EQUAL

    // Values
    OP_CONSTANT, //!< TK_CONSTANT

    // Keywords
    OP_RETURN, //!< TK_RETURN

    // Function
    OP_FUNCTION, //!< TK_IDENTIFIER

    // Labels
    OP_AND_BEGIN, //!< TK_AND_BEGIN
    OP_OR_BEGIN, //!< TK_OR_BEGIN

    // Error
    OP_ERROR, //!< Any error
};

/**
 * \brief A map of all OpCodes to their string representations
 *
 * This is used when printing out Byte information to stdout
 */
const std::map<OpCode, std::string> op_code_string = {
    // Operators
    { OpCode::OP_COMPLEMENT, "COMPLEMENT" },
    { OpCode::OP_NEGATE, "NEGATE" },
    { OpCode::OP_NOT, "NOT" },

    // { OpCode::OP_MINUS, "MINUS" }.

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

    // Labels
    { OpCode::OP_AND_BEGIN, "AND_BEGIN" },
    { OpCode::OP_OR_BEGIN, "OR_BEGIN" },

    // Error
    { OpCode::OP_ERROR, "ERROR" },
};

#endif // OP_CODE
