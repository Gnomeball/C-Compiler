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

//>> Begin Register Declarations

std::string reg_eax = "%eax";

//<< End Register Declarations

void generate_globals(Byte b) {
#ifdef DEBUG_COMPILER
    std::cout << "Entering generate_globals" << std::endl;
#endif
    instructions.push_back(Instruction(Assembly::ASM_IDENT, b.getStringValue()));
}

void generate_mov(Byte b) {
#ifdef DEBUG_COMPILER
    std::cout << "Entering generate_mov" << std::endl;
#endif
    std::string src = "$" + std::to_string(b.getIntValue());
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
        OpCode op = b.getOp();
        switch (op) {
            case OpCode::OP_IDENTIFIER: generate_globals(b); break;
            case OpCode::OP_CONSTANT:   generate_mov(b); break;
            case OpCode::OP_RETURN:     generate_ret(); break;
            default: break;
        }
    }

    return 0;
}

void output_assembly(std::string output_file) {
    std::cout << "OUTPUTTING ASSEMBLY" << std::endl;
    std::cout << "out file: " << output_file << std::endl;

    std::ofstream output;
    output.open(output_file);

    for (Instruction i : instructions) {
        Assembly a = i.getCode();
        if (a == Assembly::ASM_IDENT) {
            std::cout << "outputting globl" << std::endl;
            output << "    " << i.code_string() << "  " << i.getName() << std::endl;
            output << i.getName() << ":" << std::endl;
        } else if (a == Assembly::ASM_MOV) {
            std::cout << "outputting mov" << std::endl;
            output << "    " << i.code_string() << "    " << i.getSrc() << ", " << i.getDest() << std::endl;
        } else if (a == Assembly::ASM_RET) {
            std::cout << "outputting ret" << std::endl;
            output << "    " << i.code_string() << std::endl;
        }
    }
    output.close();
}
