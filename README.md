# C Compiler

A C Compiler I'm working on whilst making my way through Nora Sandler's 'Writing a C Compiler' book (https://nostarch.com/writing-c-compiler).  So far I have only done a small amount of it, and can only really claim to have finished the first chapter; although the parts of the code that allow me to pass the required tests are currently not hooked up, so I really haven't got any of the chapters 'working'.

The only notable change I have made from the intended design implementation outlined in the book, is that instead of populating an AST during my parse I am instead electing to reduce down to a Bytecode representation (kind of), and work from there.  I am not sure how this will work out when I get into the serious chapters, but that's what the learning adventure is all about.

For now, I am writing this in C++, whilst also re-learning the language, so everything is extremely rough... In time, and when I have a little more to show for my work, I want to attempt an implementation in C, so that I might one day have a self-hosting C Compiler.
