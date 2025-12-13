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
OP_CONSTANT    -2
OP_RETURN
```

Tacky:

```
   op       dest  src_a  src_b

FUNCTION       _   main      _
RETURN         _     -2      _
```

Instructions:

```
   op           dest    src

[ HEADER ]

FUNCTION           _   main

[ PROLOGUE ]

MOV              eax     -2

[ EPILOGUE ]

RETURN                          [ implicetly returns value from eax ]

[ FOOTER ]

```

Assembly:

```asm

    .text
    .file   "test/test.c"
    .globl  _main

_main:  ## @main

    mov     $-2, %eax
    ret

    .ident  "A Very Gnomish C Compiler"

```
