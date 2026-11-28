# Compiler pipeline

This document describes the current pipeline. For worked examples, see [pipeline.md](pipeline.md); for the supported language grammar, see [grammar/grammar.bnf](grammar/grammar.bnf).

```text
source .c -> clang preprocessor -> .i -> tokeniser -> tokens
          -> parser -> Bytes -> tackifier -> Tacky
          -> compiler -> Assembly -> code generator -> .asm
          -> clang assembler and linker -> executable
```

## Driver and preprocessing

The Python driver invokes clang to preprocess the source into a sibling `.i` file, preserving comments and omitting line commands. It passes that file to the native compiler and removes it afterwards. Unless a stage stop was requested or compilation failed, it invokes clang to assemble and link the generated `.asm`; `--keep-assembly` preserves that file.

The native compiler can also be invoked directly on an input file. Its stages can stop after tokenisation, parsing, tackification, assembly lowering, or code generation.

## Tokenisation and parsing

The tokeniser reads the input and produces a list of tokens, including an end-of-file token. Token spans use one-based line numbers and zero-based byte offsets and lengths within a line; these are not tab-expanded display columns.

The parser consumes tokens and emits a list of `Byte` records instead of an abstract syntax tree. Expressions are postfix: operands come before their operator. Logical expressions also contain begin markers between their operands, allowing later stages to preserve short-circuiting. See the [Byte reference](grammar/byte.md).

## Tackification

The tackifier consumes Bytes and produces three-address-like `Tacky` records. It keeps values on an operand stack, introduces temporary results, and lowers logical operators into conditional jumps, labels and copies that produce `0` or `1`. The [Tacky reference](grammar/tacky.md) explains operand fields and jump conventions.

## Assembly lowering and code generation

The compiler lowers Tacky into `Assembly` instruction records. When cleanup is required, it replaces temporaries with four-byte stack slots and inserts a stack frame. Code generation then writes x86-64 AT&T assembly to a sibling `.asm` file; it currently emits a hard-coded `main` label. Operand conventions, cleanup limitations and instruction details are in the [Assembly reference](grammar/assembly.md).
