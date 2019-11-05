// Author: Anthony Huynh
// Class: CS 344
// Project: Program 2 - adventure
// Date Due: 11/1/2019


//outline from https://oregonstate.instructure.com/courses/1738958/pages/2-dot-2-program-outlining-in-program-2 was used

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

//constant definitions
#define LONGEST_ROOM_NAME_LENGTH 9 //will allow 8 characters plus null
#define MAX_CONNECTIONS 6
#define MIN_CONNECTIONS 3
#define TOTAL_ROOMS 10
#define ROOMS_TO_CREATE 7
#define FILE_SUFFIX "_room"
#define LONGEST_DIRECTORY_NAME_LENGTH 100


enum bool {false = 0, true = 1};

struct Room {
	char name[LONGEST_ROOM_NAME_LENGTH];
	char type[11];
	struct Room *connections[MAX_CONNECTIONS];
	int numConnections;
};

void createRoom(struct Room*, char[TOTAL_ROOMS][LONGEST_ROOM_NAME_LENGTH], char*, int*);
void createFile(struct Room*);
enum bool isGraphFull(struct Room**);
void addRandomConnection(struct Room**);
struct Room* getRandomRoom(struct Room**);
enum bool canAddConnectionFrom(struct Room*);
enum bool connectionAlreadyExists(struct Room*, struct Room*);
void connectRoom(struct Room*, struct Room*);
enum bool isSameRoom(struct Room*, struct Room*);

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

	// Create all connections in graph
	while (isGraphFull(roomsToTraverse) == false){
		addRandomConnection(roomsToTraverse);
	}
	
	//gets current process id
	pid_t processId;
	processId = getpid();

	//creates array of characters and initializes to null
	char directoryName[LONGEST_DIRECTORY_NAME_LENGTH];
	memset(directoryName, '\0', sizeof(directoryName));

	//fills array with name of directory and process id
	snprintf(directoryName, sizeof(directoryName), "huynhant.rooms.%d", processId);

	//makes directory
	mkdir(directoryName, 0700);
	
	//moves to new directory
	chdir(directoryName);
	
	
	//creates new file for each room
	for (i = 0; i < ROOMS_TO_CREATE; i++) {
		createFile(roomsToTraverse[i]);
	}
	

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
	strcpy(roomPtr->name, roomNames[roomNumber]);
	strcpy(roomPtr->type, roomType);
	
}

//creates each file, requires a pointer to the room to be passed in
void createFile(struct Room* roomPtr) {

	//will initialize fileName to all \0
	char fileName[14];
	memset(fileName, '\0', sizeof(fileName));

	//copies room's name into fileName
	strcpy(fileName, roomPtr->name);

	//concatenates FILE_SUFFIX(_room) to end of fileName
	strcat(fileName, FILE_SUFFIX);

	//opens the file with the appropriate name
	FILE * filePointer;
	filePointer = fopen(fileName, "w+");

	//writes room description to file
	fprintf(filePointer, "ROOM NAME: %s\n", roomPtr->name);
	int i;
	for (i = 0; i < roomPtr->numConnections; i++) {
		fprintf(filePointer, "CONNECTION %d: %s\n", i + 1, roomPtr->connections[i]->name);
	}
	fprintf(filePointer, "ROOM TYPE: %s\n", roomPtr->type);

	//closes the file
	fclose(filePointer);
}

//will return false if all rooms in graph have greater than 2 and less than 7 rooms(3-6)
enum bool isGraphFull(struct Room** roomsToTraverse)
{
	int i;
	for (i = 0; i < ROOMS_TO_CREATE; i++) {
		if ((roomsToTraverse[i]->numConnections < MIN_CONNECTIONS) || (roomsToTraverse[i]->numConnections > MAX_CONNECTIONS))
			return false;
	}
	return true;
}

// Adds a random, valid outbound connection from a Room to another Room
void addRandomConnection(struct Room** roomsToTraverse){
	struct Room* A;  
	struct Room* B;

	while (true)
	{
		A = getRandomRoom(roomsToTraverse);

		if (canAddConnectionFrom(A) == true)
			break;
	}

	do
	{
		B = getRandomRoom(roomsToTraverse);
	} while (canAddConnectionFrom(B) == false || isSameRoom(A, B) == true || connectionAlreadyExists(A, B) == true);

	connectRoom(A, B);   
}


// Returns a random Room, does NOT validate if connection can be added
struct Room* getRandomRoom(struct Room** roomsToTraverse){
	int randomNumber;
	randomNumber = rand() % ROOMS_TO_CREATE;
	return roomsToTraverse[randomNumber];
}

// Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise
enum bool canAddConnectionFrom(struct Room* roomPtr) {
	if (roomPtr->numConnections < MAX_CONNECTIONS){
		return true;
	}
	return false;
}

// Returns true if a connection from Room x to Room y already exists, false otherwise
enum bool connectionAlreadyExists(struct Room* roomPtr1, struct Room* roomPtr2){
	int i;
	int strCompareResults;
	for (i = 0; i < roomPtr1->numConnections; i++) {
		strCompareResults = strcmp(roomPtr1->connections[i]->name, roomPtr2->name);
		if (strCompareResults == 0) {
			return true;
		}
	}
	return false;
}

// Connects Rooms x and y together, does not check if this connection is valid, connects both ways
void connectRoom(struct Room* roomPtr1, struct Room* roomPtr2){
	roomPtr1->connections[roomPtr1->numConnections] = roomPtr2;
	roomPtr1->numConnections++;
	roomPtr2->connections[roomPtr2->numConnections] = roomPtr1;
	roomPtr2->numConnections++;
}

// Returns true if Rooms x and y are the same Room, false otherwise
enum bool isSameRoom(struct Room* roomPtr1, struct Room* roomPtr2){
	int strCompareResults;
	strCompareResults = strcmp(roomPtr1->name, roomPtr2->name);
	if (strCompareResults == 0) {
		return true;
	}
	return false;
}

