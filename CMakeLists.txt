cmake_minimum_required(VERSION 3.12)
project(OS_Project C)

set(CMAKE_C_STANDARD 11)

# Include directories for header files
include_directories(include)

# Add all source files from src/ directory
file(GLOB SOURCES "src/*.c" "include/*.h")

# Set C compiler flags for debugging
set(CMAKE_C_FLAGS_DEBUG "-g -O0")

# Create the executable named main from the source files
add_executable(main ${SOURCES}
        include/insrtuction_set.h
        src/instruction_set.c
        src/memory.c
        src/main.c
        src/mlf_queue.c
        src/mlf_sched.c
        src/queue.c
        src/tests.c
        src/memory.c
        src/instruction_set.c
        src/dispatcher.c)

# Link the math library
target_link_libraries(main m)