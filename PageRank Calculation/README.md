# PageRank Calculation Using Parallel Processing

## Introduction

This program calculates the PageRank of nodes in a graph using parallel processing techniques. It employs the PageRank algorithm with a damping factor.

<details>
<summary>Dependencies</summary>

- POSIX Threads (pthread library)
- Standard C libraries: stdio.h, stdlib.h, math.h

</details>

<details>
<summary>Usage</summary>

To use the program, follow these steps:

1. Compile the program using the following command:
    ```bash
    gcc pagerank.c -o pagerank -lpthread
    ```

2. Run the executable with the following command:
    ```bash
    ./pagerank <graph_file> <num_threads>
    ```

    Replace `<graph_file>` with the path to the file containing the graph data and `<num_threads>` with the desired number of threads for parallel processing.

    Example:
    ```bash
    ./pagerank tests/Email-Enron.txt 4
    ```

</details>

<details>
<summary>Description</summary>

This program reads a graph as a list of edges stored in a text file, where each line describes an edge in the format "src dst", representing the starting and ending node of the edge. The node numbers are 64-bit integers, i.e., long integers in text format (read using scanf("%ld") or atol()). Comment lines (starting with the character #) should be ignored for speedup measurements.

The program calculates the PageRank of each node in parallel by executing the PageRank algorithm for 50 iterations. All nodes are initialized with a PageRank of 1.0. It utilizes the simplified form of the PageRank algorithm with a damping factor of 85%, where:

- Each node is initialized with a PageRank of 1.0.
- The PageRank of each node is equally distributed among its neighbors through outgoing edges.
- The new PageRank of each node is determined as 0.15 plus 85% of the sum sent by neighbors through incoming edges.

The output is generated in the form of a text file named pagerank.csv, containing pairs of numbers per line: "node, pagerank". For example, given a graph with the following edges:

1 2
2 3
3 1
4 5
5 4


the output for this graph would be the pagerank.csv file with contents:

node, pagerank
1, 1.0
2, 1.0
3, 1.0
4, 1.0
5, 1.0


This file represents the PageRank of each node in the graph.

</details>

<details>
<summary>Authors and Acknowledgment</summary>

I, Alexandros Papadfragkakis, ID 5084, am the sole author of this project.

</details>

<details>
<summary>License</summary>

This project is licensed under the MIT License - see the LICENSE file for details.

</details>
