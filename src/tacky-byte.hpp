/*
 * In this file we define our TackyByte class; Three Address Code
 */

#ifndef TACKY
#define TACKY

#include <string>

#include "tacky-op.hpp"
#include "tacky-val.hpp"

class TackyByte {

    private:

        TackyOp op;

        TackyVal src;
        TackyVal dst;

        std::string identifier;

    public:

        // Constructors

        TackyByte(void) {} // default

        TackyByte(TackyOp op, TackyVal src)
        : op{ op }, src{ src } {}

        TackyByte(TackyOp op, TackyVal src, TackyVal dst)
        : op{ op }, src{ src }, dst{ dst } {}

        TackyByte(TackyOp op, std::string identifier)
        : op{ op }, identifier{ identifier } {}

        // Accessors

        TackyOp get_op(void) {
            return this->op;
        }

        TackyVal get_src(void) {
            return this->src;
        }

        TackyVal get_dst(void) {
            return this->dst;
        }

        std::string get_identifier(void) {
            return this->identifier;
        }

        // Helpers

        const std::string to_string(void) {
            std::string out = "TACKY [op: " + tacky_op_string.at(this->op);

            if (this->op == TackyOp::TACKY_UNARY) {
                out += ", src: " + this->src.get_value();
                out += ", dst: " + this->dst.get_value();
            } else if (this->op == TackyOp::TACKY_RETURN) {
                out += ", src: " + this->src.get_value();
            } else if (this->op == TackyOp::TACKY_IDENTIFIER) {
                out += ", identifier: " + this->identifier;
            }

            return out + "]";
        }
};

#endif
