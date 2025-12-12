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

        std::list<Tacky> *tacky;

        std::list<Assembly> assembly;

        bool found_error = false;

    private:

        void consume_token(TackyOp expected, std::string message = "") {
            if (tacky->front().get_op() != expected) {
                // error
                this->assembly.push_back(Assembly(Instruction::ASM_ERROR, message));
                this->found_error = true;
            } else {
                // consume the token
                this->tacky->pop_front();
            }
        }

        void assemble_return() {
            // return ::= return src
            // Get value from tacky
            std::string value = this->tacky->front().get_src_a();
            // mov(exp, reg)
            this->assembly.push_back(Assembly(Instruction::ASM_MOV, value, "eax"));
            // retq
            this->assembly.push_back(Assembly(Instruction::ASM_RET));
            consume_token(TackyOp::TACKY_RETURN);
            return;
        }

        void assemble_unary() {
            // unary ::= unary_op reg
            std::string src = this->tacky->front().get_src_a();
            std::string dest = this->tacky->front().get_dest();

            switch (this->tacky->front().get_op()) {
                case TackyOp::TACKY_UNARY_COMPLEMENT: {
                    this->assembly.push_back(Assembly(Instruction::ASM_MOV, src, dest));
                    this->assembly.push_back(Assembly(Instruction::ASM_NOT, dest));
                    consume_token(TackyOp::TACKY_UNARY_COMPLEMENT);
                    break;
                }
                case TackyOp::TACKY_UNARY_NEGATE: {
                    this->assembly.push_back(Assembly(Instruction::ASM_MOV, src, dest));
                    this->assembly.push_back(Assembly(Instruction::ASM_NEG, dest));
                    consume_token(TackyOp::TACKY_UNARY_NEGATE);
                    break;
                }
                default: return;
            }
        }

        void assemble_function() {
            // function ::= function unary* return
            //            | function return
            consume_token(TackyOp::TACKY_FUNCTION);
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

        void assemble_program() {
            // program ::= function
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
            assemble_program();

            // // If we still have TackyOp left over
            // if (TokenType::TK_EOF != this->tokens->front().get_type()) {
            //     this->found_error = true;
            // }

            return this->assembly;
        }
};

#endif
