/*
 * This is where we store global / external variables that
 * need to be visible to multiple files
 */

#ifndef extern_
    #define extern_ extern
#endif

#include <cstdio>
#include <fstream>
#include <list>
#include <vector>

#ifndef TOKEN
    #include "enums/token.hpp"
#endif

#ifndef BYTE
    #include "enums/byte.hpp"
#endif

#include "enums/tacky-byte.hpp"
#ifndef TACKY
    #include "tacky.hpp"
#endif

#ifndef INSTRUCTION
    #include "enums/instruction.hpp"
#endif

// Pointer to our input file
extern_ std::ifstream input_file;

// Our list of Tokens
extern_ std::list<Token> tokens;

// Our list of bytes
extern_ std::list<Byte> bytes;

// Our list of TackyBytes
extern_ std::list<TackyByte> tacky_bytes;

// Our vector of Assembly Instructions
extern_ std::vector<Instruction> instructions;
