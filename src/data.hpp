/*
 * This is where we store global / external variables that
 * need to be visible to multiple files
 */

#ifndef extern_
 #define extern_ extern
#endif

#include <list>
#include <cstdio>
#include <fstream>
#include <vector>

#ifndef TOKEN
 #include "token.hpp"
#endif

#ifndef CHUNK
 #include "chunk.hpp"
#endif

#ifndef INSTRUCTION
 #include "instruction.hpp"
#endif

//* Pointer to our input file
extern_ std::ifstream input_file;

//* Our list of Tokens
extern_ std::list<Token> tokens;

//* Our chunk of bytecode
extern_ Chunk memory_chunk;

//* Our vector of Assembly Instructions
extern_ std::vector<Instruction> instructions;
