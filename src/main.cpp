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
#include "compiler.hpp"

// Error if startd without correct args
void usage(void) {
    std::cout << "Usage: <input_file> lex? parse? codegen? output?" << std::endl;
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

int do_codegen() {
    std::list<Byte> bytes = memory_chunk.getBytes();
    int ret_value = generate_assembly(bytes);

#ifdef DEBUG_PRINT_INSTRUCTIONS
    for (Instruction i : instructions) {
        std::cout << i.to_string() << std::endl;
    }
#endif

    return ret_value;
}

int main(int argc, char *argv[]) {
    if (argc <= 5) usage();

    // Grab command line arguments
    std::string input_file = argv[1];
    std::string lex = argv[2];
    std::string parse = argv[3];
    std::string codegen = argv[4];
    std::string output = argv[5];

    // std::cout << input_file << lex << parse << codegen << output << std::endl;

    initialise(input_file);

    int ret_value = 0;

    if ("True" == lex) { ret_value = do_scan(); }
    if (ret_value != 0) { return ret_value; } // error found while scanning

    if ("True" == parse) { ret_value = do_parse(); }
    if (ret_value != 0) { return ret_value; } // error found while parsing

    if ("True" == codegen) { ret_value = do_codegen(); }
    if (ret_value != 0) { return ret_value; } // error found while compiling

    if ("True" == output) {
        std::string output_file = input_file.substr(0, input_file.find(".")) + ".s";
        output_assembly(output_file);
    }

    return ret_value;
}
