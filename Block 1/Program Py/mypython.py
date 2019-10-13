# Anthony Huynh's Python Program


import string
import random
import array


# function that fills file passed in with 10 random lowercase letters 
# and a newline at the end
def fillFile(fileName):
	for iter in range(0, 10):
		randomChar=random.choice(string.ascii_lowercase)
		fileName.write(randomChar)
	fileName.write("\n")	


def main():
	# opening files if they exist, otherwise will create
	fileList = [ 	open("file1.txt", "w+"),
					open("file2.txt", "w+"),
					open("file3.txt", "w+")
				]
	
	# calling fillFile with each of the files
	for iter in range(0, len(fileList)):
		fillFile(fileList[iter])
		
	# generating random numbers between 1 and 42 inclusively
	firstNumber = random.randrange(1, 43)
	secondNumber = random.randrange(1, 43)

	# multiplies two random numbers together
	product = firstNumber * secondNumber
	
	# goes to beginning of each file
	for iter in range(0, len(fileList)):
		fileList[iter].seek(0)

	#output file contents and numbers
	for iter in range(0, len(fileList)):
		print(fileList[iter].read(),end="")
	print(firstNumber)
	print(secondNumber)
	print(product)

main()



	


