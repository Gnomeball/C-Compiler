/**
 * \file tackify.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implmentation of the Tackify class
 * \version 0.1
 * \date 2025-08-09
 */

#ifndef TACKIFY
#define TACKIFY

#include <list>
#include <string>

#include "../types/byte.hpp"
#include "../types/tacky.hpp"

/*
 * The aim of this class is to take in a list of Bytes;
 * and Tacify them to produce a list of Tacky.
 *
 * The primary interface will be a single public .run() method, which will
 * hand off to several private internal helper methods that encapsulate
 * the entire functionality of the class.
 *
 * This should keep things simple.
 */

class Tackify {

        std::list<Byte> *bytes;

        std::list<Tacky> tacky;

        int value_counter = 0;

        bool found_error = false;

    private:

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

        void tacky_constant() {
            // constant ::= OP_CONSTANT ( Value: integer )
            // this->tacky.push_back(Tacky(TackyOp::TACKY_RETURN, this->bytes->front().get_value(), ""));
            this->tacky.push_back(Tacky(TackyOp::TACKY_VALUE, "", this->bytes->front().get_value()));
            consume_byte(OpCode::OP_CONSTANT);
        }

        void tacky_unary() {
            // unary ::= OP_COMPLEMENT ( Value: integer )
            //         | OP_NEGATE     ( Value: integer )

            switch (this->bytes->front().get_op()) {
                case OpCode::OP_COMPLEMENT: {
                    // Get the value from the previous constant
                    std::string value = this->tacky.back().get_dest();
                    if (this->tacky.back().get_op() == TackyOp::TACKY_VALUE) {
                        this->tacky.pop_back();
                    }
                    // return that value
                    this->tacky.push_back(Tacky(TackyOp::TACKY_UNARY_COMPLEMENT, value, "tmp." + std::to_string(this->value_counter++)));
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
                    this->tacky.push_back(Tacky(TackyOp::TACKY_UNARY_NEGATE, value, "tmp." + std::to_string(this->value_counter++)));
                    consume_byte(OpCode::OP_NEGATE);
                    break;
                }
                // case OpCode::OP_DECREMENT: {
                //     break;
                // }
                default: return; // unreachable
            }

        }

        void tacky_return() {
            // return ::= OP_RETURN
            // Git destination value of previous tacky
            std::string value = this->tacky.back().get_dest();
            // Return it
            this->tacky.push_back(Tacky(TackyOp::TACKY_RETURN, value, ""));
            consume_byte(OpCode::OP_RETURN);
        }

        void tacky_function() {
            // function ::= value: name
            // We don't actually do anything with the name yet, so just skip it
            consume_byte(OpCode::OP_FUNCTION);
        }

        void tacky_program() {
            // program ::= { OP_FUNCTION ( Value: name    )
            //               OP_CONSTANT ( Value: integer )
            //               .. other Bytes ..
            //               OP_RETURN                      }
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
            tacky_program();

            // If we still have Tokens left over
            // if (TokenType::TK_EOF != this->tokens->front().get_type()) {
            //     this->found_error = true;
            // }

            return this->tacky;
        }
};

#endif
