#!/usr/bin/env python3

import argparse
import subprocess
import sys
from pathlib import Path

parser = argparse.ArgumentParser(prog='compiler')

COMPILER_PATH = Path(__file__).resolve().parent / "bin" / "compiler"

def setup_args():
    parser.add_argument('file', help='The input file')

    parser.add_argument('--lex',      help='Stop after lexing the input file',     action='store_true')
    parser.add_argument('--parse',    help='Stop after Parsing the input file',    action='store_true')
    parser.add_argument('--tacky',    help='Stop after Tackifying the input file', action='store_true')
    parser.add_argument('--assemble', help='Stop after Assembling the input file', action='store_true')
    parser.add_argument('--codegen',  help='Stop after Compling the input file',   action='store_true')

    parser.add_argument('--keep-assembly', help='Don\'t delete the assembly file after compilation', action='store_true')

    return parser.parse_args()

def do_preprocess(source_file):
    # do the preprocess
    flags = [
        "--preprocess",          #? only preprocess the file
        "--no-line-commands",    #? leaving line commands out
        # "--comments",            #? but leaving comments in
        "--traditional",         #? whilst not chomping the empty lines
        "-Wno-invalid-pp-token", #? and don't then complain about apostrophes in comments lol
    ]
    preprocessed_file = source_file.with_suffix(".i")
    ret = subprocess.run(["clang", *flags, str(source_file), "-o", str(preprocessed_file)])
    return ret.returncode

def do_compile(source_file, stop, stage):
    # do the compile
    preprocessed_file = source_file.with_suffix(".i")
    try:
        ret = subprocess.run([str(COMPILER_PATH), str(preprocessed_file), stop, stage])
        return ret.returncode
    finally:
        if preprocessed_file.exists():
            preprocessed_file.unlink()

def do_assemble(source_file, keep_assembly):
    # do the assemble
    assembly_file = source_file.with_suffix(".asm")
    executable_file = source_file.with_suffix("")
    flags = ["-arch", "x86_64"] if sys.platform == "darwin" else []
    ret = subprocess.run(
        ["clang", *flags, "-x", "assembler", str(assembly_file), "-o", str(executable_file)]
    )
    # remove the assembly file
    if not keep_assembly and assembly_file.exists():
        assembly_file.unlink()
    return ret.returncode

def main():
    # setup the arguments
    args = setup_args()

    # ensure the input file actually exists
    source_file = Path(args.file)
    if not source_file.is_file():
        print(f"Input file not found : {args.file}")
        return 1

    # work out if we're stopping, only one of these needs to be true if so
    stop = args.lex or args.parse or args.tacky or args.assemble or args.codegen

    # depending on the value of stop, set the stage we are to stop at
    stage = next(
        (i for i, enabled in enumerate(
            [args.lex, args.parse, args.tacky, args.assemble, args.codegen], start=1
        ) if enabled),
        5,
    )

    # pre-process the file before we do anything
    ret = do_preprocess(source_file)
    if ret != 0:
        preprocessed_file = source_file.with_suffix(".i")
        if preprocessed_file.exists():
            preprocessed_file.unlink()
        return ret

    # run the compiler
    ret = do_compile(source_file, str(stop), str(stage))

    # only if we produced output, can we assemble it
    if not stop and ret == 0:
        return do_assemble(source_file, args.keep_assembly)

    # print(f"Python ret value = {ret}")

    return ret

if __name__ == "__main__":
    raise SystemExit(main())

