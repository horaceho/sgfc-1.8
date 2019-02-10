cmake_minimum_required(VERSION 3.10)
project(sgfc C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(sgfc
        all.h
        execute.c
        gameinfo.c
        load.c
        main.c
        parse.c
        parse2.c
        properties.c
        protos.h
        save.c
        strict.c
        util.c)
