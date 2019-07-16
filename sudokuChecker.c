#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include "constants.h"


int checkMatrix(char*, char);
int checkRowQuestion2(int mat[ROWS_SIZE][COLS_SIZE], int numberRow, int cols);
void ToMatrixQuestion2(char* sudokuFromFile, int matrix[ROWS_SIZE][COLS_SIZE]);
int checkAllRows(int mat[ROWS_SIZE][COLS_SIZE], int rows, int cols);
int
checkCulumn(int mat[ROWS_SIZE][COLS_SIZE], int rows, int numberCol);
int checkAllColumns(int mat[ROWS_SIZE][COLS_SIZE], int rows, int cols);
int checkSubMatrix(int mat[ROWS_SIZE][COLS_SIZE], int rowStart,
		int colStart);
int checkAllSubMatrix(int mat[ROWS_SIZE][COLS_SIZE]);
void ToMatrixSudokuChecker(char* sudokuFromFile,
		int matrix[ROWS_SIZE][COLS_SIZE]);

int sudokuCheck(const char* suduku, char checkType);

int main(int argc, char* argv[]) {
	int fileRead;
	char result;
	char sudoku[SUDOKU_LENGTH];
	while (1) {
		fileRead = read(fileno(stdin), sudoku, SUDOKU_LENGTH);
		if (fileRead == 0) {
			return (0);
		}
		if (argc > 1)
			result = checkMatrix(sudoku, *argv[1]);
		else
			result = 0;
		if (write(fileno(stdout), &result, 1) != 1)
			perror("sudokuChecker didn't write enough bytes");
	}
	return 0;
}

int checkMatrix(char* sudokuChars, char checkType) {
	int matrix[ROWS_SIZE][COLS_SIZE];
	int res;
	ToMatrixQuestion2(sudokuChars, matrix);
	res = 0;
	switch (checkType) {
	case '0':
		res = checkAllRows(matrix, ROWS_SIZE, COLS_SIZE);
		return res;
		break;
	case '1':
		res = checkAllColumns(matrix, ROWS_SIZE, COLS_SIZE);
		return res;
		break;
	case '2':
		res = checkAllSubMatrix(matrix);
		return res;
		break;
	}
	return res;
}
int checkRow(int mat[ROWS_SIZE][COLS_SIZE], int numberRow, int cols) {
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
int checkAllRows(int mat[ROWS_SIZE][COLS_SIZE], int rows, int cols) {
	int i;
	int result = 0;
	for (i = 0; i < rows; i++) {
		result = checkRow(mat, i, cols);
	}
	return result;
}
int checkCulumn(int mat[ROWS_SIZE][COLS_SIZE], int rows, int numberCol) {
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
int checkAllColumns(int mat[ROWS_SIZE][COLS_SIZE], int rows, int cols) {
	int i;
	int result;
	for (i = 0; i < cols; i++) {
		result = checkCulumn(mat, rows, i);
	}
	return result;
}
int checkSubMatrix(int mat[ROWS_SIZE][COLS_SIZE], int rowStart,
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
int checkAllSubMatrix(int mat[ROWS_SIZE][COLS_SIZE]) {
	int i, result, rowToCheck, columnToCheck;
	rowToCheck = 0;
	columnToCheck = 0;
	for (i = 0; i < ROWS_SIZE; i++) {
		result = checkSubMatrix(mat, rowToCheck, columnToCheck);
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
void ToMatrixSudokuChecker(char* sudokuFromFile,
		int matrix[ROWS_SIZE][COLS_SIZE]) {
	int i, j, k;
	for (k = 0, i = 0; i < SUDOKU_NUMBERS; i++) {
		for (j = 0; j < SUDOKU_NUMBERS; j++) {
			matrix[i][j] = (int) (sudokuFromFile[k]);
			k++;
		}
	}
}
void ToMatrixQuestion2(char* sudokuFromFile, int matrix[ROWS_SIZE][COLS_SIZE]) {
	int i, j, k;
	for (k = 0, i = 0; i < SUDOKU_NUMBERS; i++) {
		for (j = 0; j < SUDOKU_NUMBERS; j++) {
			matrix[i][j] = (int) (sudokuFromFile[k]);
			k++;
		}
	}
}
