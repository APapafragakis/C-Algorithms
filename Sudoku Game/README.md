# Sudoku Checker, Genarator, Solver

This repository contains an implementation of a Sudoku solver according to a specific algorithm. Each function is implemented as a separate commit in the Git repository.

## Files Structure

- `grid.h`: Defines the structure used for storing the Sudoku grid and additional data required during grid solving.
- `grid.c`: Implements the functionalities defined in `grid.h`.
- `sudoku.h`: Defines the interface for managing and solving Sudoku puzzles.
- `sudoku.c`: Implements the Sudoku solving algorithms and functionalities specified in `sudoku.h`.
- `main.c`: Contains the `main` function to execute the Sudoku solver with various command-line options.
- `puzzle/`: Directory containing test cases for running with the executable.

## Algorithm Overview

To solve a Sudoku puzzle, the following algorithm is utilized:

1. For each empty cell in the grid, generate possible choices that do not repeat in the same row, column, or sub-grid.
2. Choose an empty cell and one of its possible choices to fill it.
3. Update the grid with the chosen value and remove that choice from the rest of the puzzle.
4. There are two cases:
   - If each step results in at least one cell with a unique choice, iterate over all cells without uncertainty.
   - If the puzzle does not have a unique choice at each step, utilize backtracking to explore different choices until a solution is found.

### 1. Grid Initialization

Implement a function `grid_init` to initialize the Sudoku grid based on the input array of values.

### 2. Grid Iteration

Implement a function `grid_iterate` to generate possible choices for each empty cell in the grid.

### 3. Grid Update

Implement a function `grid_update` to update the grid with a chosen value for a cell and remove that choice from the rest of the puzzle.

### 4. Check for Unique Solution

Implement functions to check if the grid has a unique solution:
- `grid_unique`: Checks if the grid has a unique solution.
- `grid_exist_unique`: Returns a choice if there exists a unique choice for a cell.
- `grid_clear_unique`: Clears the uniqueness status from the grid.

### 5. Backtracking

Implement the `sudoku_solve` function using recursion and backtracking.

### 6. Generate Sudoku Puzzle

Implement the `sudoku_generate` function to generate a Sudoku puzzle using random choices and backtracking.

## Extra Credit

- Ensure that the implementation does not use pointer variables or global variables.
- Identify functions that introduce unnecessary complexity and explain how to optimize them in the README.md file.

## Usage

The executable `sudoku` can be used with different parameters:

1. `sudoku`: Reads a Sudoku puzzle from standard input, prints the input puzzle to stderr, solves the puzzle, prints a message to stderr indicating if the puzzle has a unique solution, and prints the solution (if exists) to stdout.
   
2. `sudoku -c`: Reads a Sudoku puzzle from standard input, prints the input puzzle to stderr, checks if the puzzle is correct, and prints a message to stderr indicating the correctness of the puzzle along with any errors found.

3. `sudoku -g nelts -u`: Generates a new Sudoku puzzle with approximately `nelts` non-empty cells. If `-u` flag is provided, ensures the generated puzzle has a unique solution. Prints the generated puzzle to stdout.

# Example usage:

```bash
$ sudoku -g 50 -u | sudoku | sudoku -c
