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
    ASM_SUB, //!< subl \<value\> \<\src>
    // Subtract Q - used for stack arithmetic
    ASM_SUBQ, //!< subq \<value\> \<\src\>

    // Add
    ASM_ADD, //!< addl \<value\> \<\src>

    // Multiply
    ASM_IMUL, //!< imull \<value\> \<\src>

    // Divide
    ASM_IDIV, //!< idivl \<value\> \<\src\>

    // Bitwise AND
    ASM_AND, //!< andl \<value\> \<\src\>

    // Bitwise OR
    ASM_OR, //!< orl \<value\> \<\src\>

    // Bitwise XOR
    ASM_XOR, //!< xorl \<value\> \<\src\>

    // Bitwise Left Shift
    ASM_SAL, //!< sal %cl \<\src\>

    // Bitwise Right Shift
    ASM_SAR, //!< sar %cl \<\src\>

    // Comparisons
    ASM_SETE,  /* equal                        */ //!< sete
    ASM_SETNE, /* not equal                    */ //!< setne
    ASM_SETL,  /* signed less than             */ //!< setl
    ASM_SETG,  /* signed greater than          */ //!< setg
    ASM_SETLE, /* signed less than or equal    */ //!< setle
    ASM_SETGE, /* signed greater than or equal */ //!< setge

    // Compare
    ASM_CMP, //!< cmp \<src\> \<\dest\>

    // Jumps
    ASM_JE,  /* Jump if equal     */ //!< je <\label\>
    ASM_JNE, /* Jump if not equal */ //!< jne <\label\>
    ASM_JMP, /* Always Jump       */ //!< jmp <\label\>

    // Return
    ASM_RET, //!< ret

    // Labels
    ASM_LABEL,

    // Sign extend, eax -> edx
    ASM_CDQ, //!< cdq

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

    // Comparisons
    { Instruction::ASM_SETE,  "sete" },
    { Instruction::ASM_SETNE, "setne" },
    { Instruction::ASM_SETL,  "setl" },
    { Instruction::ASM_SETG,  "setg" },
    { Instruction::ASM_SETLE, "setle" },
    { Instruction::ASM_SETGE, "setge" },

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

    // Sign extend, eax -> edx
    { Instruction::ASM_CDQ, "CDQ" },

    // Error
    { Instruction::ASM_ERROR, "OP_ERROR" },
};

#endif // INSTRUCTION
