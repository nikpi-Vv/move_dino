#include <stdio.h>
#include <stdlib.h>
#include "game.h"

// Инициализация поля
void initField(struct GameState *game) {
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            game->field[y][x].type = '_';
            game->field[y][x].color = '_';
        }
    }
}

// Вывод поля
void printField(const struct GameState *game) {
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            // Динозавр всегда '#'
            if (game->field[y][x].type == '#') {
                printf("#");
            }
            // клетки (ямы, горы, деревья, камни) показываются своим типом
            else if (game->field[y][x].type != '_' ) {
                printf("%c", game->field[y][x].type);
            }
            // Пустая клетка с цветом показывается цветом
            else {
                printf("%c", game->field[y][x].color);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Сохранение поля
void saveField(const struct GameState *game, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("ERROR: Cannot create file %s\n", filename);
        return;
    }
    
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            // Динозавр всегда '#'
            if (game->field[y][x].type == '#') {
                fprintf(file, "#"); 
            }
            // клетки (ямы, горы, деревья, камни) показываются своим типом
            else if (game->field[y][x].type != '_' ) {
                fprintf(file, "%c", game->field[y][x].type); 
            }
            // Пустая клетка с цветом показывается цветом
            else {
                fprintf(file, "%c", game->field[y][x].color);  
            }
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
}