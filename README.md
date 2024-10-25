# A Very Gnomish C Compiler

A C Compiler I'm working on whilst making my way through Nora Sandler's 'Writing a C Compiler' book (https://nostarch.com/writing-c-compiler).  So far I have only done a small amount of it, and can only really claim to have finished the first chapter; although the parts of the code that allow me to pass the required tests are currently not hooked up, so I really haven't got any of the chapters 'working'.

The only notable change I have made from the intended design implementation outlined in the book, is that instead of populating an AST during my parse I am instead electing to reduce down to a Bytecode representation (kind of), and work from there.  I am not sure how this will work out when I get into the serious chapters, but that's what the learning adventure is all about.

For now, I am writing this in C++, whilst also re-learning the language, so everything is extremely rough... In time, and when I have a little more to show for my work, I want to attempt an implementation in C, so that I might one day have a self-hosting C Compiler.

## Disclaimer

Very limited compiler, definitely more of a toy than anything else!

## Outline

My current thought process for this compiler can be found in [planning](planning.md).

A general overview of the various data representations this compiler uses can be found in [pipeline](pipeline.md).

The current grammar for the compiler can be found in [grammar](grammar.ebnf).

A changelog for this project can be found within [changelog](CHANGELOG.md).

## Usage

To build this project you will require the Clang, and Clang++ compilers, as well as the command line utility `make`.

The provided [makefile](makefile) should be used to build the project with;

```
make clean all
```

You should then have a built executable at `bin/compiler`.

This file can be run directly, by following the usage guide within [main](src/main.cpp), or, more advisably, run through a compiler driver.

To run these you would run one of the following;

```
./compiler.py <stage> <file>

./compiler.raku <stage> <file>
```

Where `stage` is one of `--lex`, `--parse`, `--tacky`, or `--codegen`, and `file` is a path to a C source file containing code that is adequately descried by the current [grammar](grammar.ebnf).

## Documentation

This project uses Doxygen to generate its documentation.  To build it, simply run;

```
doxygen
```

This will build the documentation within `docs/html/`, opening `index.html` will allow you to browse.

Note: Not everything is currently documented to an acceptable standard.
