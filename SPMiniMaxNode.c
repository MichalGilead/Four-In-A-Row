/*
 * SPMiniMaxNode.c
 *
 *  Created on: 9 Jun 2017
 *      Author: Sivan
 */

#include "SPMiniMaxNode.h"


int getBestMoveRec(node* n, unsigned int maxDepth, unsigned int currDepth, char playerA, int* mallocPtr) {
	int currValue;
	int score;
	char win;
	int firstValidChild;
	node* child;

	if (currDepth<maxDepth){
		if (currDepth %2 == 0) //if the current level is a max-level then initialize game value to INT_MIN
			currValue = INT_MIN;
		else
			currValue = INT_MAX; //if the current level is a max-level then initialize game value to INT_MAX
		win = spFiarCheckWinner(n->gameStatus); //check if the current game has a winner
		if (win == '\0'){ //if there is no winner then continue building the tree
			firstValidChild=-1;
			for (int i=0; i<SP_FIAR_GAME_N_COLUMNS; i++){//goes over all possible moves
				if (spFiarGameIsValidMove(n->gameStatus,i)){ //create a node for this move
					if (firstValidChild==-1)
						firstValidChild=i;
					child = (node*)malloc(sizeof(node));//every child represents a game yielded by adding a disc to column i in gameStatus's board
					if (child == NULL){//malloc error occurred
						mallocPtr = NULL;//indicator for malloc error
						return 0;
					}
					child->gameStatus = createDemoSpFiarGame(n->gameStatus,i);
					if (child->gameStatus == NULL){//malloc error occurred
						nodeDestroy(child);
						mallocPtr = NULL;
						return 0;
					}

					score = getBestMoveRec(child, maxDepth, currDepth+1, playerA, mallocPtr);
					if (mallocPtr == NULL){
						spFiarGameDestroy(child->gameStatus);
						nodeDestroy(child);
						return 0;
					}
					if (currDepth %2 == 0){ //max level
						if (score > currValue){
							n->column = i;
							currValue = score;
						}
						spFiarGameDestroy(child->gameStatus);
						nodeDestroy(child);
						if (i==SP_FIAR_GAME_N_COLUMNS-1){
							if (currValue==INT_MIN)
								n->column = firstValidChild;
							return currValue;
						}
					}
					else{ //min level
						if (score < currValue){
							n->column = i;
							currValue = score;
						}
						spFiarGameDestroy(child->gameStatus);
						nodeDestroy(child);
						if (i==SP_FIAR_GAME_N_COLUMNS-1){
							if (currValue==INT_MAX)
								n->column = firstValidChild;
							return currValue;
						}
					}
				}
			}
		}
		else if (win == SP_FIAR_GAME_TIE_SYMBOL){
			return gameScore(n->gameStatus, playerA);
		}
		else if (win == playerA){
			return INT_MAX;
		}

		else{
			return INT_MIN;
		}
	}
	return gameScore(n->gameStatus, playerA);
}


SPFiarGame* createDemoSpFiarGame(SPFiarGame* src, int col){
	if (spFiarGameIsValidMove(src,col)){
		SPFiarGame* childGameStatus = spFiarGameCopy(src);
		if (childGameStatus == NULL)
			return NULL;

		childGameStatus->tops[col]++; //adding one disk to the top of the given column
		childGameStatus->gameBoard[childGameStatus->tops[col]-1][col] = childGameStatus->currentPlayer;

		if (spFiarGameGetCurrentPlayer(src) == SP_FIAR_GAME_PLAYER_1_SYMBOL)
			childGameStatus->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
		else
			childGameStatus->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

		return childGameStatus;
	}
	else return NULL;
}

int gameScore(SPFiarGame* src, char currPlayer){
	int weightVector[SP_FIAR_GAME_SPANS_COUNT] = {-5,-2,-1,0,1,2,5};
	int spansCnt[SP_FIAR_GAME_SPANS_COUNT];
	int gameBoardWeight[SP_FIAR_GAME_N_ROWS][SP_FIAR_GAME_N_COLUMNS];
	int spanScore;
	int gameScore = 0;

	for (int i=0; i<SP_FIAR_GAME_SPANS_COUNT; i++)
		spansCnt[i] = 0;

	GameBoardWeightCalc(gameBoardWeight, src->gameBoard, currPlayer);
	for (int i=0; i<SP_FIAR_GAME_N_ROWS;i++)
		for (int j=0; j<SP_FIAR_GAME_N_COLUMNS;j++){
			if (i<3){
				spanScore = spanValueCalc(gameBoardWeight, spansCnt, i, j, 1, 0);
				if (spanScore != 1){
					return spanScore;
				}
			}

			if (j<4){
				spanScore = spanValueCalc(gameBoardWeight, spansCnt, i, j, 0, 1);
				if (spanScore != 1){
					return spanScore;
				}
			}

			if (i<3 && j<4){
				spanScore = spanValueCalc(gameBoardWeight, spansCnt, i, j, 1, 1);
				if (spanScore != 1){
					return spanScore;
				}
			}

			if (i<3 && j>2){
				spanScore = spanValueCalc(gameBoardWeight, spansCnt, i, j, 1, -1);
				if (spanScore != 1){
					return spanScore;
				}
			}
		}
	for (int i=0; i<SP_FIAR_GAME_SPANS_COUNT; i++)
		gameScore += spansCnt[i]*weightVector[i];
	return gameScore;
}


int spanValueCalc(int gameBoardWeight[][7], int* spansCnt, int i, int j, int x, int y){
	int spanVal = 0;
	for (int k=0;k<SP_FIAR_GAME_SPAN;k++) //calculation
		spanVal+= gameBoardWeight[i+k*x][j+k*y];

	if (spanVal == SP_FIAR_GAME_SPAN)
		return INT_MAX;
	else if (spanVal == -1*SP_FIAR_GAME_SPAN)
		return INT_MIN;
	else spansCnt[spanVal+3]++;
	return 1; //update was done
}

void GameBoardWeightCalc(int gameBoardWeight[][7], char gameBoard[][7],char currPlayer){

	for (int i=0; i<SP_FIAR_GAME_N_ROWS;i++){
		for (int j=0; j<SP_FIAR_GAME_N_COLUMNS;j++){
			if (gameBoard[i][j] == SP_FIAR_GAME_EMPTY_ENTRY)
				gameBoardWeight[i][j] = 0;
			else if (gameBoard[i][j] == currPlayer)
				gameBoardWeight[i][j] = 1;
			else
				gameBoardWeight[i][j] = -1;
		}
	}
}


void nodeDestroy(node* src){
	if (src!=NULL)
		free(src);
}


