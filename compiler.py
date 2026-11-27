#!/usr/bin/env python3

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

parser = argparse.ArgumentParser(prog='compiler')

COMPILER_PATH = Path(__file__).resolve().parent / "bin" / "compiler"

def setup_args():
    """Set up the command-line arguments for the compiler.

    Registers and returns the command-line arguments for the compiler.
    """
    parser.add_argument('file', help='The input file')

    parser.add_argument('--lex',      help='Stop after lexing the input file',     action='store_true')
    parser.add_argument('--parse',    help='Stop after Parsing the input file',    action='store_true')
    parser.add_argument('--tacky',    help='Stop after Tackifying the input file', action='store_true')
    parser.add_argument('--assemble', help='Stop after Assembling the input file', action='store_true')
    parser.add_argument('--codegen',  help='Stop after Compiling the input file',  action='store_true')

    parser.add_argument('--keep-assembly', help='Don\'t delete the assembly file after compilation', action='store_true')

    return parser.parse_args()

def do_preprocess(source_file: Path) -> int:
    """Runs the clang preprocessor against the source file.

    Writes a sibling `.i` file using the same file name, preserving
    comments but dropping line commands, and returns the exit code.

    Args:
        :param source_file: The source file to preprocess.
    """
    flags = [
        "--preprocess",          #? only preprocess the file
        "--no-line-commands",    #? leaving line commands out
        "--comments",            #? but leaving comments in
        "--traditional",         #? whilst not chomping the empty lines
        "-Wno-invalid-pp-token", #? and don't then complain about apostrophes in comments lol
    ]
    preprocessed_file = source_file.with_suffix(".i")
    ret = subprocess.run(["clang", *flags, str(source_file), "-o", str(preprocessed_file)])
    return ret.returncode

def do_compile(source_file: Path, stop: str, stage: str) -> int:
    """Runs the compiler against the preprocessed source file.

    Runs the compiler against  the preprocessed source file, producing a
    sibling `.asm` file using the same file name, returning the exit code.
    The `.i` file is always removed, regardless of exit code.

    The `.asm` file is only produced if the compiler reaches the code generation stage.

    Args:
        :param source_file: The source file to compile.
        :param stage: The stage to compile up until.
        :param stop: Whether to stop after the given stage.

    Returns:
        int: 0 if compilation succeeded, non-zero otherwise.
    """
    preprocessed_file = source_file.with_suffix(".i")
    try:
        ret = subprocess.run([str(COMPILER_PATH), str(preprocessed_file), stop, stage])
        return ret.returncode
    finally:
        if preprocessed_file.exists():
            preprocessed_file.unlink()

def do_assemble(source_file: Path, keep_assembly: bool) -> int:
    """Runs the clang assembler against the output file.

    Runs clang against the output file, producing an executable file, which will match
    the source file's path but without any extension, returning clang's exit code. It
    then attempts to clean up the `.asm` file, unless keep_assembly is true.

    Args:
        :param source_file: The source file to assemble.
        :param keep_assembly: Whether to keep the assembly file after assembly.
    """
    assembly_file = source_file.with_suffix(".asm")
    executable_file = source_file.with_suffix("")
    flags = ["-arch", "x86_64"] if sys.platform == "darwin" else []
    ret = subprocess.run(["clang", *flags, "-x", "assembler", str(assembly_file), "-o", str(executable_file)])
    # remove the assembly file
    if not keep_assembly and assembly_file.exists():
        assembly_file.unlink()
    return ret.returncode

def main():
    """Main entry point for the compiler.

    Begins by checking if the input file exists, and is a file, and that clang is on the PATH.
    If any of these checks fail, the compiler stops and returns a non-zero exit code.  Otherwise,
    executes the compiler stages as specified, stopping at the earliest stage in the pipeline,
    regardless if multiple `stop` flags are supplied.  If a stage fails, the compiler stops and
    returns a non-zero exit code. If all stages succeed, the compiler returns 0.
    """

    # setup the arguments
    args = setup_args()

    # ensure the input file actually exists
    source_file = Path(args.file)
    if not source_file.is_file():
        print(f"Input file not found : {args.file}", file=sys.stderr)
        return 1

    # define our stages, and determine if we are stopping
    stage_flags = (args.lex, args.parse, args.tacky, args.assemble, args.codegen)
    stop = any(stage_flags)
    stage = next(
        (index for index, enabled in enumerate(stage_flags, start=1) if enabled),
        len(stage_flags),
    )

    # check if clang exists and can be used
    if shutil.which("clang") is None:
        print("clang not found, cannot continue", file=sys.stderr)
        return 1

    # pre-process the file before we do anything
    ret = do_preprocess(source_file)
    if ret != 0:
        preprocessed_file = source_file.with_suffix(".i")
        if preprocessed_file.exists():
            preprocessed_file.unlink()
        return ret

    # run the compiler
    ret = do_compile(source_file, str(stop), str(stage))

    # if we stopped, or the return code was non-zero, exit
    if stop or ret != 0:
        return ret
    # print(f"Python ret value = {ret}")

    # otherwise, assemble the result
    return do_assemble(source_file, args.keep_assembly)

if __name__ == "__main__":
    raise SystemExit(main())

