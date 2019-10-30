// Author: Anthony Huynh
// Class: CS 344
// Project: Program 2 - adventure
// Date Due: 11/1/2019

//this program does not run on windows, must be run in a linux setting

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>



int main(int argc, char* argv[]) {

	//gets current process id
	pid_t processId;
	processId = getpid();
	printf("The process id is: %d\n", processId);

	//creates array of characters and initializes to null
	char directoryName[25];
	memset(directoryName, '\0', 25);

	//fills array with name of directory and process id
	snprintf(directoryName, 25, "huynhant.rooms.%d", processId);

	//makes directory
	mkdir(directoryName);


	return 0;
}