

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 256/*самая длинная команда в MovDino — это, например:
IF CELL 99 99 IS a THEN PAINT b → ~30 символов,
даже с запасом на координаты и имена файлов — 256 байт более чем достаточно.*/
#define MIN_SIZE 10  //Минимальный размер поля  1010 клеток.
#define MAX_SIZE 100 //Максимальный размер поля  100100 клеток

//На этом этапе реализуйте только Часть 1 
//(SIZE, START, MOVE + тороидальная логика). 
//чтение команд,

//хранение поля (char **grid),

//позиция динозавра,

//обработка SIZE, START, MOVE,

//вывод поля.



//Добавьте команды из Части 2 (PAINT, DIG, MOUND)
//Пока всё ещё в одном файле — просто добавьте if/else 
//или switch для новых команд.

//Этап 3: Как только код превысит ~300 строк — начинайте разделять
/*main.c — только main() и обработка аргументов,
field.c — работа с полем,
commands.c — выполнение команд.
Остальное (UNDO, EXEC, IF и т.д.) — добавляйте по мере необходимости.*/

/*movdino/
├── movdino.c      // основной файл: всё, кроме main
├── main.c         // только main() и аргументы
└── Makefile*/

/*void init_field(int w, int h);
void set_dino(int x, int y);
void move_dino(char *dir);
void paint_cell(char color);
void print_field(void);
int execute_command(char *line);
// ...*/

