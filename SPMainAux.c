/*
 * SPMainAux.c
 *
 *  Created on: 18 Jun 2017
 *      Author: Sivan
 */

#include "SPMainAux.h"

bool checkValidArg(int* arg){
	if (*arg>=1 && *arg<=7)
		return true;
	return false;
}

bool checkMallocError(void* p){
	if (p == NULL){
		printf("Error: malloc has failed");
		return true;
	}
	return false;
}

void destroy(void* src){
	if (src!=NULL)
		free(src);
}


int gameInitialization(int* difficultyLevel, SPCommand* currCommand){
	char difficultyLevelStr[SP_MAX_LINE_LENGTH];//the string getting the usre's difficulty level

	do{
		printf("Please enter the difficulty level between [1-7]:\n");
		fgets(difficultyLevelStr, SP_MAX_LINE_LENGTH, stdin);

		//check if the user entered "quit" command
		*currCommand = spParserPraseLine(difficultyLevelStr);
		if (currCommand->cmd == SP_QUIT){
			printf("Exiting...\n");
			return 0;
		}

		spParserDifficultyLevel(difficultyLevelStr, difficultyLevel); //initializing *difficultyLevel to hold the difficultyLevel(int)
		if (checkMallocError(difficultyLevel)){ //memory allocation error - terminates the game
			return 0;
		}
		if (!checkValidArg(difficultyLevel)) //difficultyLevel is an invalid int - prints error
			printf("Error: invalid level (should be between 1 to 7)\n");
		}
		while(!checkValidArg(difficultyLevel)); //continue while the user has'nt entered a valid number or "quit"

	return 1;
}


int commandInterperter(bool gameOver, SPCommand* currCommand, SPFiarGame* currGame,
		int* difficultyLevel){
	int col;
	SP_FIAR_GAME_MESSAGE currMessage;

	if (gameOver){//if game is over
		if (currCommand->cmd == SP_ADD_DISC||currCommand->cmd == SP_SUGGEST_MOVE){//prints errors for invalid commands
			printf("Error: the game is over\n");
			return -1;
		}
		//calls gameOverCommands to take care of valid commands
		return gameOverCommands(currCommand, currGame, difficultyLevel);
	}
	else{//if game not over
		//calls gameOverCommands to take care of invalid command, quit, undo_move or restart commands
		int gameOverCmd = gameOverCommands(currCommand, currGame, difficultyLevel);
		if (gameOverCmd==0||gameOverCmd==1)
			return gameOverCmd;
		//else if gameOverCommands==-1 - it's still the same turn
		//if the command is add_disc
		if (currCommand->cmd == SP_ADD_DISC){
			if(!(currCommand->validArg)){//if int argument isn't a valid int - prints error
				printf("Error: column number must be in range 1-7\n");
				return -1;//stays in the same turn
			}
			currMessage = spFiarGameSetMove(currGame, currCommand->arg-1);//sets the move if it's valid
			//if int argument isn't an int between 1-7 - prints error
			if (currMessage == SP_FIAR_GAME_INVALID_ARGUMENT){
				if (checkMallocError(currGame)){ //memory allocation error - terminates the game
					return 0;
				}
				printf("Error: column number must be in range 1-7\n");
				return -1;//stays in the same turn
			}
			if (currMessage == SP_FIAR_GAME_INVALID_MOVE){//if the column selected is full - prints error
				printf( "Error: column %d is full\n", currCommand->arg);
				return -1;//stays in the same turn
			}
			if (spFiarCheckWinner(currGame) != SP_FIAR_GAME_PLAYER_1_SYMBOL){
				//computer's turn if user did not win in last move
				col = spMinimaxGetBestMove(currGame, *difficultyLevel);
				if (col == -2||col==-1){ //memory allocation error - terminates the game
					return 0;
				}
				currMessage = spFiarGameSetMove(currGame, col);
				if (checkMallocError(currGame)){ //memory allocation error - terminates the game
					return 0;
				}
				printf("Computer move: add disc to column %d\n", col+1);
			}
			return 1;//moves to next turn because a disc was added
		}
		//if the command is suggest_move
		if (currCommand->cmd == SP_SUGGEST_MOVE){
			col = spMinimaxGetBestMove(currGame, *difficultyLevel);//calls spMinimaxSuggestMove to get the suggested column
			if (col == -2||col==-1){ //memory allocation error - terminates the game
				return 0;
			}
			printf("Suggested move: drop a disc to column %d\n",col+1);
			return -1;//stays in the same turn
		}
	}
	return -1;//stays in the same turn
}

