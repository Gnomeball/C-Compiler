/**
 * \file variable-type.hpp
 * \author Gnomeball
 * \brief A file listing all currently supported Variable types
 * \version 0.1
 * \date 2025-12-19
 */

#ifndef VARIABLE_TYPE
#define VARIABLE_TYPE

/**
 * \brief An enum class representing the different types of variables
 */
enum class VariableType : int {
    EMPTY, //!< Empty variable type - used by instructions that require only a destination

    IMM,   //!< Immediate value
    TMP,   //!< Temporary variable
    REG,   //!< Register
    LABEL, //!< Label; used for jumps
};

#endif // VARIABLE_TYPE
