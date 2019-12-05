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
	return fileLength - 1;
}

//sends file to server
void sendFile (char* fileName, int socketFD, int length){
	FILE* file = fopen(fileName, "r");
	char buffer[BUFFER_SIZE];
	memset(buffer, '\0', sizeof(buffer));
	int numberBytes;
	
	while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0){
		if ((numberBytes = send(socketFD, buffer, length, 0)) < 0){
			
		}
		memset(buffer, '\0', sizeof(buffer));
	}
	fclose(file);
	return;
}

int main(int argc, char *argv[])
{
	int socketFD, portNumber;
	int encodedText = 0;
	int encodedTextLength = 0;
	int key = 0;
	int keyLength = 0;
	int validFile = 0;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[BUFFER_SIZE];
	char decodeAuthentication[6] = "decode";
    
	// Check usage & args
	if (argc != 4) { 
		error("Wrong number of arguments\n");
	}
	
	// Opening uncoded text and key and getting their length
	encodedText = open(argv[1], O_RDONLY);
	encodedTextLength = fileLength(argv[1]);
	key = open(argv[2], O_RDONLY);
	keyLength = fileLength(argv[2]);
	
	
	//will exit if key and text are not same length
	if (keyLength < encodedTextLength){
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
	write(socketFD, decodeAuthentication, sizeof(decodeAuthentication));
	read(socketFD, buffer, sizeof(buffer));
	
	//authenticate that the right connection is formed
	if(strcmp(buffer, "decode") != 0){
		error("Wrong daemon usage\n");
	}

	
	
	
	
	sendFile(argv[1], socketFD, encodedTextLength);
	sendFile(argv[2], socketFD, keyLength);
	
	memset(buffer, '\0', sizeof(buffer));
	if (read(socketFD, buffer, sizeof(buffer) - 1) < 0){
		error("Error from reading socket\n");
	}
	printf("%s", buffer);

	close(socketFD); // Close the socket
	return 0;
}