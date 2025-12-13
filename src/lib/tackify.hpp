/**
 * \file tackify.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implementation of the Tackify class
 * \version 0.1
 * \date 2025-08-09
 */

#ifndef TACKIFY
#define TACKIFY

#include <list>
#include <string>

#include "../types/byte.hpp"
#include "../types/tacky.hpp"

#ifdef DEBUG_TACKY
    #include <iostream>
#endif

/**
 * \brief A class outlining the Tackify class, which is used to turn Bytes into Tacky.
 *
 * The aim of this class is to take in a list of Bytes;
 * and Tacify them to produce a list of Tacky.
 *
 * The primary interface will be a single public .run() method, which will
 * hand off to several private internal helper methods that encapsulate
 * the entire functionality of the class.
 *
 * This should keep things simple.
 *
 * // todo: This file currently has little to no error checking / handling
 */
class Tackify {

        /**
         * \brief The vector of Bytes this Tackifier uses to build its Tacky vector
         */
        std::list<Byte> *bytes;

        /**
         * \brief A vector of Tacky built by this Tackifier
         */
        std::list<Tacky> tacky;

        /**
         * \brief Used to track temporary variables in our Tacky (poorly implemented)
         *
         * // todo: this will likely need modifying before chapter two will work
         *
         * Currently tracks the number of values we find in a program,
         * in time it needs to track the number of operations we do in a function,
         * so that we can correctly use the stack and base pointers
         *
         * I think this probably needs to be an array, with a set 'depth', so that when
         * entering or exiting from a function we are using the counter at the correct level?
         *
         * Either way, this is basically one of the next things I need to be looking at.
         */
        int value_counter = 0;

        /**
         * \brief Set to true upon finding an error
         */
        bool found_error = false;

    private:

        /**
         * \brief Attempts to consume a Byte with the expected OpCode
         *
         * \param expected The expected OpCode
         * \param message A potential error message to pass through to error Bytes
         *
         * // todo: currently this doesn't really do any error handling
         */
        void consume_byte(OpCode expected, std::string message = "") {
            if (bytes->front().get_op() != expected) {
                // error
                this->tacky.push_back(Tacky(TackyOp::TACKY_ERROR, "empty", message));
                this->found_error = true;
            } else {
                // consume the byte
                this->bytes->pop_front();
            }
        }

        /**
         * \brief Adds a Tacky to the list of found Tacky
         *
         * This function also facilitates debug output for the Tackifier
         *
         * \param tacky The Tacky
         */
        void add_tacky(Tacky tacky) {
#ifdef DEBUG_TACKY
            std::cout << "Found : " << tacky_op_string.at(tacky.get_op()) << std::endl;
#endif
            this->tacky.push_back(tacky);
        }

        /**
         * \brief Attempts to Tackify a Constant
         *
         * Currently expected Bytes:
         *
         * constant ::= OP_CONSTANT ( Value: integer )
         */
        void tacky_constant() {
            add_tacky(Tacky(TackyOp::TACKY_VALUE, "", this->bytes->front().get_value()));
            consume_byte(OpCode::OP_CONSTANT);
        }

        /**
         * \brief Attempts to Tackify a Unary
         *
         * Currently expected Bytes
         *
         * unary ::= OP_COMPLEMENT ( Value: integer )
         *         | OP_NEGATE     ( Value: integer )
         */
        void tacky_unary() {
            switch (this->bytes->front().get_op()) {
                case OpCode::OP_COMPLEMENT: {
                    // Get the value from the previous constant
                    std::string value = this->tacky.back().get_dest();
                    if (this->tacky.back().get_op() == TackyOp::TACKY_VALUE) {
                        this->tacky.pop_back();
                    }
                    // return that value
                    add_tacky(Tacky(TackyOp::TACKY_UNARY_COMPLEMENT, value, "tmp." + std::to_string(this->value_counter++)));
                    consume_byte(OpCode::OP_COMPLEMENT);
                    break;
                }
                case OpCode::OP_NEGATE: {
                    // Get the value from the previous constant
                    std::string value = this->tacky.back().get_dest();
                    if (this->tacky.back().get_op() == TackyOp::TACKY_VALUE) {
                        this->tacky.pop_back();
                    }
                    // return that value
                    add_tacky(Tacky(TackyOp::TACKY_UNARY_NEGATE, value, "tmp." + std::to_string(this->value_counter++)));
                    consume_byte(OpCode::OP_NEGATE);
                    break;
                }
                // case OpCode::OP_DECREMENT: {
                //     break;
                // }
                default: return; // unreachable
            }
        }

        /**
         * \brief Attempts to Tackify a Return
         *
         * Currently expected Bytes:
         *
         * return ::= OP_RETURN
         */
        void tacky_return() {
            //
            // Get destination value of previous tacky
            std::string value = this->tacky.back().get_dest();
            // If previous tacky was a value, pop it
            if (this->tacky.back().get_op() == TackyOp::TACKY_VALUE) {
                this->tacky.pop_back();
            }
            // Return the value
            add_tacky(Tacky(TackyOp::TACKY_RETURN, value, ""));
            consume_byte(OpCode::OP_RETURN);
        }

        /**
         * \brief Attempts to Tackify a Function
         *
         * Currently expected Bytes:
         *
         * function ::= OP_FUNCTION ( Value: name    )
         */
        void tacky_function() {
            // Get src value of tacky as function name
            std::string value = this->bytes->front().get_value();
            add_tacky(Tacky(TackyOp::TACKY_FUNCTION, value));
            consume_byte(OpCode::OP_FUNCTION);
        }

        /**
         * \brief Attempts to Tackify a Program
         *
         * Currently expected Bytes:
         *
         * program ::= { OP_FUNCTION ( Value: name    )
         *               OP_CONSTANT ( Value: integer )
         *               .. other Bytes ..
         *               OP_RETURN                      }
         */
        void tacky_program() {
            tacky_function();
            tacky_constant();
            // Check if we have outstanding bytes
            while (this->bytes->front().get_op() != OpCode::OP_RETURN) {
                tacky_unary();
            }
            tacky_return();
        }

    public:

        /**
         * \brief Default constructor for a Tackify-er
         */
        Tackify() {} // Default

        /**
         * \brief Construct a new Tackify object with a list of Tokens
         *
         * \param bytes The vector of Bytes this Parser should convert into Tacky
         */
        Tackify(std::list<Byte> *bytes)
        : bytes{ bytes } {}

        /**
         * \brief Used to check if an error was found.
         *
         * \return True if an error Byte was produced, otherwise false.
         */
        bool had_error() {
            return this->found_error;
        }

        /**
         * \brief Parses the list of Bytes and returns a list of found Tacky.
         *
         * \return A list of Tacky produced from the list of Bytes
         */
        std::list<Tacky> run() {

#ifdef DEBUG_TACKY
            std::cout << std::endl;
            std::cout << " === Beginning Tackify === " << std::endl;
            std::cout << std::endl;
#endif

            tacky_program();

#ifdef DEBUG_TACKY
            std::cout << std::endl;
            std::cout << " === Finishing Tackify === " << std::endl;
            std::cout << std::endl;
#endif

            // If we still have Tokens left over
            // if (TokenType::TK_EOF != this->tokens->front().get_type()) {
            //     this->found_error = true;
            // }

            return this->tacky;
        }
};

#endif // TACKIFY
