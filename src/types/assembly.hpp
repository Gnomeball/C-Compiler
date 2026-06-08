/**
 * \file assembly.hpp
 * \author Gnomeball
 * \brief A file outlining the implementation of the Asm class
 * \version 0.1
 * \date 2025-12-11
 */

#ifndef ASSEMBLY
#define ASSEMBLY

#include <string>
#include <utility>

#include "../enums/instructions.hpp"
#include "../enums/variable-type.hpp"

/**
 * \brief A class to outline the Assembly type
 */
class Assembly {

    private:

        /**
         * \brief The Instruction value of this Assembly object
         */
        Instruction instruction;

        /**
         * \brief The source value for this Assembly Instruction
         */
        std::string src;

        VariableType src_type;

        /**
         * \brief The destination value for this Assembly Instruction
         */
        std::string dest;

        VariableType dest_type;

    public:

        // Constructors

        /**
         * \brief The default constructor for an Assembly object
         */
        Assembly() = default; // default

        /**
         * \brief Construct a new Assembly object with an Instruction
         *
         * \param instruction The Instruction this Assembly refers to
         */
        explicit Assembly(const Instruction instruction)
            : instruction{ instruction } {
        }

        /**
         * \brief Construct a new Assembly object with an Instruction and a source
         *
         * \param instruction The Instruction this Assembly refers to
         * \param src The source for the value against this Assembly Instruction
         * \param src_type The Variable Type this source is
         */
        Assembly(const Instruction instruction, std::string src, const VariableType src_type)
        : instruction{ instruction }, src{std::move( src )}, src_type{ src_type } {}

        /**
         * \brief Construct a new Assembly object with an Instruction, a source, and a destination
         *
         * \param instruction The Instruction this Assembly refers to
         * \param src The source for the value against this Assembly Instruction
         * \param src_type The Variable Type this source is
         * \param dest The destination for the value against this Assembly Instruction
         * \param dest_type The Variable Type this destination is
         */
        Assembly(const Instruction instruction, std::string src, const VariableType src_type, std::string dest, const VariableType dest_type)
        : instruction{ instruction }, src{std::move( src )}, src_type{ src_type }, dest{std::move( dest )}, dest_type{ dest_type } {}

        // Accessors

        /**
         * \brief Get the Instruction from this Assembly
         *
         * \return The Instruction value from this Assembly
         */
        Instruction get_instruction() const {
            return this->instruction;
        }

        /**
         * \brief Get the source for this Assembly
         *
         * \return The source value for this Assembly
         */
        std::string get_src() {
            return this->src;
        }

        void set_src(const std::string &source) {
            this->src = source;
        }

        VariableType get_src_type() const {
            return this->src_type;
        }

        void set_src_type(const VariableType type) {
            this->src_type = type;
        }

        /**
         * \brief Get the destination for this Assembly
         *
         * \return The destination value for this Assembly
         */
        std::string get_dest() {
            return this->dest;
        }

        void set_dest(const std::string &destination) {
            this->dest = destination;
        }

        VariableType get_dest_type() const {
            return this->dest_type;
        }

        void set_dest_type(const VariableType type) {
            this->src_type = type;
        }

        // Helpers

        /**
         * \brief Returns a string containing the information related to this Assembly
         *
         * \return A string representational of this Assembly Instruction
         */
        std::string to_string() const {
            std::string out = "Assembly [Ins: " + asm_string.at(this->instruction);

            // if (this->instruction == Instruction::ASM_IDENT) {
            //     out += ", Value: " + this->value;
            // } else

            switch (this->instruction) {
                case Instruction::ASM_MOVL:
                case Instruction::ASM_MOVQ: {
                    out += ", src: " + this->src + ", dest: " + this->dest;
                    break;
                }
                case Instruction::ASM_NOT:
                case Instruction::ASM_NEG: {
                    out += ", reg: " + this->src;
                    break;
                }
                default:;
            }

            return out + "]";
        }

        // Overrides
};

#endif // ASSEMBLY
