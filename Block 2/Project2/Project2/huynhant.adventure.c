// Author: Anthony Huynh
// Class: CS 344 - Operating Systems
// Project: Program 2 - adventure
// Date Due: 11/1/2019

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define LONGEST_DIR_NAME_LENGTH 100
#define TARGET_DIR_PREFIX "huynhant.rooms."
#define LONGEST_ROOM_NAME_LENGTH 9
#define MAX_CONNECTIONS 6
#define ROOM_SUFFIX "_room"
#define ROOMS_TO_CREATE 7

pthread_mutex_t mutex;

struct Room {
	char name[LONGEST_ROOM_NAME_LENGTH];
	char type[11];
	struct Room *connections[MAX_CONNECTIONS];
	int numConnections;
};


void findNewestDirectory(char*);
void turnFilesIntoRooms(struct Room**);
struct Room* getRoomWithMatchingName(struct Room**, char*);
void createThread();
void displayTime();
void* getTime();

int main(int argc, char* argv[]) {
	int i;
	struct Room* endRoom;
	struct Room* currentRoom;
	struct Room* nextRoom;
	char userInput[100];
	struct Room* visitedRooms[100];
	int numberSteps = 0;
	//initializes and sets dirName to \0
	char dirName[LONGEST_DIR_NAME_LENGTH];
	memset(dirName, '\0', sizeof(dirName));

	//initializes array of room pointers and allocates space for them
	struct Room *roomsToTraverse[ROOMS_TO_CREATE];
	for (i = 0; i < ROOMS_TO_CREATE; i++) {
		roomsToTraverse[i] = malloc(sizeof(struct Room));
		roomsToTraverse[i]->numConnections = 0;
	}


	//finds newest directory and changes to that directory
	findNewestDirectory(dirName);
	chdir(dirName);
	printf("In directory: %s\n", dirName);

	turnFilesIntoRooms(roomsToTraverse);
	chdir("..");

	//will set current room and end room pointers
	for (i = 0; i < ROOMS_TO_CREATE; i++) {
		if (strstr(roomsToTraverse[i]->type, "START_ROOM")) {
			currentRoom = roomsToTraverse[i];
		}
		else if (strstr(roomsToTraverse[i]->type, "END_ROOM")) {
			endRoom = roomsToTraverse[i];
		}
	}

	//will keep looping until the current room is the end room
	while (currentRoom != endRoom) {
		if (strcmp(userInput, "time") != 0) {
			//prints current location and possible connections
			printf("CURRENT LOCATION: %s\n", currentRoom->name);
			printf("POSSIBLE CONNECTIONS: ");

		
			//displays each connection separated by a comma, period will come after last connection
			for (i = 0; i < currentRoom->numConnections; i++) {
				printf(currentRoom->connections[i]->name);
				if (i != currentRoom->numConnections - 1) {
					printf(", ");
				}
				else {
					printf(".\n");
				}
			}
		}

		//prints where to and gets user input
		memset(userInput, '\0', sizeof(userInput));
		printf("WHERE TO? >");
		scanf("%s", &userInput);

		//if user inputs time
		if (strcmp(userInput, "time") == 0) {
			createThread();
			displayTime();
		}
		nextRoom = NULL;
		printf("\n");

		//will search through connections for room with that name, then assign it to be next room 
		for (i = 0; i < currentRoom->numConnections; i++) {
			if (strstr(userInput, currentRoom->connections[i]->name)) {
				nextRoom = currentRoom->connections[i];
				
				//assigns next pointer to point to that room and incremements the number of steps taken
				visitedRooms[numberSteps] = nextRoom;
				numberSteps++;
				currentRoom = nextRoom;
				break;
			}
		}

		//if nextRoom is still null, then no room with that name was found
		if (nextRoom == NULL && strcmp(userInput, "time") != 0) {
			printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
		}
	}

	//if currentroom is end room, then they have won and will display number of steps and the rooms visited
	printf("YOU HAVE FOUND THE END ROOM! CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", numberSteps);
	for (i = 0; i < numberSteps; i++) {
		printf("%s\n", visitedRooms[i]->name);
	}

	//frees memory used by rooms array
	for (i = 0; i < ROOMS_TO_CREATE; i++) {
		free(roomsToTraverse[i]);
	}

	return 0;
}

//finds newest directory and fills a char pointer
void findNewestDirectory(char* newestDirName) {
	int newestDirTime = -1;

	DIR *dirToCheck;
	struct dirent *fileInDir;
	struct stat dirAttributes;

	//opens current directory
	dirToCheck = opendir(".");

	//makes sure directory can be opened
	if (dirToCheck)
	{
		while ((fileInDir = readdir(dirToCheck)) != NULL)
		{
			if (strstr(fileInDir->d_name, TARGET_DIR_PREFIX) != NULL) {
				stat(fileInDir->d_name, &dirAttributes);	//gets attributes of the directory
			}
			
			//if time is bigger
			if ((int)dirAttributes.st_mtime > newestDirTime) {
				newestDirTime = (int)dirAttributes.st_mtime;
				memset(newestDirName, '\0', sizeof(newestDirName));
				strcpy(newestDirName, fileInDir->d_name);
			}
		}
		closedir(dirToCheck);
	}
}

