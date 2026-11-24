/**
 * \file codegen.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implementation of the codegen class
 * \version 0.1
 * \date 2024-12-11
 */

#ifndef CODEGEN
#define CODEGEN

#include "../types/assembly.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

/**
 * \brief This file simply outputs assembly code, no real logic
 */
class Codegen {

        /**
         * \brief The list of Assembly Instructions to be outputted
         */
        std::vector<Assembly> assembly;

        /**
         * \brief Used to index the Instruction we are currently trying to output
         */
        unsigned long current_instruction = 0;

        /**
         * \brief The file we are outputting to
         */
        std::string output_file_path;

        /**
         * \brief Set to true upon finding an error
         */
        bool found_error = false;

    private:

        void consume_assembly(Instruction expected) {
            if (assembly.at(current_instruction).get_instruction() != expected) {
                // error; shouldn't happen?
                this->found_error = true;
            } else {
                // soft-consume the instruction, by increasing our index pointer
                this->current_instruction++;
            }
        }

        void output_sub(std::ofstream &output, Assembly *ins) {
            output << "    subl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_SUB);
        }

        void output_subq(std::ofstream &output, Assembly *ins) {
            output << "    subq    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_SUBQ);
        }

        void output_movl(std::ofstream &output, Assembly *ins) {
            output << "    movl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_MOVL);
        }

        void output_movq(std::ofstream &output, Assembly *ins) {
            output << "    movq    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_MOVQ);
        }

        void output_push(std::ofstream &output, Assembly *ins) {
            output << "    pushq   " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_PUSH);
        }

        void output_pop(std::ofstream &output, Assembly *ins) {
            output << "    popq    " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_POP);
        }

        void output_not(std::ofstream &output, Assembly *ins) {
            output << "    notl    " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_NOT);
        }

        void output_neg(std::ofstream &output, Assembly *ins) {
            output << "    negl    " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_NEG);
        }

        void output_add(std::ofstream &output, Assembly *ins) {
            output << "    addl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_ADD);
        }

        void output_imul(std::ofstream &output, Assembly *ins) {
            output << "    imull   " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_IMUL);
        }

        void output_idiv(std::ofstream &output, Assembly *ins) {
            output << "    idivl   " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_IDIV);
        }

        void output_and(std::ofstream &output, Assembly *ins) {
            output << "    andl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_AND);
        }

        void output_or(std::ofstream &output, Assembly *ins) {
            output << "    orl     " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_OR);
        }

        void output_xor(std::ofstream &output, Assembly *ins) {
            output << "    xorl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_XOR);
        }

        void output_sal(std::ofstream &output, Assembly *ins) {
            output << "    sall    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_SAL);
        }

        void output_sar(std::ofstream &output, Assembly *ins) {
            output << "    sarl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_SAR);
        }

        void output_ret(std::ofstream &output) {
            output << "    ret" << std::endl;
            consume_assembly(Instruction::ASM_RET);
        }

        void output_cdq(std::ofstream &output) {
            output << "    cdq" << std::endl;
            consume_assembly(Instruction::ASM_CDQ);
        }

    public:

        /**
         * \brief Default constructor for a Codegen
         */
        Codegen() = default; // Default

        /**
         * \brief Construct a new Codegen object with an output file path
         *
         * \param assembly The Assembly to generate code output from
         * \param file_path The file path to the input file, without its extension
         */
        Codegen(std::list<Assembly> *assembly, std::string file_path)
        : output_file_path{ std::move(file_path) } {
            this->assembly = { std::begin(*assembly), std::end(*assembly) };
        }

        /**
         * \brief Prints out to file our assembled Assembly Instructions
         */
        void generate() {

            // Create and open the output file
            std::ofstream output;
            output.open(output_file_path + ".asm");

            // Output file header
            output << "    .text" << std::endl;
            output << "    .file   \"" << output_file_path << ".c\"" << std::endl;
            // todo: this needs to be captured rather than hard coded?
#ifdef __APPLE__
            output << "    .globl  _main" << std::endl;
#else
            output << "    .globl  main" << std::endl;
#endif
            output << std::endl;

            // Output function label
            // todo: this needs to be captured rather than hard coded?
#ifdef __APPLE__
            output << "_main:  ## @main            # -- Begin function main" << std::endl;
#else
            output << "main:   ## @main            # -- Begin function main" << std::endl;
#endif
            output << std::endl;

            // For each assembly instruction, print out what we need
            while (current_instruction < assembly.size()) {

                Assembly *current = &assembly.at(current_instruction);

                // todo: for each line, keep track of current length,
                // and add a # at column 29, ready for an explain comment

                switch (current->get_instruction()) {
                    case Instruction::ASM_MOVL: output_movl(output, current); break;
                    case Instruction::ASM_MOVQ: output_movq(output, current); break;
                    case Instruction::ASM_PUSH: output_push(output, current); break;
                    case Instruction::ASM_POP:  output_pop(output, current); break;
                    case Instruction::ASM_NOT:  output_not(output, current); break;
                    case Instruction::ASM_NEG:  output_neg(output, current); break;
                    case Instruction::ASM_SUB:  output_sub(output, current); break;
                    case Instruction::ASM_SUBQ: output_subq(output, current); break;
                    case Instruction::ASM_ADD:  output_add(output, current); break;
                    case Instruction::ASM_IMUL: output_imul(output, current); break;
                    case Instruction::ASM_IDIV: output_idiv(output, current); break;
                    case Instruction::ASM_AND:  output_and(output, current); break;
                    case Instruction::ASM_OR:  output_or(output, current); break;
                    case Instruction::ASM_XOR: output_xor(output, current); break;
                    case Instruction::ASM_SAL: output_sal(output, current); break;
                    case Instruction::ASM_SAR: output_sar(output, current); break;
                    case Instruction::ASM_RET:  output_ret(output); break;
                    case Instruction::ASM_CDQ:  output_cdq(output); break;
                    case Instruction::ASM_ERROR: this->found_error = true; break;
                    default: this->found_error = true; break;
                }

                // If we found an error, exit
                if (this->found_error) {
                    std::cout << "Error Found whilst generating file" << std::endl;
                    return;
                }
            }

            // Output function footer
            output << std::endl;
            // todo: this needs to be captured rather than hard coded?
            output << "        ## -- End function main" << std::endl;
            output << std::endl;

            // Output file footer
            output << "    .ident  \"A Very Gnomish C Compiler\"" << std::endl;
            // output << std::endl;

            // Close the file
            output.close();
        }
};

#endif // CODEGEN
