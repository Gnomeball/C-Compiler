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
    ASM_SUB, //~< subl \<value\> \<\src>
    // Subtract Q - used for stack arithmetic
    ASM_SUBQ,

    // Add
    ASM_ADD,

    // Multiply
    ASM_IMUL,

    // Divide
    ASM_IDIV,

    // Bitwise AND
    ASM_AND,

    // Bitwise OR
    ASM_OR,

    // Bitwise XOR
    ASM_XOR,

    // Bitwise Left Shift
    ASM_SAL,

    // Bitwise Right Shift
    ASM_SAR,

    // Return
    ASM_RET, //!< ret

    // Sign extend, eax -> edx
    ASM_CDQ,

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
    // Subtract Q - used for stack arithmetic
    { Instruction::ASM_SUBQ, "SUBQ" },

    // Add
    { Instruction::ASM_ADD, "ADD" },

    // Multiply
    { Instruction::ASM_IMUL, "IMUL" },

    // Divide
    { Instruction::ASM_IDIV, "IDIV" },

    // Bitwise AND
    { Instruction::ASM_AND, "AND" },

    // Bitwise OR
    { Instruction::ASM_OR, "OR" },

    // Bitwise XOR
    { Instruction::ASM_XOR, "XOR" },

    // Bitwise Left Shift
    { Instruction::ASM_SAL, "SAL" },

    // Bitwise Right Shift
    { Instruction::ASM_SAR, "SAR" },

    // Return
    { Instruction::ASM_RET, "RET" },

    // Sign extend, eax -> edx
    { Instruction::ASM_CDQ, "CDQ" },

    // Error
    { Instruction::ASM_ERROR, "OP_ERROR" },
};

#endif // INSTRUCTION
