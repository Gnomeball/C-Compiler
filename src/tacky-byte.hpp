/*
 * In this file we define our TackyByte class; Three Address Code
 */

#ifndef TACKY
#define TACKY

#include <list>
#include <string>

#include "tacky-op.hpp"
#include "tacky-val.hpp"

class TackyByte {

    private:

        TackyOp op;

        TackyVal src;
        TackyVal dst;

        std::string name;

        std::list<TackyByte> instructions;

    public:

        // Constructors

        TackyByte(void) {} // default

        TackyByte(TackyOp op, TackyVal src)
        : op{ op }, src{ src } {}

        TackyByte(TackyOp op, TackyVal src, TackyVal dst)
        : op{ op }, src{ src }, dst{ dst } {}

        TackyByte(TackyOp op, std::string name)
        : op{ op }, name{ name } {}

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
            return this->name;
        }

        std::list<TackyByte> get_instructions(void) {
            return this->instructions;
        }

        void add_instruction(TackyByte instruction) {
            this->instructions.push_back(instruction);
        }

        // Helpers

        const std::string to_string(void) {
            std::string out = "TACKY [op: " + tacky_op_string.at(this->op);

            if (this->op == TackyOp::TACKY_UNARY) {
                out += ", src: " + this->src.get_value();
                out += ", dst: " + this->dst.get_value();
            } else if (this->op == TackyOp::TACKY_RETURN) {
                out += ", src: " + this->src.get_value();
            } else if (this->op == TackyOp::TACKY_FUNCTION) {
                out += ", name: " + this->name;
                out += ", instructions: ";
                for (TackyByte tb : this->instructions) {
                    out += tb.to_string();
                }
            }

            return out + "]";
        }
};

#endif
