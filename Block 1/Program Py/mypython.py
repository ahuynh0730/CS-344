# Anthony Huynh's Python Program

import string
import random
import array

# opening files if they exist, otherwise will create
fileList = [ 	open("file1.txt", "w+"),
				open("file2.txt", "w+"),
				open("file3.txt", "w+")
			]


# function that fills file passed in with 10 random lowercase letters 
# and a newline at the end
def fillFile(fileName):
	for iter in range(0, 10):
		randomChar=random.choice(string.ascii_lowercase)
		fileName.write(randomChar)
	fileName.write("\n")	



# calling fillFile with each of the files
for iter in range(0, len(fileList)):
	fillFile(fileList[iter])

