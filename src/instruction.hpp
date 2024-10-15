/*
 * In this file we outline our Instrurction class,
 * as well as all currently accepted Instructions
 */

#ifndef INSTRUCTION
#define INSTRUCTION

#include <string>

#include "assembly.hpp"

class Instruction {

    private:

        Assembly code;

        std::string name;
        std::string src;
        std::string dst;

    public:

        // Constructors

        Instruction(void) {} // default

        Instruction(Assembly code)
        : code{ code } {}

        Instruction(Assembly code, std::string name)
        : code{ code }, name{ name } {}

        Instruction(Assembly code, std::string src, std::string dst)
        : code{ code }, src{ src }, dst{ dst } {}

        // Accessors

        Assembly get_code(void) {
            return this->code;
        }

        std::string code_string(void) {
            return asm_string.at(this->code);
        }

        std::string get_name(void) {
            return this->name;
        }

        std::string get_src(void) {
            return this->src;
        }

        std::string get_dst(void) {
            return this->dst;
        }

        // Helpers

        const std::string to_string(void) {
            std::string out = "INS [Code: " + asm_string.at(this->code);

            if (this->code == Assembly::ASM_IDENT) {
                out += ", name: " + this->name;
            } else if (this->code == Assembly::ASM_MOV) {
                out += ", src: " + this->src + ", dest: " + this->dst;
            }

            return out + "]";
        }

        // Overrides
};

#endif
