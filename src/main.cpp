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
    std::cout << "Usage: <input_file> lex? parse? codegen?" << std::endl;
    exit(2);
}

// Initialise data variables
void initialise(std::string file) {
    input_file = std::ifstream(file);
}

int do_scan() {
    int ret_value = scan_for_tokens();

#ifdef DEBUG_PRINT_TOKENS
    for (Token t : tokens) {
        std::cout << t.to_string() << std::endl;
    }
#endif

    return ret_value;
}

int do_parse() {
    Token current_token = tokens.front();
    int ret_value = parse_program(&current_token);

#ifdef DEBUG_PRINT_CHUNK
    std::cout << memory_chunk.to_string() << std::endl;
#endif

    return ret_value;
}

// int do_codegen() {}

int main(int argc, char *argv[]) {
    if (argc <= 2) usage();

    // Grab command line arguments
    std::string file = argv[1];
    std::string lex = argv[2];
    std::string parse = argv[3];
    std::string codegen = argv[4];

    std::cout << file << lex << parse << codegen << std::endl;

    initialise(file);

    int ret_value = 0;

    if ("True" == lex) { ret_value = do_scan(); }

    std::cout << "C ret value = " << ret_value << std::endl;

    if (ret_value != 0) { return ret_value; } // error found while scanning

    if ("True" == parse) { ret_value = do_parse(); }
    if (ret_value != 0) { return ret_value; } // error found while parsing

    // if ("True" == codegen) { ret_value = do_codegen(); }
    // if (ret_value != 0) { return ret_value; } // error found while compiling

    return ret_value;
}
