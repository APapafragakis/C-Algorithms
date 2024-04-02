#include "sudoku.h"
#include "grid.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Format of command-line arguments

    ./sudoku (-g <number of empty cell for generated puzzle> (-u)) | (-c)

    if not -g or -c, read from stdin and solves the puzzle
    if -c is given, read from stdin and check if the puzzle is correct
    if -g is given, generate a puzzle with the given number of empty cells
    if -u is given, the generated puzzle must be unique
*/

int main(int argc, char *argv[]){
    Grid_T grid;
    Choice_T c;
    int unique;
    int empty_cells;

    if(argc == 1){
        grid = sudoku_read();
        c.i = 0;
        c.j = -1;
        c.n = 0;
        sudoku_print(stderr, grid);
        grid = sudoku_solve(grid, c,0);
        if(grid_unique(grid)){
            fprintf(stderr, "Unique solution: \n");
        }else{
            fprintf(stderr, "Not unique solution: \n");
        }
        sudoku_print(stdout, grid);
    }else if(argc == 2){
        if(strcmp(argv[1], "-c") == 0){
            grid = sudoku_read();
            sudoku_print(stderr, grid);
            if(sudoku_is_correct(grid)){
                printf("The given puzzle is correct\n");
            }else{
                printf("The given puzzle is incorrect\n");
            }
        }else{
            printf("Invalid command-line argument\n");
        }
    }else if(argc == 3){
        if(strcmp(argv[1], "-g") == 0){
            empty_cells = atoi(argv[2]);
            grid = sudoku_generate(empty_cells, 0);
            sudoku_print(stdout, grid);
        }else{
            printf("Invalid command-line argument\n");
        }
    }else if(argc == 4){
        if(strcmp(argv[1], "-g") == 0 && strcmp(argv[3], "-u") == 0){
            unique = 1;
            empty_cells = atoi(argv[2]);
            grid = sudoku_generate(empty_cells, unique);
            sudoku_print(stdout, grid);
        }else{
            printf("Invalid command-line argument\n");
        }
    }else{
        printf("Invalid command-line argument\n");
    }

    return 0;
}