#include "path_operations.h"

char *get_full_path(const char *dir_path, const char *filename) {
    char *file_path =
        malloc(strlen(dir_path) + strlen(filename) + 2);
    size_t maxlen = strlen(dir_path) + strlen(filename) + 2;
    snprintf(file_path, maxlen, "%s/%s", dir_path, filename);
    return file_path;
}

size_t get_file_size(const char *dir_path, const char *filename) {
    char *file_path = get_full_path(dir_path, filename);
    struct stat st;
    if (stat(file_path, &st) == 0) {
        free(file_path);
        return st.st_size;
    }
    free(file_path);
    return 0;
}

void add_slash(char *dir) {
    int csz = strlen(dir);
    dir[csz] = '/';
    dir[csz + 1] = '\0';
}

void add_to_dir(char *dir, const char *path) {
    int csz = strlen(dir);
    int psz = strlen(path);
    for (int i = 0; i < psz; ++i) {
        dir[csz + i] = path[i];
    }
    dir[csz + psz] = '/';
    dir[csz + psz + 1] = '\0';
}

const char *get_extension(const char* filename) {
    const char *cur_ch = filename;
    while (*cur_ch != '.') {
        cur_ch++;
    }
    return cur_ch;
}