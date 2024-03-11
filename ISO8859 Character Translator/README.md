# ISO8859 Character Translator

## Description

The program reads characters from standard input (stdin) and converts Greek characters to their corresponding Latin characters according to the defined mappings. It utilizes a state machine approach to determine the current state and the appropriate action to take for each input character.

## Usage

1. Compile the program using the provided Makefile:

    ```bash
    make translate
    ```

2. Run the program and provide input through standard input:

    ```bash
    ./translate
    ```

3. Input Greek characters and observe the converted output.

4. To exit the program, press `Ctrl + D`.

## Mapping

The program uses a mapping table to convert Greek characters to their Latin equivalents. The mapping is defined within the `Initialize_CharMap` function in the source code.

## State Machine

The program employs a state machine to manage the conversion process efficiently. It transitions between different states based on the input characters and applies the corresponding conversion functions.

## Files

- `src/main.c`: The main source code file containing the implementation of the conversion program.
- `Makefile`: The Makefile for compiling the program.
- `README.md`: This file, providing information about the program and instructions for usage.

## License

This program is open-source software licensed under the [MIT License](LICENSE).
