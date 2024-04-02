###################################################
#
# file: Makefile
#
# @Author:   Iacovos G. Kolokasis
# @Version:  10-02-2022
# @email:    kolokasis@csd.uoc.gr
#
# Makefile
#
####################################################

all: sudoku

sudoku: src/sudoku.c src/grid.c src/main.c
	gcc -Wall -pedantic -ansi -o sudoku src/sudoku.c src/grid.c src/main.c

clean:
	rm -f sudoku
