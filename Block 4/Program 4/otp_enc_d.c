// Author: Anthony Huynh
// Class: CS 344 - Operating Systems
// Project: Program 44 - OTP
// Date Due: 12/6/2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 71000
#define SENT_AT_ONE_TIME 1000

// Error function used for reporting issues
void error(const char *msg) { 
	fprintf(stderr, msg); 
	exit(1); 
} 

//to encrypt a message
void encryptMessage(char message[], char key[], int length){
	int i;
	int textNumber;
	int keyNumber;
	int encryptedNumber;
	length = strlen(message) - 1;
	
	for (i = 0; i < length; i++){
		//will convert spaces to 0
		if (message[i] == ' '){
			textNumber = 0;
		}
		//will subtract 64 from the ascii value to get a number between 1 and 27
		else{
			textNumber = (int)(message[i]) - 64;
		}
		
		//will convert key to number
		if (key[i] == ' '){
			keyNumber = 0;
		}
		else{
			keyNumber = (int)(key[i]) - 64;
		}
		
		//adds the two numbers together and mods 27
		encryptedNumber = (textNumber + keyNumber) % 27;
		
		//converts encrypted number to char
		if (encryptedNumber == 0){
			encryptedNumber += 32;
		}
		else{
			encryptedNumber += 64;
		}
		message[i] = (char)encryptedNumber;
	}
}

//reads specified number of chars into buffer
void readIntoBuffer(char buffer[], int establishedConnectionFD, int charsToRead){
	int keepReading = 1;
	int charsRead = 0;
	int totalChars = 0;
	int i = 0;
	char* bufferPointer = buffer;
	
	//printf("Needs to read %d chars\n", charsToRead);
	while (charsToRead > SENT_AT_ONE_TIME){
		read(establishedConnectionFD, bufferPointer, SENT_AT_ONE_TIME);
		bufferPointer += SENT_AT_ONE_TIME;
		charsToRead -= SENT_AT_ONE_TIME;
		//printf("%d chars remaining to read\n", charsToRead);
	}
	//printf("about to read final %d chars\n", charsToRead);
	read(establishedConnectionFD, bufferPointer, charsToRead);
}

int main(int argc, char *argv[])
{

	int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
	char buffer[BUFFER_SIZE];
	int numberChars = BUFFER_SIZE;
	int totalChars;
	socklen_t sizeOfClientInfo;
	struct sockaddr_in serverAddress, clientAddress;
	pid_t pid;
	char encodeAuthentication[6] = "encode";
	int newLine = 0;

	// Check usage & args
	if (argc != 2){ 
		fprintf(stderr,"USAGE: %s port\n", argv[0]); 
		exit(1); 
	} 

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) 
		error("ERROR opening socket");

	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

	while (1){
		// Accept a connection, blocking if one is not available until one connects
		sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
		if (establishedConnectionFD < 0) 
			error("ERROR on accept");
		
		
		pid = fork();
		//if error forking
		if (pid < 0){
			error("Error on fork");
		}
		
		//if the process is the cild
		else if (pid == 0){
			memset(buffer, '\0', sizeof(buffer));
			int i=0;
			char message[BUFFER_SIZE];
			char key[BUFFER_SIZE];
			totalChars = 0;
			charsRead = 0;
			int keepReading = 1;
			int charsToRead = 0;
			char* bufferPointer = buffer;
			
			//will authenticate that otp_enc called this daemon
			read(establishedConnectionFD, buffer, sizeof(buffer) - 1);
			if (strcmp(buffer, "encode") != 0){
				error("Wrong usage of daemon\n");
			}
			else{
				write(establishedConnectionFD, encodeAuthentication, sizeof(encodeAuthentication));
			}
			
			//clear buffer and read how many chars will be sent, then read into buffer and copy to message
			memset(buffer, '\0', sizeof(buffer));
			read(establishedConnectionFD, &charsToRead, sizeof(int));
			readIntoBuffer(buffer, establishedConnectionFD, charsToRead);
			memset(message, '\0', sizeof(message));
			strcpy(message, buffer);
			
			//clear buffer and read how many chars will be sent, then read into buffer and copy to key
			memset(buffer, '\0', sizeof(buffer));
			read(establishedConnectionFD, &charsToRead, sizeof(int));
			readIntoBuffer(buffer, establishedConnectionFD, charsToRead);
			memset(key, '\0', sizeof(key));
			strcpy(key, buffer);

			//encrypts the messages and sends over to client
			encryptMessage(message, key, strlen(message));
			write(establishedConnectionFD, message, strlen(message));
		}
		close(establishedConnectionFD); // Close the existing socket which is connected to the client

	}

	
	close(listenSocketFD); // Close the listening socket
	return 0; 
}

