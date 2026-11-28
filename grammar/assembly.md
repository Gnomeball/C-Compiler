# Assembly operand reference

`Assembly` is the instruction representation produced from Tacky by the compiler, optionally rewritten by cleanup, and consumed by codegen. Each record has an `Instruction` and up to two string fields, `src` and `dest`, with corresponding `VariableType` fields. The names describe storage: a unary operation's modified operand is in `src`, while a jump target is in `dest`. A dash below means the field is unused, not necessarily initialised.

Before cleanup, `IMM` denotes an immediate such as `$1`, `TMP` a temporary such as `tmp.0`, `REG` a register such as `%eax`, and `LABEL` a label name. Jumps use an empty `src` with type `EMPTY`. Temporary names need cleanup before output; codegen otherwise prints operand strings without consulting their types, adding `.L` to label names.

## Operations

Names omit the `ASM_` prefix from `Instruction`. Binary operands are printed in source, destination order.

| Operations                                        | `src`                                      | `dest`                | Meaning / output                                                                              |
|:--------------------------------------------------|:-------------------------------------------|:----------------------|:----------------------------------------------------------------------------------------------|
| `MOVL`, `MOVQ`                                    | value                                      | destination           | Copy with `movl` (32-bit) or `movq` (64-bit).                                                 |
| `PUSH`                                            | value                                      | —                     | `pushq` the value.                                                                            |
| `POP`                                             | —                                          | destination           | `popq` into the destination.                                                                  |
| `NOT`, `NEG`                                      | modified operand                           | —                     | `notl` or `negl` in place.                                                                    |
| `ADD`, `SUB`, `IMUL`, `AND`, `OR`, `XOR`          | right operand                              | modified left operand | `addl`, `subl`, `imull`, `andl`, `orl`, or `xorl` in place.                                   |
| `SUBQ`                                            | value                                      | modified operand      | `subq`, used by cleanup for stack allocation.                                                 |
| `SAL`, `SAR`                                      | shift count                                | modified operand      | `sall` or arithmetic `sarl`; the compiler uses `%cl` and `%eax`.                              |
| `IDIV`                                            | divisor                                    | —                     | `idivl`; divides signed `%edx:%eax`, leaving quotient in `%eax` and remainder in `%edx`.      |
| `CDQ`                                             | —                                          | —                     | Sign-extend `%eax` into `%edx:%eax`.                                                          |
| `CMP`                                             | right operand                              | left operand          | `cmpl src, dest` sets flags from `dest - src`.                                                |
| `SETE`, `SETNE`, `SETL`, `SETG`, `SETLE`, `SETGE` | byte destination                           | —                     | Set a byte from the comparison flags; ordered conditions are signed. The compiler uses `%al`. |
| `JE`, `JNE`, `JMP`                                | —                                          | target label          | Jump if equal, not equal, or unconditionally; codegen prefixes the target with `.L`.          |
| `LABEL`                                           | label name                                 | —                     | Define `.L` followed by the name and `:`.                                                     |
| `RET`                                             | —                                          | —                     | Emit `ret`; the compiler puts the return value in `%eax` first.                               |
| `IDENT`                                           | —                                          | —                     | Declared in the enum, but not emitted by the compiler or handled by codegen.                  |
| `ERROR`                                           | error message when constructed by compiler | —                     | Error record, not an executable instruction; codegen stops on it.                             |

## Cleanup and code generation

When requested by the compiler, cleanup assigns each distinct `TMP` a four-byte slot at a negative `%rbp` offset. It resolves temporary operands in moves, unary operations, and listed binary operations; a temporary-to-temporary move is split through `%r10d`. It prepends `pushq %rbp`, `movq %rsp, %rbp`, and `subq` by the slot total, then inserts `movq %rbp, %rsp` and `popq %rbp` before the final instruction, assumed to be `RET`. Cleanup is conditional, so a function needing no cleanup has no such frame.

Cleanup labels rewritten move and arithmetic operand types as `REG` even when their strings are immediates or stack addresses; its frame instructions also set strings without initialising operand types. Other instructions are passed through, including `IDIV`, set instructions, jumps, and labels, so cleanup is not a general operand validator or a complete rewrite of arbitrary input. `Assembly::to_string()` reports `IDIV` as if it had two operands, but compiler and codegen use only `src`.

The compiler consumes the Tacky function marker without emitting `IDENT`; codegen instead writes a hard-coded `main` label (`_main` on Apple platforms), irrespective of the Tacky function name. Codegen also writes the surrounding file directives and prints the instruction strings as x86-64 AT&T assembly.

## Example of a Conditional Jump

A Tacky conditional jump tests `src_a` and stores its target in `dest`. The compiler lowers a jump-if-zero to records equivalent to:

```text
MOVL  src=$0     dest=%r10d
CMP   src=$0     dest=%r10d
JE    src=       dest=false0
LABEL src=false0
```

Codegen emits `je .Lfalse0` and `.Lfalse0:`; unlike jumps, label definitions use `src`. The example omits any cleanup-generated frame and intervening instructions.