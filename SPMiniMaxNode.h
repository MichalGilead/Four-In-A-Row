#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "limits.h"
#include "SPFIARGame.h"
//Put all declarations and constants here

typedef struct node{
	int column; //the column that yields the optimal move
	SPFiarGame* gameStatus; //the current game
	//SPFiarGame* children[SP_FIAR_GAME_N_COLUMNS]; //every child represents a game yielded by adding a disc to column i in gameStatus's board

}node;

/**
 * Building recursively the miniMax tree
 *
 * @n - a node in the tree
 * @maxDepth - the difficulty level of the game and the maximal depth we will build the tree to
 * @currDepth - the level of the tree we are at
 * @playerA - the current player
 *
 * @return
 * The score of the suggested move  column game
 */
int getBestMoveRec(node* n, unsigned int maxDepth, unsigned int currDepth, char playerA, int* mallocPtr);

/**
 * Sets a demo of the next move in a given game by specifying column index,
 * for the information needed for nodes in the tree.
 *
 * @param src - The source game
 * @param col - The specified column
 *
 * @return
 * Game new status
 */
SPFiarGame* createDemoSpFiarGame(SPFiarGame* src, int col);

/**
 * Calculates the score of a given game status by the score function
 *
 * @param src - The source game
 * @currPlayer - the current player
 *
 * @return
 * the game's score
 */
int gameScore(SPFiarGame* src, char currPlayer);

/**
 * Calculates the value of a span from a single cell (i,j) in the weight board
 * and updates the information to an array
 *
 * @gameBoardWeight - a matrix of the gameBoard with int weight instead of char for each player and a blank space
 * @spansCnt - an array of seven cells saving the counts of spans from count -3 to +3 in each cell respectively
 * @i - the row number
 * @j - the column number
 * @x - represents vertical span direction.
 * @y - represents horizontal span direction.
 *
 * @return
 * INT_MAX if spans value is +4 (current player is a winner)
 * INT_MIN if spans value is -4 (current player is a loser)
 * 1 otherwise
 */
int spanValueCalc(int gameBoardWeight[][7], int* spansCnt, int i, int j, int x, int y);

/**
 * Creates a matrix of the gameBoard with +1 for each of the current player discs in span,
 * -1 for each of the other player discs, and 0 for a blank space
 *
 * @gameBoard - the matrix representing the game status
 * @currPlayer - The current payer
 *
 * @return
 * the matching weight matrix
 */
void GameBoardWeightCalc(int gameBoardWeight[][7], char gameBoard[][7],char currPlayer);

/**
 * frees the memory of a node
 *
 * @src - the node we want to free
 *
 */
void nodeDestroy(node* src);

#endif

