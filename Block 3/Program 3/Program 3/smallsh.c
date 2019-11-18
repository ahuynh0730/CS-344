// Author: Anthony Huynh
// Class: CS 344 - Operating Systems
// Project: Program 3 - smallsh
// Date Due: 11/20/2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROMPT_REQUEST ":"

int main(int argc, char* argv[]) {

	//2049 is to allow for 2048 characters plus terminator at end
	char userInput[2049];
	
	int i;
	char* parsedInput;
	char* arguments[512];
	int currentArgument = 0;



	while (1) {
		//prompt and flushes 
		printf("%s ", PROMPT_REQUEST);
		fflush(stdout);

		//sets userInput to all \0 before reading in
		memset(userInput, '\0', sizeof(userInput));
		fgets(userInput, sizeof(userInput), stdin);

		//parsing user input 
		parsedInput = strtok(userInput, " ");
		
		//if first argument was exit
		if (strcmp(parsedInput, "exit") == 0) {
			exit(0);
		}

		//will loop through each space separated text and place in arguments
		while (parsedInput != NULL) {
			arguments[currentArgument] = malloc(100 * sizeof(char));
			strcpy(arguments[currentArgument], parsedInput);
			currentArgument++;
			parsedInput = strtok(NULL, " ");
		}

		for (i = 0; i < currentArgument; i++) {
			printf("Argument %d: %s", i + 1, arguments[i]);
			if (i != currentArgument - 1) {
				printf("\n");
			}
		}
		

		for (i = 0; i < currentArgument; i++) {
			free(arguments[i]);
			currentArgument = 0;
		}
	
	}
	return 0;
}