cmake_minimum_required(VERSION 3.20)

# Name of the project
project(SoftwareTimer VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(include)

# Add the executable and source files
add_executable(software_timer
    src/main.cpp
    # Add more .cpp files here later if needed
)

