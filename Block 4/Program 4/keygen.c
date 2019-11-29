// Author: Anthony Huynh
// Class: CS 344 - Operating Systems
// Project: Program 44 - OTP
// Date Due: 12/6/2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	
	srand((unsigned) time(0));
	int i = 0;
	char* endPtr = NULL;
	char* key = NULL;
	int randomNumber = 0;
	
	//will inform user if there are not 2 arguments
	if (argc != 2){
		fprintf(stderr, "Wrong number of arguments. Please try again!\n");
		exit(0);
	}
	
	//converts first argument into a long int
	long lengthOfKey = strtol(argv[1], &endPtr, 0);
	
	//increments by 1 to account for newline char
	lengthOfKey++;
	key = malloc(lengthOfKey * sizeof(char));
	memset(key, '\0', lengthOfKey);
	
	//loop that will create a new random number and convert to ASCII char each iteration
	for (i = 0; i < lengthOfKey - 1 ; i++){
		randomNumber = rand() % 27;
		switch (randomNumber){
			//if random number was 0, will convert to space
			case 0:
				key[i] = (char)randomNumber +  32;
				break;
				
			//all other numbers will convert to matching letter
			default:
				key[i] = (char)randomNumber + 64;
				break;
		}
	}
	key[lengthOfKey - 1] = '\n';
	
	fprintf(stdout, "%s", key);
	free(key);
	
	return 0;	
}
