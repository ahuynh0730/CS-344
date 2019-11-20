// Author: Anthony Huynh
// Class: CS 344 - Operating Systems
// Project: Program 3 - smallsh
// Date Due: 11/20/2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PROMPT_REQUEST ":"
#define MAX_ARGUMENTS 512

int main(int argc, char* argv[]) {

	//2049 is to allow for 2048 characters plus terminator at end
	char userInput[2049];
	
	int i;
	char* parsedInput;
	char* arguments[MAX_ARGUMENTS];
	int currentArgument = 0;
	pid_t cpid;
	int childExitMethod = 0;
	char* inputFileName = NULL;
	char* outputFileName = NULL;
	int outputFile = 0;
	int inputFile = 0;
	int result = 0;

	for (i = 0; i < MAX_ARGUMENTS; i++) {
		arguments[i] = NULL;
	}

	while (1) {
		//prompt and flushes 
		printf("%s ", PROMPT_REQUEST);
		fflush(stdout);

		//sets userInput to all \0 before reading in
		memset(userInput, '\0', sizeof(userInput));
		fgets(userInput, sizeof(userInput), stdin);

		//will remove trailing \n at end of user input and replace with \0
		userInput[strlen(userInput) - 1] = '\0';

		//parsing user input, if no space, null pointer will be returned
		parsedInput = strtok(userInput, " ");
			
		//will loop through each space separated text and place in arguments
		while (parsedInput != NULL) {
			//if < was detected, indicating an input file will be next 
			if (strcmp(parsedInput, "<") == 0) {
				parsedInput = strtok(NULL, " ");
				inputFileName = malloc(100 * sizeof(char));
				strcpy(inputFileName, parsedInput);
				parsedInput = strtok(NULL, " ");
			}

			//if > was detected, indicated an output file will be next
			else if (strcmp(parsedInput, ">") == 0) {
				parsedInput = strtok(NULL, " ");
				outputFileName = malloc(100 * sizeof(char));
				strcpy(outputFileName, parsedInput);
				parsedInput = strtok(NULL, " ");
			}

			//all other arguments
			else {
				arguments[currentArgument] = malloc(100 * sizeof(char));
				strcpy(arguments[currentArgument], parsedInput);
				currentArgument++;
				parsedInput = strtok(NULL, " ");
			}
		}

		//if there were no arguments or the first argument started with #, will do nothing
		if (arguments[0] == NULL || arguments[0][0] == '#') {
			printf("This should display if no arguments or is a comment\n");
		}

		//will exit upon request, also freeing any memory that was malloc'd
		else if (strcmp(arguments[0], "exit") == 0) {
			for (i = 0; i < currentArgument; i++) {
				free(arguments[i]);
			}
			exit(0);
		}
			
		
		//to change directories, if no directory is stated, will go to home
		else if (strcmp(arguments[0], "cd") == 0) {
			if (arguments[1] == NULL) {                        
				chdir(getenv("HOME"));
			}
			else {
				chdir(arguments[1]);
			}
		}

		//prints either exit status or the terminating signal of the last foreground process
		else if (strcmp(arguments[0], "status") == 0) {

			//code from slides
			if (WIFEXITED(childExitMethod)){
				int exitStatus = WEXITSTATUS(childExitMethod);
				printf("exit value %d\n", exitStatus);
			}
			else {
				printf("Child terminated by a signal\n");
			}
		}

		//all other commands that are not built in
		else {
			cpid = fork();
			switch (cpid) {

			//if something went wrong during fork
			case -1:
				perror("Hull breach!");
				childExitMethod = 1;
				break;

			//if the process is the child process
			case 0:

				//if input file is specified, opens named file 
				if (inputFileName != NULL) {
					
					inputFile = open(inputFileName, O_RDONLY);
					if (inputFile == -1) {
						printf("cannot open %s for input\n", inputFileName);
						fflush(stdout);
						_Exit(1);
					}

					//will redirect stdout to outputFile
					result = dup2(inputFile, 0);
					if (result == -1) {
						perror("dup2 error");
						_Exit(1);
					}
					close(inputFile);
				}

				//if output file is specified, opens named file 
				if (outputFileName != NULL) {
					outputFile = open(outputFileName, O_WRONLY | O_TRUNC | O_CREAT , 0700);
					if (outputFile == -1) {
						perror("open output file error");
						_Exit(1);
					}

					//will redirect stdout to outputFile
					result = dup2(outputFile, 1);
					if (result == -1) {
						perror("dup2 error");
						_Exit(1);
					}
					close(outputFile);
				}

				//if execvp fails, will return a negative one and alert the user
				if (execvp(arguments[0], arguments) < 0) {
					printf("%s: no such file or directory\n", arguments[0]);
					fflush(stdout);
					_Exit(1);
				}
				break;

			//if the process is the parent
			default:
				waitpid(cpid, &childExitMethod, 0);
				break;
			}
		}
			
			

		//frees all memory malloc'd and sets currentArguments to 0 before prompting for more input
		for (i = 0; i < currentArgument; i++) {
			free(arguments[i]);
			arguments[i] = NULL;
		}
		currentArgument = 0;
		free(outputFileName);
		free(inputFileName);
		inputFileName = NULL;
		outputFileName = NULL;
		
		
	
	}
	return 0;
}
