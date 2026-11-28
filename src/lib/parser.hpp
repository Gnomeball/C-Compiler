/**
 * \file parser.hpp
 * \author Gnomeball
 * \brief Parse tokens into bytecode for the tackification stage.
 * \version 0.1
 * \date 2024-10-24
 */

#ifndef PARSER
#define PARSER

#include <cstdint>
#include <list>
#include <string>

#include "../enums/error-type.hpp"
#include "../types/byte.hpp"
#include "../types/error.hpp"
#include "../types/token.hpp"

#ifdef DEBUG_PARSER
    #include <iostream>
#endif

/**
 * \brief Parse a caller-owned token list into a flat Byte stream.
 *
 * Parses one function containing a return expression. Expression operators
 * are emitted in postfix order, with markers for short-circuiting. See
 * grammar/byte.md for the bytecode representation.
 */
class Parser {

        /**
         * \brief Non-owning pointer to the input list, consumed by run().
         */
        std::list<Token> *tokens{};

        /**
         * \brief A list of Bytes built by this Parser
         */
        std::list<Byte> bytes;

        /**
         * \brief Set to true upon finding an error
         */
        bool found_error = false;

        /**
         * \brief A list of any Errors found
         */
         std::list<Error> errors;

    private:
        /**
         * \brief Convert an unsigned literal constant to a signed decimal string.
         *
         * Selects hexadecimal for a 0x or 0X prefix, octal for a leading zero
         * on a multi-character constant, and decimal otherwise. The magnitude
         * must fit in 2147483647, or 2147483648 when negative is true. Invalid
         * digits and overflow fail; callers supply a tokenised literal constant.
         *
         * \param constant The literal constant without a sign.
         * \param negative Whether to negate the magnitude.
         * \param value Receives the decimal string on success; unchanged on failure.
         *
         * \return True on successful conversion, false on invalid digits or overflow.
         */
        static bool convert_integer_literal(const std::string& constant, const bool negative, std::string& value) {
            // Offset used to track if this constant has any kind of prefix
            size_t offset = 0;

            // Base used to track the base of the constant
            int base = 10;

            // Capture the actual base, and potentially update the offset
            if (constant.length() > 1 && constant[0] == '0' &&
                (constant[1] == 'x' || constant[1] == 'X')) {
                // Hexadecimal
                base = 16;
                offset = 2;
            } else if (constant.length() > 1 && constant[0] == '0') {
                // Octal
                base = 8;
            }

            // If the constant is negative, we need to handle the special case of -2147483648
            const std::uint64_t maximum = negative ? 2147483648ULL : 2147483647ULL;

            // Otherwise, we can just capture it as normal
            std::uint64_t magnitude = 0;

            // Iterate over the digits of the constant
            for (size_t index = offset; index < constant.length(); ++index) {
                const char digit = constant[index];
                int digit_value = -1;
                if (digit >= '0' && digit <= '9') {
                    digit_value = digit - '0';
                } else if (digit >= 'a' && digit <= 'f') {
                    digit_value = digit - 'a' + 10;
                } else if (digit >= 'A' && digit <= 'F') {
                    digit_value = digit - 'A' + 10;
                }

                // If at any point we encounter an invalid digit, we immediately return false
                if (digit_value < 0 || digit_value >= base ||
                    magnitude > (maximum - static_cast<std::uint64_t>(digit_value)) /
                        static_cast<std::uint64_t>(base)) {
                    return false;
                }

                // For each digit, we multiply the current magnitude by the base and add the digit value
                magnitude = magnitude * static_cast<std::uint64_t>(base) + static_cast<std::uint64_t>(digit_value);
            }

            if (negative) {
                // Handle the special case of -2147483648
                value = magnitude == 2147483648ULL? "-2147483648"
                    : std::to_string(-static_cast<std::int64_t>(magnitude));
            } else {
                // Otherwise, we just convert the magnitude to a string as-is
                value = std::to_string(magnitude);
            }

            return true;
        }

