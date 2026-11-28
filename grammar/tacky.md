# Tacky operand reference

Tacky is the intermediate representation produced by the tackifier and consumed by the compiler. Each record has an operation and up to three value fields: `src_a`, `src_b`, and `dest`. Each field has a corresponding `VariableType`. The field names describe storage, not necessarily the role of the value: an unconditional jump's target, for example, is stored in `src_a`.

The tackifier uses `IMM` for immediate values such as `$1`, `TMP` for temporaries such as `tmp.0`, and `LABEL` for jump targets and labels. Function names are currently stored in `src_a` with type `IMM`. A dash below means the field is unused by that operation, not that it is necessarily initialised.

## Operations

Names in this table omit the `TACKY_` prefix from `TackyOp`.

| Operations                                                                                 | `src_a`       | `src_b`       | `dest`           | Meaning                                                                                                     |
|:-------------------------------------------------------------------------------------------|:--------------|:--------------|:-----------------|:------------------------------------------------------------------------------------------------------------|
| `COMPLEMENT`, `NEGATE`, `NOT`                                                              | operand       | —             | result temporary | Bitwise complement, arithmetic negation, or logical negation respectively; `NOT` produces `0` or `1`.       |
| `ADD`, `SUBTRACT`, `MULTIPLY`, `DIVIDE`, `MODULO`                                          | left operand  | right operand | result temporary | Apply the arithmetic operation.                                                                             |
| `BW_AND`, `BW_OR`, `BW_XOR`, `LEFT_SHIFT`, `RIGHT_SHIFT`                                   | left operand  | right operand | result temporary | Apply the bitwise operation or shift.                                                                       |
| `EQUAL`, `NOT_EQUAL`, `LESS_THAN`, `GREATER_THAN`, `LESS_THAN_EQUAL`, `GREATER_THAN_EQUAL` | left operand  | right operand | result temporary | Compare operands, producing `0` or `1`.                                                                     |
| `COPY`                                                                                     | value         | —             | result temporary | Copy the value to the destination.                                                                          |
| `JUMP_IF_ZERO`, `JUMP_IF_NOT_ZERO`                                                         | tested value  | —             | target label     | Jump when the value is zero or nonzero, respectively.                                                       |
| `JUMP`                                                                                     | target label  | —             | —                | Jump unconditionally.                                                                                       |
| `LABEL`                                                                                    | label name    | —             | —                | Define a label.                                                                                             |
| `RETURN`                                                                                   | return value  | —             | unused           | Return the value. The tackifier constructs an empty `dest` with type `IMM`; it is not used by the compiler. |
| `FUNCTION`                                                                                 | function name | —             | —                | Start a function.                                                                                           |
| `ERROR`                                                                                    | placeholder   | —             | error message    | Record a tackifier error, rather than an executable operation.                                              |
| `VALUE`                                                                                    | —             | —             | —                | Declared in `TackyOp`, but not emitted by the current tackifier.                                            |

Constants are placed on the tackifier's operand stack as immediate values; they do not produce separate Tacky instructions.

## Short-circuit example

For `0 && 1`, the tackifier emits a sequence equivalent to the following. The first jump skips evaluation of the right-hand expression when the left side is zero; the result is normalized to `0` or `1`.

```text
JUMP_IF_ZERO  src_a=$0      dest=false0
JUMP_IF_ZERO  src_a=$1      dest=false0
COPY          src_a=$1      dest=tmp.0
JUMP          src_a=end.0
LABEL         src_a=false0
COPY          src_a=$0      dest=tmp.0
LABEL         src_a=end.0
RETURN        src_a=tmp.0
```

Here both conditional jumps use `dest` for their target, while the unconditional jump and label definitions use `src_a`. This is the Tacky convention; the later `Assembly` representation uses different fields.
