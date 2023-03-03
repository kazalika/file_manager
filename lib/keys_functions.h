#include <unistd.h>
#include <sys/wait.h>
#include "warning_screens.h"
#include "path_operations.h"
#include "deleter.h"
#include "screen.h"

void extra_exit(const char *str);
int copy_file(const char* to_dir, const char* src_dir);

void key_UP();
void key_DOWN();
void key_ENTER();
void key_Q();
void key_D();
void key_C();
void key_X();
void key_V();
void key_H();
void key_P();