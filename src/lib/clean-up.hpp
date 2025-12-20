/**
 * \file clean-up.hpp
 * \author Gnomeball
 * \brief
 * \version 0.1
 * \date 2025-12-20
 */

#ifndef CLEAN_UP
#define CLEAN_UP

#include <list>
#include <string>

#include "../enums/instructions.hpp"
#include "../enums/variable-type.hpp"
#include "../types/assembly.hpp"

class CleanUp {

        /**
         * \brief The list of Assembly Instructions which are to be cleaned
         */
        std::list<Assembly> *instructions_in;

        /**
         * \brief The list of Assembly Instructions after they have been "cleaned up"
         */
        std::list<Assembly> instructions_cleaned;

        int offset = -4;

    private:

        // todo: this
        // int value_index(std::string variable) {
        //     int index_of_dot = variable.find_first_of('.');
        //     int length = variable.size();
        //     int length_of_index = length - index_of_dot;
        //     std::string index = variable.substr(index_of_dot, length_of_index);
        //     return std::stoi(index);
        // }

        void consume_instruction() {
            this->instructions_in->pop_front();
        }

        void clean_mov(Assembly mov) {
            // always has src and dest

            if (mov.get_src_type() == VariableType::IMM) {
                // if the source is an immediate value
                Assembly cleaned_mov = Assembly(Instruction::ASM_MOVL);
                std::string value = mov.get_src();
                cleaned_mov.set_src(value);
                cleaned_mov.set_dest(std::to_string(this->offset) + "(%rbp)");
                this->instructions_cleaned.push_back(cleaned_mov);
                return;
            }

            else if (mov.get_src_type() == VariableType::TMP && mov.get_dest_type() == VariableType::TMP) {
                // if both are temporary, we need to use a 'scratch' register in between
                std::string scratch = "%r10d";

                Assembly left = Assembly(Instruction::ASM_MOVL);
                left.set_src(std::to_string(this->offset) + "(%rbp)");
                left.set_dest(scratch);
                this->instructions_cleaned.push_back(left);

                // todo: not this
                this->offset -= 4;

                Assembly right = Assembly(Instruction::ASM_MOVL);
                right.set_src(scratch);
                right.set_dest(std::to_string(this->offset) + "(%rbp)");
                this->instructions_cleaned.push_back(right);

                return;
            }

            else if (mov.get_dest_type() == VariableType::REG) {
                Assembly cleaned_mov = Assembly(Instruction::ASM_MOVL);
                std::string value = mov.get_src();
                cleaned_mov.set_src(std::to_string(this->offset) + "(%rbp)");
                cleaned_mov.set_dest(mov.get_dest());
                this->instructions_cleaned.push_back(cleaned_mov);
                return;
            }
        }

        void clean_not(Assembly noot) {
            // only has src

            if (noot.get_src_type() == VariableType::TMP) {
                Assembly cleaned_not = Assembly(Instruction::ASM_NOT);
                cleaned_not.set_src(std::to_string(this->offset) + "(%rbp)");
                this->instructions_cleaned.push_back(cleaned_not);
                return;
            }
        }

        void clean_neg(Assembly neg) {
            // only has src

            if (neg.get_src_type() == VariableType::TMP) {
                Assembly cleaned_neg = Assembly(Instruction::ASM_NEG);
                cleaned_neg.set_src(std::to_string(this->offset) + "(%rbp)");
                this->instructions_cleaned.push_back(cleaned_neg);
                return;
            }
        }

        void clean_ret(Assembly ret) {
            // shouldn't have to do anything?
            this->instructions_cleaned.push_back(ret);
        }

        void add_function_prologue() {
            Assembly subq = Assembly(Instruction::ASM_SUB);
            subq.set_src("$" + std::to_string(-this->offset));
            subq.set_dest("%rsp");
            this->instructions_cleaned.push_front(subq);

            Assembly movq = Assembly(Instruction::ASM_MOVQ);
            movq.set_src("%rsp");
            movq.set_dest("%rbp");
            this->instructions_cleaned.push_front(movq);

            Assembly pushq = Assembly(Instruction::ASM_PUSH);
            pushq.set_src("%rbp");
            this->instructions_cleaned.push_front(pushq);
        }

        void add_function_epilogue() {
            Assembly movq = Assembly(Instruction::ASM_MOVQ);
            movq.set_src("%rbp");
            movq.set_dest("%rsp");
            this->instructions_cleaned.push_back(movq);

            Assembly popq = Assembly(Instruction::ASM_POP);
            popq.set_dest("%rbp");
            this->instructions_cleaned.push_back(popq);
        }

        /**
         * \brief Used to clean up temporary variables after assembly generation
         */
        void clean_up() {

            // Clean up the instructions

            while (!this->instructions_in->empty()) {
                switch (this->instructions_in->front().get_instruction()) {
                    // case Instruction::ASM_IDENT: {
                    //     break;
                    // }
                    case Instruction::ASM_MOVL: {
                        clean_mov(this->instructions_in->front());
                        consume_instruction();
                        break;
                    }
                    case Instruction::ASM_NOT: {
                        clean_not(this->instructions_in->front());
                        consume_instruction();
                        break;
                    }
                    case Instruction::ASM_NEG: {
                        clean_neg(this->instructions_in->front());
                        consume_instruction();
                        break;
                    }
                    case Instruction::ASM_RET: {
                        clean_ret(this->instructions_in->front());
                        consume_instruction();
                        break;
                    }
                    // case Instruction::ASM_ERROR: {
                    //     break;
                    // }
                    default: break;
                }
            }

            // Add the function prologue (backwards) and epilogue
            add_function_prologue();

            Assembly ret = this->instructions_cleaned.back();
            this->instructions_cleaned.pop_back();
            add_function_epilogue();
            this->instructions_cleaned.push_back(ret);
        }

    public:

        /**
         * \brief Default constructor for a CleanUp
         */
        CleanUp() {} // Default

        /**
         * \brief Construct a new CleanUp object with a list of Assembly
         *
         * \param assembly The Assembly Instructions to clean
         */
        CleanUp(std::list<Assembly> *assembly)
        : instructions_in{ assembly } {}

        std::list<Assembly> get_cleaned_instructions() {
            return this->instructions_cleaned;
        }

        void run() {
            // call the clean up
            clean_up();
        }
};

#endif // CLEAN_UP
