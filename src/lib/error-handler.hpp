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

        std::string file_name;

        std::ifstream file;

        std::vector<std::string> file_lines;

        std::list<Error> errors;

    public:

        /**
         * \brief Default constructor for the Error Hantdler
         *
         * \param file_name The file this Error Handler is handling errors for
         */
        ErrorHandler(std::string file_name) {
            this->file.open(file_name);
            std::string line;
            while (std::getline(this->file, line)) {
                this->file_lines.push_back(line);
            }
            this->file.close();
        }

        void add_errors(std::list<Error> errors) {
            // ? this->errors.merge(errors);
            for (Error e : errors) {
                this->errors.push_back(e);
            }
        }

        void print_errors(void) {
            for (Error e : this->errors) {
                e.set_line_text(this->file_lines.at(e.get_line() - 1));
                std::cout << e.to_string();
            }
            std::cout << this->errors.size() << " errors found." << std::endl;
        }

        // todo: this
};

#endif // ERROR_HANDLER
