## Pipeline

Code:

```c
int main(void) {
    return -2;
}
```

Tokens:

```c
int         [ keyword, int  ]
main        [ idenfifier, main ]
(           [ open parenthesis ]
void        [ keyword, void ]
)           [ close parenthesis ]
{           [ open brace ]
return      [ keyword, return ]
-           [ minus ]
2           [ constant, 2 ]
;           [ semi colon ]
}           [ close brace ]
EOF         [ end of file ]
```

Bytes:

```
   op        value

OP_FUNCTION  main
OP_CONSTANT     2
OP_NEGATE
OP_RETURN
```

Tacky:

```
   op       dest  src_a  src_b

FUNCTION       _   main      _
VALUE      tmp.0      2      _
NEGATE     tmp.1  tmp.0      _
RETURN         _  tmp.1      _
```

Instructions:

```
   op           dest    src

[ HEADER ]

FUNCTION           _   main

[ PROLOGUE ]

ALLOCATE_STACK     _      1     [ 1? because we only have one expression? ]
                                [ equates to moving rbp down n*4 = -4 ]

MOVE           rbp-4      2
NEGATE             _  rbp-4

MOV              eax  rbp-4

[ EPILOGUE ]

RETURN                          [ implicetly returns value from eax ]

[ FOOTER ]

```

Assembly:

```asm

    .text
    .file   "filename.c"
    .globl  main                # -- Begin function main

main:                           # @main

    pushq   %rbp                # push base pointer onto the stack
    movq    %rsp,       %rbp    # move stack pointer to base pointer

    subq    $4,         %rbp    # subtract 4 from base pointer

    movl    $2,      -4(%rbp)   # move constant(2) into base pointer minus 4
    negl    -4(%rbp)            # negate the value at base pointer minus 4

    movl    -4(%rbp),   %eax    # move value at base pointer minus 4 into eax

    movq    %rbp,       %rsp    # move base pointer into stack pointer
    popq    %rbp                # pop stack back into base pointer

    retq                        # return

.Lfunc_end0:
	.size	main, .Lfunc_end0-main  # -- End function main

	.ident	    "A Very Gnomish C Compiler"
	.section	.note.GNU-stack,"",@progbits

```