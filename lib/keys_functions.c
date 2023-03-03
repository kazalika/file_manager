#include "keys_functions.h"
#include "init.h"
#include <curses.h>

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

void extra_exit(const char *str) {
    endwin();
    // printf("%s\n", current_dir);
    printf("%s\n", str);
    free(list);
    exit(0);
}

void key_UP() {
    cursor = (cursor - 1 + size_of_dir) % size_of_dir;
}

void key_DOWN() {
    cursor = (cursor + 1) % size_of_dir;
}

void key_ENTER() {
    struct dirent *x = list[cursor];
    if (x->d_type != DT_DIR) {
        warning_screen("It's not a directory");
        return;
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

void key_Q() {
    extra_exit("You quit\n");
}

void key_D() {
    int sure = delete_warning_screen();
    if (!sure) {
        return;
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

void key_C() {
    struct dirent *x = list[cursor];
    regim = COPY;

    strcpy(path_buffer, current_dir);
    strcpy(filename_buffer, x->d_name);
}

void key_X() {
    struct dirent *x = list[cursor];
    regim = CUT;

    strcpy(path_buffer, current_dir);
    strcpy(filename_buffer, x->d_name);
}

void key_V() {
    if (regim == EMPTY_BUF) {
        return;
    }
    int err = copy_file(current_dir, path_buffer); // 1 - the same file, 0 - ok, 

    if (err == 0 && regim == CUT) {
        char *f_path = get_full_path(path_buffer, filename_buffer);
        remove(f_path);
        free(f_path);
    }
    regim = EMPTY_BUF;
}

void key_H() {
    show_hidden ^= 1;
    cursor = D_CURSOR;
}

void middle_write(int row, const char *msg) {
    mvwprintw(stdscr, row, weight / 2 - 20, "%s", msg);
}

void my_clear() {
    move(0, 0);
    for (int i = 0; i < high; ++i) {
        printw("\n");
    }
}

void key_P() {
    int row_start = high / 2 - 5;

    my_clear();

    refresh();

    middle_write(row_start, "-> - your cursor");
    middle_write(row_start + 2, "P - help page  (you are here now)");
    middle_write(row_start + 3, "Q - quit file manager");
    middle_write(row_start + 4, "Enter - open selected folder or execute selected file");
    middle_write(row_start + 6, "C - copy seletected file");
    middle_write(row_start + 7, "X - cut selected file");
    middle_write(row_start + 8, "V - past file you copied or cut");
    middle_write(row_start + 9, "D - delete selected file or folder");
    middle_write(row_start + 10, "H - on/off showing hidden files");

    middle_write(row_start + 12, "Press any key to quit help page");

    refresh();

    int key = getch();
}