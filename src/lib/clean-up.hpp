/**
 * \file clean-up.hpp
 * \author Gnomeball
 * \brief Resolve assembly temporaries and add a stack frame
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

/**
 * \brief Rewrite selected assembly instructions and add a stack frame.
 *
 * Temporary names are assigned four-byte stack slots relative to %rbp. MOVL
 * instructions between two temporaries are routed through %r10d. Other
 * instruction forms are handled as described by clean_up(); this is not a
 * general-purpose x86 instruction validator. See grammar/assembly.md.
 */
class CleanUp {

        /**
         * \brief Non-owning pointer to the input list, consumed by run().
         */
        std::list<Assembly> *instructions_in;

        /**
         * \brief The list of Assembly Instructions after they have been "cleaned up".
         */
        std::list<Assembly> instructions_cleaned;

        /**
         * \brief Offset of the most recently allocated slot, initially zero.
         */
        int offset = 0;

        /**
         * \brief Map of temporary names to their reusable stack offsets.
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

        /**
         * \brief Remove the first input instruction; the input must be nonempty.
         */
        void consume_instruction() const {
            this->instructions_in->pop_front();
        }

        /**
         * \brief Resolve a temporary to a stack address, allocating a slot if needed.
         *
         * @param value The value we are resolving
         * @param type The type of the value we are resolving
         *
         * Note: in time this will need to account for type sizes, so can't
         * always be a hard-coded offset of 4 bytes.
         *
         * @return Non-temporary values unchanged; temporary names become an
         * offset from %rbp. The first occurrence allocates four bytes by
         * subtracting 4 from offset; later occurrences reuse the same slot.
         */
        std::string resolve(const std::string& value, VariableType type) {
            if (type != VariableType::TMP) return value;
            if (slots.find(value) == slots.end()) {
                slots[value] = (this->offset -= 4);
            }
            return std::to_string(slots.at(value)) + "(%rbp)";
        }

        /**
         * \brief Resolve operands of a MOVL and append the resulting instruction(s).
         *
         * @param mov A mov instruction
         *
         * When both operands are temporaries, use %r10d to avoid a direct
         * memory-to-memory move. The output operand types are marked REG.
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
         * \brief Resolve the source of a unary instruction and append it.
         *
         * @param ins any unary instruction
         */
        void clean_unary(Assembly ins) {
            this->instructions_cleaned.emplace_back(ins.get_instruction(), resolve(ins.get_src(), ins.get_src_type()), VariableType::REG);
        }

        /**
         * \brief Resolve both operands of a binary instruction and append it.
         *
         * @param ins any binary instruction
         */
        void clean_binary(Assembly ins) {
            this->instructions_cleaned.emplace_back(ins.get_instruction(), resolve(ins.get_src(), ins.get_src_type()), VariableType::REG, resolve(ins.get_dest(), ins.get_dest_type()), VariableType::REG);
        }

        /**
         * \brief Prepend a frame-pointer prologue and reserve the allocated slot bytes.
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
         * \brief Append instructions restoring %rsp and the saved %rbp.
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
         * \brief Consume the input, rewrite selected operands and add a stack frame.
         *
         * MOVL, NOT, NEG, and the listed binary instructions have temporary
         * operands resolved; other instructions are copied unchanged. A
         * prologue is prepended, and an epilogue is inserted immediately
         * before the final output instruction, assumed to be RET. This does
         * not insert epilogues before earlier returns or validate instructions.
         * The input pointer must be valid and the input should end in RET.
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
                    case Instruction::ASM_AND:
                    case Instruction::ASM_OR:
                    case Instruction::ASM_XOR:
                    case Instruction::ASM_SAL:
                    case Instruction::ASM_SAR:
                    case Instruction::ASM_CMP: {
                        clean_binary(this->instructions_in->front());
                        consume_instruction();
                        break;
                    }
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
         * \brief Construct without an input list.
         *
         * The input pointer is uninitialised; run() must not be called on an
         * object constructed this way.
         */
        CleanUp() {} // Default

        /**
         * \brief Construct a cleanup pass over a caller-owned list.
         *
         * \param assembly Non-null input list, which must outlive this object
         * and is consumed by run(). Its final instruction should be RET.
         */
        CleanUp(std::list<Assembly> *assembly)
        : instructions_in{ assembly } {}

        /**
         * \brief Return a copy of the cleaned instructions.
         *
         * @return a list of cleaned Assembly Instructions
         */
        std::list<Assembly> get_cleaned_instructions() {
            return this->instructions_cleaned;
        }

        /**
         * \brief Run the cleanup pass once on the supplied input list.
         *
         * Consumes the list and stores the rewritten instructions internally.
         * Requires a valid input pointer and a final RET for a correct epilogue.
         */
        void run() {
            clean_up();
        }
};

#endif // CLEAN_UP
