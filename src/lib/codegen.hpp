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
#include <vector>

/**
 * \brief This file simply outputs assembly code, no real logic
 */
class Codegen {

        /**
         * \brief The list of Assembly Instructions to be outputted
         */
        // std::list<Assembly> *assembly;
        std::vector<Assembly> assembly;

        /**
         * \brief Used to index the Instruction we are currently trying to output
         */
        int current_instruction = 0;

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

        /**
         * \brief Outputs a mov command to the output file
         *
         * \param output The output file stream
         * \param mov The mov Instruction
         */
        void output_mov(std::ofstream &output, Assembly *ins) {
            // Output the mov
            output << "    mov     $" << ins->get_src() << ", %" << ins->get_dest() << std::endl;
            // Consume the Instruction
            consume_assembly(Instruction::ASM_MOV);
        }

        // void output_not(std::ofstream &output, Assembly *ins) {
        // }

        // void output_neg(std::ofstream &output, Assembly *ins) {
        // }

        void output_ret(std::ofstream &output) {
            // Output the ret
            output << "    ret" << std::endl;
            // Consume the Instruction
            consume_assembly(Instruction::ASM_RET);
        }

        // void output_function(std::ofstream &output, Assembly *ins) {

        //     /*

        //     // Output function header
        //     output << "    pushq   %rbp            # push base pointer onto the stack" << std::endl;
        //     output << "    movq    %rsp, %rbp      # move stack pointer to base pointer" << std::endl;
        //     output << std::endl;

        //     // Subtract from the base pointer
        //     output << std::endl;

        //     // Output function footer
        //     output << std::endl;
        //     output << "    movq    %rbp, %rsp      # move base pointer into stack pointer" << std::endl;
        //     output << "    popq    %rbp            # pop stack back into base pointer" << std::endl;
        //     output << std::endl;

        //     */
        // }

        // todo: split this thing into functions

    public:

        /**
         * \brief Default constructor for a Codegen
         */
        Codegen() {} // Default

        /**
         * \brief Construct a new Codegen object with an output file path
         *
         * \param file_path The file path to the input file, without it's extension
         */
        Codegen(std::list<Assembly> *assembly, std::string file_path)
        : output_file_path{ file_path } {
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
            output << "    .globl  _main           # -- Begin function main" << std::endl;
            output << std::endl;

            // Output fuction label
            // todo: this needs to be captured rather than hard coded?
            output << "_main:  ## @main" << std::endl;
            output << std::endl;

            // For each assembly instruction, print out what we need
            while (current_instruction < assembly.size()) {

                Assembly *current = &assembly.at(current_instruction);

                // todo: for each line, keep track of current length,
                // and add a # at column 29, ready for an explain comment

                switch (current->get_instruction()) {
                    // case Instruction::ASM_IDENT: {
                    //     break;
                    // }
                    case Instruction::ASM_MOV: {
                        output_mov(output, current);
                        break;
                    }
                    case Instruction::ASM_NOT: {
                        //     output << "    notl    $" << as.get_src() << std::endl;
                        break;
                    }
                    case Instruction::ASM_NEG: {
                        //     output << "    negl    $" << as.get_src() << std::endl;
                        break;
                    }
                    case Instruction::ASM_RET: {
                        output_ret(output);
                        break;
                    }
                    case Instruction::ASM_ERROR: {
                        break;
                    }
                    default: break;
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
            output << std::endl;

            // Close the file
            output.close();

            return;
        }
};

#endif // CODEGEN
