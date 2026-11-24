# Changelog

All notable changes to this project will be documented in this file.

### In progress

Chapter Four;

- [ ] Parsing
- [ ] Tackifying
- [ ] Code Generation
- [ ] Clean up the codebase some more

Stretch Goals, listed in order of how achievable I think they might be:

- [ ] Proper Error Handling, perhaps not quite to the level of clang, but at the very least something that gives an indication of where, and what, the error(s) might be.
- [ ] Literate Commenting, not quite to the extent that Knuth envisioned, but commenting that at least tries to maximise human readability even for those unfamiliar with programming language syntax.
- [ ] Stand-alone Bytecode output, which would be invoked with a `-bytecode` flag, and would output a `.bytes` file that can also be used as input for the Compiler, this would then also necessitate a new control flow for the Compiler, in which the input would skip the Tokeniser and Parser.

#### 2026-11-24

Fixed a bug in the compiler's assembly output, whereby some mov lines prior to a shist instruction were being duplicated.  I'm not sure yet if my approach here of moving the left value into eax prior to the rest of the logic is smart or silly, but I'm sure I'll find out fairly soon.

### Chapter Three Complete (including Extra Credit) - 2026-11-24

Aside from veritably scrambling my brain over the shift operations, which probably took me over an hour of back and forth between the code and the docs, the extra credit stuff wasn't actually too tricky.

```shell
gnome@Styx tests % ./test_compiler ../compiler.py --chapter 3 --bitwise --verbose
..............................................................................
----------------------------------------------------------------------
Ran 78 tests in 17.878s

OK
```

#### 2026-11-24 (again)

Add, Multiply, Divide, and Modulus operations have been implemented, and Chapter 3 is now technically complete.

```shell
```sh
gnome@Styx tests % ./test_compiler ../compiler.py --chapter 3 --verbose
..................................................................
----------------------------------------------------------------------
Ran 66 tests in 13.969s

OK
```

Next up, Extra Credit - at which point I will consider Chapter 3 actually complete.

#### 2026-11-24

Modified the compiler driver so that I can run it on both my Linux desktop, and my MacBook, now without needing to first change my terminal architecture from arm64 to x86_64.

Additionally, the GitHub workflow now runs the compiler tests.

#### 2026-08-22

Extended the Parser to accept binary tokens, as well as introduced precedence parsing for expressions.

```sh
gnome@Styx tests % ./test_compiler ../compiler.py --chapter 3 --stage parse  --verbose
..................................................................
----------------------------------------------------------------------
Ran 66 tests in 8.548s

OK
```

#### 2026-06-07

Extended the Byte class so they can keep track of their locations, as well as extended error handling to the parser.  Main is also slightly cleaner.

#### 2026-06-06

Further clean up of some stuff, still not ready to attack the next chapter though.

#### 2026-05-02

Pushed some stuff I previously did but forgot to commit, related to error handling, but I can't say exactly what it does.. it's been too long since I looked at this.

#### 2025-12-22

Moved error printing up to the handler class and now the error class now feels a bit like a silly wrapper.. still, it's relatively clean.

#### 2025-12-21 (again)

Error Handling has been moved to a seperate class..

.. I don't think I'm happy with it though, probably the colour directives need to be up in the handler class, and instead of passing the line text in we just print everything there.. but that means more getters.. it's all a bit of a mess.

Really ought to clean up my todos soon

#### 2025-12-21

Started work on some actual error handling (using Clang as my inspiration), so far only for errors caught during Tokenisation, and I think it's a good start, althoulgh I probably want to be moving it into a seperate class so that I can run through Parsing even if I do find Tokenisation errors and report on all of them at the same time.  I'm also thinking of making the optional 'void' keyword a warning, so that I technically have the start of some warning output too, but that can wait until I add error handling to the Parser, probably over the next few days.

.. I really ought to document all of these changes too, I'm slacking.

```c
// File:

int main(@void) {
    // Hello!
    return 1foo;
}

// Output:

