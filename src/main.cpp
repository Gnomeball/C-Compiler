/*
 * Entry point for the compiler
 */

#include "token.hpp"
#include <fstream>
#include <iostream>
#include <string>

#define extern_
#include "data.hpp"
#undef extern_

#include "debug.hpp"
#include "scan.hpp"
#include "parse.hpp"

// Error if startd without correct args
void usage(void) {
    std::cout << "Missing input file" << std::endl;
    exit(2);
}

// Initialise data variables
void initialise(std::string file) {
    input_file = std::ifstream(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) usage();

    std::string file = argv[1];
    initialise(file);

    int ret_value = scan_for_tokens();

#ifdef DEBUG_PRINT_TOKENS
    for (Token t : tokens) {
        std::cout << t.to_string() << std::endl;
    }
#endif

    if (ret_value != 0) { return ret_value; } // error found while scanning

    Token current_token = tokens.front();
    ret_value = parse_program(&current_token);

#ifdef DEBUG_PRINT_CHUNK
    std::cout << memory_chunk.to_string() << std::endl;
#endif

    if (ret_value != 0) { return ret_value; } // error found while parsing

    return ret_value;
}