        /**
         * \brief Copy the front token's line and position to a Byte.
         *
         * \param byte Non-null Byte to update. The token list must be nonempty.
         */
        void set_position_of(Byte *byte) const {
            byte->set_byte_position(
                this->tokens->front().get_line(),
                this->tokens->front().get_position(),
                this->tokens->front().get_length());
        }

        /**
         * \brief Consume the front token if its type matches.
         *
         * \param expected The expected TokenType
         * \param reason Diagnostic reason used on mismatch.
         *
         * Requires a nonempty token list. A mismatch leaves the token in place,
         * appends an error Byte and a positioned Error, and sets found_error.
         */
        void consume_token(const TokenType expected, const ErrorType reason = ErrorType::ET_NONE) {
            if (tokens->front().get_type() != expected) {
                // error
                auto temp = Byte(OpCode::OP_ERROR, error_type_string(reason), reason);
                set_position_of(&temp);
                add_byte(temp);
                this->errors.emplace_back(temp);
                this->found_error = true;
            } else {
                // consume the token
                this->tokens->pop_front();
            }
        }

        /**
         * \brief Adds a Byte to the list of found Bytes
         *
         * This function also facilitates debug output for the Parser
         *
         * \param byte The Byte
         */
        void add_byte(const Byte& byte) {
#ifdef DEBUG_PARSER
            std::cout << "Found : " << op_code_string.at(byte.get_op()) << std::endl;
#endif
            this->bytes.push_back(byte);
        }

        /**
         * \brief Emit an OP_FUNCTION Byte for the front token and expect an identifier.
         *
         * Currently identifiers are only used as function names. The Byte is
         * emitted before the token type is checked by consume_token().
         */
        void parse_identifier() {
            // Right now this only supports function names, in time it needs to support variables
            add_byte(Byte(OpCode::OP_FUNCTION, this->tokens->front().get_value()));
            consume_token(TokenType::TK_IDENTIFIER, ErrorType::ET_EXPECTED_IDENTIFIER);
        }

        /**
         * \brief Attempts to Parse a Constant
         *
         * Grammar:
         *
         * constant    ::= integer
         * integer     ::= decimal | octal | hexadecimal
         * decimal     ::= digit { digit }
         * octal       ::= "0" octdigit { octdigit }
         * octdigit    ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7"
         * hexadecimal ::= ( "0x" | "0X" ) hexdigit { hexdigit }
         * hexdigit    ::= digit | "a" ... "f" | "A" ... "F"
         *
         * Conversion failure records an error using the token's full span and
         * emits an error Byte; otherwise an OP_CONSTANT carries the decimal value.
         * The constant token is consumed in either case.
         *
         * \param negative Whether a preceding minus is folded into the constant.
         */
        void parse_constant(const bool negative = false) {
            Token& token = this->tokens->front();
            std::string value;
            if (!convert_integer_literal(token.get_value(), negative, value)) {
                constexpr ErrorType reason = ErrorType::ET_UNSUPPORTED_NUMERIC_LITERAL;
                Token error(TokenType::TK_ERROR, token.get_value(), reason, token.get_line(), token.get_position(), token.get_length());
                this->errors.emplace_back(error);
                this->found_error = true;

                Byte error_byte(OpCode::OP_ERROR, error_type_string(reason), reason);
                error_byte.set_byte_position(token.get_line(), token.get_position(), token.get_length());
                add_byte(error_byte);
            } else {
                if (negative && value != "-2147483648" && value[0] != '-') {
                    value.insert(value.begin(), '-');
                }
                add_byte(Byte(OpCode::OP_CONSTANT, value));
            }
            consume_token(TokenType::TK_CONSTANT);
        }

