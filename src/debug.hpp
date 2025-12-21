/**
 * \file debug.hpp
 * \author Gnomeball
 * \brief A file listing debug directives
 * \version 0.1
 * \date 2024-10-24
 */

/*
    Un-comment any particular directive to recieve that output
*/

/**
 * \brief Prints out each Token as it is Tokenised
 *
 *   === Beginning Tokenisation ===
 *
 * Found : TK_KEYWORD_INT
 * Found : TK_IDENTIFIER
 * Found : TK_OPEN_PARENTHESIS
 * Found : TK_CLOSE_PARENTHESIS
 * Found : TK_OPEN_BRACE
 * Found : TK_KEYWORD_RETURN
 * Found : TK_CONSTANT
 * Found : TK_SEMI_COLON
 * Found : TK_CLOSE_BRACE
 * Found : TK_EOF
 *
 *  === Finishing Tokenisation ===
 */
// #define DEBUG_TOKENISER

/**
 * \brief Prints out the list of Tokens after they have been Tokenised
 *
 * Token [Type: TK_KEYWORD_INT]
 * Token [Type: TK_IDENTIFIER, Value: main]
 * Token [Type: TK_OPEN_PARENTHESIS]
 * Token [Type: TK_CLOSE_PARENTHESIS]
 * Token [Type: TK_OPEN_BRACE]
 * Token [Type: TK_KEYWORD_RETURN]
 * Token [Type: TK_CONSTANT, Value: 8]
 * Token [Type: TK_SEMI_COLON]
 * Token [Type: TK_CLOSE_BRACE]
 * Token [Type: TK_EOF]
 */
// #define DEBUG_PRINT_TOKENS

/**
 * \brief When printing out the list of Tokens this will add their line number and position in line to the output
 *
 * Has no effect if the above directive isn't in use
 *
 * Token [Type: TK_KEYWORD_INT, line: 1, pos: 0]
 * Token [Type: TK_IDENTIFIER, Value: main, line: 1, pos: 4]
 * Token [Type: TK_OPEN_PARENTHESIS, line: 1, pos: 8]
 * Token [Type: TK_CLOSE_PARENTHESIS, line: 1, pos: 9]
 * Token [Type: TK_OPEN_BRACE, line: 1, pos: 11]
 * Token [Type: TK_KEYWORD_RETURN, line: 2, pos: 4]
 * Token [Type: TK_CONSTANT, Value: 8, line: 2, pos: 11]
 * Token [Type: TK_SEMI_COLON, line: 2, pos: 12]
 * Token [Type: TK_CLOSE_BRACE, line: 3, pos: 0]
 * Token [Type: TK_EOF, line: 4, pos: 1]
 */
#define DEBUG_PRINT_TOKEN_LINE_POSITIONS

/**
 * \brief Prints out each Byte as it is Parsed
 *
 *  === Beginning Parsing ===
 *
 * Found : OP_FUNCTION
 * Found : CONSTANT
 * Found : RETURN
 *
 *  === Finishing Parsing ===
 */
// #define DEBUG_PARSER

/**
 * \brief Prints out the list of Bytes after they have been Parsed
 *
 * Byte [Op: OP_FUNCTION, Value: main]
 * Byte [Op: CONSTANT, Value: 8]
 * Byte [Op: RETURN]
 */
// #define DEBUG_PRINT_BYTES

/**
 * \brief Prints out the program Node after being Parsed into an AST
 *
 * PROGRAM(Function:
 *   FUNCTION(Identifier: main, Statement:
 *     RETURN(Expression:
 *       CONSTANT(Value: 8
 *       )
 *     )
 *   )
 * )
 */
// #define DEBUG_PRINT_AST

/**
 * \brief Prints out each Tacky as it is Tackified
 *
 *  === Beginning Tackify ===
 *
 * Found : FUNCTION
 * Found : VALUE
 * Found : RETURN
 *
 *  === Finishing Tackify ===
 */
// #define DEBUG_TACKY

/**
 * \brief Prints out the list of Tacky objects after they have been Tackified
 *
 * Tacky [Op: FUNCTION, src: main]
 * Tacky [Op: RETURN, name: 8]
 */
#define DEBUG_PRINT_TACKY

/**
 * \brief Prints out each Instruction as it is Compiled
 *
 *  === Beginning Compilation ===
 *
 * Found : MOV
 * Found : RET
 *
 *  === Finishing Compilation ===
 */
// #define DEBUG_COMPILER

/**
 * \brief Prints out the list of Assembly objects after they have been Assembled
 *
 * Assembly [Ins: MOV, src: 8, dest: eax]
 * Assembly [Ins: RET]
 */
#define DEBUG_PRINT_ASSEMBLY

/**
 * \brief Prints out the path of any created assembly file prior to it's creation
 *
 * Output path : test/test.asm
 */
#define DEBUG_PRINT_OUTPUT_PATH