int gameOverCommands(SPCommand* currCommand, SPFiarGame* currGame, int* difficultyLevel){
	int init;//initialization status indicator - for restart command
	SP_FIAR_GAME_MESSAGE currMessage;

	//if the command is invalid - prints error
	if (currCommand->cmd == SP_INVALID_LINE){
		printf("Error: invalid command\n");
		return -1;//stays in the same turn
	}
	//if the command is undo_move
	if (currCommand->cmd == SP_UNDO_MOVE){
		int computerMove;
		int userMove;
		if (!spArrayListIsEmpty(currGame->history))//if history exist saves last move as computer's move
			computerMove = spArrayListGetLast(currGame->history);
		currMessage = spFiarGameUndoPrevMove(currGame);//undoes last(computer) move
		if (currMessage == SP_FIAR_GAME_NO_HISTORY){//if there's no history doesn't undo and prints error
			printf("Error: cannot undo previous move!\n");
			return -1;//stays in the same turn
		}
		//if history exist prints message
		printf("Remove disc: remove computer\'s disc at column %d\n",computerMove+1);
		userMove = spArrayListGetLast(currGame->history);//saves last move as user's move
		currMessage = spFiarGameUndoPrevMove(currGame);//undoes last(user) move
		printf("Remove disc: remove user\'s disc at column %d\n", userMove+1);
		return 1;//moves to next turn because discs were removed
	}
	//if the command is restart
	if (currCommand->cmd == SP_RESTART){
		spFiarGameDestroy(currGame); //destroys previous game
		currGame = spFiarGameCreate(20);//creates a new game
		if (checkMallocError(currGame)) //memory allocation error - terminates the game
			return 0;
		printf("Game restarted!\n");
		init = gameInitialization(difficultyLevel, currCommand);//initialize the new game
		if (init == 0){ //allocation error or "quit" command - terminates the game
			return 0;
		}
		return 1;//moves to next turn because game restarted and we need to print the gameBoard
	}
	//if the command is quit - prints message and terminates the game
	if (currCommand->cmd == SP_QUIT){
		printf("Exiting...\n");
		return 0;
	}

	return -1;//stays in the same turn - none of the above commands was acted
}

int playGame(char win, SPCommand* currCommand, SPFiarGame* currGame, int* difficultyLevel,
		int innerIterCnt){
	char userCommand[SP_MAX_LINE_LENGTH];//initialized to read the user's command
	bool gameOver = true;
	if (win== '\0'){//if there's no winner
		if (innerIterCnt == 1)//if it's the first iteration of a turn - prints message
			printf("Please make the next move:\n");
		gameOver = false;//game is not over
	}else if (win==SP_FIAR_GAME_TIE_SYMBOL){//if there's a tie
		if (innerIterCnt == 1)//if it's the first iteration of a turn - prints message
			printf("Game over: it\'s a tie\nPlease enter \'quit\' to exit or \'restart\' to start a new game!\n");
	}else if (win==SP_FIAR_GAME_PLAYER_1_SYMBOL){//if user wins
		if (innerIterCnt == 1)//if it's the first iteration of a turn - prints message
			printf("Game over: you win\nPlease enter \'quit\' to exit or \'restart\' to start a new game!\n");
	}else{//if computer wins
		if (innerIterCnt == 1)//if it's the first iteration of a turn - prints message
			printf("Game over: computer wins\nPlease enter \'quit\' to exit or \'restart\' to start a new game!\n");
	}
	fgets(userCommand,SP_MAX_LINE_LENGTH,stdin);//reads user's command
	*currCommand = spParserPraseLine(userCommand);//parses user's command

	return commandInterperter(gameOver, currCommand, currGame, difficultyLevel);//returns commandInterperter
}

