cmake_minimum_required(VERSION 3.20)
project(Exp3 C)

set(CMAKE_C_STANDARD 11)

add_executable(Exp3
        sort.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Exp3 PRIVATE Threads::Threads)
