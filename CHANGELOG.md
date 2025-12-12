# Changelog

All notable changes to this project will be documented in this file.

### In Progress

The Great Clean up;

- [x] Tokeniser
- [x] Parser - Implemented, not fully documented
- [ ] Error Handling - Some implementation started
- [x] Tackyfier - needs a different name still, but it exists again at least
- [ ] Compiler
- [ ] Output

Chapter one;
- [x] Working
- [ ] Cleaned up and actually presentable
- [ ] Fully Documented

Chapter two;

- [x] Scanning
- [x] Parsing
- [x] Tacky - kinda?
- [ ] Compilation
- [ ] Output

### To do

- Actual Error Reporting
- The rest of the chapters!

### Done!

- October 14, 2024 - Chapter one 'complete', it does everything it needs to and nothing that it shouldn't, but it's fairly rough around the edges and needs a lot of tidying up.

- October 22, 2024 - Tokeniser has been completely re-written, and is now a stand-alone class that greatly simplifies the wider control flow, and will be the start of my attempt to make this a bit more modular - next up will be the Parser.

- October 24, 2024 - Parser has been completely re-written.  Much like the Tokeniser it is now a stand-alone class that fully encapsulates its entire functionality - next up I will be focusing on proper error handling, and print-out, for the stages thus far cleaned up.

- October 25, 2024 - As a bit of fun I've written a second compiler driver, this time in Raku; it's quite a bit slower than the Python version.

- August 10, 2025 - Long overdue relook at this; introduced a Tackify class, which does I think what's needed thus far - stil failing tests however, which I'll try to work on tomorrow.

- December 11, 2025 - Woops I forgot again, but with Advent of Code this year being shortened I've found myself wanting extra stuff to work on, and I remembered this; I have patched up some stuff in the Parser, so we now pass the chapter two parsing tests!

```
Styx → ~/D/C/G/C/tests $ ./test_compiler ../compiler.py --chapter 2 --stage parse --verbose
...........................................
----------------------------------------------------------------------
Ran 43 tests in 2.054s

OK
```

- December 11, 2025 - Finally got around to re-adding the codegen stage for at least the Chapter one stuff, so for the first time since October last year this compiler actually does something! (I have also now realised that because I am on Apple Silicon I need to do an architecture swap each time I want to run this stage)

```
Styx → ~/D/C/G/C/tests $ ./test_compiler ../compiler.py --chapter 1 --verbose
........................
----------------------------------------------------------------------
Ran 24 tests in 7.641s

OK
```

- December 12, 2025 - Fixed bug in the parser where negative constants were not being correctly parsed, and also added negate and complement operations to the assembly back end, though they do not produce valid output yet (I need to introduce variable / register mapping
  