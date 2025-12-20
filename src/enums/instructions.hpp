/**
 * \file instructions.hpp
 * \author Gnomeball
 * \brief A file listing all currently supported Assembly instructions
 * \version 0.1
 * \date 2025-12-11
 */

#ifndef INSTRUCTION
#define INSTRUCTION

#include <map>
#include <string>

/**
 * \brief An enumeration of all currently supported Instructions
 */
enum class Instruction : int {
    // Identifier
    ASM_IDENT, //!< \<name\>:

    // Mov
    ASM_MOVL, //!< movl \<src\>, \<dest\>
    ASM_MOVQ, //!< movq \<src\>, \<dest\>

    // Push
    ASM_PUSH, //!< pushq \<value\>

    // Pop
    ASM_POP,//!< popq \<dest\>

    // Complement
    ASM_NOT, //!< notl \<reg\>

    // Negate
    ASM_NEG, //!< negl \<reg\>

    // Subtract
    ASM_SUB, //~< subq ??? \<value\> \<\src>

    // Return
    ASM_RET, //!< ret

    // Error
    ASM_ERROR, //!< Any error
};

/**
 * \brief A map of all Instructions to their string representations
 *
 * This is used when printing out Assembly information to stdout
 */
const std::map<Instruction, std::string> asm_string = {
    // Identifier
    { Instruction::ASM_IDENT, "" },

    // Mov
    { Instruction::ASM_MOVL, "MOVL" },
    { Instruction::ASM_MOVQ, "MOVQ" },

    // Push
    { Instruction::ASM_PUSH, "PUSH" },

    // Pop
    { Instruction::ASM_POP, "POP" },

    // Complement
    { Instruction::ASM_NOT, "NOT" },

    // Negate
    { Instruction::ASM_NEG, "NEG" },

    // Subtract
    { Instruction::ASM_SUB, "SUB" },

    // Return
    { Instruction::ASM_RET, "RET" },

    // Error
    { Instruction::ASM_ERROR, "OP_ERROR" },
};

#endif // INSTRUCTION
