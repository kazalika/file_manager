cmake_minimum_required(VERSION 3.0)
project(file_manager)

add_subdirectory(lib)
add_executable(file_manager main.c)


find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

target_link_libraries(file_manager library ${CURSES_LIBRARIES})