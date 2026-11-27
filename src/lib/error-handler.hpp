/**
 * \file error-handler.hpp
 * \author Gnomeball
 * \brief Collect and print source diagnostics.
 * \version 0.1
 * \date 2025-12-21
 */

#ifndef ERROR_HANDLER
#define ERROR_HANDLER

#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <utility>
#include <vector>

#include "../types/error.hpp"

/**
 * \brief Collect diagnostics for one source file and print them to stderr.
 *
 * Source lines are loaded lazily and cached on the first print attempt.
 * Errors remain stored after printing and can be printed again.
 */
class ErrorHandler {

    private:

        /**
         * \brief The name of the source file.
         */
        std::string file_name;

        /**
         * \brief Stream used to load the source file on the first print.
         */
        std::ifstream file;

        /**
         * \brief Cached source lines without line endings.
         */
        std::vector<std::string> file_lines;

        /**
         * \brief Whether loading has been attempted, including a failed attempt.
         */
        bool source_loaded = false;

        /**
         * \brief Whether the source file was opened successfully.
         */
        bool source_available = false;

        /**
         * \brief The list of errors.
         */
        std::list<Error> errors;

        /**
         * Colours markers for each of the text colours.
         */
        std::string red = "\x1b[1;31m";
        std::string green = "\x1b[1;32m";
        std::string blue = "\x1b[1;36m";
        std::string reset = "\x1b[1;0m";

        // Helpers

        /**
         * \brief Returns the given text in red.
         *
         * \param text The text
         *
         * \return The text, with colour markers either side.
         */
        std::string text_red(const std::string &text) const {
            return red + text + reset;
        }

        /**
         * \brief Returns the given text in green.
         *
         * @param text The text
         *
         * @return The text, with colour markers either side.
         */
        std::string text_green(const std::string &text) const {
            return green + text + reset;
        }

        /**
         * \brief Returns the given text in blue.
         *
         * @param text The text
         *
         * @return The text, with colour markers either side.
         */
        std::string text_blue(const std::string &text) const {
            return blue + text + reset;
        }

        /**
         * \brief Attempt to open the source and cache its lines.
         *
         * Splits on newline, removes a trailing carriage return from each line,
         * and retains an empty final line after a trailing newline.
         *
         * Called once by print_errors().
         */
        void load_source_lines() {
            this->file.open(this->file_name);
            this->source_available = this->file.is_open();
            // If the file could not be opened
            if (!this->source_available) {
                return;
            }

            const std::string contents{std::istreambuf_iterator(this->file), std::istreambuf_iterator<char>()};
            size_t line_start = 0;
            while (line_start <= contents.size()) {
                const size_t line_end = contents.find('\n', line_start);
                std::string line = line_end == std::string::npos
                    ? contents.substr(line_start)
                    : contents.substr(line_start, line_end - line_start);
                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }
                this->file_lines.push_back(std::move(line));
                if (line_end == std::string::npos) {
                    break;
                }
                line_start = line_end + 1;
            }

            // If the file opened and could be loaded
            this->source_loaded = true;
        }

        /**
         * \brief Expand tabs to spaces at eight-column stops.
         *
         * \param line The line to expand tabs in.
         *
         * \return The line with tabs expanded; other bytes count as one column.
         */
        static std::string expand_tabs(const std::string& line) {
            // String to store the expanded line
            std::string expanded;

            // Used to track the current display column
            size_t display_column = 0;

            // For each character in the line
            for (const char character : line) {
                // If the character is a tab
                if (character == '\t') {
                    // Calculate the number of spaces needed to replace the tab
                    const size_t spaces = 8 - display_column % 8;
                    // And apply the expansion
                    expanded.append(spaces, ' ');
                    display_column += spaces;
                } else {
                    // Otherwise, just add the character to the expanded line
                    expanded.push_back(character);
                    ++display_column;
                }
            }

            // Return the expanded line
            return expanded;
        }

        /**
         * \brief Convert a zero-based source byte offset to a display column.
         *
         * Tabs advance to the next eight-column stop; other bytes count as one
         * column. Offsets beyond the line are clamped to its length.
         *
         * \param line The line to calculate the display column for.
         * \param source_column The source column to calculate the display column for.
         *
         * \return The display column of the source column in the line.
         */
        static size_t display_column_at(const std::string& line, const size_t source_column) {
            const size_t limit = std::min(source_column, line.size());
            size_t display_column = 0;
            for (size_t index = 0; index < limit; ++index) {
                if (line[index] == '\t') {
                    display_column += 8 - display_column % 8;
                } else {
                    ++display_column;
                }
            }
            return display_column;
        }

    public:

        /**
         * \brief Construct a handler for a source path without opening it.
         *
         * \param file_name The file this Error Handler is handling errors for
         */
        explicit ErrorHandler(std::string file_name)
        : file_name{ std::move( file_name )} {}

        /**
         * \brief Append copies of the supplied errors in order.
         *
         * \param errors The list of errors to add.
         */
        void add_errors(const std::list<Error> &errors) {
            // ? this->errors.merge(errors);
            for (const Error& e : errors) {
                this->errors.push_back(e);
            }
        }

        /**
         * \brief Print all stored diagnostics and their total to stderr.
         *
         * Loads the source on the first call, caching both success and failure.
         * Each diagnostic includes its reason, line number, expanded source
         * line and an ANSI-coloured caret/tilde highlight. The source-byte span
         * is clamped to the line; an empty or out-of-range span gets a single
         * caret. If the file or line is unavailable, a placeholder and a single
         * caret are printed instead.
         */
        void print_errors() {
            if (!this->source_loaded) {
                this->load_source_lines();
            }

            // Then, for each error;
            for (Error& e : this->errors) {
                // Capture the length of the error, defaulting to 1 if it's 0
                const size_t length = e.get_length() > 0 ? static_cast<size_t>(e.get_length()) : 1;

                // Print out the error information
                std::cerr << text_red("Error") << " | " << text_blue(error_type_string(e.get_reason())) << std::endl;
                std::cerr << std::setw(7) << "|" << std::endl;

                const int error_line = e.get_line();
                const size_t line_index = error_line > 0 ? static_cast<size_t>(error_line - 1) : 0;
                const bool has_source_line = this->source_available && error_line > 0 && line_index < this->file_lines.size();

                std::cerr << std::setw(5) << e.get_line() << " | ";
                size_t display_start = 0;
                size_t display_length = 1;
                if (has_source_line) {
                    const std::string& source_line = this->file_lines.at(line_index);
                    // Normalise tabs into spaces
                    std::cerr << expand_tabs(source_line);

                    const size_t source_start = static_cast<size_t>(std::max(0, e.get_start_pos()));
                    const size_t bounded_start = std::min(source_start, source_line.size());
                    const size_t bounded_length = std::min(length, source_line.size() - bounded_start);
                    display_start = display_column_at(source_line, bounded_start);
                    if (e.get_length() > 0 && bounded_length > 0) {
                        display_length = display_column_at(source_line, bounded_start + bounded_length) - display_start;
                    }
                } else {
                    std::cerr << "<source line unavailable>";
                }
                std::cerr << std::endl << "      | " << std::string(display_start, ' ') << text_green("^" + std::string(display_length - 1, '~')) << std::endl;
            }

            // And finally, print out the total number of errors caught
            std::cerr << this->errors.size() << " errors found." << std::endl;
        }
};

#endif // ERROR_HANDLER
