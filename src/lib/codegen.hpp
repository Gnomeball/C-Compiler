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
#include <string>

/*
 * This file simply outputs assembly code, no real logic, yet
 */
class Codegen {

        std::list<Assembly> *assembly;

        std::string output_file_path_no_ext;

        std::string output_ext = ".s";

    private:

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
        : assembly( assembly ), output_file_path_no_ext{ file_path } {}

        void generate() {

            // Create and open the output file
            std::ofstream output;
            output.open(output_file_path_no_ext + output_ext);

            // Output file header
            output << "    .globl     _main" << std::endl;
            output << "_main:"                << std::endl;

            // For each assembly instructio, print out what we need
            // todo: store the commands as well as the correct variables inside the instructions
            for (Assembly as : *assembly) {

                switch (as.get_op()) {
                    case AssemblyOp::ASM_MOV: {
                        output << "    movl    $" << as.get_value() << ", %eax" << std::endl;
                        break;
                    }
                    case AssemblyOp::ASM_RET: {
                        output << "    retq" << std::endl;
                        break;
                    }
                    default: return;
                }

            }

            // Close the file
            output.close();

            return;
        }

};

#endif
