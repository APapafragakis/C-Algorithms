#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void next_generation(int rows, int cols, int **current, int **next) {
    int i, j;
    #pragma omp parallel for collapse(2)
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            int alive_neighbors = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) continue;
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && current[ni][nj] == 1) {
                        alive_neighbors++;
                    }
                }
            }
            if (current[i][j] == 1) {
                if (alive_neighbors < 2 || alive_neighbors > 3) {
                    next[i][j] = 0; 
                } else {
                    next[i][j] = 1; 
                }
            } else {
                if (alive_neighbors == 3) {
                    next[i][j] = 1; 
                } else {
                    next[i][j] = 0; 
                }
            }
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input_file> <generations> <output_file> <num_threads>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }
    
    omp_set_num_threads(atoi(argv[4]));

    int rows, cols;
    if (fscanf(input_file, "%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Error reading dimensions from input file\n");
        fclose(input_file);
        return 1;
    }

    int **initial = malloc(rows * sizeof(int *));
    if (!initial) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(input_file);
        return 1;
    }
    for (int i = 0; i < rows; i++) {
        initial[i] = malloc(cols * sizeof(int));
        if (!initial[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(input_file);
            for (int j = 0; j < i; j++) {
                free(initial[j]);
            }
            free(initial);
            return 1;
        }
    }

    for (int i = 0; i < rows; i++) {
        char c;
        fscanf(input_file,"%*c");
        fscanf(input_file,"%*c");
        fscanf(input_file,"%*c");
        for (int j = 0; j < cols; j++) {
            if (fscanf(input_file, "%c|", &c) != 1) {
                fprintf(stderr, "Error reading input file\n");
                fclose(input_file);
                for (int i = 0; i < rows; i++) {
                    free(initial[i]);
                }
                free(initial);
                return 1;
            }
            initial[i][j] = (c == '*') ? 1 : 0;
        }
        fscanf(input_file,"|");
    }
    fclose(input_file);

    int **next = malloc(rows * sizeof(int *));
    if (!next) {
        fprintf(stderr, "Memory allocation failed\n");
        for (int i = 0; i < rows; i++) {
            free(initial[i]);
        }
        free(initial);
        return 1;
    }
    for (int i = 0; i < rows; i++) {
        next[i] = malloc(cols * sizeof(int));
        if (!next[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(next[j]);
            }
            free(next);
            for (int j = 0; j < rows; j++) {
                free(initial[j]);
            }
            free(initial);
            return 1;
        }
    }

    int generations = atoi(argv[2]);
    for (int i = 0; i < generations; i++) {
        next_generation(rows, cols, initial, next);
        int **temp = initial;
        initial = next;
        next = temp;
    }

    FILE *output_file = fopen(argv[3], "w");
    if (!output_file) {
        perror("Error opening output file");
        for (int i = 0; i < rows; i++) {
            free(initial[i]);
        }
        free(initial);
        for (int i = 0; i < rows; i++) {
            free(next[i]);
        }
        free(next);
        return 1;
    }

    fprintf(output_file, "%d %d\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        fprintf(output_file, "|");
        for (int j = 0; j < cols; j++) {
            fprintf(output_file, "%c|", (initial[i][j] == 1) ? '*' : ' ');
        }
        fprintf(output_file, "\n");
    }
    fclose(output_file);

    for (int i = 0; i < rows; i++) {
        free(initial[i]);
        free(next[i]);
    }
    free(initial);
    free(next);

    return 0;
}
