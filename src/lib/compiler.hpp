/**
 * \file compiler.hpp
 * \author Gnomeball
 * \brief Lower Tacky operations to Assembly instructions
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef COMPILER
#define COMPILER

#include <list>
#include <string>

#include "../lib/clean-up.hpp"
#include "../types/assembly.hpp"
#include "../types/tacky.hpp"

#ifdef DEBUG_COMPILER
    #include <iostream>
#endif

/**
 * \brief Lower a caller-owned Tacky list into Assembly.
 *
 * The pass consumes one function through its first return. When requested by
 * the lowering logic, CleanUp resolves temporaries and adds a stack frame.
 * See grammar/tacky.md and grammar/assembly.md for operand conventions.
 */
class Compiler {

        /**
         * \brief Non-owning pointer to the input list, consumed by run().
         */
        std::list<Tacky> *tacky{};

        /**
         * \brief The list of Assembly Instructions built by this Compiler
         */
        std::list<Assembly> assembly;

        /**
         * \brief Set to true upon finding an error
         */
        bool found_error = false;

        /**
         * \brief Set when lowering requests the cleanup pass.
         */
        bool clean_up_required = false;

    private:

        /**
         * \brief Consume the front operation if it matches the expected type.
         *
         * \param expected The expected TackyOp
         * \param message Message stored in an ASM_ERROR on mismatch.
         *
         * The input must be nonempty. On mismatch, append ASM_ERROR and set
         * found_error without consuming the operation.
         */
        void consume_tacky(TackyOp expected, const std::string& message = "") {
            if (tacky->front().get_op() != expected) {
                // error
                this->assembly.emplace_back(Instruction::ASM_ERROR, message, VariableType::IMM);
                this->found_error = true;
            } else {
                // consume the token
                this->tacky->pop_front();
            }
        }

        /**
         * \brief Adds an Assembly instruction to the list of found Assembly instructions
         *
         * This function also facilitates debug output for the Compiler
         *
         * \param assembly The Assembly instruction
         */
        void add_assembly(const Assembly& assembly) {
#ifdef DEBUG_COMPILER
            std::cout << "Found : " << asm_string.at(assembly.get_instruction()) << std::endl;
#endif
            this->assembly.push_back(assembly);
        }

        /**
         * \brief Lower and consume a supported unary operation at the front.
         *
         * src_a is the operand and dest is the result. Complement and negate
         * copy the operand to dest before applying NOT or NEG. Logical NOT
         * compares the operand with zero and uses SETE to produce 0 or 1,
         * requesting cleanup. An unsupported operation is left untouched.
         */
        void assemble_unary() {
            const std::string src = this->tacky->front().get_src_a();
            const VariableType src_type = this->tacky->front().get_src_a_type();
            const std::string dest = this->tacky->front().get_dest();
            const VariableType dest_type = this->tacky->front().get_dest_type();

            switch (this->tacky->front().get_op()) {
                case TackyOp::TACKY_COMPLEMENT: {
                    add_assembly(Assembly(Instruction::ASM_MOVL, src, src_type, dest, dest_type));
                    add_assembly(Assembly(Instruction::ASM_NOT, dest, dest_type));
                    consume_tacky(TackyOp::TACKY_COMPLEMENT);
                    break;
                }
                case TackyOp::TACKY_NEGATE: {
                    add_assembly(Assembly(Instruction::ASM_MOVL, src, src_type, dest, dest_type));
                    add_assembly(Assembly(Instruction::ASM_NEG, dest, dest_type));
                    consume_tacky(TackyOp::TACKY_NEGATE);
                    break;
                }
                case TackyOp::TACKY_NOT: {
                    add_assembly(Assembly(Instruction::ASM_MOVL, src, src_type,"%r10d", VariableType::REG));
                    add_assembly(Assembly(Instruction::ASM_CMP, "$0", VariableType::IMM,"%r10d", VariableType::REG));
                    add_assembly(Assembly(Instruction::ASM_MOVL, "$0", VariableType::IMM,"%eax", VariableType::REG));
                    add_assembly(Assembly(Instruction::ASM_SETE, "%al", VariableType::REG));
                    add_assembly(Assembly(Instruction::ASM_MOVL, "%eax", VariableType::REG,dest, dest_type));
                    // Because it may contain temporary variables, we trigger a clean-up just incase
                    this->clean_up_required = true;
                    consume_tacky(TackyOp::TACKY_NOT);
                    break;
                }
                default: return;
            }
        }

