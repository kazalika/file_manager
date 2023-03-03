#include "lib/deleter.h"
#include "lib/warning_screens.h"
#include "lib/path_operations.h"
#include "lib/keys_functions.h"
#include "lib/screen.h"

int main(int argc, char *argv[]) {

    init(argv[0]);
    
    while (true) {
        update_ud();
        size_of_dir = scr_dir(current_dir);
        key = getch();
        
        switch (key) {
            case KEY_UP:
                key_UP();
                break;
            case KEY_DOWN:
                key_DOWN();
                break;
            case '\n':
                key_ENTER();
                break;
            case 'q':
                key_Q();
                break;
            case 'd':
                key_D();
                break;
            case 'c':
                key_C();
                break;
            case 'x':
                key_X();
                break;
            case 'v':
                key_V();
                break;
            case 'h':
                key_H();
                break;
            case 'p':
                key_P();
                break;
        }

        refresh();
    }

    return 0;
}