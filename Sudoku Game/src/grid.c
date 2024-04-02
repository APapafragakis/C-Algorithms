#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

/*typedef struct grid_s {
  struct { 
    int count;       
    int choices[10];       

  } cell[9][9];     
  int unique;       
} Grid_T;  */

/*
typedef struct choice_s {
  int i,j,n;	
} Choice_T;
*/


Grid_T eliminate_row(Grid_T g, Choice_T c){
    int j = 0;
    for (j = 0; j < 9; j++) {
         /* Eliminate the cell (c.i,j) that is not the same as (c.i, c.j) and has the choice c.n*/
        if(j != c.j && g.cell[c.i][j].choices[c.n] == 1){
            g.cell[c.i][j].choices[c.n] = 0;
            g.cell[c.i][j].count--;
        }
    }

    return g;
}

Grid_T eliminate_column(Grid_T g, Choice_T c){
    int i;
    for(i = 0; i < 9; i++){
        /* Eliminate the cell (i, c.j) that is not the same as (c.i, c.j) and has the choice c.n*/
        if(i != c.i && g.cell[i][c.j].choices[c.n] == 1){
            g.cell[i][c.j].choices[c.n] = 0;
            g.cell[i][c.j].count--;
        }
    }

    return g;
}

Grid_T eliminate_square(Grid_T g, Choice_T c){
    int i, j, r, k;
    /* Calculate the row and column indices of the top-left cell in the square */
    r = (c.i / 3) * 3;
    k = (c.j / 3) * 3;
    for(i = r; i < r + 3; i++){
        for(j = k; j < k + 3; j++){
            /*Eliminate the cell (i, j) that is not the same as (c.i, c.j) and has the choice c.n */
            if(i != c.i && j != c.j && g.cell[i][j].choices[c.n] == 1){
                g.cell[i][j].choices[c.n] = 0;
                g.cell[i][j].count--;
            }
        }
    }

    return g;
}

Grid_T grid_init(Grid_T f, int v[9][9]){ /* init g with values from array v */
    int i, j, k;
    Choice_T c;
    Grid_T g;

    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){

            if(v[i][j] == 0) g.cell[i][j].count = 9;
            else g.cell[i][j].count = 0;


            /* Marking every choice as possible */
            for(k = 1; k < 10; k++){
                g.cell[i][j].choices[k] = 1;
            }
        }
    }

    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            c.i = i;
            c.j = j;
            c.n = v[i][j];

            /* Initialize the value of the cell with coordinates (i, j) */
            g = grid_update(g, c);
        }
    }

    g.unique = 1;

    return g;
}

/* update value of c.i,c.j to c.n and eliminate c from choices in grid */
Grid_T grid_update(Grid_T g, Choice_T c){

    /* Update the value of the cell */
    g.cell[c.i][c.j].choices[0] = c.n;
    if(c.n != 0){
        g.cell[c.i][c.j].count--;
    }
    
    /* Eliminate c from choices in grid */
    if(c.n != 0){
        g = eliminate_row(g, c);
        g = eliminate_column(g, c);
        g = eliminate_square(g, c);
    }

    return g;
}

/* Find the next cell with value zero starting from (t.i, t.j). 
	If no choice is left, return (0,0,0) */
Choice_T grid_iterate(Grid_T g, Choice_T t){
    int i, k, x, y;
    Choice_T c;
    int index = t.i * 9 + t.j + 1; /* + 1 to start from the next one */

    /* Iterate over all cells */
    for(i = index; i < 9 * 9; i++){
        x = i / 9;
        y = i % 9;

        /* If the cell has a value of zero, return it */
        if(g.cell[x][y].choices[0] == 0){
            c.i = x;
            c.j = y;
            
            /* Find the first available value of this cell */
            for(k = 1; k < 10; k++){
                if(g.cell[x][y].choices[k] == 1){
                    c.n = k;
                    break;
                }
            }

            return c;
        }
    }

    /* If no cell has a value of zero, return (0, 0, 0) */
    c.i = 0;
    c.j = 0;
    c.n = 0;
    return c;
}

int grid_is_valid(Grid_T g, Choice_T c){
    return g.cell[c.i][c.j].choices[c.n];
}

/* return unique flag for g */
int grid_unique(Grid_T g){
    return g.unique;
}

/* return a cell with a unique choice, if one exists, otherwise return (0,0,0) */
Choice_T grid_exist_unique(Grid_T g){
    int i, j, k;
    Choice_T c;
    /* Iterate over all cells */
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            /* If the cell has only one possible choice, return it */
            if(g.cell[i][j].count == 1 && g.cell[i][j].choices[0] == 0){

                for(k = 1; k < 10; k++){
                    if(g.cell[i][j].choices[k] == 1){
                        c.i = i;
                        c.j = j;
                        c.n = k;
                        return c;
                    }
                }

            }


        }
    }

    /* If no cell has only one possible choice, return (0, 0, 0) */
    c.i = 0;
    c.j = 0;
    c.n = 0;
    return c;
}

/* clear unique flag */
Grid_T grid_clear_unique(Grid_T g){
    g.unique = 0;
    return g;
}				

/* return value of i,j */
Choice_T grid_read_value(Grid_T g, Choice_T c){
    Choice_T c1;
    c1.i = c.i;
    c1.j = c.j;
    c1.n = g.cell[c.i][c.j].choices[0];
    return c1;
}