/*
 * SPFIARParser.c
 *
 *  Created on: 13 Jun 2017
 *      Author: Sivan
 */

#include "SPFIARParser.h"


bool spParserIsInt(const char* str){
	int i=0;
	while (str[i]!='\0'){
		if ((int)str[i] < 48 || (int)str[i] > 57)
			if (i!=0 || str[0] != '-')
				return false;
		i++;
	}
	return true;
}


SPCommand spParserPraseLine(const char* str){
	char* parsedStrCmd; //pointer for the first string in the parsed string (for use of strtok command)
	char* parsedStrArg; //pointer for the argument
	char* strCopy;
	int cnt=0; //counter for number of separated strings entered by the user
	SPCommand command;

	strCopy = (char*)malloc(SP_MAX_LINE_LENGTH*sizeof(char)); //copy of str (parameter)
	if (parserCheckMallocError(strCopy)){ //checks if there was an allocation error
		command.cmd = SP_ALLOCATION_ERROR;
		return command;
	}

	command.validArg = false; //initializing validArg to be false (i.e. invalid)

	strcpy(strCopy,str); //Copies str to strCopy
	parsedStrCmd = strtok(strCopy, " \t\r\n"); //parses strCpy and stores the result pointer in parsedStr

	while (parsedStrCmd!=NULL){ //stores the parsed string in parsedStrCmd
		cnt++;
		if (cnt == 2)
			parsedStrArg = parsedStrCmd;
		parsedStrCmd = strtok(NULL, " \t\r\n"); //advances parsedStr to point to the next string
	}

	parsedStrCmd = strtok(strCopy, " \t\r\n");

	if (cnt==0||cnt>2){ //checks if there are more than 1 command and 1 argument in the input - invalid
		command.cmd = SP_INVALID_LINE;
	}

	//the command is "add_disc"
	else if(strcmp(parsedStrCmd,"add_disc")==0){
		command.cmd = SP_ADD_DISC;
		if (cnt==2 && spParserIsInt(parsedStrArg)){ //checks if there is 1 argument and if it is integer
			command.validArg = true;
			command.arg = atoi(parsedStrArg);
		}
	}
	//the command is "undo_move"
	else if(strcmp(parsedStrCmd,"undo_move")==0&&cnt==1){
		command.cmd = SP_UNDO_MOVE;
	}
	//the command is "suggest_move"
	else if(strcmp(parsedStrCmd,"suggest_move")==0&&cnt==1){
		command.cmd = SP_SUGGEST_MOVE;
	}
	//the command is "quit"
	else if(strcmp(parsedStrCmd,"quit")==0&&cnt==1){
		command.cmd = SP_QUIT;
	}
	//the command is "restart"
	else if(strcmp(parsedStrCmd,"restart_game")==0&&cnt==1){
		command.cmd = SP_RESTART;
	}
	//the string was one or more invalid word - isn't one of the operations above
	else{
		command.cmd = SP_INVALID_LINE;
	}
	parserDestroy(strCopy);
	return command; //return
}

void spParserDifficultyLevel(char* difficultyLevel, int* arg){
	int cnt=0;
	char* parsedStrDifficultyLevel;
	char* strCopy;

	strCopy = (char*)malloc(SP_MAX_LINE_LENGTH*sizeof(char)); //copy of difficultyLevel(parameter)
	if (parserCheckMallocError(strCopy)){
		arg = NULL;
		return;
	}
	strcpy(strCopy,difficultyLevel); //Copies difficultyLevel to strCopy
	parsedStrDifficultyLevel = strtok(strCopy, " \t\r\n");

	//check if there is more then one parsed string
	while (parsedStrDifficultyLevel!=NULL){
		cnt++;
		parsedStrDifficultyLevel = strtok(NULL, " \t\r\n");
	}
	if (cnt!=1){
		parserDestroy(strCopy);
		*arg = -1;
		return;
	}
	parsedStrDifficultyLevel = strtok(strCopy, " \t\r\n");

	if(spParserIsInt(parsedStrDifficultyLevel))
		*arg = atoi(parsedStrDifficultyLevel);
	else
		*arg = -1;

	parserDestroy(strCopy);
}

bool parserCheckMallocError(void* p){
	if (p == NULL){
		printf("Error: malloc has failed");
		return true;
	}

	return false;
}

void parserDestroy(void* src){
	if (src!=NULL)
		free(src);
}

