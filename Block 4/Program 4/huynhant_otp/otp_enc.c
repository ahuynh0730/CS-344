// Author: Anthony Huynh
// Class: CS 344 - Operating Systems
// Project: Program 44 - OTP
// Date Due: 12/6/2019

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>

#define BUFFER_SIZE 71000
#define SEND_AT_ONE_TIME 1000
// Error function used for reporting issues
void error(const char *msg) {
	fprintf(stderr, msg);
	exit(1); 
} 

//function to determine size of file
int fileLength(char* file){
	FILE* filePointer = fopen(file, "r");
	fseek(filePointer, 0, SEEK_END);
	int fileLength = ftell(filePointer);
	fclose(filePointer);
	return fileLength;
}

//sends file to server
void sendFile (char* fileName, int socketFD, int length){
	FILE* file = fopen(fileName, "r");
	char buffer[BUFFER_SIZE];
	memset(buffer, '\0', sizeof(buffer));
	int charsSent;
	char* bufferPointer = buffer;
	
	//places file content in buffer
	fgets(buffer, BUFFER_SIZE, file);
	
	
	//printf("chars to be sent: %d\n", length);
	
	//sends length of file
	send(socketFD, &length, sizeof(int), 0);
	
	//will keep looping while length is greater than what is set to being sent at once
	while (length > SEND_AT_ONE_TIME){
		send(socketFD, bufferPointer, SEND_AT_ONE_TIME, 0);
		length -= SEND_AT_ONE_TIME;
		//printf("chars remaining to send: %d\n", length);
		bufferPointer += SEND_AT_ONE_TIME;
	}
	
	//sends remaining chars
	//printf("about to send last %d chars\n", length);
	send(socketFD, bufferPointer, length, 0);
	
	fclose(file);
	return;
}

int main(int argc, char *argv[])
{
	int socketFD, portNumber;
	int uncodedText = 0;
	int uncodedTextLength = 0;
	int key = 0;
	int keyLength = 0;
	int validFile = 0;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[BUFFER_SIZE];
	char encodeAuthentication[6] = "encode";
	int value = 0;
    
	// Check usage & args
	if (argc != 4) { 
		error("Wrong number of arguments\n");
	}
	
	// Opening uncoded text and key and getting their length
	uncodedText = open(argv[1], O_RDONLY);
	uncodedTextLength = fileLength(argv[1]);
	key = open(argv[2], O_RDONLY);
	keyLength = fileLength(argv[2]);
	
	
	//will exit if key and text are not same length
	if (keyLength < uncodedTextLength){
		error("key is too short\n");
	}
	
	//will check files for bad input
	memset(buffer, '\0', sizeof(buffer));
	validFile = open(argv[1], 'r');
	while (read(validFile, buffer, 1) != 0){
		if (!isupper(buffer[0]) && !isspace(buffer[0])){
			error("File has bad input\n");
		}
	}
	
	memset(buffer, '\0', sizeof(buffer));
	validFile = open(argv[2], 'r');
	while (read(validFile, buffer, 1) != 0){
		if (!isupper(buffer[0]) && !isspace(buffer[0])){
			error("File has bad input\n");
		}
	}
	
	

	
	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { 
		error("CLIENT: ERROR, no such host\n");
	}
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) 
		error("CLIENT: ERROR opening socket");
	
	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("CLIENT: ERROR connecting");

	//makes sure is connecting with enc_d
	write(socketFD, encodeAuthentication, sizeof(encodeAuthentication));
	read(socketFD, buffer, sizeof(buffer));
	
	//authenticate that the right connection is formed
	if(strcmp(buffer, "encode") != 0){
		error("Wrong daemon usage\n");
	}
	
	setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int));
	
	
	//sends the two files
	sendFile(argv[1], socketFD, uncodedTextLength);
	sendFile(argv[2], socketFD, keyLength);
	
	//clears buffer and reads over encrypted data, which it then writes to the screen
	memset(buffer, '\0', sizeof(buffer));
	if (read(socketFD, buffer, sizeof(buffer) - 1) < 0){
		error("Error from reading socket\n");
	}
	printf("%s", buffer);
	close(socketFD); // Close the socket
	return 0;
}
