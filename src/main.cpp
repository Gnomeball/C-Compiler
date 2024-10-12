/*
 * Entry point for the compiler
 */

#include <fstream>
#include <iostream>
#include <string>

#define extern_
#include "data.hpp"
#undef extern_

#include "scan.hpp"

// Error if startd without correct args
void usage() {
    std::cout << "Missing input file" << std::endl;
    exit(2);
}

// Initialise data variables
void initialise(std::string file) {
    current_line_number = 1;
    put_back = 0;
    input_file = std::ifstream(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) usage();

    std::string file = argv[1];
    initialise(file);

    int ret_value = scan_for_tokens();

    for (auto t : tokens) {
        std::cout << t.to_string() << std::endl;
    }

    return ret_value;
}
