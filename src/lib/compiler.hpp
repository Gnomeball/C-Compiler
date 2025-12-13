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
        std::list<Tacky> *tacky;

        /**
         * \brief A vector of Assembly built by this Compiler
         */
        std::list<Assembly> assembly;

        /**
         * \brief Set to true upon finding an error
         */
        bool found_error = false;

    private:

        /**
         * \brief Attempts to consume a Tacky with the expected TackyOp
         *
         * \param expected The expected TackyOp
         * \param message A potential error message to pass through to error Tacky
         *
         * // todo: currently this doesn't really do error handling
         */
        void consume_tacky(TackyOp expected, std::string message = "") {
            if (tacky->front().get_op() != expected) {
                // error
                this->assembly.push_back(Assembly(Instruction::ASM_ERROR, message));
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
        void add_assembly(Assembly assembly) {
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
            std::string src = this->tacky->front().get_src_a();
            std::string dest = this->tacky->front().get_dest();

            switch (this->tacky->front().get_op()) {
                case TackyOp::TACKY_UNARY_COMPLEMENT: {
                    add_assembly(Assembly(Instruction::ASM_MOV, src, dest));
                    add_assembly(Assembly(Instruction::ASM_NOT, dest));
                    consume_tacky(TackyOp::TACKY_UNARY_COMPLEMENT);
                    break;
                }
                case TackyOp::TACKY_UNARY_NEGATE: {
                    add_assembly(Assembly(Instruction::ASM_MOV, src, dest));
                    add_assembly(Assembly(Instruction::ASM_NEG, dest));
                    consume_tacky(TackyOp::TACKY_UNARY_NEGATE);
                    break;
                }
                default: return;
            }
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
            std::string value = this->tacky->front().get_src_a();
            // mov(exp, reg)
            add_assembly(Assembly(Instruction::ASM_MOV, value, "eax"));
            // retq
            add_assembly(Assembly(Instruction::ASM_RET));
            consume_tacky(TackyOp::TACKY_RETURN);
            return;
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
            consume_tacky(TackyOp::TACKY_FUNCTION);
            if (this->tacky->front().get_op() == TackyOp::TACKY_RETURN) {
                assemble_return();
            } else {
                // Unary
                while (this->tacky->front().get_op() != TackyOp::TACKY_RETURN) {
                    assemble_unary();
                }
                assemble_return();
            }
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
        Compiler() {} // Default

        /**
         * \brief Construct a new Compiler object with a list of TackyOp
         *
         * \param tacky The list of TackyOp this Compiler should convert into Assembly
         */
        Compiler(std::list<Tacky> *tacky)
        : tacky{ tacky } {}

        /**
         * \brief Used to check if an error was found.
         *
         * \return True if an error Token was produced, otherwise false.
         */
        bool had_error() {
            return this->found_error;
        }

        /**
         * \brief Assembles the list of TackyOp and returns a list of found Assembly.
         *
         * \return A list of Assembly produced from the list of TackyOp
         */
        std::list<Assembly> run() {

#ifdef DEBUG_TACKY
            std::cout << std::endl;
            std::cout << " === Beginning Compilation === " << std::endl;
            std::cout << std::endl;
#endif

            assemble_program();

#ifdef DEBUG_TACKY
            std::cout << std::endl;
            std::cout << " === Finishing Compilation === " << std::endl;
            std::cout << std::endl;
#endif

            // // If we still have TackyOp left over
            // if (TokenType::TK_EOF != this->tokens->front().get_type()) {
            //     this->found_error = true;
            // }

            return this->assembly;
        }
};

#endif
