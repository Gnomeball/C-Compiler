#!/usr/bin/env python3

import argparse
import subprocess
import os.path

parser = argparse.ArgumentParser(prog='compiler')

COMPILER_PATH = "/Users/gnome/Documents/Code/GitHub/C-Compiler/bin/compiler"

def setup_args():
    parser.add_argument('file', help='The input file')

    parser.add_argument('--lex',      help='Stop after lexing the input file',     action='store_true')
    parser.add_argument('--parse',    help='Stop after Parsing the input file',    action='store_true')
    parser.add_argument('--tacky',    help='Stop after Tackifying the input file', action='store_true')
    parser.add_argument('--assemble', help='Stop after Assembling the input file', action='store_true')
    parser.add_argument('--codegen',  help='Stop after Compling the input file',   action='store_true')

    parser.add_argument('--keep-assembly', help='Don\'t delete the assembly file after compilation', action='store_true')

    return parser.parse_args()

def do_preprocess(file):
    # do the preprocess
    subprocess.call(f"clang -E -P {file}.c -o {file}.i", shell=True)

def do_compile(file, stop, stage):
    # do the compile
    ret = subprocess.run(f"{COMPILER_PATH} {file}.i {stop} {stage}", shell=True)
    # remove the preprocessed file
    subprocess.call(f"rm {file}.i", shell=True)
    return ret.returncode

def do_assemble(file, keep_assembly):
    # do the assemble
    subprocess.call(f"clang {file}.s -o {file}", shell=True)
    # remove the assmebly file
    if (not keep_assembly):
        subprocess.call(f"rm {file}.s", shell=True)

def main():
    # setup the arguments
    args = setup_args()

    # ensure the input file actually exists
    if (not os.path.isfile(args.file)):
        print(f"Input file not found : {args.file}")
        exit (1)

    # extract the file name
    file, _ = args.file.split(".")

    # work out if we're stopping, only one of these needs to be true if so
    stop = args.lex or args.parse or args.tacky or args.assemble or args.codegen

    # depending on the value of stop, set the stage we are to stop at
    stage = [i for i, x in enumerate([args.lex, args.parse, args.tacky, args.assemble, args.codegen]) if x][0]+1 if stop else 5

    # pre-process the file before we do anything
    do_preprocess(file)

    # run the compiler
    ret = do_compile(file, stop, stage)

    # only if we produced output, can we assemble it
    if (not stop and ret == 0): do_assemble(file, args.keep_assembly)

    # print(f"Python ret value = {ret}")

    exit(ret)

main()

