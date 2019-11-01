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

//constant definitions
#define LONGEST_ROOM_NAME_LENGTH 9 //will allow 8 characters plus null
#define MAX_CONNECTIONS 6
#define TOTAL_ROOMS 10
#define ROOMS_TO_CREATE 7


struct Room {
	char* name;
	char* type;
	struct Room *connections[MAX_CONNECTIONS];
	int numConnections;
};

void createRoom(struct Room*, char[TOTAL_ROOMS][LONGEST_ROOM_NAME_LENGTH], char*, int*);
struct Room GetRandomRoom(int);

int main(int argc, char* argv[]) {

	srand(time(0));

	//list of possible room names
	char roomNames[TOTAL_ROOMS][LONGEST_ROOM_NAME_LENGTH] = {
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

	//array to hold pointers to each room and allocates memory
	struct Room *roomsToTraverse[ROOMS_TO_CREATE];
	int i;
	for (i = 0; i < ROOMS_TO_CREATE; i++) {
		roomsToTraverse[i] = malloc(sizeof(struct Room));
	}

	//array used to keep track of which room name was already used, initializes to 0
	int roomsCreated[TOTAL_ROOMS];
	for (i = 0; i < TOTAL_ROOMS; i++) {
		roomsCreated[i] = 0;
	}


	//creates all the rooms
	createRoom(roomsToTraverse[0], roomNames, "START_ROOM", roomsCreated);
	for (i = 1; i < ROOMS_TO_CREATE - 1; i++) {
		createRoom(roomsToTraverse[i], roomNames, "MID_ROOM", roomsCreated);
	}
	createRoom(roomsToTraverse[ROOMS_TO_CREATE - 1], roomNames, "END_ROOM", roomsCreated);


	//prints all rooms
	for (i = 0; i < ROOMS_TO_CREATE; i++) {
		printf("Room %d: %s %s\n", i, roomsToTraverse[i]->name, roomsToTraverse[i]->type);
	}

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
	//mkdir(directoryName);

	//frees memory used by rooms array
	i = 0;
	for (i; i < ROOMS_TO_CREATE; i++) {
		free(roomsToTraverse[i]);
	}


	return 0;
}

//creates a room
void createRoom(struct Room* roomPtr, char roomNames[TOTAL_ROOMS][LONGEST_ROOM_NAME_LENGTH], char* roomType, int* roomsCreated) {
	int roomNumber;
	roomNumber = rand() % TOTAL_ROOMS;
	//will keep randomizing until a new number appears
	while (roomsCreated[roomNumber] != 0) {
		roomNumber = rand() % TOTAL_ROOMS;
	}

	roomsCreated[roomNumber] = 1;
	
	//will set the room name and type
	roomPtr->name = roomNames[roomNumber];
	roomPtr->type = roomType;
	
}




struct Room GetRandomRoom(int randomRoomNumber) {

}