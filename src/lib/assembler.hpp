/**
 * \file assemble.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implementation of the assembler class
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef ASSEMBLER
#define ASSEMBLER

#include <list>

#include "../types/assembly.hpp"
#include "../types/tacky.hpp"

/*
 * The aim of this class is to take in a list of Tacky;
 * and parse them to produce a list of Assembly.
 *
 * The primary interface will be a single public .run() method, which will
 * hand off to several private internal helper methods that encapsulate
 * the entire functionality of the class.
 *
 * This should keep things simple.
 */

class Assembler {

        std::list<Tacky> *tacky;

        std::list<Assembly> assembly;

        bool found_error = false;

    private:

        void consume_token(TackyOp expected, std::string message = "") {
            if (tacky->front().get_op() != expected) {
                // error
                this->assembly.push_back(Assembly(AssemblyOp::ASM_ERROR, message));
                this->found_error = true;
            } else {
                // consume the token
                this->tacky->pop_front();
            }
        }

        void assemble_return() {
            // return ::= "return" expression ";"
            // Get value from tacky
            std::string value = this->tacky->back().get_src_a();
            // mov(exp, reg)
            this->assembly.push_back(Assembly(AssemblyOp::ASM_MOV, value));
            // retq
            this->assembly.push_back(Assembly(AssemblyOp::ASM_RET));
            consume_token(TackyOp::TACKY_RETURN);
            return;
        }

        void assemble_function() {
            // function ::= "int" identifier "(" "void" ")" block
            assemble_return();
        }

        void assemble_program() {
            // program ::= function
            assemble_function();
        }

    public:

        /**
         * \brief Default constructor for an Assembler
         */
        Assembler() {} // Default

        /**
         * \brief Construct a new Assembler object with a list of TackyOp
         *
         * \param tacky The list of TackyOp this Assembler should convert into Assembly
         */
        Assembler(std::list<Tacky> *tacky)
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
         * \return A list of Asm produced from the list of TackyOp
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
