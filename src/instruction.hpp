/*
 * In this file we outline our Instrurction class,
 * as well as all currently accepted Instructions
 */

#include <string>

enum class Assembly {
    ASM_IDENT,  //* .globl <name> /n <name>:
    ASM_MOV,    //* movl <src>, <dest>
    ASM_RET,    //* retq
};

class Instruction {

    private:

        Assembly code;

        std::string name;
        std::string src;
        std::string dest;

        // Probably a better way to do this, but at least it's fast
        std::string asm_string(void) {
            switch (this->code) {
                case Assembly::ASM_IDENT:   return ".globl";
                case Assembly::ASM_MOV:     return "movl";
                case Assembly::ASM_RET:     return "retq";
                default: return "UNKNOWN"; break;
            }
        }

    public:

        // Constructors

        Instruction(void) {} // default

        Instruction(Assembly code) :code{code} {}

        Instruction(Assembly code, std::string name) :code{code}, name{name} {}

        Instruction(Assembly code, std::string src, std::string dest) :code{code}, src{src}, dest{dest} {}

        // Accessors

        Assembly getCode(void) { return this->code; }

        std::string code_string(void) { return this->asm_string(); }

        std::string getName(void) { return this->name; }

        std::string getSrc(void) { return this->src; }

        std::string getDest(void) { return this->dest; }

        // Helpers

        std::string to_string(void) {
            std::string out = "INS [Code: " + this->asm_string();

            if (this->code == Assembly::ASM_IDENT) {
                out += ", name: " + this->name;
            } else if (this->code == Assembly::ASM_MOV) {
                out += ", src: " + this->src + ", dest: " + this->dest;
            }

            return out + "]";
        }

        // Overrides

        // std::ostream& operator<<(std::ostream &os) {
        //     return os << this->to_string();
        // }

};