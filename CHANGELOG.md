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

Chapter two;

- [x] Scanning
- [x] Parsing
- [x] Tacky - kinda?
- [ ] Compilation
- [ ] Output

### To do

- Actual Error Reporting
- Re-implement Tackyfication
- Re-implement Compilation
- Re-implement Output
- The rest of the chapters!

### Done!

- October 14, 2024 - Chapter one 'complete', it does everything it needs to and nothing that it shouldn't, but it's fairly rough around the edges and needs a lot of tidying up.

- October 22, 2024 - Tokeniser has been completely re-written, and is now a stand-alone class that greatly simplifies the wider control flow, and will be the start of my attempt to make this a bit more modular - next up will be the Parser.

- October 24, 2024 - Parser has been completely re-written.  Much like the Tokeniser it is now a stand-alone class that fully encapsulates its entire functionality - next up I will be focusing on proper error handling, and print-out, for the stages thus far cleaned up.

- October 25, 2024 - As a bit of fun I've written a second compiler driver, this time in Raku; it's quite a bit slower than the Python version.

- August 10, 2025 - Long overdue relook at this; introduced a Tackify class, which does I think what's needed thus far - stil failing tests however, which I'll try to work on tomorrow.
