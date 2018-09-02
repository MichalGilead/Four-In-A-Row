#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPFIARGame.h"
#include "SPFIARParser.h"
#include "SPMiniMax.h"


/**
 * checks if an int argument is between 1-7
 *
 * @arg - an int pointer
 *
 * @return
 * true - if the number is between 1-7
 * false - otherwise
 */
bool checkValidArg(int* arg);

/**
 * checks for a pointer memory allocation error
 *
 * @p - the pointer we want to check
 *
 * @return
 * true - if there was a memory allocation error
 * false - otherwise
 */
bool checkMallocError(void* p);

/**
 * frees the memory of any pointer (void*)
 *
 * @src - the pointer we want to free
 *
 */
void destroy(void* src);

/**
 * initializes the game
 *
 * @difficultyLevel - int pointer that saves the difficulty level if it's a valid int between 1-7
 * @currCommand - a pointer that gets the user's commands
 *
 * @return
 * 0 - if there was a malloc error or 'quit' command
 * 1 - otherwise
 *
 */
int gameInitialization(int* difficultyLevel, SPCommand* currCommand);

/**
 * Interprets and takes the user's command to action, and prints relevant errors\messages
 *
 * @game over - boolean value set to true if game over and false o.w
 * @currCommand - a pointer that gets the user's commands
 * @currGame - the current game
 * @difficultyLevel - an int pointer to the difficultyLevel int
 *
 * @return
 * -1 - if it's still the same turn of the user
 * 0 - if there was a malloc error or 'quit' command
 * 1 - if the current turn of the user is done (valid add_disc, undo_move or restart commands were acted)
 *
 */
int commandInterperter(bool gameOver, SPCommand* currCommand, SPFiarGame* currGame,
		int* difficultyLevel);


/**
 * Interprets and takes the user's command to action, and prints relevant
 * errors\messages - for the only commands that can be acted  when game is over
 * (invalid command, quit, undo_move or restart commands)
 *
 * @currCommand - a pointer that gets the user's commands
 * @currGame - the current game
 * @difficultyLevel - an int pointer to the difficultyLevel int
 *
 * @return
 * -1 - if it's still the same turn of the user
 * 0 - if there was a malloc error or 'quit' command
 * 1 - if the current turn of the user is done (undo_move or restart commands were acted)
 *
 */
int gameOverCommands(SPCommand* currCommand, SPFiarGame* currGame, int* difficultyLevel);

/**
 * checks the game status and prints a message accordingly, than calls commandsInterperter
 * with arguments based on the game status
 *
 * @win - a char representing the winner or tie if game over, else null character
 * @currCommand - a pointer that gets the user's commands
 * @currGame - the current game
 * @difficultyLevel - an int pointer to the difficultyLevel int
 * @innerItterCnt - an int indicator for first inner iteration in user's turn
 *
 * @return
 * -1 - if it's still the same turn of the user
 * 0 - if there was a malloc error or 'quit' command
 * 1 - if the current turn of the user is done (undo_move or restart commands were acted)
 *
 */
int playGame(char win, SPCommand* currCommand, SPFiarGame* currGame, int* difficultyLevel,
		int innerIterCnt);

#endif
