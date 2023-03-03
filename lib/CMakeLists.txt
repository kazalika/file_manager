cmake_minimum_required(VERSION 3.0)

project(library)

add_library(library deleter.c deleter.h init.c init.h warning_screens.c warning_screens.h path_operations.h path_operations.c keys_functions.h keys_functions.c screen.h screen.c)

