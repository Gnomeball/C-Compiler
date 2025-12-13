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
enum class TackyOp {
    // Operators
    TACKY_UNARY_COMPLEMENT, //!< OP_COMPLEMENT
    TACKY_UNARY_NEGATE,     //!< OP_NEGATE

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
    { TackyOp::TACKY_UNARY_COMPLEMENT, "UNARY_COMPLEMENT" },
    { TackyOp::TACKY_UNARY_NEGATE, "UNARY_NEGATE" },

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
