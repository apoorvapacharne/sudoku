project: sudoku.o main.o
	cc sudoku.o main.o -o project -lm -lncurses -lmenu
main.o: main.c 
	cc -c main.c -lncurses -lmenu 
sudoku.o: sudoku.c sudoku.h
	cc -c sudoku.c -lm 