//turns all files into rooms and adds connections
void turnFilesIntoRooms(struct Room** roomsToTraverse) {
	DIR *dirToCheck;
	struct dirent *fileInDir;

	//opens current directory
	dirToCheck = opendir(".");
	FILE* filePointer;
	char buffer[255];

	char separator = ':';
	int separatorPosition;
	int i;
	char textBeforeSeparator[30];
	char textAfterSeparator[30];
	int currentFileIndex = 0;

	//loops through each file to assign names to rooms to allow for creating connections
	//makes sure directory can be opened
	if (dirToCheck)
	{
		while ((fileInDir = readdir(dirToCheck)) != NULL)
		{
			//will make sure room suffix is in file name
			if (strstr(fileInDir->d_name, ROOM_SUFFIX) != NULL) {

				//opens each file
				filePointer = fopen(fileInDir->d_name, "r");

				memset(buffer, '\0', sizeof(buffer));

				//reads first line of file
				fgets(buffer, sizeof(buffer), filePointer);
					
				//changes last char from \n to \0
				buffer[strlen(buffer) - 1] = '\0';

				//finds the position of the separator
				for (i = 0; i < strlen(buffer); i++) {
					if (buffer[i] == separator) {
						separatorPosition = i;
					}
				}

				memset(textBeforeSeparator, '\0', sizeof(textBeforeSeparator));
				memset(textAfterSeparator, '\0', sizeof(textAfterSeparator));

				//copies text before and after the separator
				strncpy(textBeforeSeparator, buffer, separatorPosition);
				strcpy(textAfterSeparator, buffer + separatorPosition + 2);

				//copies text into room's name
				strcpy(roomsToTraverse[currentFileIndex]->name, textAfterSeparator);
				

				currentFileIndex++;
				fclose(filePointer);
			}
		}
		closedir(dirToCheck);
	}

	currentFileIndex = 0;
	dirToCheck = opendir(".");
	//goes through directory again but is able to form connections since rooms have been made
	//makes sure directory can be opened
	if (dirToCheck)
	{
		while ((fileInDir = readdir(dirToCheck)) != NULL)
		{
			//will make sure room suffix is in file name
			if (strstr(fileInDir->d_name, ROOM_SUFFIX) != NULL) {

				//opens each file
				filePointer = fopen(fileInDir->d_name, "r");

				memset(buffer, '\0', sizeof(buffer));

				//goes through each line of the file
				while (fgets(buffer, sizeof(buffer), filePointer)) {

					//changes last char from \n to \0
					buffer[strlen(buffer) - 1] = '\0';

					//finds the position of the separator
					for (i = 0; i < strlen(buffer); i++) {
						if (buffer[i] == separator) {
							separatorPosition = i;
						}
					}

					memset(textBeforeSeparator, '\0', sizeof(textBeforeSeparator));
					memset(textAfterSeparator, '\0', sizeof(textAfterSeparator));

					//copies text before and after the separator
					strncpy(textBeforeSeparator, buffer, separatorPosition);
					strcpy(textAfterSeparator, buffer + separatorPosition + 2);

					//creates connection from room name to outbound room name
					if (strstr(textBeforeSeparator, "CONNECTION")) {
						struct Room* room1 = roomsToTraverse[currentFileIndex];
						struct Room* room2 = getRoomWithMatchingName(roomsToTraverse, textAfterSeparator);
						room1->connections[room1->numConnections] = room2;
						room1->numConnections++;
					}

					//will set room type
					else if (strstr(textBeforeSeparator, "ROOM TYPE")) {
						strcpy(roomsToTraverse[currentFileIndex]->type, textAfterSeparator);
					}
				}
				currentFileIndex++;
				fclose(filePointer);
				
			}
		}
		closedir(dirToCheck);
	}
}

//returns a pointer to a room with a name that has matching room name
struct Room* getRoomWithMatchingName(struct Room** roomsToTraverse, char* roomName)
{
	int i;
	for (i = 0; i < ROOMS_TO_CREATE; i++) {
		if (strstr(roomName, roomsToTraverse[i]->name)) {
			return roomsToTraverse[i];
		}
	}
	return NULL;
}

//gets time and writes to currentTime.txt overriding it if it exists
void* getTime() {
	FILE* outputFile = fopen("currentTime.txt", "w+");
	char buffer[100];
	memset(buffer, '\0', sizeof(buffer));
	struct tm *sTm;

	time_t currentTime = time(0);
	sTm = gmtime(&currentTime);

	strftime(buffer, sizeof(buffer), "%I:%M%p, %A, %B %d, %Y", sTm);
	fputs(buffer, outputFile);
	fclose(outputFile);

}

//creates the thread
void createThread() {
	pthread_t thread;
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_lock(&mutex);

	int tid = pthread_create(&thread, NULL, getTime, NULL);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);

}

//will read time from currentTime.txt and display it
void displayTime() {
	FILE* inputFile = fopen("currentTime.txt", "r");
	char buffer[100];
	memset(buffer, '\0', sizeof(buffer));

	//currentTime.txt must exist
	if (inputFile == NULL) {               
		perror("Not found\n");
	}

	else {
		fgets(buffer, sizeof(buffer), inputFile);
		printf("\n %s\n", buffer);
	}
	fclose(inputFile);
}