        /**
         * \brief Helper function to convert TackyOp to a corresponding assembly instruction type
         *
         * @param op the operation we are converting
         *
         * @return The mapped instruction, or ASM_ERROR for an unsupported operation.
         */
        static Instruction binary_instruction(TackyOp op) {
            switch (op) {
                case TackyOp::TACKY_ADD:      return Instruction::ASM_ADD;
                case TackyOp::TACKY_SUBTRACT: return Instruction::ASM_SUB;
                case TackyOp::TACKY_MULTIPLY: return Instruction::ASM_IMUL;
                case TackyOp::TACKY_BW_AND:   return Instruction::ASM_AND;
                case TackyOp::TACKY_BW_OR:    return Instruction::ASM_OR;
                case TackyOp::TACKY_BW_XOR:   return Instruction::ASM_XOR;
                default: return Instruction::ASM_ERROR;
            }
        }

        /**
         * \brief Lower and consume the binary operation at the front.
         *
         * src_a and src_b are the left and right operands; dest receives the
         * result. The left operand is loaded into %eax. Division and modulo
         * use CDQ and IDIV with the divisor in %r10d; modulo takes %edx.
         * Shifts use %cl, and comparisons use CMP and SETcc to produce 0 or 1.
         * Other operations use binary_instruction(), including its ASM_ERROR
         * fallback. The result is copied to dest and cleanup is requested.
         */
        void assemble_binary() {
            Tacky &item = this->tacky->front();
            const auto op = item.get_op();
            const auto src_a = item.get_src_a();
            const auto src_a_type = item.get_src_a_type();
            const auto src_b = item.get_src_b();
            const auto src_b_type = item.get_src_b_type();
            const auto dest = item.get_dest();
            const auto dest_type = item.get_dest_type();

            add_assembly(Assembly(Instruction::ASM_MOVL, src_a, src_a_type, "%eax", VariableType::REG));

            if (op == TackyOp::TACKY_DIVIDE || op == TackyOp::TACKY_MODULO) {
                // extend the sign
                add_assembly(Assembly(Instruction::ASM_CDQ));
                // right hand side into a temp register
                add_assembly(Assembly(Instruction::ASM_MOVL, src_b, src_b_type, "%r10d", VariableType::REG));
                // then call idiv on r10d, eax
                add_assembly(Assembly(Instruction::ASM_IDIV, "%r10d", VariableType::REG));
            } else if (op == TackyOp::TACKY_LEFT_SHIFT || op == TackyOp::TACKY_RIGHT_SHIFT) {
                // left hand into eax - already done outside the loop, though idk if that's the correct way to do it yet
                // right hand into ecx
                add_assembly(Assembly(Instruction::ASM_MOVL, src_b, src_b_type, "%ecx", VariableType::REG));
                // then call sal/r on cl, eax
                add_assembly(Assembly(op == TackyOp::TACKY_LEFT_SHIFT ? Instruction::ASM_SAL : Instruction::ASM_SAR,
                    "%cl", VariableType::REG, "%eax", VariableType::REG));
            } else if (op == TackyOp::TACKY_EQUAL        || op == TackyOp::TACKY_NOT_EQUAL       ||
                       op == TackyOp::TACKY_LESS_THAN    || op == TackyOp::TACKY_LESS_THAN_EQUAL ||
                       op == TackyOp::TACKY_GREATER_THAN || op == TackyOp::TACKY_GREATER_THAN_EQUAL) {
                Instruction condition;
                switch (op) {
                    case TackyOp::TACKY_EQUAL:              condition = Instruction::ASM_SETE;  break;
                    case TackyOp::TACKY_NOT_EQUAL:          condition = Instruction::ASM_SETNE; break;
                    case TackyOp::TACKY_LESS_THAN:          condition = Instruction::ASM_SETL;  break;
                    case TackyOp::TACKY_GREATER_THAN:       condition = Instruction::ASM_SETG;  break;
                    case TackyOp::TACKY_LESS_THAN_EQUAL:    condition = Instruction::ASM_SETLE; break;
                    case TackyOp::TACKY_GREATER_THAN_EQUAL: condition = Instruction::ASM_SETGE; break;
                    default: return;
                }
                add_assembly(Assembly(Instruction::ASM_CMP, src_b, src_b_type,"%eax", VariableType::REG));
                add_assembly(Assembly(Instruction::ASM_MOVL, "$0", VariableType::IMM,"%eax", VariableType::REG));
                add_assembly(Assembly(condition, "%al", VariableType::REG));
            } else {
                // collect the instruction type
                Instruction instruction = binary_instruction(op);
                add_assembly(Assembly(instruction, src_b, src_b_type, "%eax", VariableType::REG));
            }
            add_assembly(Assembly(Instruction::ASM_MOVL, op == TackyOp::TACKY_MODULO ? "%edx" : "%eax", VariableType::REG, dest, dest_type));
            this->clean_up_required = true;
            consume_tacky(op);
        }

