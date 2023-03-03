#pragma once

#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib/deleter.h"
#include "lib/init.h"
#include "lib/warning_screens.h"
#include "lib/path_operations.h"

int selector(const struct dirent *obj) {
    if (strcmp(obj->d_name, ".") == 0) {
        return 0;
    }
    if (strcmp(obj->d_name, "..") == 0) {
        return 1;
    }
    if (obj->d_name[0] == '.' && show_hidden == H_OFF) {
        return 0;
    }
    return (strcmp(obj->d_name, ".") != 0);
}

void update_ud() {
    if (cursor < up_string) {
        up_string = cursor;
        down_string = up_string + high - 1;
    }
    if (cursor > down_string) {
        down_string = cursor;
        up_string = down_string - high + 1;
    }
}


int scr_dir(const char *path) {
    move(0, 0);
    int n = scandir(path, &list, selector, alphasort);
    int printed = 0;

    // n < 0 case TODO upd: meh... it's not necessary 

    for (int i = 0; i < n; ++i) {
        if (i < up_string || i > down_string) {
            continue;
        }

        if (list[i]->d_type == DT_DIR) {
            attron(COLOR_PAIR(DIR_COLOR));

            if (cursor == i) {
                addstr(cstr);
            } else {
                addstr(ncstr);
            }
            printw(ostr_fmt_dir, list[i]->d_name);
            printed++;
            
            attron(COLOR_PAIR(NEUTRAL_COLOR));
        } else if (list[i]->d_type == DT_FIFO) {
            attron(COLOR_PAIR(FIFO_COLOR));

            if (cursor == i) {
                addstr(cstr);
            } else {
                addstr(ncstr);
            }
            printw(ostr_fmt_file, list[i]->d_name, get_file_size(path, list[i]->d_name));
            printed++;

            attron(COLOR_PAIR(NEUTRAL_COLOR));
        } else if (list[i]->d_type == DT_LNK) {
            attron(COLOR_PAIR(LNK_COLOR));

            if (cursor == i) {
                addstr(cstr);
            } else {
                addstr(ncstr);
            }
            printw(ostr_fmt_file, list[i]->d_name, get_file_size(path, list[i]->d_name));
            printed++;

            attron(COLOR_PAIR(NEUTRAL_COLOR));
            
        } else {
            attron(COLOR_PAIR(FILE_COLOR));

            if (cursor == i) {
                addstr(cstr);
            } else {
                addstr(ncstr);
            }
            printw(ostr_fmt_file, list[i]->d_name, get_file_size(path, list[i]->d_name));
            printed++;
            
            attron(COLOR_PAIR(NEUTRAL_COLOR));
        }
    }
    while (printed < high) {
        printw("\n");
        printed++;
    }
    return n;
}

void extra_exit(const char *str) {
    endwin();
    // printf("%s\n", current_dir);
    printf("%s\n", str);
    free(list);
    exit(0);
}


int copy_file(const char* to_dir, const char* src_dir) {
    if (strcmp(to_dir, src_dir) == 0) {
        warning_screen("Source file is here. Can't create another one");
        return 1;
    }
    char *filename = filename_buffer;
    char *full_from_path = get_full_path(src_dir, filename);
    int fd_src = open(full_from_path, O_RDONLY);
    
    // fd < 0 TODO
    if (fd_src < 0) {
        warning_screen("Can't open source file");
        return -1;
    }


    char *full_to_path = get_full_path(to_dir, filename);
    int fd_to = open(full_to_path, O_WRONLY | O_CREAT | O_EXCL, 0666);

    // fd < 0 TODO
    if (fd_to < 0) {
        warning_screen("Can't create file");
        return -2;
    }

    char buf[4096];
    ssize_t nread = 0;
    while ((nread = read(fd_src, buf, sizeof(buf))) > 0) {

        ssize_t nwrite = write(fd_to, buf, nread);
        if (nwrite != nread) {
            extra_exit("Wrong Copy!");
        }

    }
    if (nread < 0) {
        warning_screen("Error. Can't read file. It isn't copied");
    }
    if (close(fd_src) < 0) {
        warning_screen("Error. Can't close source file. Wtf, man...");
    }
    if (close(fd_to) < 0) {
        warning_screen("Error. Can't close new file. Um...");
    }
    return 0;
}


int main(int argc, char *argv[]) {

    init(argv[0]);
    
    while (true) {
        update_ud();
        int size_of_dir = scr_dir(current_dir);
        int key = getch();
        
        if (key == KEY_UP) {
            cursor = (cursor - 1 + size_of_dir) % size_of_dir;    
        }
        if (key == KEY_DOWN) {
            cursor = (cursor + 1) % size_of_dir;
        }
        if (key == '\n') {
            struct dirent *x = list[cursor];
            if (x->d_type != DT_DIR) {
                warning_screen("It's not a directory");
                continue;
            }
            add_to_dir(current_dir, x->d_name);
            realpath(current_dir, current_dir);
            add_slash(current_dir);
            cursor = D_CURSOR;
            int err = scr_dir(current_dir);
            if (err == -1) {
                warning_screen("Can't read a directory");
                add_to_dir(current_dir, "..");
                realpath(current_dir, current_dir);
                add_slash(current_dir);
            }
        }
        if (key == 'q') {
            extra_exit("You quit\n");
        }
        if (key == 'd') {
            int sure = delete_warning_screen();
            if (!sure) {
                continue;
            }

            struct dirent *x = list[cursor];
            if (x->d_type == DT_DIR) {
                char *f_name = get_full_path(current_dir, x->d_name);
                rmrf(f_name);
                free(f_name);
            } else {
                char *f_name = get_full_path(current_dir, x->d_name);
                remove(f_name);
                free(f_name);
            }
            cursor = D_CURSOR;
        }
        if (key == 'c') {
            struct dirent *x = list[cursor];
            regim = COPY;

            strcpy(path_buffer, current_dir);
            strcpy(filename_buffer, x->d_name);
        }
        if (key == 'x') {
            struct dirent *x = list[cursor];
            regim = CUT;

            strcpy(path_buffer, current_dir);
            strcpy(filename_buffer, x->d_name);
        }
        if (key == 'v') {
            if (regim == EMPTY_BUF) {
                continue;
            }
            int err = copy_file(current_dir, path_buffer); // 1 - the same file, 0 - ok, 

            if (err == 0 && regim == CUT) {
                char *f_path = get_full_path(path_buffer, filename_buffer);
                remove(f_path);
                free(f_path);
            }
            regim = EMPTY_BUF;
        }
        if (key == 'h') {
            show_hidden ^= 1;
        }

        refresh();
    }

    return 0;
}