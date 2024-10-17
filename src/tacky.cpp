/*
 * In this file we define our the Tacky stage of compilation, taking in Bytes,
 * and outputting TackyBytes, containing TackyOps and TackyVals
 */

#include <iostream>
#include <list>

#include "byte.hpp"
#include "data.hpp"
#include "debug.hpp"
#include "tacky-byte.hpp"

int tacky_error = 0;

void consume_byte(Byte *current_byte, OpCode expected) {
#ifdef DEBUG_TACKY
    std::cout << "Entered tackify byte - " << current_byte->to_string() << std::endl;
#endif
    // if current byte not expected type
    if (current_byte->get_op() != expected) {
        std::cout << "Error found near " << current_byte->to_string() << std::endl;
        tacky_error = 1;
    } else {
        bytes.pop_front();
        if (!bytes.empty()) {
            *current_byte = bytes.front();
        }
    }
}

TackyVal tackify_constant(Byte *current_byte) {
#ifdef DEBUG_TACKY
    std::cout << "Entered tackify constant - " << current_byte->to_string() << std::endl;
#endif
    // Get and consume the constant
    TackyVal constant = TackyVal(TackyValType::TACKY_VALUE, current_byte->get_value());
    consume_byte(current_byte, OpCode::OP_CONSTANT);
    return constant;
}

TackyByte tackify_return(Byte *current_byte, TackyVal constant) {
#ifdef DEBUG_TACKY
    std::cout << "Entered tackify return - " << current_byte->to_string() << std::endl;
#endif
    // Box the constant into a return
    TackyByte ret = TackyByte(TackyOp::TACKY_RETURN, constant);
    consume_byte(current_byte, OpCode::OP_RETURN);
    return ret;
}

TackyByte tackify_instruction(Byte *current_byte) {
#ifdef DEBUG_TACKY
    std::cout << "Entered tackify instruction - " << current_byte->to_string() << std::endl;
#endif
    // instruction = Return(val)
    // val         = Constant(int)

    // Bytes: Constant(2)
    //        Return

    // Tacky: Return( Constant(2) )

    // Get and consume the constant
    TackyVal constant = tackify_constant(current_byte);

    // Box it into a Return instruction
    TackyByte ret = tackify_return(current_byte, constant);

    return ret;
}

void tackify_function(Byte *current_byte) {
#ifdef DEBUG_TACKY
    std::cout << "Entered tackify function - " << current_byte->to_string() << std::endl;
#endif
    // function = Function(identifier, instruction *body)

    // Bytes: Function("main")
    //        Constant(2)
    //        Return

    // Tacky: Function( "main", Return( Constant(2) ) )

    // Make a tacky function
    TackyByte function = TackyByte(TackyOp::TACKY_FUNCTION, current_byte->get_value());
    // and consume it's byte
    consume_byte(current_byte, OpCode::OP_FUNCTION);

    // Collect the instruction; eventually this will need to be in a loop/call or something
    TackyByte instruction = tackify_instruction(current_byte);
    function.add_instruction(instruction);

    tacky_bytes.push_back(function);
}

int generate_tacky(Byte *current_byte) {
#ifdef DEBUG_TACKY
    std::cout << "Entered generating Tacky" << std::endl;
#endif
    tackify_function(current_byte);

    // while (bytes.size() != 0) {
    //     switch (current_byte->get_op()) {
    //         // case OpCode::OP_COMPLEMENT:
    //         // case OpCode::OP_NEGATE:
    //         // case OpCode::OP_MINUS_MINUS:
    //         // case OpCode::OP_CONSTANT: tackify_constant(current_byte); break;
    //         // case OpCode::OP_RETURN: tackify_return(current_byte); break;
    //         case OpCode::OP_FUNCTION: tackify_function(current_byte); break;
    //         default: break;
    //     }
    //     if (!bytes.empty()) {
    //         *current_byte = bytes.front();
    //     }
    // }

    return tacky_error;
}

/*

    return 2        ->  Code : int main(void) { return 2; }

                    ->  Bytes: Function("main")
                               Constant(2)
                               Return

                    ->  Tacky: Function("main",
                                 Return(Constant(2))
                               )

                    ->  ASM  : movl     $2, %eax       # move constant(2) into eax
                               retq                    # return



    return -2       ->  Code : int main(void) { return -2; }

                    ->  Bytes: Function("main")
                               Constant(2)
                               Unary(Negate)
                               Return

                    ->  Tacky: Function("main",
                                 Unary(Negate, Constant(2), Var("tmp.0"))
                                 Return(Var("tmp.0"))
                               )

                    ->  ASM  : pushq    %rbp           # push base pointer onto the stack
                               movq     %rsp, %rbp     # move stack pointer to base pointer

                               subq     $4, %rbp       # subtract 4 from base pointer

                               movl     $2, -4(%rbp)   # move constant(2) into base pointer minus 4
                               negl     -4(%rbp)       # negate the value at base pointer minus 4

                               movl     -4(%rbp), %eax # move value at base pointer minus 4 into eax

                               movq     %rbp, %rsp     # move base pointer into stack pointer
                               popq     %rbp           # pop stack back into base pointer

                               retq                    # return





    return ~4       ->  "AST": Return(Unary(Complement, Constant(4)))

                    ->  Bytes: Constant(4)
                               Unary(Complement)
                               Return

                    ->  Tacky: Unary(Complement, Constant(4), Var("tmp.0"))
                               Return(Var("tmp.0"))

    return -(~5)    ->  "AST": Return(Unary(Negate, Unary(Complement, Constant(5))))

                    ->  Bytes: Constant(5)
                               Unary(Complement)
                               Unary(Negate)
                               Return

                    ->  Tacky: Unary(Complement, Constant(5), Var("tmp.0"))
                               Unary(Negate, Var("tmp.0"), Var("tmp.1"))
                               Return(Var("tmp.1"))

    return ~(-7)    ->  "AST": Return(Unary(Complement, Unary(Negate, Constant(7))))

                    ->  Bytes: Constant(7)
                               Unary(Negate)
                               Unary(Complement)
                               Return

                    ->  Tacky: Unary(Negate, Constant(7), Var("tmp.0"))
                               Unary(Complement, Var("tmp.0"), Var("tmp.1"))
                               Return(Var("tmp.1"))

*/