        /**
         * \brief Attempts to Parse a Factor
         *
         * A factor is the operand of a binary expression: a unary operator
         * applied to another factor, a parenthesised expression, or an integer.
         *
         * Note: This function may change substantially, or be removed entirely in future.
         */
        void parse_factor() {
            switch (this->tokens->front().get_type()) {
                // unary_op factor
                case TokenType::TK_TILDE: {
                    consume_token(TokenType::TK_TILDE);
                    parse_factor();
                    add_byte(Byte(OpCode::OP_COMPLEMENT));
                    break;
                }
                case TokenType::TK_BANG: {
                    consume_token(TokenType::TK_BANG);
                    parse_factor();
                    add_byte(Byte(OpCode::OP_NOT));
                    break;
                }
                case TokenType::TK_MINUS: {
                    consume_token(TokenType::TK_MINUS);
                    // A '-' immediately followed by an integer is folded into a
                    // single negative constant; otherwise it negates the factor.
                    if (this->tokens->front().get_type() == TokenType::TK_CONSTANT) {
                        parse_constant(true);
                    } else {
                        parse_factor();
                        add_byte(Byte(OpCode::OP_NEGATE));
                    }
                    break;
                }
                // "(" expression ")"
                case TokenType::TK_OPEN_PARENTHESIS: {
                    consume_token(TokenType::TK_OPEN_PARENTHESIS);
                    parse_expression();
                    consume_token(TokenType::TK_CLOSE_PARENTHESIS, ErrorType::ET_EXPECTED_CLOSE_PARENTHESIS);
                    break;
                }
                // integer
                case TokenType::TK_CONSTANT: {
                    parse_constant();
                    break;
                }
                default: {
                    // error, expected a factor
                    consume_token(TokenType::TK_CONSTANT, ErrorType::ET_EXPECTED_EXPRESSION);
                }
            }
        }

        /**
         * \brief Returns the precedence of a binary operator Token
         *
         * A higher value binds more tightly. Any Token that is not a binary_op
         * returns -1, which is below every valid minimum precedence and so ends
         * the precedence-climbing loop in parse_expression.
         *
         * \param type The TokenType to check
         * \return The precedence of the operator, or -1 if it is not a binary_op
         */
        static int precedence_of(const TokenType type) {
            switch (type) {
                case TokenType::TK_STAR:
                case TokenType::TK_SLASH:
                case TokenType::TK_PERCENTAGE:
                    return 50;
                case TokenType::TK_PLUS:
                case TokenType::TK_MINUS:
                    return 45;
                case TokenType::TK_LEFT_CHEVRONS:
                case TokenType::TK_RIGHT_CHEVRONS:
                    return 40;
                case TokenType::TK_LESS:
                case TokenType::TK_LESS_EQUAL:
                case TokenType::TK_GREATER:
                case TokenType::TK_GREATER_EQUAL:
                    return 39;
                case TokenType::TK_EQUAL_EQUAL:
                case TokenType::TK_BANG_EQUAL:
                    return 38;
                case TokenType::TK_AMPERSAND:
                    return 35;
                case TokenType::TK_CARET:
                    return 30;
                case TokenType::TK_PIPE:
                    return 25;
                case TokenType::TK_AMPE_AMPE:
                    return 20;
                case TokenType::TK_PIPE_PIPE:
                    return 15;
                default:
                    return -1;
            }
        }

