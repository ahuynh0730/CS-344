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

void findNewestDirectory(char*);

int main(int argc, char* argv[]) {
	char dirName[100];
	memset(dirName, '\0', sizeof(dirName));
	findNewestDirectory(dirName);
	chdir(dirName);
	mkdir("testfile", 0700);

	return 0;
}

//finds newest directory and fills a char pointer
void findNewestDirectory(char* newestDirName) {
	int newestDirTime = -1;
	
	
	//directory prefix to search for
	char targetDirPrefix[15] = "huynhant.rooms.";

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
			if (strstr(fileInDir->d_name, targetDirPrefix) != NULL) {
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



