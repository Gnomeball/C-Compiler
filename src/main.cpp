/*
 * Entry point for the compiler
 */

#include <iostream>
#include <string>

#define extern_
#include "globals.hpp"
#undef extern_

#include "debug.hpp"

#include "lib/compiler.hpp"
#include "lib/parse.hpp"
#include "lib/tacky.hpp"
#include "lib/tokenise.hpp"

#include "enums/token.hpp"

// Error if started without correct args
static void usage(void) {
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

// TODO: Add error checking on the input file; doesn't exist, can't open, etc

// Initialise data variables
void initialise(std::string file) {
    input_file = std::ifstream(file);
}

static int do_tokenise(void) {
    int ret_value = scan_for_tokens();

#ifdef DEBUG_PRINT_TOKENS
    for (Token t : tokens) {
        std::cout << t.to_string() << std::endl;
    }
#endif

    return ret_value;
}

// int do_parse(void) {
//     Token current_token = tokens.front();
//     int ret_value = parse_program(&current_token);

// #ifdef DEBUG_PRINT_BYTES
//     for (Byte b : bytes) {
//         std::cout << b.to_string() << std::endl;
//     }
// #endif

//     return ret_value;
// }

// int do_tacky(void) {
//     Byte current_byte = bytes.front();
//     int ret_value = generate_tacky(&current_byte);

// #ifdef DEBUG_PRINT_TACKY_BYTES
//     for (TackyByte tb : tacky_bytes) {
//         std::cout << tb.to_string() << std::endl;
//     }
// #endif

//     return ret_value;
// }

// int do_codegen(void) {
//     int ret_value = generate_assembly(bytes);

// #ifdef DEBUG_PRINT_INSTRUCTIONS
//     for (Instruction i : instructions) {
//         std::cout << i.to_string() << std::endl;
//     }
// #endif

//     return ret_value;
// }

// TODO: Maybe move errors to seperate file and return them all from there, for reasons of readability

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        usage();
    }

    // Grab command line arguments
    const std::string input_file = argv[1]; // string
    const std::string stop = argv[2];       // bool string
    int stage = 5;                          // int
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

    //! for now we only tokenise
    stage = 1;

    // If the value in stage == 1, we will only tokenise
    if (stage >= 1) {
        // Scan
        ret_value = do_tokenise();
        if (ret_value != 0) {
            return ret_value;
        } // error found while scanning
    }

    // // If the value in stage == 2, we will lex, and parse
    // if (stage >= 2) {
    //     // Parse
    //     ret_value = do_parse();
    //     if (ret_value != 0) {
    //         return ret_value;
    //     } // error found while parsing
    // }

    // // If the value in stage == 3, we will lex, parse, and tacky
    // if (stage >= 3) {
    //     // Compile
    //     ret_value = do_tacky();
    //     if (ret_value != 0) {
    //         return ret_value;
    //     } // error found while tackifying
    // }

    // // If the value in stage == 4, we will lex, parse, tacky, and compile
    // if (stage >= 4) {
    //     // Compile
    //     ret_value = do_codegen();
    //     if (ret_value != 0) {
    //         return ret_value;
    //     } // error found while compiling
    // }

    // If the value in stage == 5, we will lex, parse, tacky, compile, and output
    // if (stage >= 5) {
    //     // Output
    //     std::string output_file = input_file.substr(0, input_file.find_last_of("."));
    //     output_assembly(output_file);
    // }

    return ret_value;
}
