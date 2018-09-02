/*
 * SPFIARGame.c
 *
 *  Created on: 9 Jun 2017
 *      Author: Sivan
 */

#include "SPFIARGame.h"


SPFiarGame* spFiarGameCreate(int historySize){
	if (historySize<=0)
		return NULL;
	SPFiarGame* newGame = (SPFiarGame*)malloc(sizeof(SPFiarGame));
	if (newGame == NULL){
		//printf("Error: malloc has failed");
		return NULL;
	}

	//initializing gameBoard to be empty;
	for (int i=0; i<SP_FIAR_GAME_N_ROWS;i++)
		for (int j=0; j<SP_FIAR_GAME_N_COLUMNS;j++)
			newGame->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;

	//initializing tops to be empty;
	for (int j=0; j<SP_FIAR_GAME_N_COLUMNS;j++)
		newGame->tops[j]=0;

	//initializing currentPlayer to be user;
	newGame->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

	//initializing history to a new spArrayList;
	newGame->history = spArrayListCreate(historySize);
	if (newGame->history==NULL){
		spFiarGameDestroy(newGame);
		return NULL;
	}
	return newGame;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src){
	if (src==NULL)
		return NULL;
	SPFiarGame* copy = spFiarGameCreate(src->history->maxSize);
	if (copy == NULL){
		//printf("Error: malloc has failed");
		return NULL;
	}
	copy->currentPlayer = src->currentPlayer;//updates currentPlayer to be the same as src's

	for (int i=0; i<SP_FIAR_GAME_N_ROWS;i++)//initializes gameBoard to be the same as src's
		for (int j=0; j<SP_FIAR_GAME_N_COLUMNS;j++)
			copy->gameBoard[i][j] = src->gameBoard[i][j];

	for (int j=0; j<SP_FIAR_GAME_N_COLUMNS;j++)//initializes tops to be the same as src's
		copy->tops[j] = src->tops[j];

	for (int k=0; k<src->history->actualSize; k++)//initializes history to be the same as src's
		copy->history->elements[k] = src->history->elements[k];

	copy->history->actualSize = src->history->actualSize;

	return copy;
}

void spFiarGameDestroy(SPFiarGame* src){
	if (src!=NULL){
		spArrayListDestroy(src->history);
		free(src);
	}
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col){
	if (src==NULL || col<0 || col >= SP_FIAR_GAME_N_COLUMNS)// col is out of bound or src==NULL
		return SP_FIAR_GAME_INVALID_ARGUMENT;

	if (!spFiarGameIsValidMove(src, col))// not a valid move
		return SP_FIAR_GAME_INVALID_MOVE;

	src->tops[col]++; //adding one disc to the top of the given column
	src->gameBoard[src->tops[col]-1][col] = src->currentPlayer;//adding the current player char to the game board entry

	if (spFiarGameGetCurrentPlayer(src) == SP_FIAR_GAME_PLAYER_1_SYMBOL)//changes current player
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	else
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

	SP_ARRAY_LIST_MESSAGE temp = spArrayListAddLast(src->history, col);//updates the history
	if (temp == SP_ARRAY_LIST_FULL){
		spArrayListRemoveFirst(src->history);
		spArrayListAddLast(src->history, col);
	}
	return SP_FIAR_GAME_SUCCESS;

}

