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
#include <map>
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

        /**
         * \brief The current memory offset value
         */
        int offset = 0;

        /**
         * \brief A map of temporary variables and their memory offsets
         */
        std::map<std::string, int> slots;

    private:

        // todo: this
        // int value_index(std::string variable) {
        //     int index_of_dot = variable.find_first_of('.');
        //     int length = variable.size();
        //     int length_of_index = length - index_of_dot;
        //     std::string index = variable.substr(index_of_dot, length_of_index);
        //     return std::stoi(index);
        // }

        void consume_instruction() const {
            this->instructions_in->pop_front();
        }

        /**
         * \brief Function to calculate the memory offset of a value
         *
         * @param value The value we are resolving
         * @param type The type of the value we are resolving
         *
         * @return If the value is temporary, we return the value,
         * otherwise we calculate the offset by subtracting 4 from the previous offset.
         */
        std::string resolve(const std::string& value, VariableType type) {
            if (type != VariableType::TMP) return value;
            if (slots.find(value) == slots.end()) {
                slots[value] = (this->offset -= 4);
            }
            return std::to_string(slots.at(value)) + "(%rbp)";
        }

        /**
         * \brief Resolves the memory offsets either side of a mov instruction
         *
         * @param mov A mov instruction
         *
         * If the mov is moving values from memory to memory, this will result in two mov instructions being output.
         */
        void clean_mov(Assembly mov) {
            const bool memory_to_memory = mov.get_src_type() == VariableType::TMP && mov.get_dest_type() == VariableType::TMP;
            const auto src = resolve(mov.get_src(), mov.get_src_type());
            const auto dest = resolve(mov.get_dest(), mov.get_dest_type());
            if (memory_to_memory) {
                this->instructions_cleaned.emplace_back(Instruction::ASM_MOVL, src, VariableType::REG, "%r10d", VariableType::REG);
                this->instructions_cleaned.emplace_back(Instruction::ASM_MOVL, "%r10d", VariableType::REG, dest, VariableType::REG);
            } else {
                this->instructions_cleaned.emplace_back(Instruction::ASM_MOVL, src, VariableType::REG, dest, VariableType::REG);
            }
        }

        /**
         * \brief Resolves the memory offset for a unary instruction
         *
         * @param ins any unary instruction
         */
        void clean_unary(Assembly ins) {
            this->instructions_cleaned.emplace_back(ins.get_instruction(), resolve(ins.get_src(), ins.get_src_type()), VariableType::REG);
        }

        /**
         * \brief Resolves the memory offset for a binary instruction
         *
         * @param ins any binary instruction
         */
        void clean_binary(Assembly ins) {
            this->instructions_cleaned.emplace_back(ins.get_instruction(), resolve(ins.get_src(), ins.get_src_type()), VariableType::REG, resolve(ins.get_dest(), ins.get_dest_type()), VariableType::REG);
        }

        /**
         * \brief Adds a function prologue to the output
         */
        void add_function_prologue() {
            auto subq = Assembly(Instruction::ASM_SUBQ);
            subq.set_src("$" + std::to_string(-this->offset));
            subq.set_dest("%rsp");
            this->instructions_cleaned.push_front(subq);

            auto movq = Assembly(Instruction::ASM_MOVQ);
            movq.set_src("%rsp");
            movq.set_dest("%rbp");
            this->instructions_cleaned.push_front(movq);

            auto pushq = Assembly(Instruction::ASM_PUSH);
            pushq.set_src("%rbp");
            this->instructions_cleaned.push_front(pushq);
        }

        /**
         * \brief Adds a function epilogue to the output
         */
        void add_function_epilogue() {
            auto movq = Assembly(Instruction::ASM_MOVQ);
            movq.set_src("%rbp");
            movq.set_dest("%rsp");
            this->instructions_cleaned.push_back(movq);

            auto popq = Assembly(Instruction::ASM_POP);
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
                    case Instruction::ASM_NOT:
                    case Instruction::ASM_NEG: {
                        clean_unary(this->instructions_in->front());
                        consume_instruction();
                        break;
                    }
                    case Instruction::ASM_ADD:
                    case Instruction::ASM_SUB:
                    case Instruction::ASM_IMUL:
                        clean_binary(this->instructions_in->front());
                        consume_instruction();
                        break;
                    case Instruction::ASM_CDQ:
                    case Instruction::ASM_IDIV:
                    case Instruction::ASM_RET: {
                        this->instructions_cleaned.push_back(this->instructions_in->front());
                        consume_instruction();
                        break;
                    }
                    // case Instruction::ASM_ERROR: {
                    //     break;
                    // }
                    default:
                        this->instructions_cleaned.push_back(this->instructions_in->front());
                        consume_instruction();
                        break;
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
