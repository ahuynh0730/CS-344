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

//to decrypt a message
void decryptMessage(char message[], char key[], int length){
	int i;
	int textNumber;
	int keyNumber;
	int unencryptedNumber;
	length--;
	
	for (i = 0; i < length; i++){
		textNumber = (int)(message[i]);
		if (textNumber == 32){
			textNumber -= 32;
		}
		else {
			textNumber -= 64;
		}
		
		keyNumber = (int)(key[i]);
		if (keyNumber == 32){
			keyNumber -= 32;
		}
		else {
			keyNumber -= 64;
		}
		
		//subtracts ascii value of key from ascii value of text and mods 27
		unencryptedNumber = (textNumber - keyNumber);
		while (unencryptedNumber < 0){
			unencryptedNumber += 27;
		}
		
		//converts unencryptedNumber to ascii value
		if(unencryptedNumber == 0){
			unencryptedNumber += 32;
		}
		else{
			unencryptedNumber += 64;
		}
		message[i] = (char)unencryptedNumber;
	}
}

//reads specified number of chars into buffer
void readIntoBuffer(char buffer[], int establishedConnectionFD, int charsToRead){
	int keepReading = 1;
	int charsRead = 0;
	int totalChars = 0;
	int i = 0;
	char* bufferPointer = buffer;
	while (charsToRead > SENT_AT_ONE_TIME){
		read(establishedConnectionFD, bufferPointer, SENT_AT_ONE_TIME);
		bufferPointer += SENT_AT_ONE_TIME;
		charsToRead -= SENT_AT_ONE_TIME;
	}
	read(establishedConnectionFD, bufferPointer, charsToRead);
}

int main(int argc, char *argv[])
{

	int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
	char buffer[BUFFER_SIZE];
	int numberChars = sizeof(buffer);
	int totalChars = 0;
	socklen_t sizeOfClientInfo;
	struct sockaddr_in serverAddress, clientAddress;
	pid_t pid;
	char decodeAuthentication[6] = "decode";
	int newLine = 0;
	int charsLeft;

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
			
			//will authenticate that otp_dec called this daemon
			read(establishedConnectionFD, buffer, sizeof(buffer) - 1);
			if (strcmp(buffer, "decode") != 0){
				error("Wrong usage of daemon\n");
			}
			else{
				write(establishedConnectionFD, decodeAuthentication, sizeof(decodeAuthentication));
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


			//decrypts the messages and sends over to client
			decryptMessage(message, key, strlen(message));
			bufferPointer = message;
			charsLeft = strlen(message);
			send(establishedConnectionFD, &charsLeft, sizeof(int), 0);
			while (charsLeft > SENT_AT_ONE_TIME){
				send(establishedConnectionFD, bufferPointer, SENT_AT_ONE_TIME, 0);
				charsLeft -= SENT_AT_ONE_TIME;
				bufferPointer += SENT_AT_ONE_TIME;
			}
			send(establishedConnectionFD, bufferPointer, charsLeft, 0);
		}
		close(establishedConnectionFD); // Close the existing socket which is connected to the client

	}

	
	close(listenSocketFD); // Close the listening socket
	return 0; 
}


