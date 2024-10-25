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

class Tacky {

    private:

        TackyOp op;

        std::string src_a;
        std::string src_b;

        std::string dest;

    public:

        // Constructors

        Tacky() {} // Default

        Tacky(TackyOp op, std::string src, std::string dest)
        : op{ op }, src_a{ src }, dest{ dest } {}

        Tacky(TackyOp op, std::string src_a, std::string src_b, std::string dest)
        : op{ op }, src_a{ src_a }, src_b{ src_b }, dest{ dest } {}

        // Accessors

        TackyOp get_op() {
            return this->op;
        }

        std::string get_src_a() {
            return this->src_a;
        }

        std::string get_src_b() {
            return this->src_b;
        }

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
                case TackyOp::TACKY_UNARY:
                case TackyOp::TACKY_VALUE: {
                    out += ", src_a: " + this->src_a;
                    out += ", src_b: " + this->src_b;
                    break;
                }
                case TackyOp::TACKY_RETURN:
                case TackyOp::TACKY_FUNCTION: {
                    out += ", src: " + this->src_a;
                    break;
                }
                default: break;
            }

            return out + "]";
        }

        // Overrides
};

#endif
