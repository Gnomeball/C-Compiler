/*
 * In this file we define our the Tacky stage of compilation, taking in Bytes,
 * and outputting TackyBytes, containing TackyOps and TackyVals
 */

#include <iostream>
#include <list>
#include <stack>

#include "byte.hpp"
#include "data.hpp"
#include "debug.hpp"
#include "tacky-byte.hpp"

std::stack<Byte> byte_stack;

int tacky_error = 0;

void consume_byte(Byte *current_byte, OpCode expected) {
#ifdef DEBUG_TACKY
    std::cout << "Entered consume byte - " << current_byte->to_string() << std::endl;
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

void tackify_constant(Byte *current_byte) {
#ifdef DEBUG_TACKY
    std::cout << "Entered consume constant - " << current_byte->to_string() << std::endl;
#endif
    // add it to the stack
    byte_stack.push(*current_byte);
    // consume the byte, and return
    consume_byte(current_byte, OpCode::OP_CONSTANT);
}

void tackify_return(Byte *current_byte) {
#ifdef DEBUG_TACKY
    std::cout << "Entered consume return - " << current_byte->to_string() << std::endl;
#endif
    if (byte_stack.size() != 1) { // error
        std::cout << "Byte stack has " << byte_stack.size() << " items still!" << std::endl;
    }
    // pop the stack
    Byte byte_constant = byte_stack.top();
    byte_stack.pop();
    // build the tacky return
    TackyVal tv_value = TackyVal(TackyValType::TACKY_VALUE, byte_constant.get_value());
    TackyByte tb_return = TackyByte(TackyOp::TACKY_RETURN, tv_value);
    // add the tacky return to our TackyBytes
    tacky_bytes.push_back(tb_return);
    // consume the byte, and return
    consume_byte(current_byte, OpCode::OP_RETURN);
}

void tackify_identifier(Byte *current_byte) {
#ifdef DEBUG_TACKY
    std::cout << "Entered consume identifier - " << current_byte->to_string() << std::endl;
#endif
    // for now just create the TackyByte
    TackyByte tb = TackyByte(TackyOp::TACKY_IDENTIFIER, current_byte->get_value());
    // add it to the list
    tacky_bytes.push_back(tb);
    // and consume the byte
    consume_byte(current_byte, OpCode::OP_IDENTIFIER);
}

int generate_tacky(Byte *current_byte) {
#ifdef DEBUG_TACKY
    std::cout << "Entered generating Tacky" << std::endl;
#endif
    while (bytes.size() != 0) {
        switch (current_byte->get_op()) {
            // case OpCode::OP_COMPLEMENT:
            // case OpCode::OP_NEGATE:
            // case OpCode::OP_MINUS_MINUS:
            case OpCode::OP_CONSTANT: tackify_constant(current_byte); break;
            case OpCode::OP_RETURN: tackify_return(current_byte); break;
            case OpCode::OP_IDENTIFIER: tackify_identifier(current_byte); break;
            default: break;
        }
        if (!bytes.empty()) {
            *current_byte = bytes.front();
        }
    }

    return tacky_error;
}

/*

    return 2        ->  "AST": Return(Constant(2))

                    ->  Bytes: Constant(2)
                               Return

                    ->  Tacky: Return(Constant(2))

                    ->  ASM  : movl     $2, %eax       # move constant(2) into eax
                               retq                    # return



    return -2       ->  "AST": Return(Unary(Negate, Constant(2)))

                    ->  Bytes: Constant(2)
                               Unary(Negate)
                               Return

                    ->  Tacky: Unary(Negate, Constant(2), Var("tmp.0"))
                               Return(Var("tmp.0"))

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
