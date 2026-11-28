/**
 * \file instructions.hpp
 * \author Gnomeball
 * \brief Instruction vocabulary for the Assembly representation
 * \version 0.1
 * \date 2025-12-11
 */

#ifndef INSTRUCTION
#define INSTRUCTION

#include <map>
#include <string>

/**
 * \brief Instructions and markers used in Assembly records.
 *
 * Examples use x86-64 AT&T source, destination order. See
 * grammar/assembly.md for field conventions and code generation details.
 */
enum class Instruction : int {
    // Identifier
    ASM_IDENT, //!< Identifier marker; declared but not emitted or handled by codegen.

    // Mov
    ASM_MOVL, //!< `movl src, dest`: copy 32 bits.
    ASM_MOVQ, //!< `movq src, dest`: copy 64 bits.

    // Push
    ASM_PUSH, //!< `pushq value`: push a 64-bit value.

    // Pop
    ASM_POP, //!< `popq dest`: pop a 64-bit value.

    // Complement
    ASM_NOT, //!< `notl operand`: bitwise complement in place.

    // Negate
    ASM_NEG, //!< `negl operand`: negate in place.

    // Subtract
    ASM_SUB, //!< `subl src, dest`: subtract from the 32-bit destination.
    // Subtract Q - used for stack arithmetic
    ASM_SUBQ, //!< `subq src, dest`: subtract from the 64-bit destination.

    // Add
    ASM_ADD, //!< `addl src, dest`: add to the destination.

    // Multiply
    ASM_IMUL, //!< `imull src, dest`: multiply into the destination.

    // Divide
    ASM_IDIV, //!< `idivl divisor`: divide signed %edx:%eax; quotient in %eax, remainder in %edx.

    // Bitwise AND
    ASM_AND, //!< `andl src, dest`: bitwise AND into the destination.

    // Bitwise OR
    ASM_OR, //!< `orl src, dest`: bitwise OR into the destination.

    // Bitwise XOR
    ASM_XOR, //!< `xorl src, dest`: bitwise XOR into the destination.

    // Bitwise Left Shift
    ASM_SAL, //!< `sall count, dest`: shift the destination left; the compiler uses %cl as count.

    // Bitwise Right Shift
    ASM_SAR, //!< `sarl count, dest`: arithmetic right shift; the compiler uses %cl as count.

    // Comparisons
    ASM_SETE,  //!< `sete byte`: set to 1 if equal, otherwise 0.
    ASM_SETNE, //!< `setne byte`: set to 1 if not equal, otherwise 0.
    ASM_SETL,  //!< `setl byte`: set to 1 if signed less than, otherwise 0.
    ASM_SETG,  //!< `setg byte`: set to 1 if signed greater than, otherwise 0.
    ASM_SETLE, //!< `setle byte`: set to 1 if signed less than or equal, otherwise 0.
    ASM_SETGE, //!< `setge byte`: set to 1 if signed greater than or equal, otherwise 0.

    // Compare
    ASM_CMP, //!< `cmpl src, dest`: set flags from `dest - src` without storing the result.

    // Jumps
    ASM_JE,  //!< `je .Llabel`: jump if equal.
    ASM_JNE, //!< `jne .Llabel`: jump if not equal.
    ASM_JMP, //!< `jmp .Llabel`: jump unconditionally.

    // Return
    ASM_RET, //!< `ret`: return to the caller.

    // Labels
    ASM_LABEL, //!< Define a label from `src`; codegen emits `.L` followed by the name and `:`.

    // Sign extension
    ASM_CDQ, //!< `cdq`: sign-extend %eax into %edx:%eax for signed division.

    // Error
    ASM_ERROR, //!< Error record rather than an executable instruction.
};

/**
 * \brief A map of Instructions to debug labels.
 *
 * Used when printing Assembly records; labels are not necessarily assembly
 * mnemonics (and ASM_IDENT maps to an empty string).
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

    // Comparisons
    { Instruction::ASM_SETE,  "SETE" },
    { Instruction::ASM_SETNE, "SETNE" },
    { Instruction::ASM_SETL,  "SETL" },
    { Instruction::ASM_SETG,  "SETG" },
    { Instruction::ASM_SETLE, "SETLE" },
    { Instruction::ASM_SETGE, "SETGE" },

    // Compare
    { Instruction::ASM_CMP, "CMP" },

    // Jumps
    { Instruction::ASM_JE, "JE" },
    { Instruction::ASM_JNE, "JNE" },
    { Instruction::ASM_JMP, "JMP" },

    // Return
    { Instruction::ASM_RET, "RET" },

    // Labels
    { Instruction::ASM_LABEL, "LABEL" },

    // Sign extension
    { Instruction::ASM_CDQ, "CDQ" },

    // Error
    { Instruction::ASM_ERROR, "OP_ERROR" },
};

#endif // INSTRUCTION
