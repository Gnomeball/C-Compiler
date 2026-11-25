/**
 * \file tackify.hpp
 * \author Gnomeball
 * \brief A file outlining and specifying the implementation of the Tackify class
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
 * \brief A class outlining the Tackify class, which is used to turn Bytes into Tacky.
 *
 * The aim of this class is to take in a list of Bytes;
 * and Tackify them to produce a list of Tacky.
 *
 * The primary interface will be a single public .run() method, which will
 * hand off to several private internal helper methods that encapsulate
 * the entire functionality of the class.
 *
 * This should keep things simple.
 *
 * // todo: This file currently has little to no error checking / handling
 */
class Tackify {

        /**
         * \brief The vector of Bytes this Tackifier uses to build its Tacky vector
         */
        std::list<Byte> *bytes{};

        /**
         * \brief A vector of Tacky built by this Tackifier
         */
        std::list<Tacky> tacky;

        /**
         * \brief Used to track temporary variables in our Tacky (poorly implemented)
         *
         * // todo: this will likely need modifying before chapter two will work
         *
         * Currently tracks the number of values we find in a program,
         * in time it needs to track the number of operations we do in a function,
         * so that we can correctly use the stack and base pointers
         *
         * I think this probably needs to be an array, with a set 'depth', so that when
         * entering or exiting from a function we are using the counter at the correct level?
         *
         * Either way, this is basically one of the next things I need to be looking at.
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
            std::string value;
            VariableType type;
        };

        /**
         * \brief A vector containing all discovered operands.
         */
        std::vector<Operand> operands;

        /**
         * \brief Struct defining the outline of a logical context.
         */
        struct LogicalContext {
            OpCode begin_op;
            std::string result;
            std::string short_label;
            std::string end_label;
            std::size_t depth;
        };

        /**
         * \brief A vector containing all discovered logical contexts.
         */
        std::vector<LogicalContext> logical_contexts;

        /**
         * \brief A counter tracking the number of logical context jumps in the Tacky.
         */
        int label_counter = 0;

    private:

        /**
         * \brief Attempts to consume a Byte with the expected OpCode
         *
         * \param expected The expected OpCode
         * \param message A potential error message to pass through to error Bytes
         *
         * // todo: currently this doesn't really do any error handling
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
         * \brief Attempts to Tackify a Constant
         */
        void tacky_constant() {
            // todo: adding the $ here is such a bodge ..
            this->operands.push_back({"$" + this->bytes->front().get_value(), VariableType::IMM});
            consume_byte(OpCode::OP_CONSTANT);
        }

