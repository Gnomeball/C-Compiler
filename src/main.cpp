/*
 * Entry point for the compiler
 */

#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

#define extern_
#include "data.hpp"
#undef extern_

#include "compiler.hpp"
#include "debug.hpp"
#include "parse.hpp"
#include "scan.hpp"
#include "token.hpp"

// Error if started without correct args
void usage(void) {
    std::cout << "Usage: <file> <stop> <stage?>" << std::endl
              << "" << std::endl
              << "arguments:" << std::endl
              << "  file        which file you wish to compile, should point to a file with a .c extension" << std::endl
              << "  stop        should the compiler stop early, either \"True\" or \"False\"" << std::endl
              << "" << std::endl
              << "optional:" << std::endl
              << "  stage       when should the compiler stop, only used if stop is specified as \"True\";" << std::endl
              << "              possible values are 1 (lex), 2 (parse), and 3 (compile);" << std::endl
              << "              if stop is set to \"False\" then this value is ignored" << std::endl;
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
    std::list<Byte> bytes = memory_chunk.get_bytes();
    int ret_value = generate_assembly(bytes);

#ifdef DEBUG_PRINT_INSTRUCTIONS
    for (Instruction i : instructions) {
        std::cout << i.to_string() << std::endl;
    }
#endif

    return ret_value;
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        usage();
    }

    // Grab command line arguments
    std::string input_file = argv[1]; // string
    std::string stop = argv[2];       // bool string
    int stage = 4;                    // int
    if (argc == 4) {
        stage = std::stoi(argv[3]);
    }

    // std::cout << input_file << stop << stage << std::endl;

    int ret_value = 0;

    if (stage < 1 || stage > 4) {
        // value given for the stage is malformed, return an error
        std::cout << "Error: Value of <stage> is malformed" << std::endl;
        return 3;
    }

    if ("True" != stop && "False" != stop) {
        // value given for the stop is malformed, return an error
        std::cout << "Error: Value of <stop> is malformed" << std::endl;
        return 3;
    }

    initialise(input_file);

    // If the value in stage == 1, we will only lex (scan)
    if (stage >= 1) {
        // Scan
        ret_value = do_scan();
        if (ret_value != 0) {
            return ret_value;
        } // error found while scanning
    }

    // If the value in stage == 2, we will lex, and parse
    if (stage >= 2) {
        // Parse
        ret_value = do_parse();
        if (ret_value != 0) {
            return ret_value;
        } // error found while parsing
    }

    // If the value in stage == 3, we will lex, parse, and compile
    if (stage >= 3) {
        // Compile
        ret_value = do_codegen();
        if (ret_value != 0) {
            return ret_value;
        } // error found while compiling
    }

    // If the value in stage == 4, we will lex, parse, compile, and output
    if (stage >= 4) {
        // Output
        std::string output_file = input_file.substr(0, input_file.find_last_of("."));
        output_assembly(output_file);
    }

    return ret_value;
}
