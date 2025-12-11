/**
 * \file assembly-op.hpp
 * \author Gnomeball
 * \brief A file listing all currently supported Assembly operations
 * \version 0.1
 * \date 2025-12-11
 */

#ifndef ASMEMBLY_OP
#define ASMEMBLY_OP

#include <map>
#include <string>

enum class AssemblyOp : int {
    // Identifier
    ASM_IDENT, // <name>:

    // Mov
    ASM_MOV, // movl <src>, <dest>

    // Return
    ASM_RET, // ret

    // Error
    ASM_ERROR,
};

const std::map<AssemblyOp, std::string> asm_string = {
    // Identifier
    { AssemblyOp::ASM_IDENT, "" },

    // Mov
    { AssemblyOp::ASM_MOV, "MOV" },

    // Return
    { AssemblyOp::ASM_RET, "RET" },

    // Error
    { AssemblyOp::ASM_ERROR, "OP_ERROR" },
};

#endif
