// Author: Anthony Huynh
// Class: CS 344 - Operating Systems
// Project: Program 3 - smallsh
// Date Due: 11/20/2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROMPT_REQUEST ":"

int main(int argc, char* argv[]) {

	char userInput[100];


	while (1) {
		//prompt and flushes 
		printf("%s ", PROMPT_REQUEST);
		fflush(stdout);

		//sets userInput to all \0 before reading in
		memset(userInput, '\0', sizeof(userInput));
		fgets(userInput, sizeof(userInput), stdin);

		printf("You entered: %s", userInput);
	}
	return 0;
}