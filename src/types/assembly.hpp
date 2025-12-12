/**
 * \file asmembly.hpp
 * \author Gnomeball
 * \brief A file outlining the implementation of the Asm class
 * \version 0.1
 * \date 2025-12-11
 */

#ifndef ASMEMBLY
#define ASMEMBLY

#include <string>

#include "../enums/assembly-op.hpp"

class Assembly {

    private:

        AssemblyOp op;

        std::string src;
        std::string dest;

    public:

        // Constructors

        Assembly(void) {} // default

        Assembly(AssemblyOp op)
        : op{ op } {}

        Assembly(AssemblyOp op, std::string src)
        : op{ op }, src{ src } {}

        Assembly(AssemblyOp op, std::string src, std::string dest)
        : op{ op }, src{ src }, dest{ dest } {}

        // Accessors

        AssemblyOp get_op(void) {
            return this->op;
        }

        std::string get_src(void) {
            return this->src;
        }

        std::string get_dest(void) {
            return this->dest;
        }

        // Helpers

        const std::string to_string(void) {
            std::string out = "ASM [Op: " + asm_string.at(this->op);

            // if (this->op == AssemblyOp::ASM_IDENT) {
            //     out += ", Value: " + this->value;
            // } else

            switch (this->op) {
                case AssemblyOp::ASM_MOV: {
                    out += ", src: " + this->src + ", dest: " + this->dest;
                    break;
                }
                case AssemblyOp::ASM_NOT:
                case AssemblyOp::ASM_NEG: {
                    out += ", reg: " + this->src;
                    break;
                }
                // case AssemblyOp::ASM_RET: {
                //     out += ", src: " + this->src;
                //     break;
                // }
                default:;
            }

            return out + "]";
        }

        // Overrides
};

#endif
