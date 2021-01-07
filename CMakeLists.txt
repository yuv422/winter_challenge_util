cmake_minimum_required(VERSION 3.17)
project(winter_challenge_util C)

set(CMAKE_C_STANDARD 11)

add_executable(extract
        extract.c file.h defs.h file.c)

add_executable(pack_exe
        pack_exe.c file.h defs.h file.c)
