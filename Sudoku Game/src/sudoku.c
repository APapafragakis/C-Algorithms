#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Grid_T sudoku_read(void){
    Grid_T grid;
    int i, j;
    int values[9][9];

    fflush(stdin);

    /* Read the sudoku from stdin, each number followed by space and each line by \n */
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            scanf("%d", &values[i][j]);
        }
    }

    grid = grid_init(grid, values);
    grid.unique = 1;

    return grid;
}						

void sudoku_print(FILE *s, Grid_T g){
    int i, j;
    Choice_T c;

    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            c.i = i;
            c.j = j;
            c = grid_read_value(g, c);

            fprintf(s, "%d ", c.n);
        }

        fprintf(s, "\n");
    }
}

/* Returns if the grid g is complete and correct or not */
int sudoku_is_correct(Grid_T g){
    int i, j, k;
    Choice_T c, res;

    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            c.i = i;
            c.j = j;
            res = grid_read_value(g, c);

            /* If the cell is empty the puzzle is incomplete */
            if(res.n == 0){
                return 0;
            }

            /* Check the numbers in the same row */
            for(k = 0; k < 9; k++){
                if(k != j){
                    c.j = k;
                    c = grid_read_value(g, c);
                    
                    /* If the value reappears in the same row it is not correct */
                    if(c.n == res.n){
                        return 0;
                    }
                }
            }

            c.j = j;

            /* Check the numbers in the same column */
            for(k = 0; k < 9; k++){
                if(k != i){
                    c.i = k;
                    c = grid_read_value(g, c);
                    
                    /* If the value reappears in the same column it is not correct */
                    if(c.n == res.n){
                        return 0;
                    }
                }
            }

            c.i = i;

            /* Check the numbers in the same square */
            for(k = 0; k < 9; k++){
                c.i = (i/3)*3 + k/3;
                c.j = (j/3)*3 + k%3;
                c = grid_read_value(g, c);

                /* If the value reappears in the same square it is not correct */
                if(c.n == res.n && (c.i != i || c.j != j)){
                    return 0;
                }
            }
        }
    }

    /* Return true, no mistakes found */
    return 1;
}

/* On the first call we use (0, -1, 0) as value for c */
Grid_T sudoku_solve(Grid_T g, Choice_T c, int u){
    int i;
    Grid_T grid_res;
    Choice_T res;

    if(c.j != -1) g = grid_update(g, c);
    

    res = grid_exist_unique(g);

    /* If there is a unique value apply it */
    while(res.n != 0){
        g = grid_update(g, res);
        res = grid_exist_unique(g);
    }   

    /* If not find the next empty cell */
    res = grid_iterate(g, c);

    /* If there are no more choice left */
    if(res.n == 0){
        return g;
    }

    /* If the code reached this point, no unique solution exists */
    g = grid_clear_unique(g);

    if(u){
        return g;
    }

    for(i = res.n; i < 10; i++){
        res.n = i;
        if(grid_is_valid(g, res)){
            grid_res = sudoku_solve(g, res,u);

            /* If the grid is correct, return it */
            if(sudoku_is_correct(grid_res)){
                return grid_res;
            }
        }
    }

    /* If no solution was found */
    return g;
}

/* Initializes a array of integers[9][9] with 0 and then
   puts 10 random values to it and initializes a grid. THen
   it solves the grid and removes <nelts> values from it */
Grid_T sudoku_generate(int nelts, int unique){
    int i, j;
    int values[9][9];
    Grid_T grid;
    Choice_T c;
    int counter = 0;
    int x,y;
    srand(time(NULL));

    do {
        counter++;

        /* Initialize the array with 0 */
        for(i = 0; i < 9; i++){
            for(j = 0; j < 9; j++){
                values[i][j] = 0;
            }
        }

        /* Put 1 random values in the array */
        for(i = 0; i < 1; i++){
            /* We don't care if it *hits* the same
            box more than once */
            c.i = rand()%9;
            c.j = rand()%9;
            c.n = rand()%9 + 1;

            values[c.i][c.j] = c.n;
        }

        grid = grid_init(grid, values);

        c.i = 0;
        c.j = -1;
        c.n = 0;

        /* Solve the grid */
        grid = sudoku_solve(grid, c, 0);
        
        for(i = 0; i < 9; i++){
            for(j = 0; j < 9; j++){
                c.i = i;
                c.j = j;
                values[i][j] = grid_read_value(grid, c).n;
            }
        }

        /* Remove <nelts> values from the grid */
        for(i = 0; i < nelts; i++){
            do{
                x = rand()%9;
                y = rand()%9;
            } while(values[x][y] == 0);
            values[x][y] = 0;
        }

        grid = grid_init(grid, values); 

        c.i = 0;
        c.j = -1;
        c.n = 0;
        
    } while(unique && !grid_unique(sudoku_solve(grid, c, unique)) && counter < 1000);

    return grid;
}