        /**
         * \brief Move the return value from src_a to %eax and emit RET.
         *
         * Consumes the return operation and requests cleanup for a temporary value.
         */
        void assemble_return() {
            // Get value from tacky
            const std::string value = this->tacky->front().get_src_a();
            const VariableType value_type = this->tacky->front().get_src_a_type();
            // If the source is a temporary variable, set the toggle for clean up
            if (this->tacky->front().get_src_a_type() == VariableType::TMP) {
                this->clean_up_required = true;
            }
            // mov(exp, reg)
            add_assembly(Assembly(Instruction::ASM_MOVL, value, value_type, "%eax", VariableType::REG));
            // ret
            add_assembly(Assembly(Instruction::ASM_RET));
            consume_tacky(TackyOp::TACKY_RETURN);
        }

        /**
         * \brief Lower one function through its first return operation.
         *
         * Requires a FUNCTION marker, followed by supported unary, binary,
         * copy, jump and label operations and a RETURN.
         */
        void assemble_function() {
            if (this->tacky->empty() || this->tacky->front().get_op() != TackyOp::TACKY_FUNCTION) {
                this->found_error = true;
                return;
            }
            consume_tacky(TackyOp::TACKY_FUNCTION);
            while (!this->tacky->empty() && this->tacky->front().get_op() != TackyOp::TACKY_RETURN) {
                switch (this->tacky->front().get_op()) {
                    case TackyOp::TACKY_COMPLEMENT:
                    case TackyOp::TACKY_NEGATE:
                    case TackyOp::TACKY_NOT: assemble_unary(); break;
                    case TackyOp::TACKY_ADD:
                    case TackyOp::TACKY_SUBTRACT:
                    case TackyOp::TACKY_MULTIPLY:
                    case TackyOp::TACKY_DIVIDE:
                    case TackyOp::TACKY_MODULO:
                    case TackyOp::TACKY_BW_AND:
                    case TackyOp::TACKY_BW_OR:
                    case TackyOp::TACKY_BW_XOR:
                    case TackyOp::TACKY_LEFT_SHIFT:
                    case TackyOp::TACKY_RIGHT_SHIFT:
                    case TackyOp::TACKY_EQUAL:
                    case TackyOp::TACKY_NOT_EQUAL:
                    case TackyOp::TACKY_LESS_THAN:
                    case TackyOp::TACKY_GREATER_THAN:
                    case TackyOp::TACKY_LESS_THAN_EQUAL:
                    case TackyOp::TACKY_GREATER_THAN_EQUAL: assemble_binary(); break;
                    case TackyOp::TACKY_COPY: {
                        // For a copy, we neet to extract the variables;
                        auto& item = this->tacky->front();
                        const auto src = item.get_src_a();
                        const auto src_type = item.get_src_a_type();
                        const auto dest = item.get_dest();
                        const auto dest_type = item.get_dest_type();

                        // And use a movl instruction
                        add_assembly(Assembly(Instruction::ASM_MOVL, src, src_type, dest, dest_type));

                        // If we find temporary variables, trigger the clean-up stage
                        if (src_type == VariableType::TMP || dest_type == VariableType::TMP) {
                            this->clean_up_required = true;
                        }

                        consume_tacky(TackyOp::TACKY_COPY);
                        break;
                    }
                    case TackyOp::TACKY_JUMP_IF_ZERO:
                    case TackyOp::TACKY_JUMP_IF_NOT_ZERO: {
                        // For Jumps, we need to extract the variables;
                        auto& item = this->tacky->front();
                        const auto op = item.get_op();
                        const auto value = item.get_src_a();
                        const auto value_type = item.get_src_a_type();
                        const auto label = item.get_dest();

                        // And in its place we insert a mov, a cmp, and the correct jump type
                        add_assembly(Assembly(Instruction::ASM_MOVL,value, value_type, "%r10d", VariableType::REG));
                        add_assembly(Assembly(Instruction::ASM_CMP,"$0", VariableType::IMM, "%r10d", VariableType::REG));
                        // Jump instructions have an empty source because they only need a destination
                        add_assembly(Assembly(op == TackyOp::TACKY_JUMP_IF_ZERO ? Instruction::ASM_JE : Instruction::ASM_JNE, "", VariableType::EMPTY, label, VariableType::LABEL));

                        // If we find temporary variables, trigger the clean-up stage
                        if (value_type == VariableType::TMP) {
                            this->clean_up_required = true;
                        }

                        consume_tacky(op);
                        break;
                    }
                    case TackyOp::TACKY_JUMP:
                    case TackyOp::TACKY_LABEL: {
                        // For Labels, we need to extract the variables;
                        auto& item = this->tacky->front();
                        const auto op = item.get_op();
                        const auto label = item.get_src_a();

                        // And in its place we insert a jmp or a label
                        if (op == TackyOp::TACKY_JUMP) {
                            // Jump instructions have an empty source because they only need a destination
                            add_assembly(Assembly(Instruction::ASM_JMP, "", VariableType::EMPTY, label, VariableType::LABEL));
                        } else {
                            // Labels however, use the source variable
                            add_assembly(Assembly(Instruction::ASM_LABEL, label, VariableType::LABEL));
                        }

                        consume_tacky(op);
                        break;
                    }
                    default: this->found_error = true; return;
                }
            }
            if (this->tacky->empty()) { this->found_error = true; return; }
            assemble_return();
        }

