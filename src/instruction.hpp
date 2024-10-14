/*
 * In this file we outline our Instrurction class,
 * as well as all currently accepted Instructions
 */

#ifndef INSTRUCTION
#define INSTRUCTION

#include <string>

enum class Assembly {
    ASM_IDENT, //* .globl <name> /n <name>:
    ASM_MOV,   //* movl <src>, <dest>
    ASM_RET,   //* retq
};

class Instruction {

    private:

        Assembly code;

        std::string name;
        std::string src;
        std::string dest;

        // Probably a better way to do this, but at least it's fast
        const std::string asm_string(void) {
            switch (this->code) {
                case Assembly::ASM_IDENT: return ".globl";
                case Assembly::ASM_MOV: return "movl";
                case Assembly::ASM_RET: return "retq";
            }
        }

    public:

        // Constructors

        Instruction(void) {} // default

        Instruction(Assembly code)
        : code{ code } {}

        Instruction(Assembly code, std::string name)
        : code{ code }, name{ name } {}

        Instruction(Assembly code, std::string src, std::string dest)
        : code{ code }, src{ src }, dest{ dest } {}

        // Accessors

        Assembly get_code(void) {
            return this->code;
        }

        std::string code_string(void) {
            return this->asm_string();
        }

        std::string get_name(void) {
            return this->name;
        }

        std::string get_src(void) {
            return this->src;
        }

        std::string get_dest(void) {
            return this->dest;
        }

        // Helpers

        const std::string to_string(void) {
            std::string out = "INS [Code: " + this->asm_string();

            if (this->code == Assembly::ASM_IDENT) {
                out += ", name: " + this->name;
            } else if (this->code == Assembly::ASM_MOV) {
                out += ", src: " + this->src + ", dest: " + this->dest;
            }

            return out + "]";
        }

        // Overrides
};

#endif
