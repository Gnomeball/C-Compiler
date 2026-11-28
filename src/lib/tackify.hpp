/**
 * \file tackify.hpp
 * \author Gnomeball
 * \brief Lower postfix bytecode into Tacky operations.
 * \version 0.1
 * \date 2025-08-09
 */

#ifndef TACKIFY
#define TACKIFY

#include <list>
#include <string>
#include <vector>

#include "../types/byte.hpp"
#include "../types/tacky.hpp"

#ifdef DEBUG_TACKY
    #include <iostream>
#endif

/**
 * \brief Lower a caller-owned Byte list into a flat Tacky list.
 *
 * Postfix expression values are tracked on an operand stack; logical begin
 * markers open nested contexts for short-circuit control flow. See
 * grammar/tacky.md for the resulting operand conventions.
 */
class Tackify {

        /**
         * \brief Non-owning input pointer; successfully processed Bytes are consumed.
         */
        std::list<Byte> *bytes{};

        /**
         * \brief A list of Tacky built by this Tackifier
         */
        std::list<Tacky> tacky;

        /**
         * \brief Counter for generated tmp.N names, including logical results.
         */
        int value_counter = 0;

        /**
         * \brief Set to true upon finding an error.
         */
        bool found_error = false;

        /**
         * \brief Struct defining the outline of an operand.
         *
         * Operands contain both a value and a type.
         */
        struct Operand {
            std::string value; //!< The value of the operand
            VariableType type; //!< The type of this operand
        };

        /**
         * \brief Stack of expression values awaiting their postfix operators.
         */
        std::vector<Operand> operands;

        /**
         * \brief State retained while lowering the right side of a logical expression.
         *
         * The saved depth is the operand stack size after removing the left side;
         * exactly one right-side value must remain above it at the closing operator.
         */
        struct LogicalContext {
            OpCode begin_op;         //!< Begin marker used to match the closing operator
            std::string result;      //!< Temporary receiving the normalised result
            std::string short_label; //!< Target when either operand short-circuits
            std::string end_label;   //!< Target after assigning the non-short-circuit result
            std::size_t depth;       //!< Operand stack size before the right side
        };

        /**
         * \brief Stack of active logical contexts, innermost last.
         */
        std::vector<LogicalContext> logical_contexts;

        /**
         * \brief A counter tracking the number of logical context jumps in the Tacky.
         */
        int label_counter = 0;

    private:

        /**
         * \brief Consume the front Byte if its opcode matches.
         *
         * \param expected The expected OpCode
         * \param message Message placed in the TACKY_ERROR destination on failure.
         *
         * A null or empty input, or a mismatch, appends TACKY_ERROR and sets
         * found_error without consuming a Byte.
         */
        void consume_byte(const OpCode expected, const std::string& message = "") {
            if (!bytes || bytes->empty() || bytes->front().get_op() != expected) {
                // error
                this->tacky.emplace_back(TackyOp::TACKY_ERROR, "empty", VariableType::IMM, message, VariableType::IMM);
                this->found_error = true;
            } else {
                // consume the byte
                this->bytes->pop_front();
            }
        }

        /**
         * \brief Adds a Tacky to the list of found Tacky
         *
         * This function also facilitates debug output for the Tackifier
         *
         * \param tacky The Tacky
         */
        void add_tacky(const Tacky& tacky) {
#ifdef DEBUG_TACKY
            std::cout << "Found : " << tacky_op_string.at(tacky.get_op()) << std::endl;
#endif
            this->tacky.push_back(tacky);
        }

        /**
         * \brief Push the constant as a $-prefixed immediate and consume its Byte.
         *
         * No Tacky instruction is emitted; the value is used by a later operator.
         */
        void tacky_constant() {
            // todo: adding the $ here is such a bodge ..
            this->operands.push_back({"$" + this->bytes->front().get_value(), VariableType::IMM});
            consume_byte(OpCode::OP_CONSTANT);
        }

