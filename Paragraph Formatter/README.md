# Paragraph Formatter

This C program is designed to process and format paragraphs of text based on a specified accepted paragraph length. It takes a user-defined paragraph length and a text input as command-line arguments and then formats the paragraphs according to the specified length.

## Features

- **LengthCounter Function:**
  - Counts the number of characters in a given string until it encounters the null terminator '\0'.
  - Used to determine the length of the input text.

- **CutP Function:**
  - Takes three parameters: `count` (total characters in the original string), `acceptedParagraphLength` (the desired length of a paragraph), and `string` (the original text).
  - Allocates memory for a temporary string (`runner`) to store the initial part of the paragraph.
  - Adjusts the length of the paragraph based on the last space in the `runner`.
  - Prints the formatted paragraph.
  - Moves the original string pointer to the next position after the processed paragraph.

- **Main Function:**
  - Accepts command-line arguments: `argv[1]` is the accepted paragraph length, and `argv[2]` is the input text.
  - Calls `LengthCounter` to find the total length of the input text.
  - Uses a while loop to process paragraphs until the entire input text is processed.
  - Iterates through the input text until it finds a period ('.').
  - If the length of the current paragraph exceeds the accepted paragraph length, it calls the `CutP` function to format and print the paragraph accordingly.
  - Adjusts pointers and counters to move to the next paragraph.

