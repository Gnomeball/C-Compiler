# A rough grammar for the Tacky type

Labels used here are simply field names akin to how they appear in the code, and not representative of how each Tacky is actually used.  

Jump for example appears to use a source, when in fact it uses this as a destination.

## Operators

| Tacky OP              | Representation                               |
|:----------------------|:---------------------------------------------|
| COMPLEMENT            | op, src_a, type_a, dest, type                |
| NEGATE                | op, src_a, type_a, dest, type                |
| ADD                   | op, src_a, type_a, src_b, type_b, dest, type |
| SUBTRACT              | op, src_a, type_a, src_b, type_b, dest, type |
| MULTIPLY              | op, src_a, type_a, src_b, type_b, dest, type |
| DIVIDE                | op, src_a, type_a, src_b, type_b, dest, type |
| MODULO                | op, src_a, type_a, src_b, type_b, dest, type |
| Bitwise AND           | op, src_a, type_a, src_b, type_b, dest, type |
| Bitwise OR            | op, src_a, type_a, src_b, type_b, dest, type |
| Bitwise XOR           | op, src_a, type_a, src_b, type_b, dest, type |
| Bitwise Left Shift    | op, src_a, type_a, src_b, type_b, dest, type |
| Bitwise Right Shift   | op, src_a, type_a, src_b, type_b, dest, type |
| Jump if Zero          | op, src_a, type_a, dest, type                |
| Jump if not Zero      | op, src_a, type_a, dest, type                |
| Jump                  | op, src_a, type_a                            |
| Label                 | op, src_a, type_a                            |
| Copy                  | op, src_a, type_a, dest, type                |
| Equal                 | op, src_a, type_a, dest, type                |
| Not Equal             | op, src_a, type_a, dest, type                |
| Less Than             | op, src_a, type_a, dest, type                |
| Greater Than          | op, src_a, type_a, dest, type                |
| Less Than or Equal    | op, src_a, type_a, dest, type                |
| Greater Than or Equal | op, src_a, type_a, dest, type                |

## Values

| Tacky OP | Representation |
|:---------|:---------------|
| Constant | op, src_a, type_a  |

## Keywords

| Tacky OP | Representation            |
|:---------|:--------------------------|
| Return   | op, src_a, type_a, dest, type |

## Function

| Tacky OP | Representation |
|:---------|:---------------|
| Function | op, src_a, type_a  |