        /**
         * \brief Helper function to map an operator Token to its OpCode
         *
         * \param type The TokenType of the operator
         *
         * \return The matching OpCode, or OP_ERROR for an unsupported token.
         */
        static OpCode op_code_for(const TokenType type) {
            switch (type) {
                case TokenType::TK_PLUS:           return OpCode::OP_ADD;
                case TokenType::TK_MINUS:          return OpCode::OP_SUBTRACT;
                case TokenType::TK_STAR:           return OpCode::OP_MULTIPLY;
                case TokenType::TK_SLASH:          return OpCode::OP_DIVIDE;
                case TokenType::TK_PERCENTAGE:     return OpCode::OP_MODULO;
                case TokenType::TK_AMPERSAND:      return OpCode::OP_BW_AND;
                case TokenType::TK_PIPE:           return OpCode::OP_BW_OR;
                case TokenType::TK_CARET:          return OpCode::OP_BW_XOR;
                case TokenType::TK_LEFT_CHEVRONS:  return OpCode::OP_LEFT_SHIFT;
                case TokenType::TK_RIGHT_CHEVRONS: return OpCode::OP_RIGHT_SHIFT;
                case TokenType::TK_AMPE_AMPE:      return OpCode::OP_LOGIC_AND;
                case TokenType::TK_PIPE_PIPE:      return OpCode::OP_LOGIC_OR;
                case TokenType::TK_EQUAL_EQUAL:    return OpCode::OP_EQUAL;
                case TokenType::TK_BANG_EQUAL:     return OpCode::OP_NOT_EQUAL;
                case TokenType::TK_LESS:           return OpCode::OP_LESS_THAN;
                case TokenType::TK_GREATER:        return OpCode::OP_GREATER_THAN;
                case TokenType::TK_LESS_EQUAL:     return OpCode::OP_LESS_THAN_EQUAL;
                case TokenType::TK_GREATER_EQUAL:  return OpCode::OP_GREATER_THAN_EQUAL;
                default:                           return OpCode::OP_ERROR;
            }
        }

        /**
         * \brief Attempts to Parse an Expression
         *
         * Uses precedence climbing so that binary operators chain with the
         * correct precedence and left-associativity. The operator Byte is
         * emitted last (postfix), after both of its operands, keeping the flat
         * Byte stream consumed downstream valid. For && and ||, an OP_AND_BEGIN
         * or OP_OR_BEGIN marker is emitted after the left operand and before
         * the right, allowing the tackifier to generate short-circuit control flow.
         *
         * Grammar:
         *
         * expression     ::= logical_or
         * logical_or     ::= logical_and { "||" logical_and }
         * logical_and    ::= bitwise_or { "&&" bitwise_or }
         * bitwise_or     ::= bitwise_xor { "|" bitwise_xor }
         * bitwise_xor    ::= bitwise_and { "^" bitwise_and }
         * bitwise_and    ::= equality { "&" equality }
         * equality       ::= relational { ( "==" | "!=" ) relational }
         * relational     ::= shift { ( "<" | ">" | "<=" | ">=" ) shift }
         * shift          ::= additive { ( "<<" | ">>" ) additive }
         * additive       ::= multiplicative { ( "+" | "-" ) multiplicative }
         * multiplicative ::= factor { ( "*" | "/" | "%" ) factor }
         * factor         ::= ( "-" | "~" | "!" ) factor | "(" expression ")" | integer
         *
         * \param min_precedence The minimum operator precedence this call may consume
         */
        void parse_expression(const int min_precedence = 0) {
            //! Currently maybe a bodge?, but does it enables us to pass tests?
            // I'm asking this because at one point this function was starting a process to pop an empty list
            if (this->tokens->empty()) {
                this->bytes.emplace_back(OpCode::OP_ERROR,"",ErrorType::ET_EXPECTED_EXPRESSION);
                this->found_error = true;
                return;
            }

            // Left operand
            parse_factor();

            // { binary_op factor }
            while (precedence_of(this->tokens->front().get_type()) >= min_precedence) {
                const TokenType op = this->tokens->front().get_type();
                const int precedence = precedence_of(op);

                // Consume the next token
                consume_token(op);

                // Insert label bytes for conditional logic
                if (op == TokenType::TK_AMPE_AMPE) {
                    add_byte(Byte(OpCode::OP_AND_BEGIN));
                } else if (op == TokenType::TK_PIPE_PIPE) {
                    add_byte(Byte(OpCode::OP_OR_BEGIN));
                }

                // Parse the right operand at a higher minimum precedence so that
                // operators of equal precedence are left-associative.
                parse_expression(precedence + 1);
                // Emit the operator Byte last (postfix).
                add_byte(Byte(op_code_for(op)));
            }
        }

