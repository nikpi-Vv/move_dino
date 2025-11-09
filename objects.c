#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "objects.h"
#include "errors.h"


// Покраска клетки
void paintCell(struct GameState *game, char color) {
    if (color >= 'a' && color <= 'z') {
        int x = game->dino_x;
        int y = game->dino_y;
        game->field[y][x].color = color; // сохраняем всегда
        printf("Cell (%d, %d) painted '%c'\n", x, y, color);
    } else {
        printf("ERROR: Color must be between 'a' and 'z'\n");
    }
}

// Копать яму
int digHole(struct GameState *game, const char *direction) {
    if (!isValidDirection(direction)) {
        PRINT_ERROR_DETAILS(ERROR_DIRECTION, direction);
        return 0;
    }

    int target_x, target_y;
    calculateNewPosition(game->dino_x, game->dino_y, game->width, game->height,
                        direction, 1, &target_x, &target_y);
    
    // Можно копать только в пустую клетку
    if (game->field[target_y][target_x].type != '_') {
        printf("ERROR: Can only dig in empty cells\n");
        return 0;
    }
    
    game->field[target_y][target_x].type = '%';
    printf("Hole created at (%d, %d)\n", target_x, target_y);
    return 1;
}

// Создать гору
int makeMound(struct GameState *game, const char *direction) {
    if (!isValidDirection(direction)) {
        PRINT_ERROR_DETAILS(ERROR_DIRECTION, direction);
        return 0;
    }

    int target_x, target_y;
    calculateNewPosition(game->dino_x, game->dino_y, game->width, game->height,
                        direction, 1, &target_x, &target_y);
    
    // Если яма - засыпаем
    if (game->field[target_y][target_x].type == '%') {
            game->field[target_y][target_x].type = '_';
        printf("Hole filled at (%d, %d)\n", target_x, target_y);
        return 1;
    }
    
    // Если пусто - создаем гору
    if (game->field[target_y][target_x].type == '_') {
        game->field[target_y][target_x].type = '^';
        printf("Mountain created at (%d, %d)\n", target_x, target_y);
        return 1;
    }
    
    printf("ERROR: Cannot build mountain here\n");
    return 0;
}

// Деревья

// Вырастить дерево в соседней клетке
int growTree(struct GameState *game, const char *direction) {
    if (!isValidDirection(direction)) {
        PRINT_ERROR_DETAILS(ERROR_DIRECTION, direction);
        return 0;
    }

    int target_x, target_y;
    calculateNewPosition(game->dino_x, game->dino_y, game->width, game->height,
                        direction, 1, &target_x, &target_y);
    
    // Можно выращивать только в пустые клетки
    if (game->field[target_y][target_x].type != '_') {
        printf("ERROR: Can only grow trees in empty cells\n");
        return 0;
    } else {
        game->field[target_y][target_x].type = '&';
        printf("Tree grown at (%d, %d)\n", target_x, target_y);
        return 1;
    }   
}

// Срубить дерево в соседней клетке
int chopTree(struct GameState *game, const char *direction) {
    if (!isValidDirection(direction)) {
        PRINT_ERROR_DETAILS(ERROR_DIRECTION, direction);
        return 0;
    }

    int target_x, target_y;
    calculateNewPosition(game->dino_x, game->dino_y, game->width, game->height,
                        direction, 1, &target_x, &target_y);
    
    // Можно рубить только деревья
    if (game->field[target_y][target_x].type != '&') {
        printf("ERROR: No tree to chop at (%d, %d)\n", target_x, target_y);
        return 0;
    }
    
    game->field[target_y][target_x].type = '_';
    printf("Tree chopped at (%d, %d)\n", target_x, target_y);
    return 1;
}

// Камни

// Создать камень в соседней клетке
int makeStone(struct GameState *game, const char *direction) {
    if (!isValidDirection(direction)) {
        PRINT_ERROR_DETAILS(ERROR_DIRECTION, direction);
        return 0;
    }

    int target_x, target_y;
    calculateNewPosition(game->dino_x, game->dino_y, game->width, game->height,
                        direction, 1, &target_x, &target_y);
    
    // Можно создавать только в пустые клетки
    if (game->field[target_y][target_x].type != '_') {
        printf("ERROR: Can only make stones in empty cells\n");
        return 0;
    }
    
    game->field[target_y][target_x].type = '@';
    printf("Stone created at (%d, %d)\n", target_x, target_y);
    return 1;
}

// Толкнуть камень в соседней клетке
int pushStone(struct GameState *game, const char *direction) {
    if (!isValidDirection(direction)) {
        PRINT_ERROR_DETAILS(ERROR_DIRECTION, direction);
        return 0;
    }

    int stone_x, stone_y;
    calculateNewPosition(game->dino_x, game->dino_y, game->width, game->height,
                        direction, 1, &stone_x, &stone_y);
    
    // Проверяем что в соседней клетке есть камень
    if (game->field[stone_y][stone_x].type != '@') {
        printf("ERROR: No stone to push at (%d, %d)\n", stone_x, stone_y);
        return 0;
    }
    
    // Вычисляем куда толкаем камень (в том же направлении)
    int new_x, new_y;
    calculateNewPosition(stone_x, stone_y, game->width, game->height,
                        direction, 1, &new_x, &new_y);
    
    // Проверяем можно ли толкнуть камень
    char target_type = game->field[new_y][new_x].type;
    
    // Нельзя толкать если на пути дерево или гора
    if (target_type == '&' || target_type == '^') {
        printf("ERROR: Cannot push stone - blocked by %c at (%d, %d)\n", target_type, new_x, new_y);
        return 0;
    }
    
    // Если толкаем в яму - яма засыпается
    if (target_type == '%') {
        printf("Stone filled hole at (%d, %d)\n", new_x, new_y);
        // Сохраняем цвет ямы если он есть
        char saved_color = game->field[new_y][new_x].color;
        game->field[new_y][new_x].type = '_';
        game->field[new_y][new_x].color = saved_color;
    }
    // Если толкаем в пустую клетку - просто перемещаем
    else if (target_type == '_') {
        // Просто перемещаем камень
        game->field[new_y][new_x].type = '@';
    }
    else {
        printf("ERROR: Cannot push stone to cell with '%c'\n", target_type);
        return 0;
    }
    
    // Освобождаем старую клетку
    game->field[stone_y][stone_x].type = '_';
    
    printf("Stone pushed from (%d, %d) to (%d, %d)\n", stone_x, stone_y, new_x, new_y);
    return 1;
}