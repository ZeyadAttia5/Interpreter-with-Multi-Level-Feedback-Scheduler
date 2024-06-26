# Mini-Language Interpreter with Multi-Level Feedback Queue

This project implements an interpreter for a custom mini-language. The interpreter supports various commands for basic input/output operations, file handling, and mutual exclusion using semaphores. The interpreter operates with a multi-level feedback queue (MLFQ) and supports single-level nested commands.

## Team
<!-- 
  <a href="https://github.com/ZeyadAttia5/Interpreter-with-Multi-Level-Feedback-Scheduler/graphs/contributors">
    <img src="https://contrib.rocks/image?repo=ZeyadAttia5/Interpreter-with-Multi-Level-Feedback-Scheduler" />
  </a> 
-->
- [Amr Hegazy](https://github.com/Amr-Hegazy1)

- [Ali Hany](https://github.com/alilabib03)

- [Mohamed Wael](https://github.com/mowaell)

- [Omar Saqr](https://github.com/Omarsaqr)

- [Zeyad Attia](https://github.com/ZeyadAttia5)


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

2. Build the Project:
     ```shell
     mkdir build
     cd build
  - Run CMake to configure the project:
     ```shell
        cmake ..
  - Compile the project:
     ```shell
        make
3. Create Your Programs:
  - Create up to 3 program files with a .txt extension.
  - Populate these files with logic using the mini-language described in the [Commands](#commands) section.

4. Run the Interpreter:
  - In the build directory, execute the interpreter with your program files and their respective arrival times:
      ```shell
      ./main program1.txt arrival_time1 program2.txt arrival_time2 program3.txt arrival_time3
5. Supply Input:
   - Provide necessary input via the command line as prompted by your programs until execution completes.


## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

