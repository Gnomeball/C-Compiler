/*
 * In this file we outline various explainer functions, used to comment produced assembly
 */

#include <string>

#ifndef INSTRUCTION
    #include "instruction.hpp"
#endif

const std::string explain_identifier(Instruction ins) {
    return "Begin " + ins.get_name() + " function";
}

const std::string explain_mov(Instruction ins) {
    return "Moving " + ins.get_src() + " into " + ins.get_dest() + "";
}

const std::string explain_ret(void) {
    return "End of function";
}

const std::string get_comment(Instruction ins) {
    switch (ins.get_code()) {
        case Assembly::ASM_IDENT:
            return explain_identifier(ins);
        case Assembly::ASM_MOV:
            return explain_mov(ins);
        case Assembly::ASM_RET:
            return explain_ret();
    }
}

int calculate_width_used_by(Instruction ins) {
    switch (ins.get_code()) {
        case Assembly::ASM_IDENT: //* '<name>:'
            return 1 + ins.get_name().size();
        case Assembly::ASM_MOV: //* '    movl        $2, %eax'
            //! this one won't always be 14, moving from a register uses more characters
            return 14 + ins.code_string().size() + ins.get_src().size() + ins.get_dest().size();
        case Assembly::ASM_RET: //* '    retq'
            return 4 + ins.code_string().size();
    }
}

const std::string explain(Instruction ins) {
    std::string explain_string = "";
    int desired_padding = 40;

    // get the already used width, so we know how much to pad by
    int width_already_used = calculate_width_used_by(ins);
    int padding = desired_padding - width_already_used;
    explain_string.append(padding, ' ');

    // add the comment
    std::string comment = "# " + get_comment(ins);
    explain_string.append(comment);

    return explain_string;
}