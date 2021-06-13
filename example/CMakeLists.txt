
cmake_minimum_required(VERSION 3.0.0)

add_executable(cli main.cpp)
target_link_libraries(cli PRIVATE mecli)