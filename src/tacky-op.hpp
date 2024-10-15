#include <map>
#include <string>

enum class TackyOp {
    // Operators
    TACKY_UNARY, // OP_COMPLEMENT | OP_NEGATE

    // Keywords
    TACKY_RETURN, // OP_RETURN

    // Identifier
    TACKY_IDENTIFIER, // OP_IDENTIFIER
};

const std::map<TackyOp, std::string> tacky_op_string = {
    { TackyOp::TACKY_UNARY, "UNARY" },
    { TackyOp::TACKY_RETURN, "RETURN" },
    { TackyOp::TACKY_IDENTIFIER, "IDENTIFIER" },
};
