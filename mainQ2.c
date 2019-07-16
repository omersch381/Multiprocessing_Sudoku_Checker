#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "mainQ2.h"

int checkMatrixQ2(char*, char);

int main(int argc, char* argv[]) {
	int i;
	char* fileName;
	if (argc <= 1)
		checkMatrixFromFileOrFromTerminalQuestion2(NULL);
	else {
		for (i = 1; i < argc; i++) {
			fileName = argv[i];
			if (checkMatrixFromFileOrFromTerminalQuestion2(fileName) == 1)
				printf("\n%s is legal\n", fileName);
			else
				printf("\n%s is not legal\n", fileName);
		}
	}
	return 0;
}
int checkMatrixFromFileOrFromTerminalQuestion2(char* fileName) {
	char sudokuFromInput[SUDOKU_WITH_WHITESPACES_LENGTH];
	int fileDescriptor = -1;
	int pid, result;
	mmap_t* pMmap = mmap(NULL, sizeof(mmap_t), PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	int i, j;
	for (i = 0; i < 3; i++) {
		pid = fork();
		if (pid <= 0)
			break;
	}

	switch (pid) {

	// Child
	case 0:
		while (1) {
			while (pMmap->parentRead) {
				if (pMmap->parentRead == -1)
					exit(0);
				pMmap->childsResult[i] = checkMatrixQ2(pMmap->sudoku, i);
				pMmap->finish[i] = 1;

				while (pMmap->parentRead == 1)
					;
			}
		}

		break;
		// Parent

	default:
		if (fileName == NULL) { //which means that we need to ask for the matrix from the console
			printf(
					"Please Enter The Matrix - (int - enter - int without spaces):\n");
			for (i = 0; i < SUDOKU_LENGTH; i++)
				scanf("%c", &pMmap->sudoku[i]);

		} else {
			fileDescriptor = open(fileName, O_RDONLY);
			if (read(fileDescriptor, sudokuFromInput,
					SUDOKU_WITH_WHITESPACES_LENGTH)
					!= SUDOKU_WITH_WHITESPACES_LENGTH)
				perror("Q2 didn't read enough bytes #1");

		}
		transformFromCharsToIntQuestion2(sudokuFromInput, pMmap->sudoku);
		pMmap->parentRead = 1;
		for (j = 0, result = 1; j < NUM_OF_CHILDREN; j++) {
			while (pMmap->finish[j] == 0)
				;
			result &= pMmap->childsResult[j];
			pMmap->finish[j] = 0;
		}
		memset(pMmap, 0, sizeof(mmap_t));
		if (fileDescriptor > 0)
			if (close(fileDescriptor) == -1)
				perror("close #1");
	}
	pMmap->parentRead = -1;
	return result;
}

int checkMatrixQ2(char* sudokuChars, char checkType) {
	int matrix[ROWS_SIZE][COLS_SIZE];
	int res;
	ToMatrix(sudokuChars, matrix);
	res = 0;
	switch (checkType) {
	case 0:
		res = checkAllRowsQuestion2(matrix, ROWS_SIZE, COLS_SIZE);
		return res;
		break;
	case 1:
		res = checkAllColumnsQuestion2(matrix, ROWS_SIZE, COLS_SIZE);
		return res;
		break;
	case 2:
		res = checkAllSubMatrixQuestion2(matrix);
		return res;
		break;
	}
	return res;
}
int checkRowQuestion2(int mat[ROWS_SIZE][COLS_SIZE], int numberRow, int cols) {
	int i, j;
	int result;
	for (i = 0; i < 9; i++) {
		result = 0;
		for (j = 0; j < cols; j++) {
			if (sudokuNumbers[i] == mat[numberRow][j]) {
				result = 1;
			}
		}
		if (result != 1)
			return 0;
	}
	return 1;
}
int checkAllRowsQuestion2(int mat[ROWS_SIZE][COLS_SIZE], int rows, int cols) {
	int i;
	int result = 0;
	for (i = 0; i < rows; i++) {
		result = checkRowQuestion2(mat, i, cols);
	}
	return result;
}
void transformFromCharsToIntQuestion2(const char* sudokuFromInput, char* sudoku) {
	int i, j;
	for (i = 0, j = 0; i < SUDOKU_WITH_WHITESPACES_LENGTH; i += 2, j++) {
		sudoku[j] = sudokuFromInput[i] - '0';
	}
}
int checkCulumnQuestion2(int mat[ROWS_SIZE][COLS_SIZE], int rows, int numberCol) {
	int i, j;
	int result;
	for (i = 0; i < 9; i++) {
		result = 0;
		for (j = 0; j < rows; j++) {
			if (sudokuNumbers[i] == mat[j][numberCol]) {
				result = 1;
			}
		}
		if (result != 1)
			return 0;
	}
	return 1;
}
int checkAllColumnsQuestion2(int mat[ROWS_SIZE][COLS_SIZE], int rows, int cols) {
	int i;
	int result;
	for (i = 0; i < cols; i++) {
		result = checkCulumnQuestion2(mat, rows, i);
	}
	return result;
}
int checkSubMatrixQuestion2(int mat[ROWS_SIZE][COLS_SIZE], int rowStart,
		int colStart) {
	int i;
	int x, y;
	int result;
	int rowEnd = rowStart + 2;
	int colEnd = colStart + 2;
	for (i = 0; i < 9; i++) {
		result = 0;
		for (x = rowStart; x < rowEnd + 1; x++) {
			for (y = colStart; y < colEnd + 1; y++) {
				if (sudokuNumbers[i] == mat[x][y]) {
					result = 1;
				}
			}
		}
		if (result != 1)
			return 0;
	}
	return 1;
}
int checkAllSubMatrixQuestion2(int mat[ROWS_SIZE][COLS_SIZE]) {
	int i, result, rowToCheck, columnToCheck;
	rowToCheck = 0;
	columnToCheck = 0;
	for (i = 0; i < ROWS_SIZE; i++) {
		result = checkSubMatrixQuestion2(mat, rowToCheck, columnToCheck);
		if (result == 0)
			return 0;
		else {
			columnToCheck += 3;
			if (columnToCheck > 6) {
				columnToCheck = 0;
				rowToCheck += 3;
			}
		}
	}
	return 1;
}
void ToMatrix(char* sudokuFromFile, int matrix[ROWS_SIZE][COLS_SIZE]) {
	int i, j, k;
	for (k = 0, i = 0; i < SUDOKU_NUMBERS; i++) {
		for (j = 0; j < SUDOKU_NUMBERS; j++) {
			matrix[i][j] = (int) (sudokuFromFile[k]);
			k++;
		}
	}
}
