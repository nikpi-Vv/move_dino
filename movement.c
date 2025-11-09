#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "errors.h"

// Вспомогательная функция для вычисления новых координат
void calculateNewPosition(int old_x, int old_y, int width, int height, 
                         const char *direction, int distance, 
                         int *new_x, int *new_y) {
    *new_x = old_x;
    *new_y = old_y;
    
    if (strcmp(direction, "UP") == 0) {
        *new_y = (old_y - distance + height) % height;
    } else if (strcmp(direction, "DOWN") == 0) {
        *new_y = (old_y + distance) % height;
    } else if (strcmp(direction, "LEFT") == 0) {
        *new_x = (old_x - distance + width) % width;
    } else if (strcmp(direction, "RIGHT") == 0) {
        *new_x = (old_x + distance) % width;
    }
}

// Проверка валидности направления
int isValidDirection(const char *direction) {
    return (strcmp(direction, "UP") == 0 ||
            strcmp(direction, "DOWN") == 0 ||
            strcmp(direction, "LEFT") == 0 ||
            strcmp(direction, "RIGHT") == 0);
}

// Движение динозавра
void moveDino(struct GameState *game, const char *direction) {
    if (!isValidDirection(direction)) {
        PRINT_ERROR_DETAILS(ERROR_DIRECTION, direction);
        return;
    }

    int old_x = game->dino_x;
    int old_y = game->dino_y;
    int new_x, new_y;
    
    calculateNewPosition(old_x, old_y, game->width, game->height, 
                        direction, 1, &new_x, &new_y);
    
    // Проверяем можно ли двигаться
    char target_type = game->field[new_y][new_x].type;
    
    if (target_type == '%') {
        printf("ERROR: Cannot move to hole! Game over.\n");
        exit(1);
    }
    
    if (target_type == '^') {
        printf("WARNING: Cannot move to mountain. Command ignored.\n");
        return;
    }
    
    // Двигаемся
    game->field[old_y][old_x].type = '_';
    game->dino_x = new_x;
    game->dino_y = new_y;
    game->field[new_y][new_x].type = '#';
}

// Размещение динозавра
int placeDino(struct GameState *game, int x, int y) {
    if (x < 0 || x >= game->width || y < 0 || y >= game->height) {
        printf("ERROR: Coordinates (%d, %d) outside field\n", x, y);
        return 0;
    }
    
    if (game->field[y][x].type != '_') {
        printf("ERROR: Cell (%d, %d) already occupied\n", x, y);
        return 0;
    }
    
    game->dino_x = x;
    game->dino_y = y;
    game->field[y][x].type = '#';
    return 1;
}

int isDinoPlaced(const struct GameState *game) {
    return (game->dino_x >= 0 && game->dino_y >= 0);
}

// Прыжок динозавра
int jumpDino(struct GameState *game, const char *direction, int distance) {
    if (!isValidDirection(direction)) {
        PRINT_ERROR_DETAILS(ERROR_DIRECTION, direction);
        return 0;
    }

    if (distance <= 0) {
        printf("ERROR: Jump distance must be positive\n");
        return 0;
    }

    int old_x = game->dino_x;
    int old_y = game->dino_y;
    int new_x, new_y;

    // Вычисляем конечную позицию прыжка
    calculateNewPosition(old_x, old_y, game->width, game->height,
                        direction, distance, &new_x, &new_y);

    // Проверяем путь прыжка на наличие гор
    int check_x, check_y;

    for(int step = 1; step <= distance; step++) {
        calculateNewPosition(old_x, old_y, game->width, game->height,
                            direction, step, &check_x, &check_y);
        if (game->field[check_y][check_x].type == '^') {
            printf("WARNING: Mountain in jump path at (%d, %d). Stopping before.\n", check_x, check_y);
            calculateNewPosition(old_x, old_y, game->width, game->height,
                                direction, step - 1, &new_x, &new_y);
            break;
        }
    }

    // Проверяем конечную позицию приземления
    if (game->field[new_y][new_x].type == '%') {
        printf("ERROR: Landed in hole at (%d, %d)! Game over.\n", new_x, new_y);
        exit(1);
    }

    if (game->field[new_y][new_x].type == '^') {
        printf("WARNING: Cannot land on mountain at (%d, %d)\n", new_x, new_y);
        return 0;
    }

    // Выполняем прыжок
    game->field[old_y][old_x].type = '_';
    game->dino_x = new_x;
    game->dino_y = new_y;
    game->field[new_y][new_x].type = '#';
    
    printf("Jumped from (%d, %d) to (%d, %d)\n", old_x, old_y, new_x, new_y);
    return 1;
}