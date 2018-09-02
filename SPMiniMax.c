/*
 * SPMiniMax.c
 *
 *  Created on: 9 Jun 2017
 *      Author: Sivan
 */

#include "SPMiniMax.h"

//assume that the currentGame is not a winning game
int spMinimaxGetBestMove(SPFiarGame* currentGame, unsigned int maxDepth){
	int column;
	int* mallocPtr = (int*)malloc(sizeof(int)); //indicator for malloc error
	if (mallocPtr==NULL)
		return -2;
	node* root = (node*)malloc(sizeof(node));

	if(root == NULL){
		free(mallocPtr);
		return -2;
	}

	if (currentGame == NULL || maxDepth <= 0 )
		return -1;

	root->gameStatus = spFiarGameCopy(currentGame);

	getBestMoveRec(root, maxDepth, 0, currentGame->currentPlayer, mallocPtr);

	if (mallocPtr == NULL){
		return -2;
	}

	column = root->column;

	spFiarGameDestroy(root->gameStatus);
	nodeDestroy(root);
	free(mallocPtr);
	return column;
}