Error | Unrecognised Character
      |
    1 | int main(@void) {
      |          ^
Error | Malformed Constant
      |
    3 |    return 1foo;
      |           ^
2 errors found.
```

### Chapter Two Complete - 2025-12-20

- [x] The Everything - not entirely sure the output is memory safe still, but it passes

Last nights wavebrain has been cleaned up and implemented in a slightly better way, and at the same time I have also removed the ast stuff because I just don't want it right now, chapter two tests are also passing, which honestly surprises me as I was expecting at least a few to fail, but uhh .. I guess chapter two is complete?

```sh
Styx → ~/D/C/G/C/tests $ ./test_compiler ../compiler.py --chapter 2 --verbose
...........................................
----------------------------------------------------------------------
Ran 43 tests in 15.254s

OK
```

#### 2025-12-19

Wavebrain has occured, and I now finally have an idea of how to populate register addresses for temporary variables, I just need to work on the 'scratch' register logic for which I'll likely just split out my clean up logic into a new class, and have it as a kind of sub-phase of my Instruction generation (primarily for reasons of keeping stuff seperate, and maintining the 'everything is a list' aproach that I have built upon so far).

Work on the AST Parser is also likely temporarily abandoned at this point, as I return to focus on my primary aims of keeping this as a Bytecode compiler, but I don't know, I may yet introduce it as an option (though I also would not be surprised if I were to extract the logic into a seperate branch).

#### 2025-12-16

Beginnings of an AST Parser have been added, it can correctly parse and generate trees for extremely simple files, but I don't believe it would currently pass the chapter one tests, and it also doesn't compile witnout warnings yet because there is essenially no error checking.  So there's still a lot of work to do, but for now I'm committing what I have, and will clecn it up another time.

#### 2025-12-13

Lots more documentation; to my understanding everything currently present in the code base now has at the very least a doc comment, but we are still lacking a lot of logic comments, and the code is far from literate.

The code generation phase has also been re-factored to be a bit smarter, we now convert the Instructions to a vector, so we can now use index access; this isn't particularly useful at the moment, but I think in time it might be; and the whole thing now opens up into separate functions to add in future additions.  Next up will be a final clean up pass on all the chapter one code, so that I may tick that final box above and finally remove the list.

#### 2025-12-12

Fixed bug in the parser where negative constants were not being correctly parsed, and also added negate and complement operations to the assembly back end, though they do not produce valid output yet (I need to introduce variable / register mapping)

Documentation documentation documentation, not all of it, but a lot more classes and functions are now documented (outstanding are still the Parser, Tackify, and Compiler classes)

### Chapter One Complete - 2025-12-11

Woops I forgot again, but with Advent of Code this year being shortened I've found myself wanting extra stuff to work on, and I remembered this; I have patched up some stuff in the Parser, so we now pass the chapter two parsing tests.

Finally got around to re-adding the codegen stage for at least the Chapter one stuff, so for the first time since October last year this compiler actually does something! (I have also realised that because I am on Apple Silicon I need to do an architecture swap each time I want to run this stage or it's tests)

```sh
Styx → ~/D/C/G/C/tests $ ./test_compiler ../compiler.py --chapter 1 --verbose
........................
----------------------------------------------------------------------
Ran 24 tests in 7.641s

OK
```

#### 2025-08-10

Long overdue relook at this; introduced a Tackify class, which does I think what's needed thus far - still failing tests however, which I'll try to work on tomorrow.

#### 2024-10-25

As a bit of fun I've written a second compiler driver, this time in Raku; it's quite a bit slower than the Python version.

#### 2024-10-24

Parser has been completely re-written.  Much like the Tokeniser it is now a stand-alone class that fully encapsulates its entire functionality - next up I will be focusing on proper error handling, and print-out, for the stages thus far cleaned up.

#### 2024-10-22

Tokeniser has been completely re-written, and is now a stand-alone class that greatly simplifies the wider control flow, and will be the start of my attempt to make this a bit more modular - next up will be the Parser.

#### 2024-10-14

Chapter one 'complete', it does everything it needs to and nothing that it shouldn't, but it's fairly rough around the edges and needs a lot of tidying up.

