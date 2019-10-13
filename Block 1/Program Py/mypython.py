# Anthony Huynh's Python Program

import string
import random

# opening files if they exist, otherwise will create
file1= open("file1.txt", "w+")
file2= open("file2.txt", "w+")
file3= open("file3.txt", "w+")

# function that fills file passed in with 10 random lowercase letters 
# and a newline at the end
def fillFile(fileName):
	for iter in range(0, 10):
		randomChar=random.choice(string.ascii_lowercase)
		fileName.write(randomChar)
	fileName.write("\n")	

# calling fillFile with each of the files
fillFile(file1)
fillFile(file2)
fillFile(file3)

#closing the files
file1.close()
file2.close()
file3.close()