/**
 * \file codegen.hpp
 * \author Gnomeball
 * \brief Emit assembly instructions as an assembly source file
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
#include <utility>
#include <vector>

/**
 * \brief Dispatch Assembly instructions to emitters for x86 assembly text.
 *
 * The input is copied and traversed with a cursor; operand strings are emitted
 * as supplied rather than validated or resolved here. See grammar/assembly.md
 * for instruction and operand conventions.
 */
class Codegen {

        /**
         * \brief Copy of the Assembly instructions to emit.
         */
        std::vector<Assembly> assembly;

        /**
         * \brief Used to index the Instruction we are currently trying to output
         */
        unsigned long current_instruction = 0;

        /**
         * \brief Output path prefix; generate() appends .asm.
         */
        std::string output_file_path;

        /**
         * \brief Set on an instruction mismatch or unsupported instruction.
         */
        bool found_error = false;

    private:

        /**
         * \brief Advance past the current instruction if it matches.
         *
         * \param expected The instruction expected at the cursor.
         *
         * A mismatch sets found_error without advancing. Access uses vector::at,
         * so an out-of-range cursor throws std::out_of_range.
         */
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
         * \name Instruction emitters
         * Each writes its mnemonic and applicable operands to output, then
         * calls consume_assembly() with its expected instruction. Thus the
         * cursor advances on a match; on a mismatch the text has already been
         * written and the error flag is set. The ins pointer, where present,
         * must refer to the current instruction. RET and CDQ need no operand.
         * Jump targets use dest, while label definitions use src; both receive
         * a .L prefix. Other operand strings are written unchanged.
         * \{
         */

        void output_sub(std::ofstream &output, Assembly *ins) {
            output << "    subl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_SUB);
        }

        void output_subq(std::ofstream &output, Assembly *ins) {
            output << "    subq    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_SUBQ);
        }

        void output_movl(std::ofstream &output, Assembly *ins) {
            output << "    movl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_MOVL);
        }

        void output_movq(std::ofstream &output, Assembly *ins) {
            output << "    movq    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_MOVQ);
        }

        void output_push(std::ofstream &output, Assembly *ins) {
            output << "    pushq   " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_PUSH);
        }

