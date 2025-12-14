/**
 * \file tacky.hpp
 * \author Gnomeball
 * \brief A file oulining the implementation of the Tacky class
 * \version 0.1
 * \date 2024-10-25
 */

#ifndef TACKY
#define TACKY

#include <string>

#include "../enums/tacky-op.hpp"

/**
 * \brief A class to outline the Tacky type
 */
class Tacky {

    private:

        /**
         * \brief The TackyOp this Tacky contains
         */
        TackyOp op;

        /**
         * \brief The first source of this Tacky
         */
        std::string src_a;

        /**
         * \brief The second source of this Tacky
         */
        std::string src_b;

        /**
         * \brief The destination value for this Tacky
         */
        std::string dest;

    public:

        // Constructors

        /**
         * \brief The default constructor for a Tacky object
         */
        Tacky() {} // Default

        /**
         * \brief Construct a new Tacky object with a TackyOp and a single source
         *
         * \param op Which OpCode this Tacky carries
         * \param src The source value for this Tacky
         */
        Tacky(TackyOp op, std::string src)
        : op{ op }, src_a{ src } {}

        /**
         * \brief Construct a new Tacky object with a TackyOp, a single source, and a destination
         *
         * \param op Which OpCode this Tacky carries
         * \param src The source value for this Tacky
         * \param dest The destination value for this Tacky
         */
        Tacky(TackyOp op, std::string src, std::string dest)
        : op{ op }, src_a{ src }, dest{ dest } {}

        /**
         * \brief Construct a new Tacky object with a TackyOp, two source values, and a destination
         *
         * \param op Which OpCode this Tacky carries
         * \param src_a The first source value for this Tacky
         * \param src_b The second source value for this Tacky
         * \param dest The destination value for this Tacky
         */
        Tacky(TackyOp op, std::string src_a, std::string src_b, std::string dest)
        : op{ op }, src_a{ src_a }, src_b{ src_b }, dest{ dest } {}

        // Accessors

        /**
         * \brief Get the TackyOp of this Tacky
         *
         * \return The TackyOp of the Tacky
         */
        TackyOp get_op() {
            return this->op;
        }

        /**
         * \brief Get the first source of this Tacky
         *
         * \return The first source value of this Tacky
         */
        std::string get_src_a() {
            return this->src_a;
        }

        /**
         * \brief Get the second source of this Tacky
         *
         * \return The second source value of this Tacky
         */
        std::string get_src_b() {
            return this->src_b;
        }

        /**
         * \brief Get the destination of this Tacky
         *
         * \return The destination value of this Tacky
         */
        std::string get_dest() {
            return this->dest;
        }

        // Helpers

        /**
         * \brief Returns a string contrining the information related to this Tacky
         *
         * \return A string represententation of this Tacky
         */
        const std::string to_string(void) {
            std::string out = "Tacky [Op: " + tacky_op_string.at(this->op);

            switch (this->op) {
                case TackyOp::TACKY_COMPLEMENT:
                case TackyOp::TACKY_NEGATE: {
                    out += ", Source: " + this->src_a;
                    out += ", Dest: " + this->dest;
                    break;
                }
                // case TackyOp::TACKY_VALUE: {
                //     out += ", Dest: " + this->dest;
                //     break;
                // }
                case TackyOp::TACKY_RETURN: {
                    out += ", Source: " + this->src_a;
                    break;
                }
                case TackyOp::TACKY_FUNCTION: {
                    out += ", Identifier: " + this->src_a;
                    break;
                }
                default: break;
            }

            return out + "]";
        }

        // Overrides
};

#endif // TACKY
