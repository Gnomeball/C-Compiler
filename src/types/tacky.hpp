/**
 * \file tacky.hpp
 * \author Gnomeball
 * \brief A file oulining the implementation of the Tacky class
 * \version 0.1
 * \date 2024-10-25
 */

#ifndef TACKY
#define TACKY

#include <string>

#include "../enums/tacky-op.hpp"
#include "../enums/variable-type.hpp"

/**
 * \brief A class to outline the Tacky type
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

        VariableType src_a_type;

        /**
         * \brief The second source of this Tacky
         */
        std::string src_b;

        VariableType src_b_type;

        /**
         * \brief The destination value for this Tacky
         */
        std::string dest;

        VariableType dest_type;

    public:

        // Constructors

        /**
         * \brief The default constructor for a Tacky object
         */
        Tacky() {} // Default

        /**
         * \brief Construct a new Tacky object with a TackyOp and a single source
         *
         * \param op Which OpCode this Tacky carries
         * \param src The source value for this Tacky
         * \param src_type The Variable Type of the source value
         */
        Tacky(TackyOp op, std::string src, VariableType src_type)
        : op{ op }, src_a{ src }, src_a_type{ src_type } {}

        /**
         * \brief Construct a new Tacky object with a TackyOp, a single source, and a destination
         *
         * \param op Which OpCode this Tacky carries
         * \param src The source value for this Tacky
         * \param src_type The Variable Type of the source value
         * \param dest The destination value for this Tacky
         * \param dest_type The Variable Type of the destination value
         */
        Tacky(TackyOp op, std::string src, VariableType src_type, std::string dest, VariableType dest_type)
        : op{ op }, src_a{ src }, src_a_type{ src_type }, dest{ dest }, dest_type{ dest_type } {}

        /**
         * \brief Construct a new Tacky object with a TackyOp, two source values, and a destination
         *
         * \param op Which OpCode this Tacky carries
         * \param src_a The first source value for this Tacky
         * \param src_a_type The Variable Type of the first source value
         * \param src_b The second source value for this Tacky
         * \param src_b_type The Variable Type of the second source value
         * \param dest The destination value for this Tacky
         * \param dest_type The Variable Type of the destination value
         */
        Tacky(TackyOp op, std::string src_a, VariableType src_a_type, std::string src_b, VariableType src_b_type, std::string dest, VariableType dest_type)
        : op{ op }, src_a{ src_a }, src_a_type{ src_a_type }, src_b{ src_b }, src_b_type{ src_b_type }, dest{ dest }, dest_type{ dest_type } {}

        // Accessors

        /**
         * \brief Get the TackyOp of this Tacky
         *
         * \return The TackyOp of the Tacky
         */
        TackyOp get_op() {
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

        VariableType get_src_a_type() {
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

        VariableType get_src_b_type() {
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

        VariableType get_dest_type() {
            return this->dest_type;
        }

        // Helpers

        /**
         * \brief Returns a string contrining the information related to this Tacky
         *
         * \return A string represententation of this Tacky
         */
        const std::string to_string(void) {
            std::string out = "Tacky [Op: " + tacky_op_string.at(this->op);

            switch (this->op) {
                case TackyOp::TACKY_COMPLEMENT:
                case TackyOp::TACKY_NEGATE: {
                    out += ", Source: " + this->src_a;
                    out += ", Dest: " + this->dest;
                    break;
                }
                // case TackyOp::TACKY_VALUE: {
                //     out += ", Dest: " + this->dest;
                //     break;
                // }
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
