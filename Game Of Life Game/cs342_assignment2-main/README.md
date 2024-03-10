# Game of Life Parallel Implementation using OpenMP


<summary><strong>Overview</strong></summary>

---

This project implements Conway's Game of Life using parallel programming techniques with OpenMP in C. The Game of Life is a cellular automaton devised by John Conway in 1970, simulating a world where cells evolve based on certain rules.



<summary><strong>Requirements</strong></summary>

---

- C compiler
- OpenMP library


<summary><strong>Usage</strong></summary>

---

To compile the program:
\```bash
gcc -fopenmp game_of_life.c -o game_of_life
\```
To run the program:
\```bash
./game_of_life input_file.txt generations output_file.txt
\```
- `input_file.txt`: File containing the initial state of the world.
- `generations`: Number of generations to simulate.
- `output_file.txt`: File to store the final state of the world after simulation.


<summary><strong>Input File Format</strong></summary>

---

The input file should follow this format:

### Grid Structure

- **Dimensions**: `rows` rows x `cols` columns

```
rows cols
| | | | | | |*|
| | | | | |*|*|
| |*| | | |*|*|
| | | | | | | |
| | | | | | |*|
| | |*|*| | |*|
| | |*|*| | | |
| | | | | | | |
```
- Each `*` represents a living cell.
- Each ` ` (space) represents a dead cell.


- The first line contains the dimensions of the world (rows and columns).
- Subsequent lines represent the state of each cell in the world, where `*` represents a living cell and ` ` represents a dead cell.

<summary><strong>Performance Measurements</strong></summary>

---

Performance measurements were conducted using `glid1.txt` and `glid2.txt`, measuring execution time for 100 and 1000 generations. Each measurement was repeated 5 times.

### Results
1. **Average Speedup**
   - Speedup for parallelism levels 1, 2, and 4 was calculated.
2. **Execution Time Variance**
   - Variance of execution time for each experimental configuration and for the serial program was calculated.
3. **Overhead Analysis**
   - Overhead of OpenMP parallelization with level 1 compared to the serial program was analyzed.
4. **Impact of Iterations and Input Size**
   - Changes in speedup were observed based on the number of iterations (100-1000) and input size (glid1.txt, glid2.txt).

<summary><strong>Conclusion</strong></summary>

---

The parallel implementation using OpenMP demonstrates significant speedup compared to the serial program, especially with larger input sizes and iterations. However, overhead and load balancing issues may affect performance.

