/*
 * main.c
 *
 *  Created on: 8 Jun 2017
 *      Author: Sivan
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPFIARParser.h"
#include "SPFIARGame.h"
#include "SPMiniMax.h"
#include "SPMainAux.h"
#define UNDO_MOVE "undo_move"

int main() {
	int* difficultyLevel = (int*)malloc(sizeof(int));//pointer to the int value of game's difficulty level
	int init;//initialization status indicator
	char win;//gets winner or tie if game over, else NULL character
	int currTurn; //indication for the same turn of the user
	int innerIterCnt=1; //indication for first inner iteration in user's turn
	SPFiarGame* currGame;//saves the game
	SPCommand* currCommand = (SPCommand*)malloc(sizeof(SPCommand));//saves user's commands
	int commandInt; //helps determine how to continue the game after a user's command

	if (checkMallocError(difficultyLevel)){ //if there was an allocation error than terminates the game
		return 0;
	}

	//Initialization
	init = gameInitialization(difficultyLevel, currCommand);
	if (init == 0){ //allocation error or "quit" command - terminates the game
		destroy(difficultyLevel);
		destroy(currCommand);
		return 0;
	}

	//Game
	currGame = spFiarGameCreate(20);//creates new game
	if (checkMallocError(currGame)){//allocation error - terminates the game
		destroy(difficultyLevel);
		destroy(currCommand);
		return 0;
	}
	//each iteration represents 2 turns- user's turn followed by computer's turn
	while(true){
		spFiarGamePrintBoard(currGame); //prints the game board
		win = spFiarCheckWinner(currGame); //checks if there is a winner, and determines the game status
		currTurn = 1;
		innerIterCnt = 1;
		while (currTurn){ //while it is still the user's turn
			commandInt = playGame(win, currCommand, currGame, difficultyLevel, innerIterCnt);//activates game moves
			if (commandInt==0){//quit move in initialization or memory allocation error - terminates the game
				spFiarGameDestroy(currGame);
				destroy(difficultyLevel);
				destroy(currCommand);
				return 0;
			}
			if (commandInt==1)//moving to the next turn of the user
				currTurn=0;
			innerIterCnt++;
		}
	}
	return 0;
}

