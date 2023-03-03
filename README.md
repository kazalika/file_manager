# file_manager
File manager in terminal on C with ncurses


## How to build
```
mkdir build && cd build
cmake ..
cmake --build .
```

## How to add executable types of files

В корне репозитория есть файл extensions.txt

В нем в следующем формате записаны настройки открытия файлов

```
<extension>- --<program>   | |
```

Причем важно, что \<extension\> должен умещаться в отведенную рамку, ровно как и \<program\>. Также важно количество символов в каждой строке (30 не считая '\n'), и что перевод строки есть в конце последней строки файла.


### Default extensions.txt:

```
.txt       - --vim         | |
.c         - --code        | |
.cpp       - --code        | |

```

.txt открывается vim-ом, .c и .cpp файлы открываются в vscode. 

Менеджер ждет, пока вы закончите работу в файле и после этого снова прорисовывает окно.

## Start
```
./file_manager
```
## Help

```
-> - your cursor

P - help page  < not implemented yet
Q - quit file manager
Enter - open selected folder or execute selected file 
                                 ^ not implemeted yet
C - copy seletected file
X - cut selected file
V - past file you copied or cut
D - delete selected file or folder
H - on/off showing hidden files
```