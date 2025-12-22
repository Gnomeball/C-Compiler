/**
 * \file error-handler.hpp
 * \author Gnomeball
 * \brief
 * \version 0.1
 * \date 2025-12-21
 */

#ifndef ERROR_HANDLER
#define ERROR_HANDLER

#include <fstream>
#include <iostream>
#include <list>
#include <vector>

#include "../types/error.hpp"

class ErrorHandler {

    private:

        // todo: documentation!

        std::string file_name;

        std::ifstream file;

        std::vector<std::string> file_lines;

        std::list<Error> errors;

        std::string red = "\x1b[1;31m";
        std::string green = "\x1b[1;32m";
        std::string blue = "\x1b[1;36m";
        std::string reset = "\x1b[1;0m";

        // Helpers

        std::string text_red(std::string text) {
            return red + text + reset;
        }

        std::string text_green(std::string text) {
            return green + text + reset;
        }

        std::string text_blue(std::string text) {
            return blue + text + reset;
        }

    public:

        /**
         * \brief Default constructor for the Error Hantdler
         *
         * \param file_name The file this Error Handler is handling errors for
         */
        ErrorHandler(std::string file_name)
        : file_name{ file_name } {}

        void add_errors(std::list<Error> errors) {
            // ? this->errors.merge(errors);
            for (Error e : errors) {
                this->errors.push_back(e);
            }
        }

        void print_errors(void) {
            // First, read in the file to we can populate the lines vector
            if (this->file_lines.empty()) {
                // Because this happens after parsing we already know the file will open successfully
                this->file.open(file_name);
                std::string line;
                do { // I think this will work because the .i file should always have an empty line at the end
                    this->file_lines.push_back(line);
                } while (std::getline(this->file, line));
            }
            // Then, for each error, print it's information in a nicely formatted way
            for (Error e : this->errors) {
                std::cout << text_red("Error") << " | " << text_blue(e.get_reason()) << std::endl;
                std::cout << std::setw(7) << "|" << std::endl;
                std::cout << std::setw(5) << e.get_line() << " | " << this->file_lines.at(e.get_line()) << std::endl;
                std::cout << "      | " << std::setw(e.get_pos()) << " " << text_green("^") << std::endl;
            }
            // And finally, print out the total number of errors caught
            std::cout << this->errors.size() << " errors found." << std::endl;
        }
};

#endif // ERROR_HANDLER
