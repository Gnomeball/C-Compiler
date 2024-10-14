/*
 * In this file we convert our Chunk's list of Bytes into Assembly
 */

#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "byte.hpp"
#include "data.hpp"
#include "debug.hpp"
#include "explain.hpp"
#include "instruction.hpp"

//>> Begin Register Declarations

std::string reg_eax = "%eax";

//<< End Register Declarations

void generate_globals(Byte b) {
#ifdef DEBUG_COMPILER
    std::cout << "Entering generate_globals" << std::endl;
#endif
    instructions.push_back(Instruction(Assembly::ASM_IDENT, b.get_string_value()));
}

void generate_mov(Byte b) {
#ifdef DEBUG_COMPILER
    std::cout << "Entering generate_mov" << std::endl;
#endif
    std::string src = "$" + std::to_string(b.get_int_value());
    instructions.push_back(Instruction(Assembly::ASM_MOV, src, reg_eax));
}

void generate_ret() {
#ifdef DEBUG_COMPILER
    std::cout << "Entering generate_ret" << std::endl;
#endif
    instructions.push_back(Instruction(Assembly::ASM_RET));
}

int generate_assembly(std::list<Byte> bytes) {
#ifdef DEBUG_COMPILER
    std::cout << "Entering generate_assembly" << std::endl;
#endif
    for (Byte b : bytes) {
#ifdef DEBUG_COMPILER
        std::cout << b.to_string() << std::endl;
#endif
        OpCode op = b.get_op();
        switch (op) {
            case OpCode::OP_IDENTIFIER: generate_globals(b); break;
            case OpCode::OP_CONSTANT: generate_mov(b); break;
            case OpCode::OP_RETURN: generate_ret(); break;
            default: break;
        }
    }

    return 0;
}

void output_assembly(std::string output_file) {
#ifdef DEBUG_COMPILER
    std::cout << "OUTPUTTING ASSEMBLY" << std::endl;
    std::cout << "out file: " << output_file + ".s" << std::endl;
#endif

    std::ofstream output;
    output.open(output_file + ".s");

    // Output file header
    output << "    .text" << std::endl;
    output << "    .file       \"" << output_file.substr(output_file.find_last_of("/") + 1, output_file.size()) + ".c\"" << std::endl;

    for (Instruction i : instructions) {
        Assembly a = i.get_code();
        if (a == Assembly::ASM_IDENT) {
#ifdef DEBUG_COMPILER
            std::cout << "outputting globl" << std::endl;
#endif
            output << "    " << i.code_string() << "      " << i.get_name() << std::endl;
            output << i.get_name() << ":" << explain(i) << std::endl;
        } else if (a == Assembly::ASM_MOV) {
#ifdef DEBUG_COMPILER
            std::cout << "outputting mov" << std::endl;
#endif
            output << "    " << i.code_string() << "        " << i.get_src() << ", " << i.get_dest() << explain(i) << std::endl;
        } else if (a == Assembly::ASM_RET) {
#ifdef DEBUG_COMPILER
            std::cout << "outputting ret" << std::endl;
#endif
            output << "    " << i.code_string() << explain(i) << std::endl;
        }
    }

    // Output file footer
    output << "    .ident      \"A very Gnomish C Compiler\"" << std::endl;
    output << "    .section    \".note.GNU-stack\",\"\",@progbits" << std::endl;

    output.close();
}
