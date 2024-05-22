# Mini-Language Interpreter with Multi-Level Feedback Queue

This project implements an interpreter for a custom mini-language. The interpreter supports various commands for basic input/output operations, file handling, and mutual exclusion using semaphores. The interpreter operates with a multi-level feedback queue (MLFQ) and supports single-level nested commands.

## Team
<a href="https://github.com/your-repo-link/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=https://github.com/ZeyadAttia5/Interpreter-with-Multi-Level-Feedback-Scheduler" />
</a>

## Table of Contents
- [Features](#features)
- [Commands](#commands)
- [Tools Used](#tools-used)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

## Features

1. **Command Execution:** Supports commands for printing, variable assignment, file operations, and mutual exclusion.
2. **Nested Commands:** Allows single-level nested command execution.
3. **Multi-Level Feedback Queue:** Uses MLFQ for managing command execution.

## Commands

1. **print:** Outputs the value of a variable.
   - Example: `print x`

2. **assign:** Initializes a variable and assigns a value to it. Supports integers and strings.
   - Example: `assign x y`
   - If `y` is `input`, prompts the user for a value.

3. **writeFile:** Writes data to a file.
   - Example: `writeFile filename data`

4. **readFile:** Reads data from a file.
   - Example: `readFile filename`

5. **printFromTo:** Prints all numbers between two values.
   - Example: `printFromTo x y`

6. **semWait:** Acquires a semaphore resource.
   - Example: `semWait resourceName`

7. **semSignal:** Releases a semaphore resource.
   - Example: `semSignal resourceName`

## Tools Used

- C
- Linux

## Installation

1. Clone the repository:
   ```shell
   git clone https://github.com/your-repo-link.git
  shell```
2. 