        /**
         * \brief Lower a unary operator using the top operand and a new temporary.
         *
         * Emits the operation, pushes its temporary result and consumes the Byte.
         * A missing operand records an error without consuming it. An unsupported
         * opcode records an error after removing the operand, then still pushes the
         * temporary and consumes the Byte.
         */
        void tacky_unary() {
            if (this->operands.empty()) {
                error("Unary operator requires an operand");
                return;
            }
            const OpCode op = this->bytes->front().get_op();
            const Operand operand = this->operands.back();
            this->operands.pop_back();
            const std::string dest = "tmp." + std::to_string(this->value_counter++);

            switch (op) {
                case OpCode::OP_COMPLEMENT:
                    add_tacky(Tacky(TackyOp::TACKY_COMPLEMENT,operand.value, operand.type, dest, VariableType::TMP));
                    break;
                case OpCode::OP_NEGATE:
                    add_tacky(Tacky(TackyOp::TACKY_NEGATE,operand.value, operand.type, dest, VariableType::TMP));
                    break;
                case OpCode::OP_NOT:
                    add_tacky(Tacky(TackyOp::TACKY_NOT,operand.value, operand.type, dest, VariableType::TMP));
                    break;
                default: error("Unknown operator");
            }
            this->operands.push_back({dest, VariableType::TMP});
            consume_byte(op);
        }

        /**
         * \brief Lower a binary operator from the top two operand stack values.
         *
         * Pops right then left, emits the operation into a new temporary, pushes
         * that result and consumes the Byte. Missing operands or an unsupported
         * opcode record an error without consuming the Byte or changing the stack.
         */
        void tacky_binary() {
            if (this->operands.size() < 2) {
                error("Binary operator requires two operands");
                return;
            }
            const OpCode op = this->bytes->front().get_op();
            TackyOp tacky_op;
            switch (op) {
                case OpCode::OP_ADD:                tacky_op = TackyOp::TACKY_ADD; break;
                case OpCode::OP_SUBTRACT:           tacky_op = TackyOp::TACKY_SUBTRACT; break;
                case OpCode::OP_MULTIPLY:           tacky_op = TackyOp::TACKY_MULTIPLY; break;
                case OpCode::OP_DIVIDE:             tacky_op = TackyOp::TACKY_DIVIDE; break;
                case OpCode::OP_MODULO:             tacky_op = TackyOp::TACKY_MODULO; break;
                case OpCode::OP_BW_AND:             tacky_op = TackyOp::TACKY_BW_AND; break;
                case OpCode::OP_BW_OR:              tacky_op = TackyOp::TACKY_BW_OR; break;
                case OpCode::OP_BW_XOR:             tacky_op = TackyOp::TACKY_BW_XOR; break;
                case OpCode::OP_LEFT_SHIFT:         tacky_op = TackyOp::TACKY_LEFT_SHIFT; break;
                case OpCode::OP_RIGHT_SHIFT:        tacky_op = TackyOp::TACKY_RIGHT_SHIFT; break;
                case OpCode::OP_EQUAL:              tacky_op = TackyOp::TACKY_EQUAL; break;
                case OpCode::OP_NOT_EQUAL:          tacky_op = TackyOp::TACKY_NOT_EQUAL; break;
                case OpCode::OP_LESS_THAN:          tacky_op = TackyOp::TACKY_LESS_THAN; break;
                case OpCode::OP_GREATER_THAN:       tacky_op = TackyOp::TACKY_GREATER_THAN; break;
                case OpCode::OP_LESS_THAN_EQUAL:    tacky_op = TackyOp::TACKY_LESS_THAN_EQUAL; break;
                case OpCode::OP_GREATER_THAN_EQUAL: tacky_op = TackyOp::TACKY_GREATER_THAN_EQUAL; break;
                default: error("Unexpected binary operator"); return;
            }
            const Operand right = this->operands.back();
            this->operands.pop_back();
            const Operand left = this->operands.back();
            this->operands.pop_back();
            const std::string dest = "tmp." + std::to_string(this->value_counter++);
            add_tacky(Tacky(tacky_op, left.value, left.type, right.value, right.type, dest, VariableType::TMP));
            this->operands.push_back({dest, VariableType::TMP});
            consume_byte(op);
        }

