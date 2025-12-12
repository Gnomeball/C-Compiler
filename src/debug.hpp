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

// todo: Comment all of these!

/**
 * \brief Prints out the each Token as it is Tokenised
 */
#define DEBUG_TOKENISER

/**
 * \brief Prints out the list of Tokens after they have been Tokenised
 */
#define DEBUG_PRINT_TOKENS

/**
 * \brief When printing out the list of Tokens, this will add their line numbers to the output
 */
#define DEBUG_PRINT_TOKEN_LINE_NUMBERS

/**
 * \brief When printing out the list of Tokens, this will add their position in line to the output
 */
#define DEBUG_PRINT_TOKEN_POSITIONS

//      #define DEBUG_PARSER

/**
 * \brief Prints out the list of Bytes after they have been Parsed
 */
#define DEBUG_PRINT_BYTES

//      #define DEBUG_TACKY

/**
 * \brief Prints out the list of Tacky objects after they have been Tackified
 */
#define DEBUG_PRINT_TACKY

//      #define DEBUG_ASSEMBLER

/**
 * \brief Prints out the list of Assembly objects after they have been Assembled
 */
#define DEBUG_PRINT_ASSEMBLY

/**
 * \brief Prints out the path of any created assembly file prior to it's creation
 */
#define DEBUG_PRINT_OUTPUT_PATH
