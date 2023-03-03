#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

char *get_full_path(const char *dir_path, const char *filename);

size_t get_file_size(const char *dir_path, const char *filename);

void add_slash(char *dir);

void add_to_dir(char *dir, const char *path);

const char *get_extension(const char* filename);