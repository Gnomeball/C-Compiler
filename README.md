# A Very Gnomish C Compiler

A C Compiler I'm working on whilst making my way through Nora Sandler's 'Writing a C Compiler' book (https://nostarch.com/writing-c-compiler).  Current progress is listed in the [changelog](CHANGELOG.md), but put succinctly everything up to Chapter Three, and it's Extra Credit is complete, and Chapter Four is currently in progress (technically all done, but in need of a clean up).

The only notable change I have made from the intended design implementation outlined in the book is that, instead of populating an AST during my parse, I am electing to reduce down to a Bytecode representation.  I am not sure how this will work out when I get into the serious chapters, but that's what the learning adventure is all about.

For now, I am writing this in C++, whilst also re-learning the language, so everything is extremely rough... In time, and when I have a little more to show for my work, I want to attempt an implementation in C, so that I might one day have a self-hosting C Compiler.

## Disclaimer

Very limited compiler, definitely more of a toy than anything else!

## Outline

The current grammar for the compiler can be found in [grammar](grammar/grammar.bnf).

A changelog for this project can be found within [changelog](CHANGELOG.md).

## Usage

To build this project, you will require the Clang, and Clang++ compilers, as well as the command line utility `make`.

The provided [makefile](makefile) should be used to build the project with;

```sh
make clean all
```

You should then have a built executable at `bin/compiler`.

This file can be run directly, by following the usage guide within [main](src/main.cpp), or, more advisably, run through the compiler driver, which requires Python 3.

To run this you use the following;

```sh
./compiler.py <stage> <file>
```

Where `stage` is one of `--lex`, `--parse`, `--tacky`, `--codegen`, or `--assemble`, and `file` is a path to a C source file containing code that is adequately descried by the current [grammar](grammar/grammar.bnf).  In addition to these, you can run the compiler driver with the `--keep-assembly` flag to keep the generated assembly file.

## GitHub Actions tests

[![C/C++ CI](https://github.com/Gnomeball/C-Compiler/actions/workflows/c-cpp.yml/badge.svg?branch=chapter-four)](https://github.com/Gnomeball/C-Compiler/actions/workflows/c-cpp.yml)

The CI workflow fetches the [compiler test suite](https://github.com/nlsandler/writing-a-c-compiler-tests) into `tests/` and runs everything up to Chapter 4's tests, including Extra Credit.

## Documentation

This project uses Doxygen to generate its documentation.  To build it, simply run;

```sh
doxygen
```

This will build the documentation within `docs/html/`, opening `index.html` will allow you to browse.

Note: Whilst every effort is being made to have everything documented to an acceptable standard, there are likely many areas of doc comments that I haven't kept updated.  My ultimate aim is for the 'documentation' within the code to be as readable and as literate as possible, which will take some time, however, as the codebase expands, new functionality should come with this built-in, and thus require less 'post-documenting'.
