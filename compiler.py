#!/usr/bin/env python

import argparse
import subprocess

parser = argparse.ArgumentParser(prog='compiler')

COMPILER_PATH = "/home/gnome/Documents/Code/C++/C-Compiler/bin/compiler"

def setup_args():
    parser.add_argument('file', help='The input file')

    parser.add_argument('--lex',     help='Stop after lexing the input file',   action='store_true')
    parser.add_argument('--parse',   help='Stop after Parsing the input file',  action='store_true')
    parser.add_argument('--codegen', help='Stop after Compling the input file', action='store_true')

    return parser.parse_args()

def do_preprocess(file):
    # do the preprocess
    subprocess.call(f"clang -E -P {file}.c -o {file}.i", shell=True)

def do_compile(file, lex, parse, codegen, output):
    # do the compile
    ret = subprocess.run(f"{COMPILER_PATH} {file}.i {lex or parse or codegen} {parse or codegen} {codegen} {output}", shell=True)
    # remove the preprocessed file
    subprocess.call(f"rm {file}.i", shell=True)
    return ret.returncode

def do_assemble(file):
    # do the assemble
    subprocess.call(f"clang {file}.s -o {file}", shell=True)
    # remove the assmebly file
    subprocess.call(f"rm {file}.s", shell=True)

def main():
    # setup the arguments
    args = setup_args()

    # extract the file name
    file, _ = args.file.split(".")

    # work out if we're producing assembly - all three flags must be False
    output = not (args.lex or args.parse or args.codegen)

    # run the compilation
    do_preprocess(file)
    ret = do_compile(file, args.lex or output, args.parse or output, args.codegen or output, output)

    # if we are outputting, we can assemble
    if (output): do_assemble(file)

    # print(f"Python ret value = {ret}")

    exit(ret)

main()