        /**
         * \brief Lower one function; trailing input is not checked.
         */
        void assemble_program() {
            assemble_function();
        }

    public:

        /**
         * \brief Construct without an input list; run() requires a valid list.
         */
        Compiler() = default; // Default

        /**
         * \brief Construct a compiler over a caller-owned Tacky list.
         *
         * \param tacky Non-null input list, which must outlive this object and
         * will be consumed through the first return by run().
         */
        explicit Compiler(std::list<Tacky> *tacky)
        : tacky{ tacky } {}

        /**
         * \brief Used to check if an error was found.
         *
         * \return True if the compiler detected an error, otherwise false.
         */
        bool had_error() const {
            return this->found_error;
        }

        /**
         * \brief Lower one function and return the resulting Assembly list.
         *
         * Requires a valid input pointer and consumes operations through the
         * first return; trailing operations are not checked. On an error the
         * result may be partial, and had_error() reports the failure. Cleanup
         * runs only when requested and no error was detected. State is not
         * reset between runs; use a fresh object for another pass.
         *
         * \return A copy of the generated Assembly instructions.
         */
        std::list<Assembly> run() {

#ifdef DEBUG_COMPILER
            std::cout << std::endl;
            std::cout << " === Beginning Compilation === " << std::endl;
            std::cout << std::endl;
#endif

            assemble_program();

#ifdef DEBUG_COMPILER
            std::cout << std::endl;
            std::cout << " === Finishing Compilation === " << std::endl;
            std::cout << std::endl;
#endif

            // Clean up temporary variables

            if (this->clean_up_required && !this->found_error) {
                auto clean = CleanUp(&this->assembly);
                clean.run();
                this->assembly = clean.get_cleaned_instructions();
            }

            // // If we still have TackyOp left over
            // if (TokenType::TK_EOF != this->tokens->front().get_type()) {
            //     this->found_error = true;
            // }

            return this->assembly;
        }
};

#endif
