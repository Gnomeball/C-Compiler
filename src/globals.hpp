/**
 * \file globals.hpp
 * \author Gnomeball
 * \brief A file used to store global variabls that ar required by multiple stages of the compiler.
 * \version 0.1
 * \date 2024-10-21
 */

/*
    The aim of the current clean up is to get rid of this file!
*/

#ifndef extern_
    #define extern_ extern
#endif

#include <cstdio>
#include <fstream>
#include <list>
#include <vector>

#ifndef TACKY
    #include "lib/tacky.hpp"
#endif

#ifndef BYTE
    #include "enums/byte.hpp"
#endif

#ifndef INSTRUCTION
    #include "enums/instruction.hpp"
#endif

/**
 * \brief Pointer to our input file
 */
extern_ std::ifstream input_file;

/**
 * \brief Pointer to a list of Bytes
 */
extern_ std::list<Byte> bytes;

/**
 * \brief Pointer to a list of TackyBytes
 */
extern_ std::list<TackyByte> tacky_bytes;

/**
 * \brief Pointer to a vector of Assembly Instructions
 */
extern_ std::vector<Instruction> instructions;
