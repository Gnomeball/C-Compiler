#include <map>
#include <string>

enum class Assembly {
    ASM_IDENT, // <name>:
    ASM_MOV,   // movl <src>, <dest>
    ASM_RET,   // retq
};

const std::map<Assembly, std::string> asm_string = {
    { Assembly::ASM_IDENT, "" },
    { Assembly::ASM_MOV, "movl" },
    { Assembly::ASM_RET, "retq" },
};
