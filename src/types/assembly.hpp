/**
 * \file assembly.hpp
 * \author Gnomeball
 * \brief A file outlining the implementation of the Assembly class
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
 * \brief An assembly instruction record with typed operand fields.
 *
 * Operand use depends on the instruction; see grammar/assembly.md for the
 * complete conventions. Jump targets use dest, whereas label definitions use
 * src. Operand types are stored separately and must only be read when initialised.
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

        /**
         * \brief The source type for this Assembly Instruction
         */
        VariableType src_type;

        /**
         * \brief The destination value for this Assembly Instruction
         */
        std::string dest;

        /**
         * \brief The destination type for this Assembly Instruction
         */
        VariableType dest_type;

    public:

        // Constructors

        /**
         * \brief Default-construct an Assembly with empty operand strings.
         *
         * With default initialisation, instruction and operand types are
         * uninitialised; value initialisation value-initialises those enums.
         */
        Assembly() = default; // default

        /**
         * \brief Construct a new Assembly object with an Instruction
         *
         * Both operand strings are empty and their types are uninitialised.
         *
         * \param instruction The Instruction this Assembly refers to
         */
        explicit Assembly(const Instruction instruction)
            : instruction{ instruction } {
        }

        /**
         * \brief Construct a new Assembly object with an Instruction and a source
         *
         * The destination string is empty and its type is uninitialised.
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

        /**
         * \brief Set the source for this Assembly
         *
         * \param source The source value for this Assembly
         */
        void set_src(const std::string &source) {
            this->src = source;
        }

        /**
         * \brief Get the source type for this Assembly
         *
         * \return The source type for this Assembly
         */
        VariableType get_src_type() const {
            return this->src_type;
        }

        /**
         * \brief Set the source type for this Assembly
         *
         * \param type The source type for this Assembly
         */
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

        /**
         * \brief Get the destination type for this Assembly
         *
         * \return The destination type for this Assembly
         */
        VariableType get_dest_type() const {
            return this->dest_type;
        }

        /**
         * \brief Set the destination for this Assembly
         *
         * \param destination The destination value for this Assembly
         */
        void set_dest(const std::string &destination) {
            this->dest = destination;
        }

        /**
         * \brief Set the destination type for this Assembly
         *
         * \param type The destination type for this Assembly
         */
        void set_dest_type(const VariableType type) {
            this->dest_type = type;
        }

        // Helpers

        /**
         * \brief Return a selective debug representation of this Assembly.
         *
         * Includes the instruction and selected operand strings according to
         * the instruction; unhandled instructions show only the instruction.
         * Operand types are omitted. An unmapped instruction can throw
         * std::out_of_range.
         *
         * \return A string representational of this Assembly Instruction
         */
        std::string to_string() const {
            std::string out = "Assembly [Ins: " + asm_string.at(this->instruction);

            switch (this->instruction) {
                case Instruction::ASM_MOVL:
                case Instruction::ASM_MOVQ: {
                    // Mov operations have both a source and a destination we need to report
                    out += ", src: " + this->src + ", dest: " + this->dest;
                    break;
                }
                case Instruction::ASM_PUSH: {
                    // Push operations have only a source we need to report
                    out += ", src: " + this->src;
                    break;
                }
                case Instruction::ASM_POP: {
                    // Pop operations have only a destination we need to report
                    out += ", dest: " + this->dest;
                    break;
                }
                case Instruction::ASM_NOT:
                case Instruction::ASM_NEG: {
                    // Both Not and Neg operations have only a source we need to report
                    out += ", reg: " + this->src;
                    break;
                }
                case Instruction::ASM_SUB:
                case Instruction::ASM_SUBQ:
                case Instruction::ASM_ADD:
                case Instruction::ASM_IMUL:
                case Instruction::ASM_IDIV:
                case Instruction::ASM_AND:
                case Instruction::ASM_OR:
                case Instruction::ASM_XOR:
                case Instruction::ASM_SAL:
                case Instruction::ASM_SAR:
                case Instruction::ASM_CMP: {
                    // Report both stored fields; IDIV has only one explicit operand
                    out += ", src: " + this->src + ", dest: " + this->dest;
                    break;
                }
                case Instruction::ASM_SETE:
                case Instruction::ASM_SETNE:
                case Instruction::ASM_SETL:
                case Instruction::ASM_SETG:
                case Instruction::ASM_SETLE:
                case Instruction::ASM_SETGE: {
                    // Set instructions only have a value we need to report
                    out += ", reg: " + this->src;
                    break;
                }
                case Instruction::ASM_JE:
                case Instruction::ASM_JNE:
                case Instruction::ASM_JMP: {
                    // Jumps only have a destination we need to report
                    out += ", dest: " + this->dest;
                    break;
                }
                case Instruction::ASM_LABEL: {
                    // Labels only have a value we need to report
                    out += ", value: " + this->src;
                    break;
                }
                default:;
            }

            return out + "]";
        }

        // Overrides
};

#endif // ASSEMBLY
