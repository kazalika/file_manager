#pragma once

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
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

enum {
    D_CURSOR = 0,
    EMPTY_BUF = 0,
    COPY = 1,
    CUT = 2,
    H_SHOW = 1,
    H_OFF = 0
};

enum {
    COLOR_GREY = 123,
    COLOR_PINK = 124,

    FILE_COLOR = 1,
    DIR_COLOR = 2,
    FIFO_COLOR = 3,
    LNK_COLOR = 4,
    NEUTRAL_COLOR = 5
};

void init_colors();
void set_out_format(int w);
void init(const char* start_path);


extern int show_hidden;
extern int cursor;
extern int up_string;
extern int down_string;
extern int high;
extern int weight;
extern int key;
extern int size_of_dir;
extern struct dirent **list;
extern char current_dir[1000];

extern int regim;
extern char path_buffer[1000];
extern char filename_buffer[1000];
extern char ostr_fmt_file[1000];
extern const char *ostr_fmt_dir;

extern const char *cstr;
extern const char *ncstr;

extern const char* del_warning_msg1;
extern const char* del_warning_msg2;