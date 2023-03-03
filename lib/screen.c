#include "screen.h"

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

