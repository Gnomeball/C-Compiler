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
#include <list>
#include <ostream>
#include <string>

/**
 * \brief This file simply outputs assembly code, no real logic
 */
class Codegen {

        /**
         * \brief The list of Assembly Instructions to be outputted
         */
        std::list<Assembly> *assembly;

        /**
         * \brief The file we are outputting to
         */
        std::string output_file_path;

    private:

        // .. Tumbleweed ..

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
        : assembly( assembly ), output_file_path{ file_path } {}

        /**
         * \brief Prints out to file our assembled Assembly Instructions
         */
        void generate() {

            // Create and open the output file
            std::ofstream output;
            output.open(output_file_path + ".asm");

            // Output file header
            output << "    .text"                                            << std::endl;
            output << "    .file   \"" << output_file_path << ".c\""         << std::endl;
            output << "    .globl  _main           # -- Begin function main" << std::endl;
            output <<                                                           std::endl;
            output << "_main:  ## @main"                                     << std::endl;
            output <<                                                           std::endl;

            // Output function header
            output << "    pushq   %rbp            # push base pointer onto the stack"   << std::endl;
            output << "    movq    %rsp, %rbp      # move stack pointer to base pointer" << std::endl;
            output <<                                                                       std::endl;

            // Subtract from the base pointer
            output <<                                                                       std::endl;

            // For each assembly instruction, print out what we need

            for (Assembly as : *assembly) {

                switch (as.get_instruction()) {
                    case Instruction::ASM_NEG: {
                        output << "    negl    $" << as.get_src() << std::endl;
                        break;
                    }
                    case Instruction::ASM_NOT: {
                        output << "    notl    $" << as.get_src() << std::endl;
                        break;
                    }
                    case Instruction::ASM_MOV: {
                        /* // todo: for each line, keep track of current length,
                             and add a # at column 29, ready for an explain comment */
                        output <<                                                              std::endl;
                        output << "    movl    $" << as.get_src() << ", %" << as.get_dest() << std::endl;
                        break;
                    }
                    case Instruction::ASM_RET: {
                        // Output function footer
                        output <<                                                                         std::endl;
                        output << "    movq    %rbp, %rsp      # move base pointer into stack pointer" << std::endl;
                        output << "    popq    %rbp            # pop stack back into base pointer"     << std::endl;
                        output <<                                                                         std::endl;
                        output << "    retq"                                                           << std::endl;
                        break;
                    }
                    default: return;
                }

            }

            output <<                                                std::endl;
            output << "        ## -- End function main"           << std::endl;
            output <<                                                std::endl;
            output << "    .ident  \"A Very Gnomish C Compiler\"" << std::endl;
            // output <<                                                std::endl;

            // Close the file
            output.close();

            return;
        }

};

#endif // CODEGEN