        /**
         * \brief Attempts to Parse a Return Statement
         *
         * Grammar:
         *
         * return ::= "return" expression ";"
         */
        void parse_return() {
            consume_token(TokenType::TK_KEYWORD_RETURN, ErrorType::ET_EXPECTED_RETURN_KEYWORD);
            parse_expression();
            add_byte(Byte(OpCode::OP_RETURN));
            consume_token(TokenType::TK_SEMI_COLON, ErrorType::ET_EXPECTED_SEMICOLON);
        }

        /**
         * \brief Attempts to Parse a Block
         *
         * Grammar:
         *
         * block ::= "{" return "}"
         */
        void parse_block() {
            consume_token(TokenType::TK_OPEN_BRACE, ErrorType::ET_EXPECTED_OPEN_BRACE);
            parse_return();
            consume_token(TokenType::TK_CLOSE_BRACE, ErrorType::ET_EXPECTED_CLOSE_BRACE);
        }

        /**
         * \brief Attempts to Parse a Function
         *
         * Grammar:
         *
         * function ::= "int" identifier "(" [ "void" ] ")" block
         *
         * Note: The void keyword is optional
         */
        void parse_function() {
            consume_token(TokenType::TK_KEYWORD_INT, ErrorType::ET_EXPECTED_INT_KEYWORD);
            parse_identifier();
            consume_token(TokenType::TK_OPEN_PARENTHESIS, ErrorType::ET_EXPECTED_OPEN_PARENTHESIS);
            // void keyword should be optional
            if (this->tokens->front().get_type() == TokenType::TK_KEYWORD_VOID) {
                consume_token(TokenType::TK_KEYWORD_VOID);
            }
            consume_token(TokenType::TK_CLOSE_PARENTHESIS, ErrorType::ET_EXPECTED_CLOSE_PARENTHESIS);
            parse_block();
        }

        /**
         * \brief Attempts to Parse a Program
         *
         * Grammar:
         *
         * program ::= function
         */
        void parse_program() {
            parse_function();
        }

    public:

        /**
         * \brief Construct without an input list; run() requires a valid list.
         */
        Parser() = default; // Default

        /**
         * \brief Construct a parser over a caller-owned token list.
         *
         * \param tokens Non-null list that must outlive the parser. Parsing
         * consumes matching tokens from its front; it must end with an EOF token.
         */
        explicit Parser(std::list<Token> *tokens)
        : tokens{ tokens } {}

        /**
         * \brief Used to check if an error was found.
         *
         * \return True if parsing detected an error, including trailing tokens.
         */
        bool had_error() const {
            return this->found_error;
        }

        /**
         * \brief Get the list of Errors
         *
         * \return The list of Errors, if any were found
         */
        std::list<Error> get_errors() {
            return this->errors;
        }

        /**
         * \brief Parse one function and return the resulting Byte list.
         *
         * Requires a valid, nonempty token list terminated by EOF. Matching
         * tokens are consumed. Mismatches can leave tokens unconsumed and the
         * returned bytecode may be partial or contain error Bytes. Trailing tokens
         * toggle the error flag and produce a diagnostic byte.
         *
         * \return A copy of the generated Bytes.
         */
        std::list<Byte> run() {

#ifdef DEBUG_PARSER
            std::cout << std::endl;
            std::cout << " === Beginning Parsing === " << std::endl;
            std::cout << std::endl;
#endif

            parse_program();

#ifdef DEBUG_PARSER
            std::cout << std::endl;
            std::cout << " === Finishing Parsing === " << std::endl;
            std::cout << std::endl;
#endif

            // Check we've used up every token
            if (!this->found_error && this->tokens->front().get_type() != TokenType::TK_EOF) {
                consume_token(TokenType::TK_EOF, ErrorType::ET_UNEXPECTED_TOKEN_AFTER_FUNCTION);
            }

            return this->bytes;
        }
};

#endif // PARSER
