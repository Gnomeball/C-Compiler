/**
 * \file tacky.hpp
 * \author Gnomeball
 * \brief A file outlining the implementation of the Tacky class
 * \version 0.1
 * \date 2024-10-25
 */

#ifndef TACKY
#define TACKY

#include <string>
#include <utility>

#include "../enums/tacky-op.hpp"
#include "../enums/variable-type.hpp"

/**
 * \brief A three-address-like operation with typed operand fields.
 *
 * Meaningful fields depend on the operation; see grammar/tacky.md for the
 * complete conventions. Conditional jumps use dest for their target, while
 * unconditional jumps and label definitions use src_a. Operand types are
 * stored separately from their strings and must only be read when initialised.
 */
class Tacky {

    private:

        /**
         * \brief The TackyOp this Tacky contains
         */
        TackyOp op;

        /**
         * \brief The first source of this Tacky
         */
        std::string src_a;

        /**
         * \brief The Variable Type of the first source of this Tacky
         */
        VariableType src_a_type;

        /**
         * \brief The second source of this Tacky
         */
        std::string src_b;

        /**
         * \brief The Variable Type of the second source of this Tacky
         */
        VariableType src_b_type;

        /**
         * \brief The destination value for this Tacky
         */
        std::string dest;

        /**
         * \brief The Variable Type of the destination value for this Tacky
         */
        VariableType dest_type;

    public:

        // Constructors

        /**
         * \brief Default-construct a Tacky with empty operand strings.
         *
         * With default initialisation, op and operand types are uninitialised;
         * value initialisation instead value-initialises those enum fields.
         */
        Tacky() = default; // Default

        /**
         * \brief Construct a new Tacky object with a TackyOp and a single source
         *
         * The other operand strings are empty and their types are uninitialised.
         *
         * \param op Which TackyOp this Tacky carries
         * \param src The source value for this Tacky
         * \param src_type The Variable Type of the source value
         */
        Tacky(const TackyOp op, std::string src, const VariableType src_type)
            : op{op}, src_a{std::move(src)}, src_a_type{src_type}, src_b_type(), dest_type() {
        }

        /**
         * \brief Construct a new Tacky object with a TackyOp, a single source, and a destination
         *
         * The second source is empty and its type is uninitialised.
         *
         * \param op Which TackyOp this Tacky carries
         * \param src The source value for this Tacky
         * \param src_type The Variable Type of the source value
         * \param dest The destination value for this Tacky
         * \param dest_type The Variable Type of the destination value
         */
        Tacky(const TackyOp op, std::string src, const VariableType src_type, std::string dest, const VariableType dest_type)
            : op{op}, src_a{std::move(src)}, src_a_type{src_type}, src_b_type(), dest{std::move(dest)},
              dest_type{dest_type} {
        }

        /**
         * \brief Construct a new Tacky object with a TackyOp, two source values, and a destination
         *
         * \param op Which TackyOp this Tacky carries
         * \param src_a The first source value for this Tacky
         * \param src_a_type The Variable Type of the first source value
         * \param src_b The second source value for this Tacky
         * \param src_b_type The Variable Type of the second source value
         * \param dest The destination value for this Tacky
         * \param dest_type The Variable Type of the destination value
         */
        Tacky(TackyOp op, std::string src_a, VariableType src_a_type, std::string src_b, VariableType src_b_type, std::string dest, VariableType dest_type)
        : op{ op }, src_a{std::move( src_a )}, src_a_type{ src_a_type }, src_b{std::move( src_b )}, src_b_type{ src_b_type }, dest{std::move( dest )}, dest_type{ dest_type } {}

        // Accessors

        /**
         * \brief Get the TackyOp of this Tacky
         *
         * \return The TackyOp of the Tacky
         */
        TackyOp get_op() const {
            return this->op;
        }

        /**
         * \brief Get the first source of this Tacky
         *
         * \return The first source value of this Tacky
         */
        std::string get_src_a() {
            return this->src_a;
        }

        /**
         * \brief Get the type of the first source for this Tacky
         *
         * @return VariableType the type of the first source for this Tacky
         */
        VariableType get_src_a_type() const {
            return this->src_a_type;
        }

        /**
         * \brief Get the second source of this Tacky
         *
         * \return The second source value of this Tacky
         */
        std::string get_src_b() {
            return this->src_b;
        }

        /**
         * \brief Get the type of the second source for this Tacky
         *
         * @return VariableType the type of the second source for this Tacky
         */
        VariableType get_src_b_type() const {
            return this->src_b_type;
        }

        /**
         * \brief Get the destination of this Tacky
         *
         * \return The destination value of this Tacky
         */
        std::string get_dest() {
            return this->dest;
        }

        /**
         * \brief Get the type of the destination for this Tacky
         *
         * @return VariableType the type of the destination for this Tacky
         */
        VariableType get_dest_type() const {
            return this->dest_type;
        }

        // Helpers

        /**
         * \brief Return a selective debug representation of this Tacky.
         *
         * Includes the operation and selected operand strings according to the
         * operation; unhandled operations show only the operation. Operand types
         * are omitted. An unmapped operation can throw std::out_of_range.
         *
         * \return A string representational of this Tacky
         */
        const std::string to_string() const {
            std::string out = "Tacky [Op: " + tacky_op_string.at(this->op);

            switch (this->op) {
                case TackyOp::TACKY_COMPLEMENT:
                case TackyOp::TACKY_NEGATE:
                case TackyOp::TACKY_COPY:
                case TackyOp::TACKY_NOT: {
                    out += ", Source: " + this->src_a;
                    out += ", Dest: " + this->dest;
                    break;
                }
                // case TackyOp::TACKY_VALUE: {
                //     out += ", Dest: " + this->dest;
                //     break;
                // }
                case TackyOp::TACKY_ADD:
                case TackyOp::TACKY_SUBTRACT:
                case TackyOp::TACKY_MULTIPLY:
                case TackyOp::TACKY_DIVIDE:
                case TackyOp::TACKY_MODULO:
                case TackyOp::TACKY_BW_AND:
                case TackyOp::TACKY_BW_OR:
                case TackyOp::TACKY_BW_XOR:
                case TackyOp::TACKY_LEFT_SHIFT:
                case TackyOp::TACKY_RIGHT_SHIFT:
                case TackyOp::TACKY_EQUAL:
                case TackyOp::TACKY_NOT_EQUAL:
                case TackyOp::TACKY_LESS_THAN:
                case TackyOp::TACKY_GREATER_THAN:
                case TackyOp::TACKY_LESS_THAN_EQUAL:
                case TackyOp::TACKY_GREATER_THAN_EQUAL: {
                    out += ", Source A: " + this->src_a;
                    out += ", Source B: " + this->src_b;
                    out += ", Dest: " + this->dest;
                    break;
                }
                case TackyOp::TACKY_JUMP_IF_ZERO:
                case TackyOp::TACKY_JUMP_IF_NOT_ZERO: {
                    out += ", Source: " + this->src_a;
                    out += ", Target: " + this->dest;
                    break;
                }
                case TackyOp::TACKY_JUMP: {
                    out += ", Target: " + this->src_a;
                    break;
                }
                case TackyOp::TACKY_LABEL: {
                    out += ", Label: " + this->src_a;
                    break;
                }
                case TackyOp::TACKY_RETURN: {
                    out += ", Source: " + this->src_a;
                    break;
                }
                case TackyOp::TACKY_FUNCTION: {
                    out += ", Identifier: " + this->src_a;
                    break;
                }
                default: break;
            }

            return out + "]";
        }

        // Overrides
};

#endif // TACKY
