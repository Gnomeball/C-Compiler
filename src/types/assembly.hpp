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

        std::string value;

    public:

        // Constructors

        Assembly(void) {} // default

        Assembly(AssemblyOp op)
        : op{ op } {}

        Assembly(AssemblyOp op, std::string value)
        : op{ op }, value{ value } {}

        // Accessors

        AssemblyOp get_op(void) {
            return this->op;
        }

        std::string get_value(void) {
            return this->value;
        }

        // Helpers

        const std::string to_string(void) {
            std::string out = "ASM [Op: " + asm_string.at(this->op);

            if (this->op == AssemblyOp::ASM_IDENT) {
                out += ", Value: " + this->value;
            } else if (this->op == AssemblyOp::ASM_MOV) {
                out += ", Value: " + this->value;
            }

            return out + "]";
        }

        // Overrides
};

#endif
