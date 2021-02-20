cmake_minimum_required(VERSION 3.0.0)
project(cli VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include_directories(include)
file(GLOB_RECURSE SRCS src/*.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(cli main.cpp ${SRCS})
target_link_libraries(cli PRIVATE Threads::Threads)