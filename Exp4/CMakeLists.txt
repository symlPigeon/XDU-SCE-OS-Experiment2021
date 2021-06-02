cmake_minimum_required(VERSION 3.20)
project(Exp4 C)

set(CMAKE_C_STANDARD 11)

add_executable(Exp4
        producer_and_consumer.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Exp4 PRIVATE Threads::Threads)
