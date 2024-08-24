cmake_minimum_required(VERSION 3.18)

project(vicky/os_vicky LANGUAGES C)

add_executable(lsh parse.c lsh.c)
target_link_libraries(lsh PRIVATE readline termcap)
target_compile_options(lsh PRIVATE "-ggdb3" "-O0" "-Wall" "-Wextra")
