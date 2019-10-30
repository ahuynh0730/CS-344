// Author: Anthony Huynh
// Class: CS 344
// Project: Program 2 - adventure
// Date Due: 11/1/2019

//this program does not run on windows, must be run in a linux setting

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>



int main(int argc, char* argv[]) {

	//gets current process id
	pid_t processId;
	processId = getpid();

	printf("The process id is %d\n", processId);


	return 0;
}