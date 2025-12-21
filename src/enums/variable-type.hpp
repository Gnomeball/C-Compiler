/**
 * \file variable-type.hpp
 * \author Gnomeball
 * \brief A file listing listing all currently supported Variable types
 * \version 0.1
 * \date 2025-12-19
 */

#ifndef VARIABLE_TYPE
#define VARIABLE_TYPE

enum class VariableType : int {
    IMM, //!< Immediate value
    TMP, //!< Temporary variable
    REG  //!< Register
};

#endif // VARIABLE_TYPE
