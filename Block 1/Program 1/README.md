# Exercise 1
I've prepared an exercise for you all. It's intended to target tools and techniques you'll need for program 1. This is not a required assignment and is not worth any points.

# Overview
This exercise asks you to write a bash shell script that counts the number of lines of all files in the current directory and any subdirectories.

# Specifications
Suggested tools: wc, ls, cd, for loops, if statements
Many programs, such as cp, have a -r recursive option. You will be creating a very specialized version of that option for wc. Your script should add the lines of all files in the current directory. If any of these files are directories, it should also add the lines of any files in those directories, and then any directores in there, and so on and so forth. Finally, your program should send this result to stdout. This is generally a recursive process. You could try to implement a queue or stack in bash, but I would not recommend it. You should not use any of the tools banned in program 1.
I've provided a directory set up you can use for testing. (It is named dir.)

Example usage(using instructor given solution, mine is slightly different):

		$ wc -l *
		wc: dir1: Is a directory
			0 dir1
		wc: dir2: Is a directory
			0 dir2
		wc: dir4: Is a directory
			0 dir4
			8 file1
		 	11 file2
		 	13 file3
		 	32 total
		$ wc -l dir1/*
		wc: dir1/dir3: Is a directory
			0 dir1/dir3
			11 dir1/file4
			11 total
		$ wc -l dir2/*
			 15 dir2/file5
			 12 dir2/file7
			 27 total
		$ wc -l dir1/dir3/*
		13 dir1/dir3/file6
		$ allLines
		83            # Notice this total is the sum of all previous totals
		
The solution given is named allLines(instructor's solution) inside of dir.
