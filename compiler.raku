#!/opt/homebrew/bin/rakudo

my $COMPILER_PATH = "/Users/gnome/Documents/Code/GitHub/C-Compiler/bin/compiler";

sub do_preprocess($file) {
    # do the preprocess
    shell "clang -E -P $file.c -o $file.i";
}

sub do_compile($file, $stop, $stage) returns Int {
    # do the compile
    my $ret_val = shell "$COMPILER_PATH $file.i $stop $stage";
    # remove the preprocessed file
    shell "rm $file.i";
    return $ret_val.exitcode;
}

sub do_assemble($file, $keep) {
    # do the assemble
    shell "clang $file.s -o $file";
    # remove the assembly file
    if (!$keep) { shell "rm $file.s"; }
}

my %*SUB-MAIN-OPTS = :named-anywhere; # allow named variables at any location

sub MAIN(
    Str  $file,              #= The input file
    Bool :$lex     = False,  #= Stop after lexing the input file
    Bool :$parse   = False,  #= Stop after parsing the input file
    Bool :$tacky   = False,  #= Stop after tackifying the input file
    Bool :$codegen = False,  #= Stop after compiling the input file
    Bool :$keepasm = False,  #= Don't delete the assembly file after compilation
) {
    # Ensure the input file exists, and is a file
    if (!$file.IO.f) {
        say "Input file not found : $file";
        exit 1;
    }

    # Remove the suffix from the file name
    my $file_name = $file.split('.')[0];

    # Work out if we're stopping, only one of these needs to be true if so
    my $stop = $lex || $parse || $tacky || $codegen;

    say $stop;

    # Set the stage we are to stop at;
    #   1: Lex, 2: Parse, 3: Tackify, 4: Codegen
    my $stage = 1 + ($lex, $parse, $tacky, $codegen).first: * == True, :k;

    # Preprocess the file before we do anything
    do_preprocess($file_name);

    # Run our compiler
    my $ret_val = do_compile($file_name, $stop, $stage);

    # Only if we produced output, can we assemble it
    if (!$stop && $ret_val == 0) { do_assemble($file_name, $keepasm); }

    exit $ret_val;
}
