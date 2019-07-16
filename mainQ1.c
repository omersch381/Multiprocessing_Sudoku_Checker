#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "constants.h"
int checkMatrixFromFileOrFromTerminal(char* fileName);

void transformFromCharsToIntQuestion1(const char* sudokuFromInputFromInput,
		char* suduku);

int main(int argc, char* argv[]) {
	int i;
	char* fileName;
	if (argc <= 1)
		checkMatrixFromFileOrFromTerminal(NULL);
	else {

		for (i = 1; i < argc; i++) {
			fileName = argv[i];
			if (checkMatrixFromFileOrFromTerminal(fileName) == 1)
				printf("\n%s is legal\n", fileName);
			else
				printf("\n%s is not legal\n", fileName);
		}
	}
	return 0;

}

int checkMatrixFromFileOrFromTerminal(char* fileName) {
	int fileDescriptor = -1;
	char childsResult[NUM_OF_CHILDREN];
	char sudokuFromInput[SUDOKU_WITH_WHITESPACES_LENGTH];
	char* methodsToCheck[3] = { "0", "1", "2" };
	int i, j, pid, result, pipesToRead[2] = { 0 };
	pipe(pipesToRead);
	int pipesToWrite[6] = { 0 };
	char sudoku[SUDOKU_LENGTH];

	for (i = 0; i < NUM_OF_CHILDREN; i++) {
		pipe(&pipesToWrite[2 * i]);
		if ((pid = fork()))
			close(pipesToWrite[2 * i]);
		else
			break;
	}

	switch (pid) {
	// Child
	case 0:
		if (close(pipesToWrite[1 + i * 2]) == -1)
			perror("close #1");
		if (close(pipesToRead[0]) == -1)
			perror("close #2");
		if (dup2(pipesToWrite[i * 2], fileno(stdin)) == -1)
			perror("dup #1");
		if (close(pipesToWrite[i * 2]) == -1)
			perror("close #3");
		if (dup2(pipesToRead[1], fileno(stdout)) == -1)
			perror("dup #2");
		if (close(pipesToRead[1]) == -1)
			perror("close #4");
		execlp("./sudokuChecker", "sudokuChecker", methodsToCheck[i], NULL);
		perror("exec failed");
		break;

		// Parent
	default:
		if (fileName == NULL) { //which means that we need to ask for the matrix from the console
			printf(
					"Please Enter The Matrix - (int - enter - int without spaces):\n");
			for (i = 0; i < SUDOKU_LENGTH; i++)
				scanf("%c", &sudoku[i]);

		} else {

			fileDescriptor = open(fileName, O_RDONLY);
			if (read(fileDescriptor, sudokuFromInput,
					SUDOKU_WITH_WHITESPACES_LENGTH)
					!= SUDOKU_WITH_WHITESPACES_LENGTH)
				perror("Q1 didn't read enough bytes #1");
		}
		transformFromCharsToIntQuestion1(sudokuFromInput, sudoku);

		for (j = 0; j < NUM_OF_CHILDREN; j++)
			if (write(pipesToWrite[1 + j * 2], sudoku, SUDOKU_LENGTH)
					!= SUDOKU_LENGTH)
				perror("Q1 didn't write enough bytes #1");
		for (j = 0, result = 1; j < NUM_OF_CHILDREN; j++) {
			if (read(pipesToRead[0], &childsResult[j], 1) != 1)
				perror("Q1 didn't read enough bytes #2");
			result &= childsResult[j];
		}
		if (close(fileDescriptor) == -1)
			perror("fileDescriptor was never closed");
	}
	if (close(pipesToRead[0]) == -1)
		perror("close #5");
	for (j = 0; j < NUM_OF_CHILDREN; j++) {
		if (close(pipesToWrite[1 + j * 2]) == -1)
			perror("close #6");
	}
	return result;
}

void transformFromCharsToIntQuestion1(const char* sudokuFromInput, char* sudoku) {
	int i, j;
	for (i = 0, j = 0; i < SUDOKU_WITH_WHITESPACES_LENGTH; i += 2, j++) {
		sudoku[j] = sudokuFromInput[i] - '0';
	}
}
