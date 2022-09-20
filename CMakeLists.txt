cmake_minimum_required(VERSION 3.22)
project(OneDatabase C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(OneDatabase
        main.c
        )
