# Program 1 – CS 344
This assignment asks you to write bash shell scripts to compute matrix operations. The purpose is to get you familiar with the Unix shell, shell programming, Unix utilities, standard input, output, and error, pipelines, process ids, exit values, and signals (at a basic level).

What you're going to submit is your script, called simply "matrix".

## Overview
In this assignment, you will write a bash shell script that calculates basic matrix operations using input from either a file or stdin. The input will be whole number values separated by tabs into a rectangular matrix. Your script should be able to print the dimensions of a matrix, transpose a matrix, calculate the mean vector of a matrix, add two matrices, and multiply two matrices.

You will be using bash builtins and Unix utilities to complete the assignment. Some commands to read up on are while, cat, read, expr, cut, head, tail, wc, and sort.

Your script must be called simply "matrix". The general format of the matrix command is:
	matrix OPERATION [ARGUMENT]...
Refer to man(1) (You can do this with the command `man 1 man`) for an explanation of the conventional notation regarding command syntax, to understand the line above. Note that many terminals render italic font style as an underline:
	matrix OPERATION [ARGUMENT]...