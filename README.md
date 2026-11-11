# Mini C Preprocessor in C

## Overview

Mini C Preprocessor is a simplified C preprocessor developed using C programming. It performs basic preprocessing operations on a C source file before compilation.

## Features

- Removes single-line (`//`) and multi-line (`/* ... */`) comments.
- Handles standard header file inclusion.
- Handles user-defined header file inclusion.
- Supports object-like macro replacement.
- Supports function-like macro replacement.
- Supports header inclusion within user-defined headers.
- Generates a preprocessed output file with the `.i` extension.

## Project Structure

| File | Description |
|---|---|
| `main.c` | Main program and preprocessing flow |
| `header.h` | Header file containing function declarations |
| `removeComments.c` | Removes comments from source code |
| `headerFile_Inclusion.c` | Handles header file inclusion |
| `replaceMacro.c` | Performs macro replacement |
| `Makefile` | Compiles and builds the project |

## Requirements

- GCC Compiler
- GNU Make
- Linux Environment

## Compilation

Open the terminal in the project directory and execute:

```bash
make
```

This generates the executable:

```bash
my_preprocessor
```

## Usage

Run the preprocessor by providing a C source file:

```bash
./my_preprocessor input.c
```

The preprocessed output will be generated as:

```text
input.i
```

## Example

**Input (input.c):**

```c
#define PI 3.14

#include <stdio.h>

// Sample comment

int main()
{
    printf("%f", PI);
    return 0;
}
```

**Preprocessed Output (input.i):**

```c
int main()
{
    printf("%f", 3.14);
    return 0;
}
```

## Clean Build Files

To remove generated object files, execute:

```bash
make clear
```

## Limitations

This is an educational mini preprocessor that supports basic preprocessing operations. It is not a complete replacement for the standard C preprocessor.

## Author

**Ganga Maheswari**

Interested in Embedded Systems and Systems Programming.