        /**
         * \brief Open a short-circuit context after evaluating the left operand.
         *
         * Pops the left operand, allocates a result temporary and labels, saves
         * the remaining stack depth, and emits a jump to the short label if the
         * left side is zero for AND or nonzero for OR. Contexts nest on a stack.
         * A missing left operand records an error without consuming the marker.
         *
         * \param op OP_AND_BEGIN or OP_OR_BEGIN.
         * \param short_label Prefix for the short-circuit label.
         */
        void tacky_jump_begin(OpCode op, const std::string& short_label) {
            if (this->operands.empty()) {
                error(op == OpCode::OP_AND_BEGIN ? "Logical AND requires a left side operand" : "Logical OR requires a left side operand");
                return;
            }

            // Pop left
            const Operand left = this->operands.back();
            this->operands.pop_back();

            // Create context for this jump
            const int label_id = this->label_counter++;
            const LogicalContext context { op,
                "tmp." + std::to_string(this->value_counter++),
                short_label + std::to_string(label_id),
                "end." + std::to_string(label_id),
                this->operands.size()
            };

            // Add the Tacky
            add_tacky(Tacky(op == OpCode::OP_AND_BEGIN ? TackyOp::TACKY_JUMP_IF_ZERO : TackyOp::TACKY_JUMP_IF_NOT_ZERO,
                left.value, left.type, context.short_label, VariableType::LABEL));

            // Store the context
            this->logical_contexts.push_back(context);

            // And consume the byte
            consume_byte(op);
        }

        /**
         * \brief Complete the innermost short-circuit context.
         *
         * Requires a matching begin marker and exactly one right operand above
         * its saved depth. Emits a conditional jump on that operand, copies 0 or
         * 1 into the result along both paths, and pushes the result temporary.
         * A mismatch or invalid depth records an error without consuming the Byte.
         *
         * \param op OP_LOGIC_AND or OP_LOGIC_OR.
         */
        void tacky_jump(OpCode op) {
            if (this->logical_contexts.empty() ||
                this->logical_contexts.back().begin_op != (op == OpCode::OP_LOGIC_AND ? OpCode::OP_AND_BEGIN : OpCode::OP_OR_BEGIN)) {
                error(op == OpCode::OP_LOGIC_AND
                    ? "Logical AND has no matching begin marker"
                    : "Logical OR has no matching begin marker");
                return;
            }

            // Grab the context
            const LogicalContext context = this->logical_contexts.back();
            if (this->operands.size() != context.depth + 1) {
                error(op == OpCode::OP_LOGIC_AND
                    ? "Logical AND requires a right side operand"
                    : "Logical OR requires a right side operand");
                return;
            }

            const Operand right = this->operands.back();
            this->operands.pop_back();
            this->logical_contexts.pop_back();

            // Add the Tacky
            add_tacky(Tacky(op == OpCode::OP_LOGIC_AND ? TackyOp::TACKY_JUMP_IF_ZERO : TackyOp::TACKY_JUMP_IF_NOT_ZERO,
                right.value, right.type,context.short_label, VariableType::LABEL));
            add_tacky(Tacky(TackyOp::TACKY_COPY,op == OpCode::OP_LOGIC_AND ? "$1" : "$0", VariableType::IMM,context.result, VariableType::TMP));
            add_tacky(Tacky(TackyOp::TACKY_JUMP,context.end_label, VariableType::LABEL));
            add_tacky(Tacky(TackyOp::TACKY_LABEL,context.short_label, VariableType::LABEL));
            add_tacky(Tacky(TackyOp::TACKY_COPY,op == OpCode::OP_LOGIC_AND ? "$0" : "$1", VariableType::IMM,context.result, VariableType::TMP));
            add_tacky(Tacky(TackyOp::TACKY_LABEL,context.end_label, VariableType::LABEL));

            // Push back the result
            this->operands.push_back({context.result, VariableType::TMP});

            // And consume the byte
            consume_byte(op);

        }

        /**
         * \brief Append a TACKY_ERROR and set the error flag.
         *
         * @param message The error message to add.
         */
        void error(const std::string& message) {
            add_tacky(Tacky(TackyOp::TACKY_ERROR, "empty", VariableType::IMM, message, VariableType::IMM));
            this->found_error = true;
        }

        /**
         * \brief Emit a return from the sole operand and consume the Byte.
         *
         * If the stack does not contain exactly one operand, records an error
         * without consuming the Byte.
         */
        void tacky_return() {
            if (this->operands.size() != 1) {
                error("Return requires exactly one operand");
                return;
            }
            const Operand operand = this->operands.back();
            this->operands.pop_back();
            // Return the value
            add_tacky(Tacky(TackyOp::TACKY_RETURN, operand.value, operand.type, "", VariableType::IMM));
            consume_byte(OpCode::OP_RETURN);
        }

