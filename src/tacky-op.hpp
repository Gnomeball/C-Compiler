#include <map>
#include <string>

enum class TackyOp {
    // Operators
    TACKY_UNARY, // OP_COMPLEMENT | OP_NEGATE

    // Values
    TACKY_VALUE, // Identifier to a temporary variable

    // Keywords
    TACKY_RETURN, // OP_RETURN

    // Function
    TACKY_FUNCTION, // OP_FUNCTION
};

const std::map<TackyOp, std::string> tacky_op_string = {
    { TackyOp::TACKY_UNARY, "UNARY" },
    { TackyOp::TACKY_VALUE, "VALUE" },
    { TackyOp::TACKY_RETURN, "RETURN" },
    { TackyOp::TACKY_FUNCTION, "FUNCTION" },
};
