
cmake_minimum_required(VERSION 3.17)

project(mecli VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(mecli)

if (FOX_MECLI_EXAMPLE)
    add_subdirectory(example)
endif()