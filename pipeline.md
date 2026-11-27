# Pipeline examples

These examples show the representations produced by the native compiler. Fields not used by an operation are omitted; see the [Byte](grammar/byte.md), [Tacky](grammar/tacky.md), and [Assembly](grammar/assembly.md) references for complete operand conventions.

## Returning a negative literal

```c
int main(void) {
    return -2;
}
```

The tokeniser produces `int`, `main`, `(`, `void`, `)`, `{`, `return`, `-`, `2`, `;`, `}`, and an end-of-file token. The parser folds the minus immediately before the literal into the constant:

```text
Byte                     Tacky
FUNCTION value=main      FUNCTION src_a=main
CONSTANT value=-2        RETURN   src_a=$-2
RETURN
```

The constant becomes an immediate operand on the tackifier's stack, not a separate Tacky instruction. The compiler consumes the function marker and emits these Assembly records; no temporary is needed, so cleanup does not add a stack frame:

```text
MOVL src=$-2 dest=%eax
RET
```

The generated assembly includes file directives and a hard-coded function label. On Linux, its essential instructions are:

```asm
    .globl main
main:
    movl $-2, %eax
    ret
```

On Apple platforms the function label is `_main` instead.

## Short-circuiting and temporaries

For `int main(void) { return 0 && 1; }`, the parser produces:

```text
FUNCTION value=main
CONSTANT value=0
AND_BEGIN
CONSTANT value=1
LOGIC_AND
RETURN
```

The begin marker allows the tackifier to put a jump before the right-hand expression. The resulting Tacky sequence is:

```text
FUNCTION     src_a=main
JUMP_IF_ZERO src_a=$0   dest=false0
JUMP_IF_ZERO src_a=$1   dest=false0
COPY         src_a=$1   dest=tmp.0
JUMP         src_a=end.0
LABEL        src_a=false0
COPY         src_a=$0   dest=tmp.0
LABEL        src_a=end.0
RETURN       src_a=tmp.0
```

The first jump skips evaluation of the right-hand side when the left side is zero; the copies normalise the result to `0` or `1`. Conditional Tacky jumps store their target in `dest`, while unconditional jumps and labels use `src_a`. Assembly jumps instead use `dest`, and Assembly labels use `src`. Cleanup assigns `tmp.0` a stack slot and adds a frame before code generation.