//col is 0-based
bool spFiarGameIsValidMove(SPFiarGame* src, int col){
	if (src==NULL||col<0||col>6)// col is out of bound or src==NULL
		return false;
	return ((src->tops[col]) < SP_FIAR_GAME_N_ROWS);// false if col is full, else true
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src){
	if (src==NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	if (spArrayListIsEmpty(src->history)){//no history
		return SP_FIAR_GAME_NO_HISTORY;
	}
	int currCol;
	currCol = spArrayListGetLast(src->history);//saves last column a disc was added to
	src->gameBoard[src->tops[currCol]-1][currCol] = SP_FIAR_GAME_EMPTY_ENTRY;//clears the disc entry from game board
	src->tops[currCol]--;//reduces the top of the column
	spArrayListRemoveLast(src->history);//removes move from history
	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
	if (src==NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	for (int i=SP_FIAR_GAME_N_ROWS-1; i>=-2; i--)//prints out the board to the user upside down
		for (int j=0; j<=SP_FIAR_GAME_N_COLUMNS+1; j++){
			if (i>-1){
				if (j==0)
					printf("| ");
				else if (j==SP_FIAR_GAME_N_COLUMNS+1)
					printf("|\n");
				else if (0<j && j<SP_FIAR_GAME_N_COLUMNS+1)
					printf("%c ", src->gameBoard[i][j-1]);//prints the entry char
			}
			else if(i==-1){
				if (j<SP_FIAR_GAME_N_COLUMNS+1)
					printf("--");//prints a row of "-" in the row before last
				else
					printf("-\n");
			}
			else{
				if (j==0)
					printf("  ");
				else if (0<j && j<SP_FIAR_GAME_N_COLUMNS+1)//prints the columns numbers in the last line
					printf("%d ", j);
				else
					printf(" \n");
			}
		}

	return SP_FIAR_GAME_SUCCESS;
}


char spFiarGameGetCurrentPlayer(SPFiarGame* src){
	if (src==NULL)
		return SP_FIAR_GAME_EMPTY_ENTRY;
	return src->currentPlayer;//return current player
}

char spFiarCheckWinner(SPFiarGame* src){
	if (src==NULL)
		return '\0';
	char checkWinner;
	int blanksCnt = 0;//counts blank entries to separate a tie from an unfinished game
	for (int i=0; i<SP_FIAR_GAME_N_ROWS;i++)//goes over each entry in the game board
		for (int j=0; j<SP_FIAR_GAME_N_COLUMNS;j++){
			if (src->gameBoard[i][j] == SP_FIAR_GAME_EMPTY_ENTRY)
				blanksCnt++;//increments blanks counter for each blank entry

			else{//if there's a player disc in entry
				if (i<3){ //checks vertical downwards span
					checkWinner = checkIfWinnerSpan(src->gameBoard, i, j, 1, 0);
					if (checkWinner == SP_FIAR_GAME_PLAYER_1_SYMBOL || checkWinner == SP_FIAR_GAME_PLAYER_2_SYMBOL)
						return checkWinner;
				}

				if (j<4){ //checks horizontal span to the right
					checkWinner = checkIfWinnerSpan(src->gameBoard, i, j, 0, 1);
					if (checkWinner == SP_FIAR_GAME_PLAYER_1_SYMBOL || checkWinner == SP_FIAR_GAME_PLAYER_2_SYMBOL)
						return checkWinner;
				}

				if (i<3 && j<4){ //checks diagonal downwards span to the right
					checkWinner = checkIfWinnerSpan(src->gameBoard, i, j, 1, 1);
					if (checkWinner == SP_FIAR_GAME_PLAYER_1_SYMBOL || checkWinner == SP_FIAR_GAME_PLAYER_2_SYMBOL)
						return checkWinner;
				}

				if (i<3 && j>2){ //checks diagonal downwards span to the left
					checkWinner = checkIfWinnerSpan(src->gameBoard, i, j, 1, -1);
					if (checkWinner == SP_FIAR_GAME_PLAYER_1_SYMBOL || checkWinner == SP_FIAR_GAME_PLAYER_2_SYMBOL)
						return checkWinner;
				}
			}
		}

	if (blanksCnt == 0)//if all entries have a player's char and no winner span was found - it's a tie
		return SP_FIAR_GAME_TIE_SYMBOL;

	return '\0';//if there are blank entries nd no winner span was found - no winner
}

char checkIfWinnerSpan(char gameBoard[][SP_FIAR_GAME_N_COLUMNS], int i, int j, int x, int y){
	int cnt = 1; //counts number of identical cells in the current span
	char currCell;
	currCell = gameBoard[i][j];

	for (int k=1;k<SP_FIAR_GAME_SPAN;k++) //increments counter for each identical char cell
		if (gameBoard[i+k*x][j+k*y] == currCell)
			cnt++;

	if(cnt==SP_FIAR_GAME_SPAN)//if the span is of count 4 - there's a winner - returns winner char
		return currCell;

	else
		return '\0';
}