        void output_pop(std::ofstream &output, Assembly *ins) {
            output << "    popq    " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_POP);
        }

        void output_not(std::ofstream &output, Assembly *ins) {
            output << "    notl    " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_NOT);
        }

        void output_neg(std::ofstream &output, Assembly *ins) {
            output << "    negl    " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_NEG);
        }

        void output_add(std::ofstream &output, Assembly *ins) {
            output << "    addl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_ADD);
        }

        void output_imul(std::ofstream &output, Assembly *ins) {
            output << "    imull   " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_IMUL);
        }

        void output_idiv(std::ofstream &output, Assembly *ins) {
            output << "    idivl   " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_IDIV);
        }

        void output_and(std::ofstream &output, Assembly *ins) {
            output << "    andl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_AND);
        }

        void output_or(std::ofstream &output, Assembly *ins) {
            output << "    orl     " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_OR);
        }

        void output_xor(std::ofstream &output, Assembly *ins) {
            output << "    xorl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_XOR);
        }

        void output_sal(std::ofstream &output, Assembly *ins) {
            output << "    sall    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_SAL);
        }

        void output_sar(std::ofstream &output, Assembly *ins) {
            output << "    sarl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_SAR);
        }

        void output_sete(std::ofstream &output, Assembly *ins) {
            output << "    sete    " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_SETE);
        }

        void output_setne(std::ofstream &output, Assembly *ins) {
            output << "    setne   " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_SETNE);
        }

        void output_setl(std::ofstream &output, Assembly *ins) {
            output << "    setl    " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_SETL);
        }

        void output_setg(std::ofstream &output, Assembly *ins) {
            output << "    setg    " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_SETG);
        }

        void output_setle(std::ofstream &output, Assembly *ins) {
            output << "    setle   " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_SETLE);
        }

        void output_setge(std::ofstream &output, Assembly *ins) {
            output << "    setge   " << ins->get_src() << std::endl;
            consume_assembly(Instruction::ASM_SETGE);
        }

        void output_cmp(std::ofstream &output, Assembly *ins) {
            output << "    cmpl    " << ins->get_src() << ", " << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_CMP);
        }

        void output_je(std::ofstream &output, Assembly *ins) {
            output << "    je      .L" << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_JE);
        }

        void output_jne(std::ofstream &output, Assembly *ins) {
            output << "    jne     .L" << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_JNE);
        }

        void output_jmp(std::ofstream &output, Assembly *ins) {
            output << "    jmp     .L" << ins->get_dest() << std::endl;
            consume_assembly(Instruction::ASM_JMP);
        }

        void output_ret(std::ofstream &output) {
            output << "    ret" << std::endl;
            consume_assembly(Instruction::ASM_RET);
        }

        void output_label(std::ofstream &output, Assembly *ins) {
            output << ".L" << ins->get_src() << ":" << std::endl;
            consume_assembly(Instruction::ASM_LABEL);
        }

        void output_cdq(std::ofstream &output) {
            output << "    cdq" << std::endl;
            consume_assembly(Instruction::ASM_CDQ);
        }

        /** \} */

    public:

        /**
         * \brief Construct with no instructions and an empty output path prefix.
         */
        Codegen() = default; // Default

        /**
         * \brief Copy instructions and retain the output path prefix.
         *
         * \param assembly Non-null pointer to the caller's instruction list;
         * the list is not modified or retained.
         * \param file_path Path prefix without an extension, used for the
         * output file and the .file directive.
         */
        Codegen(std::list<Assembly> *assembly, std::string file_path)
        : output_file_path{ std::move(file_path) } {
            this->assembly = { std::begin(*assembly), std::end(*assembly) };
        }

        /**
         * \brief Write the copied instructions to the path prefix plus .asm.
         *
         * Writes a header and a hard-coded main label (_main on Apple), then
         * dispatches instructions to their emitters and writes a footer. An
         * ASM_ERROR, unsupported instruction, or emitter mismatch sets the
         * error flag, prints a message to stdout, and returns early, leaving
         * potentially partial output without the footer.
         */
        void generate() {

            // Create and open the output file
            std::ofstream output;
            output.open(output_file_path + ".asm");

            // Output file header
            output << "    .text" << std::endl;
            output << "    .file   \"" << output_file_path << ".c\"" << std::endl;

            // todo: this needs to be captured rather than hard coded?
#ifdef __APPLE__
            output << "    .globl  _main" << std::endl;
#else
            output << "    .globl  main" << std::endl;
#endif
            output << std::endl;

            // Output function label
            // todo: this needs to be captured rather than hard coded?
#ifdef __APPLE__
            output << "_main:  ## @main            # -- Begin function main" << std::endl;
#else
            output << "main:   ## @main            # -- Begin function main" << std::endl;
#endif
            output << std::endl;

            // For each assembly instruction, print out what we need
            while (current_instruction < assembly.size()) {

                Assembly *current = &assembly.at(current_instruction);

                // todo: for each line, keep track of current length,
                // and add a # at column 29, ready for an explain comment

                switch (current->get_instruction()) {
                    case Instruction::ASM_MOVL:  output_movl(output, current); break;
                    case Instruction::ASM_MOVQ:  output_movq(output, current); break;
                    case Instruction::ASM_PUSH:  output_push(output, current); break;
                    case Instruction::ASM_POP:   output_pop(output, current); break;
                    case Instruction::ASM_NOT:   output_not(output, current); break;
                    case Instruction::ASM_NEG:   output_neg(output, current); break;
                    case Instruction::ASM_SUB:   output_sub(output, current); break;
                    case Instruction::ASM_SUBQ:  output_subq(output, current); break;
                    case Instruction::ASM_ADD:   output_add(output, current); break;
                    case Instruction::ASM_IMUL:  output_imul(output, current); break;
                    case Instruction::ASM_IDIV:  output_idiv(output, current); break;
                    case Instruction::ASM_AND:   output_and(output, current); break;
                    case Instruction::ASM_OR:    output_or(output, current); break;
                    case Instruction::ASM_XOR:   output_xor(output, current); break;
                    case Instruction::ASM_SAL:   output_sal(output, current); break;
                    case Instruction::ASM_SAR:   output_sar(output, current); break;
                    case Instruction::ASM_SETE:  output_sete(output, current); break;
                    case Instruction::ASM_SETNE: output_setne(output, current); break;
                    case Instruction::ASM_SETL:  output_setl(output, current); break;
                    case Instruction::ASM_SETG:  output_setg(output, current); break;
                    case Instruction::ASM_SETLE: output_setle(output, current); break;
                    case Instruction::ASM_SETGE: output_setge(output, current); break;
                    case Instruction::ASM_CMP:   output_cmp(output, current); break;
                    case Instruction::ASM_JE:    output_je(output, current); break;
                    case Instruction::ASM_JNE:   output_jne(output, current); break;
                    case Instruction::ASM_JMP:   output_jmp(output, current); break;
                    case Instruction::ASM_RET:   output_ret(output); break;
                    case Instruction::ASM_LABEL: output_label(output, current); break;
                    case Instruction::ASM_CDQ:   output_cdq(output); break;
                    case Instruction::ASM_ERROR: this->found_error = true; break;
                    default: this->found_error = true; break;
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
            // output << std::endl;

            // Close the file
            output.close();
        }
};

#endif // CODEGEN
