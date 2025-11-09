#ifndef GAME_H
#define GAME_H

#define MAX_WIDTH 100
#define MAX_HEIGHT 100
#define MIN_WIDTH 10
#define MIN_HEIGHT 10
#define MAX_LINE_LENGTH 100

// Структура для клетки
struct Cell {
    char type;    // '_'=пусто, '#'=динозавр, '%'=яма, '^'=гора, '&'=дерево, '@'=камень
    char color;   // '_'=нет цвета, 'a'-'z'=цвет
};

struct GameState {
    struct Cell field[MAX_HEIGHT][MAX_WIDTH];
    int width;
    int height;
    int dino_x;
    int dino_y;
};

void calculateNewPosition(int old_x, int old_y, int width, int height, 
                         const char *direction, int distance, 
                         int *new_x, int *new_y);
int isValidDirection(const char *direction);

// Функции для поля
void initField(struct GameState *game);
void printField(const struct GameState *game);
void saveField(const struct GameState *game, const char *filename);

// Функции движения
void moveDino(struct GameState *game, const char *direction);
int placeDino(struct GameState *game, int x, int y);
int isDinoPlaced(const struct GameState *game);
int jumpDino(struct GameState *game, const char *direction, int distance);

#endif