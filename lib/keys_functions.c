#include "keys_functions.h"
#include "init.h"
#include <curses.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

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


int find_prog_by_extension(const char *ext, char* progname) {
    int fd = open("../extensions.txt", O_RDONLY);
    if (fd < 0) {
        return 1123;
    }
    int n = 0;
    char buffer[100];
    while ((n = read(fd, buffer, 31)) == 31) {
        char *lst = buffer;
        while (*lst != ' ') {
            ++lst;
        }
        *lst = '\0';
        // warning_screen(buffer);
        if (strcmp(ext, buffer) == 0) {
            while (*lst != '-') {
                ++lst;
            }
            lst += 4;
            char *prog_start = lst;
            while (*lst != ' ') {
                ++lst;
            }
            *lst = '\0';
            strcpy(progname, prog_start);
            close(fd);
            return 0;
        }
    }
    close(fd);
    return -1;
}

int execute(const char* progname, const char* filename) {
    pid_t pid;
    int status;

    // extra_exit(filename);

    pid = fork();
    if (pid == -1) {
        extra_exit("BRUH -1");
        return -1;
    }
    if (pid == 0) {
        execlp(progname, progname, filename, NULL);
        // extra_exit("BRUH 1");
        return 1;
    } else {
        waitpid(pid, &status, 0);
    }
    return 0;
}

void key_ENTER() {
    struct dirent *x = list[cursor];
    if (x->d_type != DT_DIR) {

        const char *ext = get_extension(x->d_name);
        char *prog = calloc(100, sizeof(*prog));
        int err = find_prog_by_extension(ext, prog);

        if (err == 1123) {
            free(prog);
            warning_screen("extensions.txt is lost((");
            return;   
        }
        if (err != 0) {
            free(prog);
            warning_screen("I can't open file with this extension");
            return;
        }

        char *f_name = get_full_path(current_dir, x->d_name);
        realpath(f_name, f_name);

        endwin();
        err = execute(prog, f_name);
        initscr();

        if (err != 0) {
            free(prog);
            free(f_name);
            // extra_exit(f_name);
            warning_screen("I couldn't execute the file");
            return;
        }

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
    refresh();
}

void key_P() {
    int row_start = high / 2 - 5;

    my_clear();

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