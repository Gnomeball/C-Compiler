# Changelog

All notable changes to this project will be documented in this file.

## In Progress

The Great Clean up;

- [x] Debug output - done to a reasonable extent, but can obviously still be extended
- [x] Tackyfier - needs a different name still
- [x] Compiler - does what it needs for the moment, will expand with future additions
- [x] Output - could still use some work, but that'll get done eventually

Chapter two;

- [x] The Everything - not entirely sure the output is memory safe still, but it passes

Stretch Goals, listed in order of how achievable I think they might be:

- [ ] Proper Error Handling, perhaps not quite to the level of clang, but at the very least something that gives an indication of where, and what, the error(s) might be.
- [ ] Introduce an AST Parser, which would be invoked with a `-ast` flag, and would parse tokens into an AST rather than bytes, which would then go through a different Tacky and Compile phase in order to produce assembly code in a way more aligned with that of the book (which may, over time, guide my decisions on the bytecode side of things).
- [ ] Literate Commenting, not quite to the extent that Knuth envisioned, but commenting that at least tries to maximise human readability even for those unfamiliar with programming language syntax.
- [ ] Stand-alone Bytecode output, which would be invoked with a `-bytecode` flag, and would output a `.bytes` file that can also be used as input for the Compiler, this would then also necessitate a new control flow for the Compiler, in which the input would skip the Tokeniser and Parser.

## So far done

Stages will be broken down into chapters, then by date; each entry under a date will be something I feel worth noting.

 - [Chapter One](#chapter-one)
 - [Chapter Two](#chapter-two)

### Chapter One

 - October 14, 2024

Chapter one 'complete', it does everything it needs to and nothing that it shouldn't, but it's fairly rough around the edges and needs a lot of tidying up.

 - October 22, 2024

Tokeniser has been completely re-written, and is now a stand-alone class that greatly simplifies the wider control flow, and will be the start of my attempt to make this a bit more modular - next up will be the Parser.

- October 24, 2024

Parser has been completely re-written.  Much like the Tokeniser it is now a stand-alone class that fully encapsulates its entire functionality - next up I will be focusing on proper error handling, and print-out, for the stages thus far cleaned up.

- October 25, 2024

As a bit of fun I've written a second compiler driver, this time in Raku; it's quite a bit slower than the Python version.

- August 10, 2025

Long overdue relook at this; introduced a Tackify class, which does I think what's needed thus far - still failing tests however, which I'll try to work on tomorrow.

- December 11, 2025

Woops I forgot again, but with Advent of Code this year being shortened I've found myself wanting extra stuff to work on, and I remembered this; I have patched up some stuff in the Parser, so we now pass the chapter two parsing tests.

Finally got around to re-adding the codegen stage for at least the Chapter one stuff, so for the first time since October last year this compiler actually does something! (I have also realised that because I am on Apple Silicon I need to do an architecture swap each time I want to run this stage or it's tests)

### Chapter One; Working

```sh
Styx → ~/D/C/G/C/tests $ ./test_compiler ../compiler.py --chapter 1 --verbose
........................
----------------------------------------------------------------------
Ran 24 tests in 7.641s

OK
```

-----

### Chapter Two

- December 12, 2025

Fixed bug in the parser where negative constants were not being correctly parsed, and also added negate and complement operations to the assembly back end, though they do not produce valid output yet (I need to introduce variable / register mapping)

Documentation documentation documentation, not all of it, but a lot more classes and functions are now documented (outstanding are still the Parser, Tackify, and Compiler classes)

- December 13, 2025

Lots more documentation; to my understanding everything currently present in the code base now has at the very least a doc comment, but we are still lacking a lot of logic comments, and the code is far from literate.

The code generation phase has also been re-factored to be a bit smarter, we now convert the Instructions to a vector, so we can now use index access; this isn't particularly useful at the moment, but I think in time it might be; and the whole thing now opens up into separate functions to add in future additions.  Next up will be a final clean up pass on all the chapter one code, so that I may tick that final box above and finally remove the list.

- December 16, 2025

Beginnings of an AST Parser have been added, it can correctly parse and generate trees for extremely simple files, but I don't believe it would currently pass the chapter one tests, and it also doesn't compile witnout warnings yet because there is essenially no error checking.  So there's still a lot of work to do, but for now I'm committing what I have, and will clecn it up another time.

- December 19, 2025

Wavebrain has occured, and I now finally have an idea of how to populate register addresses for temporary variables, I just need to work on the 'scratch' register logic for which I'll likely just split out my clean up logic into a new class, and have it as a kind of sub-phase of my Instruction generation (primarily for reasons of keeping stuff seperate, and maintining the 'everything is a list' aproach that I have built upon so far).

Work on the AST Parser is also likely temporarily abandoned at this point, as I return to focus on my primary aims of keeping this as a Bytecode compiler, but I don't know, I may yet introduce it as an option (though I also would not be surprised if I were to extract the logic into a seperate branch).

- Decembes 20, 2025

Last nights wavebrain has been cleaned up and implemented in a slightly better way, and at the same time I have also removed the ast stuff because i just don't want it right now, chapter two tests are also passing, which honestly surprises me as I was expecting at least a few to fail, but uhh .. I guess chapter two is complete?

### Chapter Two; Working

```sh
Styx → ~/D/C/G/C/tests $ ./test_compiler ../compiler.py --chapter 2 --verbose
...........................................
----------------------------------------------------------------------
Ran 43 tests in 15.254s

OK
```
