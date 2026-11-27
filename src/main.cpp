/**
 * \file main.cpp
 * \brief Native compiler entry point and pipeline stage wrappers.
 */

#include <iostream>
#include <list>
#include <stdexcept>
#include <string>

#include "debug.hpp"

#include "lib/codegen.hpp"
#include "lib/compiler.hpp"
#include "lib/error-handler.hpp"
#include "lib/parser.hpp"
#include "lib/tackify.hpp"
#include "lib/tokeniser.hpp"

#include "types/tacky.hpp"
#include "types/token.hpp"

/**
 * \brief Print command-line usage to standard error and exit with status 2.
 */
static void usage() {
    std::cerr <<   "Usage: <file> <stop> <stage?>" << std::endl
              <<   "" << std::endl
              <<   "arguments:" << std::endl
              <<   "  file        which file you wish to compile, should point to a file with a .c extension" << std::endl
              << R"(  stop        should the compiler stop early, either "True" or "False")" << std::endl
              <<   "" << std::endl
              <<   "optional:" << std::endl
              << R"(  stage       when should the compiler stop, only used if stop is specified as "True";)" << std::endl
              <<   "              possible values are 1 (lex), 2 (parse), 3 (tacky), 4 (assemble), and 5 (codegen, default);" << std::endl
              << R"(              if stop is set to "False" then this value is ignored)" << std::endl;
    exit(2);
}

// >> Begin Forward Reference

int tokenise(const std::string &input_file, std::list<Token> &tokens, ErrorHandler &error_handler);
int parse(std::list<Token> &tokens, std::list<Byte> &bytes, ErrorHandler &error_handler);
int tackify(std::list<Byte> &bytes, std::list<Tacky> &tacky/*, ErrorHandler &error_handler*/);
int assemble(std::list<Tacky> &tacky, std::list<Assembly> &assembly);
int generate(std::list<Assembly> &assembly, const std::string &input_file);

// << End Forward Reference

/**
 * \brief Run the compiler pipeline up to the requested stage.
 *
 * The stages are tokenisation, parsing, tackification, assembly lowering and
 * code generation. A stop value of "True" selects the last stage to run;
 * the optional stage defaults to 5. "False" runs all stages and ignores the
 * optional stage argument. Processing stops immediately on a stage error.
 *
 * \param argc Number of command-line arguments, including the program name
 * \param argv Program name, input path, stop flag and optional stage
 *
 * \return 0 on success, 1 on a reported stage error, or 3 for a malformed
 * stop flag or stage. An incorrect argument count exits with status 2.
 * Code generation failures are not reflected in the return status.
 */
int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        usage();
    }

    // Grab command line arguments
    const std::string input_file = argv[1]; // string
    const std::string stop = argv[2]; // bool string
    int stage = 5; // int
    if ("True" != stop && "False" != stop) {
        std::cerr << "Error: Value of <stop> is malformed" << std::endl;
        return 3;
    }

    if ("True" == stop && argc == 4) {
        try {
            size_t parsed_length = 0;
            stage = std::stoi(argv[3], &parsed_length);
            if (parsed_length != std::string(argv[3]).length()) {
                throw std::invalid_argument("invalid stage");
            }
        } catch (const std::exception&) {
            std::cerr << "Error: Value of <stage> is malformed" << std::endl;
            return 3;
        }
    }

    // std::cout << input_file << stop << stage << std::endl;

    if (stage < 1 || stage > 5) {
        // value given for the stage is malformed, return an error
        std::cerr << "Error: Value of <stage> is malformed" << std::endl;
        return 3;
    }


    // initialise(input_file);

    auto error_handler = ErrorHandler(input_file);
    int error = 1; // Assume failure for safety

    std::list<Token> tokens;

    // If the value in stage == 1, we will only tokenise
    if (stage >= 1) {
        error = tokenise(input_file, tokens, error_handler);
    }

    if (error != 0) {
        return error;
    }

    std::list<Byte> bytes;

    // If the value in stage == 2, we will lex, and parse
    if (stage >= 2) {
        error = parse(tokens, bytes, error_handler);
    }

    if (error != 0) {
        return error;
    }

    std::list<Tacky> tacky;

    // If the value in stage == 3, we will lex, parse, and tacky
    if (stage >= 3) {
        error = tackify(bytes, tacky);
    }

    if (error != 0) {
        return error;
    }

    std::list<Assembly> assembly;

    // If the value in stage == 4, we will lex, parse, tacky, and assemble
    if (stage >= 4) {
        error = assemble(tacky, assembly);
    }

    if (error != 0) {
        return error;
    }

    // If the value in stage == 5, we will lex, parse, tacky, assemble, and codegen
    if (stage >= 5) {
        error = generate(assembly, input_file);
    }

    return 0;
}

