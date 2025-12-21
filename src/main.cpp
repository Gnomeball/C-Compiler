/*
 * Entry point for the compiler
 */

#include <iostream>
#include <list>
#include <string>

#include "debug.hpp"

#include "lib/codegen.hpp"
#include "lib/compiler.hpp"
#include "lib/parser.hpp"
#include "lib/tackify.hpp"
#include "lib/tokeniser.hpp"

#include "types/tacky.hpp"
#include "types/token.hpp"

/**
 * \brief Prints out usage if compiler is started without correct arguments
 */
static void usage(void) {
    std::cout << "Usage: <file> <stop> <ast?> <stage?>" << std::endl
              << "" << std::endl
              << "arguments:" << std::endl
              << "  file        which file you wish to compile, should point to a file with a .c extension" << std::endl
              << "  stop        should the compiler stop early, either \"True\" or \"False\"" << std::endl
              << "" << std::endl
              << "optional:" << std::endl
              << "  stage       when should the compiler stop, only used if stop is specified as \"True\";" << std::endl
              << "              possible values are 1 (lex), 2 (parse), 3 (tacky), 4 (assemble), and 5 (codegen);" << std::endl
              << "              if stop is set to \"False\" then this value is ignored" << std::endl;
    exit(2);
}

// TODO: Maybe move errors to seperate file and return them all from there, for reasons of readability

// >> Begin Forward Reference

int bytecode(std::list<Token> tokens, std::string input_file, int stage);
int ast_parse(std::list<Token> tokens);

// << End Forward Reference

/**
 * \brief Entry point for the Compiler
 *
 * \param argc How many arguments were passed in
 * \param argv The argument values
 *
 * \return An exit condition; 0 if no errors were found, 1 otherwise
 */
int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 5) {
        usage();
    }

    // Grab command line arguments
    const std::string input_file = argv[1]; // string
    const std::string stop = argv[2];       // bool string
    int stage = 5; // int
    if (argc == 4) {
        stage = std::stoi(argv[3]);
    }

    // std::cout << input_file << stop << stage << std::endl;

    if (stage < 1 || stage > 5) {
        // value given for the stage is malformed, return an error
        std::cout << "Error: Value of <stage> is malformed" << std::endl;
        return 3;
    }

    if ("True" != stop && "False" != stop) {
        // value given for the stop is malformed, return an error
        std::cout << "Error: Value of <stop> is malformed" << std::endl;
        return 3;
    }

    // initialise(input_file);

    std::list<Token> tokens;

    // If the value in stage == 1, we will only tokenise
    if (stage >= 1) {
        // Scan
        Tokeniser tokeniser(input_file);

        if (!tokeniser.opened()) {
            // Something went wrong
            return 1;
        }

        tokens = tokeniser.run();

#ifdef DEBUG_PRINT_TOKENS
        for (Token t : tokens) {
            std::cout << t.to_string() << std::endl;
        }
#endif

        // check for error, return if so
        if (tokeniser.had_error()) {
            tokeniser.print_errors();
            return 1;
        }
    }

    return bytecode(tokens, input_file, stage);
}

/**
 * \brief Parses the Tokens into Bytecode (rather than an AST - code removed)
 *
 * \param tokens The Tokens to Parse
 * \param input_file The name of the input file, used during output
 * \param stage Which stage to stop at, if any
 *
 * \return 0 if no errors occurred, 1 otherwise
 */
int bytecode(std::list<Token> tokens, std::string input_file, int stage) {
    std::list<Byte> bytes;

    // If the value in stage == 2, we will lex, and parse
    if (stage >= 2) {
        // Parse
        Parser parser(&tokens);

        bytes = parser.run();

#ifdef DEBUG_PRINT_BYTES
        for (Byte b : bytes) {
            std::cout << b.to_string() << std::endl;
        }
#endif

        // check for error, return if so
        if (parser.had_error()) {
            return 1;
        }
    }

    std::list<Tacky> tacky;

    // If the value in stage == 3, we will lex, parse, and tacky
    if (stage >= 3) {
        // Tacky
        Tackify tackify(&bytes);

        tacky = tackify.run();

#ifdef DEBUG_PRINT_TACKY
        for (Tacky t : tacky) {
            std::cout << t.to_string() << std::endl;
        }
#endif

        // check for error, return if so
        if (tackify.had_error()) {
            return 1;
        }
    }

    std::list<Assembly> assembly;

    // If the value in stage == 4, we will lex, parse, tacky, and assemble
    if (stage >= 4) {
        // Assemble
        Compiler assembler(&tacky);

        assembly = assembler.run();

#ifdef DEBUG_PRINT_ASSEMBLY
        for (Assembly a : assembly) {
            std::cout << a.to_string() << std::endl;
        }
#endif

        // check for error, return if so
        if (assembler.had_error()) {
            return 1;
        }
    }

    // If the value in stage == 5, we will lex, parse, tacky, assemble, and codegen
    if (stage >= 5) {
        // Output
        std::string file_path_no_ext = input_file.substr(0, input_file.find_last_of("."));

#ifdef DEBUG_PRINT_OUTPUT_PATH
        std::cout << "Output path : " << file_path_no_ext << ".asm" << std::endl;
#endif

        Codegen codegen(&assembly, file_path_no_ext);

        codegen.generate();
    }

    return 0;
}
