/*
 * This is where we store global / external variables that
 * need to be visible to multiple files
 */

#include <list>
#ifndef extern_
 #define extern_ extern
#endif

#include <cstdio>
#include <fstream>

#ifndef TOKEN
 #include "token.hpp"
#endif

//* Tracks the current line in our input file
extern_ int current_line_number;

//* Stores any character which we may need to put back
extern_ char put_back;

//* Pointer to our input file
extern_ std::ifstream input_file;

//* Our list of Tokens
extern_ std::list<Token> tokens;