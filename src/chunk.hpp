/*
 * In this file we define our Chunk class, used by the parser
 * to store bytecode, as well as it's internal functions
 */

#ifndef CHUNK
#define CHUNK

#include <cstdio>
#include <list>
#include <string>

#include "byte.hpp"

class Chunk {

    private:

        std::string name;

        std::list<Byte> bytes;

    public:

        // Constructors

        Chunk(void) {
            this->name = "code";
        } // default

        Chunk(std::string name)
        : name(name) {}

        // Accessors

        std::list<Byte> get_bytes() {
            return this->bytes;
        }

        // Helpers

        void add_byte(Byte b) {
            this->bytes.insert(bytes.end(), b);
        }

        const std::string to_string(void) {
            std::string out = "==== " + this->name + " ====" + "\n";
            for (Byte b : bytes) {
                out += b.to_string() + "\n";
            }
            out.erase(out.length() - 1); // remove the final newline
            return out;
        }

        // Overrides
};

#endif
