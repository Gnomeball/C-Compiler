# The Plan

The aim of this file is to outline the general pipeline for this compiler, broken down into logical stages.  Each stage will be listed in order from front to back.

## Overview

Anything marked with a '?' is currently planning-in-progress, and is subject to change.

```
Source code 'file.c' -> Compiler Driver -> preprocesses -> 'file.i'

Source code 'file.i' -> Tokenise -> Tokens

Tokens -> Rarse -> Bytes

Bytes -> Tackify -> Tacky

Tacky -> Compile -> Instructions

Instructions -> Code Gen -> assembly file 'file.asm'

Assembly file 'file.asm' -> clang -> Executable
```

## Logical Breakdown

### Before the Front - Preprocessing

Before the compiler actually does anything, the compiler driver must pass the source code through a C preprocessor; in the case of this project this is the clang compiler.  This preprocesses the source code and outputs a `.i` file.

This compiler driver is written in Python, and at the current time there are no plans for this project to include any kind of pre-processing.

### Tokenise

The compiler takes in C Source code, in the form of a single file, with the extension `.i`.  This file is read in its entirety by the tokeniser, and is converted into a list of tokens.

This is done up-front, and not bit-by-bit.

### Parse

This list of tokens is then handed over to the parser, and is converted into a list of bytes.

I have not yet decided the exact layout or implementation of this bytecode, only that I intend to do this rather than a more classic abstract syntax tree because I find it a more interesting approach.

At some point I would also like to have this be output-able, almost akin to clang's `-s` flag which outputs assembly, but to output my 'bytecode', as well as to provide a secondary input for this output to simply turn the 'bytecode' into assembly code.  This is however a definite stretch goal.

### Tackification

This list of bytes is then tackified (I really need a better name for this step, or I might just dump it entirely and go straight from bytes into instructions, I don't yet know .. maybe once it gets more complex there might be a reason for this additional pass), into a list of tacky (essentially a list of three address code like objects).  Not a great deal of logic happens here yet, but I expect this to drastically change going forwards.

### Compilation

This list of tacky is now compiled (again, bad name, should probably be an assembler?) into a list of instructions, which are essentially holder objects for which assembly instructions they will require.

### Code Generation

This list of instructions are then outputted, into a `.asm` file.

### The Backend - Linking

As and when the compiler is producing Assembly code, the compiler driver once again kicks in, this time to call upon clang to assemble and link the Assembly code to an executable binary.
