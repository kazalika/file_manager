#include "warning_screens.h"
#include "init.h"
#include <string.h>

void warning_screen(const char* msg) {
    clear();
    const char* paktc = "Press any key to continue";
    mvwprintw(stdscr, high / 2, (weight - strlen(msg)) / 2, "%s", msg);
    mvwprintw(stdscr, high / 2 + 1, (weight - strlen(paktc)) / 2, "%s", paktc);
    int key = getch();
}

int delete_warning_screen() {
    clear();
    mvwprintw(stdscr, high / 2, (weight - strlen(del_warning_msg1)) / 2, "%s", del_warning_msg1);
    mvwprintw(stdscr, high / 2 + 1, (weight - strlen(del_warning_msg2)) / 2, "%s", del_warning_msg2);
    int key = getch();
    if (key == 'y') {
        return 1; // delete
    }
    return 0;
}