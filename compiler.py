#!/usr/bin/env python

import argparse
import subprocess

parser = argparse.ArgumentParser(prog='compiler')

COMPILER_PATH = "../bin/compiler"

def setup_args():
    parser.add_argument('file', help='The input file')

    parser.add_argument('--lex',     help='Stop after lexing the input file',   action='store_true')
    parser.add_argument('--parse',   help='Stop after Parsing the input file',  action='store_true')
    parser.add_argument('--codegen', help='Stop after Compling the input file', action='store_true')

    return parser.parse_args()

def do_preprocess(file):
    # do the preprocess
    subprocess.call(f"clang -E -P {file}.c -o {file}.i", shell=True)

def do_compile(file, lex, parse, codegen):
    # do the compile
    ret = subprocess.call(f"{COMPILER_PATH} {file}.i {lex or parse or codegen} {parse or codegen} {codegen}", shell=True)
    # remove the preprocessed file
    subprocess.call(f"rm {file}.i", shell=True)
    return ret

# def do_assemble(file):
    # do the assemble
    # os.system(f"clang {file}.s -o {file}")

def main():
    # setup the arguments
    args = setup_args()

    # extract the file name
    file, _ = args.file.split(".")

    # run the compilation
    do_preprocess(file)
    ret = do_compile(file, args.lex, args.parse, args.codegen)

    # Only if none of lex, parse, or codegen are specified should we emit assembly
    # Therefore, only if none are specified will we be able to run this step

    # assemble = not (args.lex and args.parse and args.codegen)
    # if (assemble): do_assemble(file)

    print(f"Python ret value = {ret}")

    exit(ret)

main()
