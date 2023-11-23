# Racing Simulator

## Overview

This C++ console application simulates a racing competition with multiple racers. It takes user input for the number of racers, total distance, speed, distance traveled, and acceleration for each racer. The program then calculates and displays the race results based on the time taken by each racer.

## How to Use

1. **Compile the Program:**
   - Ensure you have a C++ compiler installed on your Windows system.
   - Compile the program using the provided source code file (`main.cpp`) and the necessary header file (`std_lib_facilities.h`).

     ```powershell
     g++ main.cpp -o racing_simulator.exe
     ```

2. **Run the Program:**
   - Execute the compiled program.

     ```powershell
     .\racing_simulator.exe
     ```

3. **Input:**
   - Enter the number of racers and the total distance when prompted.
   - For each racer, enter the speed, distance traveled, and acceleration.

4. **Output:**
   - The program will display the race results, including each racer's position and time.

## Input Constraints

- The number of racers should be a positive integer.
- Speed, distance traveled, and acceleration should be positive values.
- The total distance should be greater than or equal to the distance traveled by any racer.

## Error Handling

- The program includes error handling for invalid inputs, such as negative numbers or incorrect relationships between inputs.

## Sorting Results

The program sorts the race results based on the time taken by each racer in ascending order.

