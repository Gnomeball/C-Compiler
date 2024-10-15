/*
 * In this file we define our TackyByte class; Three Address Code
 */

#ifndef TACKY_BYTE
    #define TACKY_BYTE

enum class TackyOp {
    // Operators
    TACKY_UNARY, // OP_COMPLEMENT | OP_NEGATE

    // Keywords
    TACKY_RETURN, // OP_RETURN
};

enum class TackyVal {
    // Values
    TACKY_CONSTANT, // OP_CONSTANT
    TACKY_VALUE,    // Temporary value
};

class TackyByte {

    private:

        TackyOp op;

        TackyVal src;
        TackyVal dst;

    public:

        // fixme : Implement this class!
};

#endif

/*

    return 2        ->  "AST": Return(Constant(2))

                    ->  Bytes: Constant(2)
                               Return

                    ->  Tacky: Return(Constant(2))

    return -2       ->  "AST": Return(Unary(Negate, Constant(2)))

                    ->  Bytes: Constant(2)
                               Unary(Negate)
                               Return

                    ->  Tacky: Unary(Negate, Constant(2), Var("tmp.0"))
                               Return(Var("tmp.0"))

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
