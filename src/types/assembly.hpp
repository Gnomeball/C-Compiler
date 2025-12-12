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

#include "../enums/instructions.hpp"

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

        /**
         * \brief The destination value for this Assembly Instruction
         */
        std::string dest;

    public:

        // Constructors

        /**
         * \brief The default constructor for an Assembly object
         */
        Assembly(void) {} // default

        /**
         * \brief Construct a new Assembly object with an Instruction
         *
         * \param instruction The Instruction this Assembly refers to
         */
        Assembly(Instruction instruction)
        : instruction{ instruction } {}

        /**
         * \brief Construct a new Assembly object with an Instruction and a source
         *
         * \param instruction The Instruction this Assembly refers to
         * \param src The source for the value against this Assembly Instruction
         */
        Assembly(Instruction instruction, std::string src)
        : instruction{ instruction }, src{ src } {}

        /**
         * \brief Construct a new Assembly object with an Instruction, a source, and a destination
         *
         * \param instruction The Instruction this Assembly refers to
         * \param src The source for the value against this Assembly Instruction
         * \param dest The destination for the value against this Assembly Instruction
         */
        Assembly(Instruction instruction, std::string src, std::string dest)
        : instruction{ instruction }, src{ src }, dest{ dest } {}

        // Accessors

        /**
         * \brief Get the Instruction from this Assembly
         *
         * \return The Instruction value from this Assembly
         */
        Instruction get_instruction(void) {
            return this->instruction;
        }

        /**
         * \brief Get the source for this Assembly
         *
         * \return The source value for this Assembly
         */
        std::string get_src(void) {
            return this->src;
        }

        /**
         * \brief Get the destination for this Assembly
         *
         * \return The destination value for this Assembly
         */
        std::string get_dest(void) {
            return this->dest;
        }

        // Helpers

        /**
         * \brief Returns a string contrining the information related to this Assembly
         *
         * \return A string represententation of this Assembly Instruction
         */
        const std::string to_string(void) {
            std::string out = "Assembly [Ins: " + asm_string.at(this->instruction);

            // if (this->instruction == Instruction::ASM_IDENT) {
            //     out += ", Value: " + this->value;
            // } else

            switch (this->instruction) {
                case Instruction::ASM_MOV: {
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
