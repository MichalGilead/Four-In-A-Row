#ifndef SPFIARPARSER_H_
#define SPFIARPARSER_H_

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//specify the maximum line length
#define SP_MAX_LINE_LENGTH 1024

//a type used to represent a command
typedef enum {
	SP_UNDO_MOVE,
	SP_ADD_DISC,
	SP_SUGGEST_MOVE,
	SP_QUIT,
	SP_RESTART,
	SP_INVALID_LINE,
	SP_ALLOCATION_ERROR,
} SP_COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	SP_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg;
} SPCommand;

/**
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool spParserIsInt(const char* str);

/**
 * Parses a specified line. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true. In any other case then 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command is add_disc and the integer argument
 *              is valid
 *   arg      - the integer argument in case validArg is set to true
 */
SPCommand spParserPraseLine(const char* str);

/**
 * Parses a specified line in the difficulty level phase. If the line is only 1 integer
 * argument then the argument is parsed and is saved in to the pointer arg. In case of
 * memory allocation error arg gets the value NULL
 *
 * @difficultyLevel - string argument
 * @arg - int pointer that saves the difficulty level if it's a valid int
 *
 *
 */
void spParserDifficultyLevel(char* difficultyLevel, int* arg);

/**
 * checks for a pointer memory allocation error
 *
 * @p - the pointer we want to check
 *
 * @return
 * true - if there was a memory allocation error
 * false - otherwise
 *
 */
bool parserCheckMallocError(void* p);

/**
 * frees the memory of any pointer (void*)
 *
 * @src - the pointer we want to free
 *
 */
void parserDestroy(void* src);


#endif