/**
 * \brief Tokenise the input file.
 *
 * Replaces tokens with the scanner's output, including on tokenisation error.
 * If the file cannot be opened, tokens is left unchanged. Scanner errors are
 * added to error_handler and printed; an open failure is printed directly.
 *
 * \param input_file Path to the input file
 * \param tokens Output token list
 * \param error_handler Handler for scanner diagnostics
 *
 * \return 0 on success, 1 on an open or tokenisation error
 */
int tokenise(const std::string &input_file, std::list<Token> &tokens, ErrorHandler &error_handler) {
    // Scan
    Tokeniser tokeniser(input_file);

    if (!tokeniser.opened()) {
        // Something went wrong
        std::cerr << "Error: Could not open input file: " << input_file << std::endl;
        return 1;
    }

    tokens = tokeniser.run();

#ifdef DEBUG_PRINT_TOKENS
    for (Token t: tokens) {
        std::cout << t.to_string() << std::endl;
    }
#endif

    // check for error, return if so
    if (tokeniser.had_error()) {
        error_handler.add_errors(tokeniser.get_errors());
        error_handler.print_errors();
        return 1;
        // for now, we always return,
        // in time we can push ahead to parsing,
        // which may find other errors, before returning
    }

    // All good, return 0 so we can continue
    return 0;
}

/**
 * \brief Parse tokens into bytecode.
 *
 * Consumes tokens and replaces bytes with the parser's output, which may be
 * partial on failure. Collected parser errors are printed by error_handler.
 *
 * \param tokens Input token list, modified by the parser
 * \param bytes Output bytecode list
 * \param error_handler Handler for parser diagnostics
 *
 * \return 0 on success, 1 on a parser error
 */
int parse(std::list<Token> &tokens, std::list<Byte> &bytes, ErrorHandler &error_handler) {
    // Parse
    Parser parser(&tokens);

    bytes = parser.run();

#ifdef DEBUG_PRINT_BYTES
    for (const Byte& b: bytes) {
        std::cout << b.to_string() << std::endl;
    }
#endif

    // check for error, return if so
    if (parser.had_error()) {
        error_handler.add_errors(parser.get_errors());
        error_handler.print_errors();
        return 1;
    }

    // All good, return 0 so we can continue
    return 0;
}

/**
 * \brief Lower bytecode into Tacky instructions.
 *
 * Consumes bytes and replaces tacky with the tackifier's output, which may be
 * partial on failure. This wrapper does not print diagnostics for this stage.
 *
 * \param bytes Input bytecode list, consumed by the tackifier
 * \param tacky Output Tacky list
 *
 * \return 0 on success, 1 on a tackifier error
 */
int tackify(std::list<Byte> &bytes, std::list<Tacky> &tacky/*, ErrorHandler &error_handler*/) {
    // Tacky
    Tackify tackify(&bytes);

    tacky = tackify.run();

#ifdef DEBUG_PRINT_TACKY
    for (const Tacky &t: tacky) {
        std::cout << t.to_string() << std::endl;
    }
#endif

    // check for error, return if so
    if (tackify.had_error()) {
        return 1;
    }

    // All good, return 0 so we can continue
    return 0;
}

/**
 * \brief Lower Tacky instructions into assembly instructions.
 *
 * Consumes tacky and replaces assembly with the compiler's output, which may
 * be partial on failure. This wrapper does not print diagnostics for this stage.
 *
 * \param tacky Input Tacky list, consumed by the compiler
 * \param assembly Output assembly instruction list
 *
 * \return 0 on success, 1 on a compiler error
 */
int assemble(std::list<Tacky> &tacky, std::list<Assembly> &assembly) {
    // Assemble
    Compiler assembler(&tacky);

    assembly = assembler.run();

#ifdef DEBUG_PRINT_ASSEMBLY
    for (const Assembly &a: assembly) {
        std::cout << a.to_string() << std::endl;
    }
#endif

    // check for error, return if so
    if (assembler.had_error()) {
        return 1;
    }

    // All good, return 0 so we can continue
    return 0;
}

/**
 * \brief Generate an assembly file from assembly instructions.
 *
 * Copies assembly into the code generator, leaving the input list unchanged.
 * The output path is input_file with the suffix starting at its last dot
 * removed and ".asm" appended (or just ".asm" appended if there is no dot).
 * An unsupported instruction stops generation and may leave a partial file;
 * output stream open and write failures are not checked.
 *
 * \param assembly Input assembly instruction list
 * \param input_file Input path used to derive the output path
 *
 * \return 0 unconditionally, including when generation encounters an error
 */
int generate(std::list<Assembly> &assembly, const std::string &input_file) {
    // Output
    const std::string file_path_no_ext = input_file.substr(0, input_file.find_last_of('.'));

#ifdef DEBUG_PRINT_OUTPUT_PATH
    std::cout << "Output path : " << file_path_no_ext << ".asm" << std::endl;
#endif

    Codegen codegen(&assembly, file_path_no_ext);

    codegen.generate();

    // All good, return 0 so we can continue
    return 0;
}
