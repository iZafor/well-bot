cmake_minimum_required(VERSION 3.25)
set(CMAKE_C_STANDARD 11)

project(player)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} libplayer)

add_subdirectory(include)
