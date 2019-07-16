#ifndef MAINQ2_H_
#define MAINQ2_H_
#include "constants.h"

int checkMatrixMine(char* sudokuChars, char checkType);
int checkMatrixFromFileOrFromTerminalQuestion2(char* fileName);
void ToMatrix(char* sudokuFromFile, int matrix[ROWS_SIZE][COLS_SIZE]);
int checkAllRowsQuestion2(int mat[ROWS_SIZE][COLS_SIZE], int rows, int cols);
int checkAllColumnsQuestion2(int mat[ROWS_SIZE][COLS_SIZE], int rows, int cols);
int checkAllSubMatrixQuestion2(int mat[ROWS_SIZE][COLS_SIZE]);
void
transformFromCharsToIntQuestion2(const char* sudokuFromInput, char* sudoku);

typedef struct {
	char finish[NUM_OF_CHILDREN];
	char sudoku[SUDOKU_LENGTH];
	char parentRead;
	char childsResult[NUM_OF_CHILDREN];
} mmap_t;

#endif /* MAINQ2_H_ */
