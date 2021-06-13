
cmake_minimum_required(VERSION 3.17)

file(GLOB_RECURSE SRCS src/*.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(mecli STATIC ${SRCS})
target_link_libraries(mecli PRIVATE Threads::Threads)
target_include_directories(mecli PUBLIC include)
