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
#include <time.h>

enum roomType { START_ROOM = 0, MID_ROOM = 1, END_ROOM = 2};

struct Room {
	char Name[8];
	enum roomType type;
	struct Room *connections[6];
	int numConnections;
};

struct Room GetRandomRoom(int);

int main(int argc, char* argv[]) {

	srand(time(0));

	int totalRooms = 10;
	//list of possible room names
	char roomNames[10][9] = {
		"UnderSea",
		"Lava",
		"Darkness",
		"WanoKuni",
		"Galar",
		"Hyrule",
		"Magnolia",
		"Wakanda",
		"Tropical",
		"Yorknew"
	};

	//array to hold pointers to each room
	struct Room *roomsToTraverse[7];

	//creates first room
	int roomNumber;
	roomNumber = rand() % 10;
	printf("random number: %d\n", roomNumber);

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

struct Room GetRandomRoom(int randomRoomNumber) {

}