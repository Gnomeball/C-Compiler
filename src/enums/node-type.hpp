/**
 * \file node-type.hpp
 * \author Gnomeball
 * \brief A file listing all currently supported NodeType's
 * \version 0.1
 * \date 2025-12-15
 */

#ifndef NODE_TYPE
#define NODE_TYPE

#include <map>
#include <string>

/**
 * \brief An enumeration of all currently supported NodeType
 */
enum class NodeType : int {
    // Operators
    NT_COMPLEMENT, //!< TK_TILDE
    NT_NEGATE,     //!< TK_MINUS

    // NT_MINUS, //!< TK_MINUS

    NT_DECREMENT, //!< TK_MINUS_MINUS

    // Values
    NT_CONSTANT, //!< TK_CONSTANT

    // Keywords
    NT_RETURN, //!< TK_RETURN

    // Function
    NT_FUNCTION, //!< TK_IDENTIFIER

    // Program ?
    NT_PROGRAM,

    // Error
    NT_ERROR, //!< Any error
};

/**
 * \brief A map of all NodeType to their string representations
 *
 * This is used when printing out Node information to stdout
 */
const std::map<NodeType, std::string> node_type_string = {
    // Operators
    { NodeType::NT_COMPLEMENT, "COMPLEMENT" },
    { NodeType::NT_NEGATE, "NEGATE" },

    // { NodeType::NT_MINUS, "MINUS" }.

    { NodeType::NT_DECREMENT, "DECREMENT" },

    // Values
    { NodeType::NT_CONSTANT, "CONSTANT" },

    // Keywords
    { NodeType::NT_RETURN, "RETURN" },

    // Function
    { NodeType::NT_FUNCTION, "FUNCTION" },

    // Program ?
    { NodeType::NT_PROGRAM, "PROGRAM" },

    // Error
    { NodeType::NT_ERROR, "ERROR" },
};

#endif // NODE_TYPE
