#include "init.h"

int show_hidden = H_OFF;
int cursor = D_CURSOR;
int up_string = D_CURSOR;
int down_string = D_CURSOR;
int high;
int weight;
int key = 0;
int size_of_dir = 0;
struct dirent **list = NULL;
char current_dir[1000] = "./";

int regim = EMPTY_BUF;
char path_buffer[1000];
char filename_buffer[1000];
char ostr_fmt_file[1000];
const char *ostr_fmt_dir = "%-40s\n";

const char *cstr = "-> ";
const char *ncstr = "   ";

const char* del_warning_msg1 = "Are you sure you want to delete this file?";
const char* del_warning_msg2 = "Press 'y' if you sure. Otherwise press any another key";

void set_out_format(int w) {
    // "%-13s...%10ld\n"
    strcpy(ostr_fmt_file, "%-40s");
    for (size_t i = 5; i < w - (50); ++i) {
        ostr_fmt_file[i] = ' ';
    }
    strcpy(ostr_fmt_file + w - (50), "%10ld\n\0");
}

void init_colors() {
    attron(A_BOLD);
    init_color(COLOR_GREY, 224, 224, 224);
    init_color(COLOR_PINK, 255, 102, 102);
    init_pair(FILE_COLOR, COLOR_GREY, COLOR_BLACK);
    init_pair(DIR_COLOR, COLOR_PINK, COLOR_BLACK);
    init_pair(NEUTRAL_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(FIFO_COLOR, COLOR_GREEN, COLOR_BLACK);
    init_pair(LNK_COLOR, COLOR_BLUE, COLOR_BLACK);
    attron(COLOR_PAIR(NEUTRAL_COLOR));
}

void init(const char *start_path) {
    realpath(start_path, current_dir);
    while (current_dir[strlen(current_dir) - 1] != '/') {
        current_dir[strlen(current_dir) - 1] = '\0';
    }

    initscr();
    getmaxyx(stdscr, high, weight);
    set_out_format(weight);
    down_string = up_string + high - 1;

    keypad(stdscr, TRUE); 
    if (!has_colors()) {
        endwin();
        printf("Bruh there is no color, so I can't work sorry\n");
        exit(0);
    }

    start_color();
    init_colors();
}