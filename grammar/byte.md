# Byte operation reference

`Byte` is the parser's intermediate representation, produced from tokens and consumed by the tackifier. A byte is a record, not a machine byte: it carries an `OpCode`, a string `value`, an error `reason`, and source position fields. Operator operands are represented by the surrounding byte sequence rather than fields on the operator itself.

The parser emits expressions in postfix order: operands precede their operator. Binary operators use the earlier operand as the left side and the later one as the right side. Logical operators also have a begin marker between their left and right operands so the tackifier can generate short-circuit control flow. A dash below means that the parser does not supply a meaningful `value` for the operation.

## Operations

Names omit the `OP_` prefix from `OpCode`.

| Operations                                                                                 | `value`                        | Meaning                                                                                                                                                                     |
|:-------------------------------------------------------------------------------------------|:-------------------------------|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `CONSTANT`                                                                                 | decimal integer string         | Supply an integer operand. The parser converts supported decimal, octal and hexadecimal literals to decimal, and folds a minus immediately before a literal into the value. |
| `COMPLEMENT`, `NEGATE`, `NOT`                                                              | —                              | Apply bitwise complement, arithmetic negation or logical negation to the preceding operand.                                                                                 |
| `ADD`, `SUBTRACT`, `MULTIPLY`, `DIVIDE`, `MODULO`                                          | —                              | Apply the arithmetic operation to the preceding left and right operands.                                                                                                    |
| `BW_AND`, `BW_OR`, `BW_XOR`, `LEFT_SHIFT`, `RIGHT_SHIFT`                                   | —                              | Apply the bitwise operation or shift to the preceding operands.                                                                                                             |
| `EQUAL`, `NOT_EQUAL`, `LESS_THAN`, `GREATER_THAN`, `LESS_THAN_EQUAL`, `GREATER_THAN_EQUAL` | —                              | Compare the preceding operands, producing `0` or `1`.                                                                                                                       |
| `AND_BEGIN`, `OR_BEGIN`                                                                    | —                              | Mark the boundary after the left operand and before the right operand of `&&` or `                                                                                          ||`, respectively. |
| `LOGIC_AND`, `LOGIC_OR`                                                                    | —                              | Finish the matching logical expression after its right operand; the result is `0` or `1`.                                                                                   |
| `RETURN`                                                                                   | —                              | Return the expression result.                                                                                                                                               |
| `FUNCTION`                                                                                 | function name                  | Begin the function; currently the parser emits one function per program.                                                                                                    |
| `ERROR`                                                                                    | error message, sometimes empty | Record a parser error, not an executable operation. Error details may also be recorded separately.                                                                          |
| `DECREMENT`                                                                                | —                              | Declared in `OpCode`, but not emitted by the current parser or handled by the tackifier.                                                                                    |

The tackifier prefixes constant values with `$` when turning them into immediate operands; the byte value itself has no `$` prefix.

## Example of Short-Circuiting

For `int main(void) { return 0 && 1; }`, the relevant byte sequence is:

```text
FUNCTION    value=main
CONSTANT    value=0
AND_BEGIN
CONSTANT    value=1
LOGIC_AND
RETURN
```

The begin marker lets the tackifier emit a conditional jump before the right operand's code. Nested expressions can contain nested begin markers, each matched with its corresponding logical operation.

## Other fields

`reason` defaults to `ET_NONE`; parser error bytes may carry a specific `ErrorType`. The line and position fields default to zero and can be set separately. When copied from tokens, lines are one-based and positions are zero-based source-byte offsets within the line, not tab-expanded display columns. The parser does not set positions on every byte. `Byte::to_string` prints the value only for `FUNCTION`, `CONSTANT` and `ERROR`; it does not print the reason or position.