        /**
         * \brief Emit a function record using the front Byte's name and consume it.
         */
        void tacky_function() {
            // Get src value of tacky as function name
            const std::string value = this->bytes->front().get_value();
            add_tacky(Tacky(TackyOp::TACKY_FUNCTION, value, VariableType::IMM));
            consume_byte(OpCode::OP_FUNCTION);
        }

        /**
         * \brief Lower one function and its postfix expression through return.
         *
         * Requires a function Byte first, then dispatches expression Bytes until
         * a return or error. Missing return, unsupported Bytes and Bytes after a
         * return are errors; output can be partial and input can remain unconsumed.
         */
        void tacky_program() {
            if (!this->bytes || this->bytes->empty() || this->bytes->front().get_op() != OpCode::OP_FUNCTION) {
                error("Expected function");
                return;
            }
            tacky_function();
            while (!this->bytes->empty() && !this->found_error) {
                switch (this->bytes->front().get_op()) {
                    case OpCode::OP_CONSTANT: tacky_constant(); break;
                    case OpCode::OP_COMPLEMENT:
                    case OpCode::OP_NEGATE:
                    case OpCode::OP_NOT: tacky_unary(); break;
                    case OpCode::OP_ADD:
                    case OpCode::OP_SUBTRACT:
                    case OpCode::OP_MULTIPLY:
                    case OpCode::OP_DIVIDE:
                    case OpCode::OP_MODULO:
                    case OpCode::OP_BW_AND:
                    case OpCode::OP_BW_OR:
                    case OpCode::OP_BW_XOR:
                    case OpCode::OP_LEFT_SHIFT:
                    case OpCode::OP_RIGHT_SHIFT:
                    case OpCode::OP_EQUAL:
                    case OpCode::OP_NOT_EQUAL:
                    case OpCode::OP_LESS_THAN:
                    case OpCode::OP_GREATER_THAN:
                    case OpCode::OP_LESS_THAN_EQUAL:
                    case OpCode::OP_GREATER_THAN_EQUAL: tacky_binary(); break;
                    case OpCode::OP_AND_BEGIN: tacky_jump_begin(OpCode::OP_AND_BEGIN, "false."); break;
                    case OpCode::OP_LOGIC_AND: tacky_jump(OpCode::OP_LOGIC_AND); break;
                    case OpCode::OP_OR_BEGIN: tacky_jump_begin(OpCode::OP_OR_BEGIN, "true."); break;
                    case OpCode::OP_LOGIC_OR: tacky_jump(OpCode::OP_LOGIC_OR); break;
                    case OpCode::OP_RETURN:
                        tacky_return();
                        if (!this->found_error && !this->bytes->empty()) {
                            error("Unexpected bytes after return");
                        }
                        return;
                    default: error("Unexpected byte in expression"); break;
                }
            }
            if (!this->found_error) error("Expected return");
        }

    public:

        /**
         * \brief Construct without an input list; run() will report an error.
         */
        Tackify() = default; // Default

        /**
         * \brief Construct a tackifier over a caller-owned Byte list.
         *
         * \param bytes Input list, which must outlive the tackifier and is consumed
         * from the front during processing; null input is reported as an error.
         */
        explicit Tackify(std::list<Byte> *bytes)
        : bytes{ bytes } {}

        /**
         * \brief Used to check if an error was found.
         *
         * \return True if tackification recorded an error, otherwise false.
         */
        bool had_error() const {
            return this->found_error;
        }

        /**
         * \brief Lower the input and return the generated Tacky list.
         *
         * Consumes successfully processed Bytes from the caller's list. Errors
         * set had_error() and append a TACKY_ERROR; output may be partial and
         * input may remain. State is not reset; use a fresh tackifier per pass.
         *
         * \return A copy of the generated Tacky list.
         */
        std::list<Tacky> run() {

#ifdef DEBUG_TACKY
            std::cout << std::endl;
            std::cout << " === Beginning Tackify === " << std::endl;
            std::cout << std::endl;
#endif

            tacky_program();

#ifdef DEBUG_TACKY
            std::cout << std::endl;
            std::cout << " === Finishing Tackify === " << std::endl;
            std::cout << std::endl;
#endif

            return this->tacky;
        }
};

#endif // TACKIFY
