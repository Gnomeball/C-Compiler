/**
 * \file compiler.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implementation of the Compiler class
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
 * \brief A class outlining the Compiler class, which is used to Tacky into Assembly
 *
 * The aim of this class is to take in a list of Tacky;
 * and parse them to produce a list of Assembly.
 *
 * The primary interface will be a single public .run() method, which will
 * hand off to several private internal helper methods that encapsulate
 * the entire functionality of the class.
 *
 * This should keep things simple.
 */
class Compiler {

        /**
         * \brief The vector of Tacky this Compiler uses to build its Tacky vector
         */
        std::list<Tacky> *tacky{};

        /**
         * \brief A vector of Assembly built by this Compiler
         */
        std::list<Assembly> assembly;

        /**
         * \brief Set to true upon finding an error
         */
        bool found_error = false;

        /**
         * \brief Set to true upon finding any temporary variables
         */
        bool clean_up_required = false;

    private:

        /**
         * \brief Attempts to consume a Tacky with the expected TackyOp
         *
         * \param expected The expected TackyOp
         * \param message A potential error message to pass through to error Tacky
         *
         * // todo: currently this doesn't really do error handling
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
         * \brief Attempts to Compile a Unary
         *
         * Currently expected Tacky:
         *
         * unary ::= unary_op reg
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
                default: return;
            }
        }

        /**
         * \brief Helper function to convert TackyOp to a corresponding assembly instruction type
         *
         * @param op the operation we are converting
         * @return the corresponding assembly instruction
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
         * \brief Assembles a binary operation into assembly
         *
         * Currently expected Tacky:
         *
         * binary ::= binary_op src reg
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
                // left hand into eax
                add_assembly(Assembly(Instruction::ASM_MOVL, src_a, src_a_type, "%eax", VariableType::REG));
                // right hand into ecx
                add_assembly(Assembly(Instruction::ASM_MOVL, src_b, src_b_type, "%ecx", VariableType::REG));
                // then call sal/r on cl, eax
                add_assembly(Assembly(op == TackyOp::TACKY_LEFT_SHIFT ? Instruction::ASM_SAL : Instruction::ASM_SAR,
                    "%cl", VariableType::REG, "%eax", VariableType::REG));
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
         * \brief Attempts to Compile a Return
         *
         * Currently expected Tacky:
         *
         * return ::= return src
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
         * \brief Attempts to Compile a Function
         *
         * Currently expected Tacky:
         *
         * function ::= function unary* return
         *            | function return
         */
        void assemble_function() {
            // function ::= function unary* return
            //            | function return
            if (this->tacky->empty() || this->tacky->front().get_op() != TackyOp::TACKY_FUNCTION) {
                this->found_error = true;
                return;
            }
            consume_tacky(TackyOp::TACKY_FUNCTION);
            while (!this->tacky->empty() && this->tacky->front().get_op() != TackyOp::TACKY_RETURN) {
                switch (this->tacky->front().get_op()) {
                    case TackyOp::TACKY_COMPLEMENT:
                    case TackyOp::TACKY_NEGATE: assemble_unary(); break;
                    case TackyOp::TACKY_ADD:
                    case TackyOp::TACKY_SUBTRACT:
                    case TackyOp::TACKY_MULTIPLY:
                    case TackyOp::TACKY_DIVIDE:
                    case TackyOp::TACKY_MODULO:
                    case TackyOp::TACKY_BW_AND:
                    case TackyOp::TACKY_BW_OR:
                    case TackyOp::TACKY_BW_XOR:
                    case TackyOp::TACKY_LEFT_SHIFT:
                    case TackyOp::TACKY_RIGHT_SHIFT: assemble_binary(); break;
                    default: this->found_error = true; return;
                }
            }
            if (this->tacky->empty()) { this->found_error = true; return; }
            assemble_return();
        }

        /**
         * \brief Attempts to Compile a Program
         *
         * Currently expected Tacky:
         *
         * program ::= function
         */
        void assemble_program() {
            assemble_function();
        }

    public:

        /**
         * \brief Default constructor for a Compiler
         */
        Compiler() = default; // Default

        /**
         * \brief Construct a new Compiler object with a list of TackyOp
         *
         * \param tacky The list of TackyOp this Compiler should convert into Assembly
         */
        explicit Compiler(std::list<Tacky> *tacky)
        : tacky{ tacky } {}

        /**
         * \brief Used to check if an error was found.
         *
         * \return True if an error Token was produced, otherwise false.
         */
        bool had_error() const {
            return this->found_error;
        }

        /**
         * \brief Assembles the list of TackyOp and returns a list of found Assembly.
         *
         * \return A list of Assembly produced from the list of TackyOp
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
