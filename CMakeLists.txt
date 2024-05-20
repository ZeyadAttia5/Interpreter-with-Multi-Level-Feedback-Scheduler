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
        include/STRING_OPERATIONS.h
        src/STRING_OPERATIONS.c
        include/mutex.h
        src/mutex.c
        src/pcb.c)

# Link the math library
target_link_libraries(main m)