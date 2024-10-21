#include <map>
#include <string>

enum class OpCode {
    // Operators
    OP_COMPLEMENT, // TK_TILDE
    OP_NEGATE,     // TK_MINUS

    // OP_MINUS, // TK_MINUS

    OP_MINUS_MINUS, // TK_MINUS_MINUS

    // Values
    OP_CONSTANT, // TK_CONSTANT

    // Keywords
    OP_RETURN, // TK_RETURN

    // Function
    OP_FUNCTION, // TK_IDENTIFIER () {}

    // Identifier
    // OP_IDENTIFIER, // TK_IDENT
};

const std::map<OpCode, std::string> op_code_string = {
    // Operators
    { OpCode::OP_COMPLEMENT, "COMPLEMENT" },
    { OpCode::OP_NEGATE, "NEGATE" },

    // { OpCode::OP_MINUS, "MINUS" }.

    { OpCode::OP_MINUS_MINUS, "MINUS_MINUS" },

    // Values
    { OpCode::OP_CONSTANT, "CONSTANT" },

    // Keywords
    { OpCode::OP_RETURN, "RETURN" },

    // Function
    { OpCode::OP_FUNCTION, "OP_FUNCTION" },

    // Identifier
    // { OpCode::OP_IDENTIFIER, "IDENTIFIER" },
};
