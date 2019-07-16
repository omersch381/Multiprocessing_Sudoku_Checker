all: v1 v2 sudokuChecker

v1:
	gcc -o v1 mainQ1.c
v2:
	gcc -o v2 mainQ2.c 
sudokuChecker:
	gcc -o sudokuChecker sudokuChecker.c 
clean:
	rm -f v1 v2 sudokuChecker