        /**
         * \brief Attempts to Tackify a Unary
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

        void tacky_and_begin() {
            if (this->operands.empty()) {
                error("Logical AND expects a left operand");
                return;
            }

            // Pop left
            const Operand left = this->operands.back();
            this->operands.pop_back();

            // Create context for this jump
            const int label_id = this->label_counter++;
            const LogicalContext context { OpCode::OP_AND_BEGIN,
                "tmp." + std::to_string(this->value_counter++),
                "false." + std::to_string(label_id),
                "end." + std::to_string(label_id),
                this->operands.size()
            };

            // Add the Tacky
            add_tacky(Tacky(TackyOp::TACKY_JUMP_IF_ZERO, left.value, left.type, context.short_label, VariableType::LABEL));

            // Store the context
            this->logical_contexts.push_back(context);

            // And consume the byte
            consume_byte(OpCode::OP_AND_BEGIN);
        }

        void tacky_and() {
            if (this->logical_contexts.empty() ||
                this->logical_contexts.back().begin_op != OpCode::OP_AND_BEGIN) {
                error("Logical AND has no matching begin marker");
                return;
            }

            // Grab the context
            const LogicalContext context = this->logical_contexts.back();
            if (this->operands.size() != context.depth + 1) {
                error("Logical AND requires a right side operand");
                return;
            }

            const Operand right = this->operands.back();
            this->operands.pop_back();
            this->logical_contexts.pop_back();

            // Add the Tacky
            add_tacky(Tacky(TackyOp::TACKY_JUMP_IF_ZERO,right.value, right.type,context.short_label, VariableType::LABEL));
            add_tacky(Tacky(TackyOp::TACKY_COPY,"$1", VariableType::IMM,context.result, VariableType::TMP));
            add_tacky(Tacky(TackyOp::TACKY_JUMP,context.end_label, VariableType::LABEL));
            add_tacky(Tacky(TackyOp::TACKY_LABEL,context.short_label, VariableType::LABEL));
            add_tacky(Tacky(TackyOp::TACKY_COPY,"$0", VariableType::IMM,context.result, VariableType::TMP));
            add_tacky(Tacky(TackyOp::TACKY_LABEL,context.end_label, VariableType::LABEL));

            // Push back the result
            this->operands.push_back({context.result, VariableType::TMP});

            // And consume the byte
            consume_byte(OpCode::OP_LOGIC_AND);
        }

        void tacky_or_begin() {
            if (this->operands.empty()) {
                error("Logical OR expects a left operand");
                return;
            }

            // Pop left
            const Operand left = this->operands.back();
            this->operands.pop_back();

            // Create context for this jump
            const int label_id = this->label_counter++;
            const LogicalContext context { OpCode::OP_OR_BEGIN,
                "tmp." + std::to_string(this->value_counter++),
                "true." + std::to_string(label_id),
                "end." + std::to_string(label_id),
                this->operands.size()
            };

            // Add the Tacky
            add_tacky(Tacky(TackyOp::TACKY_JUMP_IF_NOT_ZERO, left.value, left.type, context.short_label, VariableType::LABEL));

            // Store the context
            this->logical_contexts.push_back(context);

            // And consume the byte
            consume_byte(OpCode::OP_OR_BEGIN);
        }

        void tacky_or() {
            if (this->logical_contexts.empty() ||
                this->logical_contexts.back().begin_op != OpCode::OP_OR_BEGIN) {
                error("Logical OR has no matching begin marker");
                return;
            }

            // Grab the context
            const LogicalContext context = this->logical_contexts.back();
            if (this->operands.size() != context.depth + 1) {
                error("Logical OR requires a right side operand");
                return;
            }

            const Operand right = this->operands.back();
            this->operands.pop_back();
            this->logical_contexts.pop_back();

            // Add the Tacky
            add_tacky(Tacky(TackyOp::TACKY_JUMP_IF_NOT_ZERO,right.value, right.type,context.short_label, VariableType::LABEL));
            add_tacky(Tacky(TackyOp::TACKY_COPY,"$0", VariableType::IMM,context.result, VariableType::TMP));
            add_tacky(Tacky(TackyOp::TACKY_JUMP,context.end_label, VariableType::LABEL));
            add_tacky(Tacky(TackyOp::TACKY_LABEL,context.short_label, VariableType::LABEL));
            add_tacky(Tacky(TackyOp::TACKY_COPY,"$1", VariableType::IMM,context.result, VariableType::TMP));
            add_tacky(Tacky(TackyOp::TACKY_LABEL,context.end_label, VariableType::LABEL));

            // Push back the result
            this->operands.push_back({context.result, VariableType::TMP});

            // And consume the byte
            consume_byte(OpCode::OP_LOGIC_OR);
        }

        void error(const std::string& message) {
            add_tacky(Tacky(TackyOp::TACKY_ERROR, "empty", VariableType::IMM, message, VariableType::IMM));
            this->found_error = true;
        }

        /**
         * \brief Attempts to Tackify a Return
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
         * \brief Attempts to Tackify a Function
         */
        void tacky_function() {
            // Get src value of tacky as function name
            const std::string value = this->bytes->front().get_value();
            add_tacky(Tacky(TackyOp::TACKY_FUNCTION, value, VariableType::IMM));
            consume_byte(OpCode::OP_FUNCTION);
        }

        /**
         * \brief Attempts to Tackify a Program
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
                    case OpCode::OP_AND_BEGIN: tacky_and_begin(); break;
                    case OpCode::OP_LOGIC_AND: tacky_and(); break;
                    case OpCode::OP_OR_BEGIN: tacky_or_begin(); break;
                    case OpCode::OP_LOGIC_OR: tacky_or(); break;
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
         * \brief Default constructor for a Tackify-er
         */
        Tackify() = default; // Default

        /**
         * \brief Construct a new Tackify object with a list of Tokens
         *
         * \param bytes The vector of Bytes this Parser should convert into Tacky
         */
        explicit Tackify(std::list<Byte> *bytes)
        : bytes{ bytes } {}

        /**
         * \brief Used to check if an error was found.
         *
         * \return True if an error Byte was produced, otherwise false.
         */
        bool had_error() const {
            return this->found_error;
        }

        /**
         * \brief Parses the list of Bytes and returns a list of found Tacky.
         *
         * \return A list of Tacky produced from the list of Bytes
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

            // If we still have Tokens left over
            // if (TokenType::TK_EOF != this->tokens->front().get_type()) {
            //     this->found_error = true;
            // }

            return this->tacky;
        }
};

#endif // TACKIFY
