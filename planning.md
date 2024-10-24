# The Plan

The aim of this file is to outline the general pipeline for this compiler, broken down into logical stages.  Each stage will be listed in order from front to back.

## Overview

Anything marked with a '?' is currently planning-in-progress, and is subject to change.

```
Source code 'file.c' -> compiler driver -> preprocesses -> 'file.i'

Source code 'file.i' -> tokenise -> Tokens

Tokens -> parse -> Bytecode Chunk ?

Bytecode Chunk ? -> ?

Some other intermediate representation ?

Assembly code -> assemble -> Executable
```

## Logical Breakdown

### Before the Front - Preprocessing

Before the compiler actually does anything, the compiler driver must pass the source code through a C preprocessor; in the case of this project this is the clang compiler.  This preprocesses the source code and outputs a `.i` file.

This compiler driver is written in Python, and at the current time there are no plans for this project to include any kind of pre-processing.

### Tokenise

The compiler takes in C Source code, in the form of a single file, with the extension `.i`.  This file is read in its entirety by the tokeniser, and is converted into a list of tokens.

This is done up-front, and not bit-by-bit.

### Parse

This list of tokens is then handed over to the parser, and is converted into chunk of bytecode.

I have not yet decided the exact layout or implementation of this bytecode, only that I intend to do this rather than a more classic abstract syntax tree because I find it a more interesting approach.

### Tackification, Code Generation, and Emission

For the most part this is entirely unknown, I don't yet know what I will be doing after the parse.

Having said this I am possibly considering providing the option to export the bytecode to a file, which can then be executed by a virtual machine.  This virtual machine might have two modes; a mode for direct execution of the code (an interpreter), and a mode for converting into Assembly code.  There should be other intermediate representations required for this, namely some kind of three-address-code, but I am as yet unsure of the specifics of their implementation.

Any files produced by this step will be exported as a `.s` file.

TLDR; I have no real idea what this stage will look like.

### The Backend - Linking

As and when the compiler is producing Assembly, the compiler driver once again kicks in, this time to call upon clang to assemble and link the Assembly code to an executable binary.
