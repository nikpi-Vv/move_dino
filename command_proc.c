#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "command_proc.h"
#include "errors.h"
#include "objects.h"

int processSIZE(struct GameState *game, const char *line, int line_number, int *size_command_found) {
    if (*size_command_found) RETURN_ERROR(line_number, ERROR_SIZE_USED);
    if (line[0] == ' ' || line[0] == '\t') RETURN_ERROR(line_number, ERROR_LEADING_SPACES);
    
    int w, h;
    if (sscanf(line, "SIZE %d %d", &w, &h) != 2) {
        RETURN_ERROR(line_number, ERROR_SIZE_FORMAT);
    }
    
    if (w < MIN_WIDTH || h < MIN_HEIGHT || w > MAX_WIDTH || h > MAX_HEIGHT) {
        RETURN_ERROR(line_number, ERROR_DIMENSIONS);
    }
    
    game->width = w;
    game->height = h;
    *size_command_found = 1;
    return 1;
}

int processSTART(struct GameState *game, const char *line, int line_number, int size_command_found, int *start_command_found) {
    if (!size_command_found) RETURN_ERROR(line_number, ERROR_START_BEFORE_SIZE);
    if (*start_command_found) RETURN_ERROR(line_number, ERROR_START_USED);
    if (line[0] == ' ' || line[0] == '\t') RETURN_ERROR(line_number, ERROR_LEADING_SPACES);
    
    int x, y;
    if (sscanf(line, "START %d %d", &x, &y) != 2) {
        RETURN_ERROR(line_number, ERROR_START_FORMAT);
    }
    
    if (x < 0 || x >= game->width || y < 0 || y >= game->height) {
        printf("ERROR line %d: %s at (%d, %d)\n", line_number, ERROR_START_OUTSIDE, x, y);
        return 0;
    }
    
    if (placeDino(game, x, y)) {
        *start_command_found = 1;
        return 1;
    }
    return 0;
}

int processMOVE(struct GameState *game, const char *line, int line_number, int start_command_found) {
    if (!start_command_found) RETURN_ERROR(line_number, ERROR_NO_DINOSAUR);
    
    char direction[10];
    if (sscanf(line, "MOVE %s", direction) != 1) {
        RETURN_ERROR(line_number, ERROR_MOVE_FORMAT);
    }
    
    moveDino(game, direction);
    return 1;
}

int processPAINT(struct GameState *game, const char *line, int line_number, int start_command_found) {
    if (!start_command_found) RETURN_ERROR(line_number, ERROR_NO_DINOSAUR);
    
    char color;
    if (sscanf(line, "PAINT %c", &color) != 1) {
        RETURN_ERROR(line_number, ERROR_PAINT_FORMAT);
    }
    
    if (color < 'a' || color > 'z') {
        RETURN_ERROR(line_number, ERROR_COLOR);
    }
    
    paintCell(game, color);
    return 1;
}

int processDIG(struct GameState *game, const char *line, int line_number, int start_command_found) {
    if (!start_command_found) RETURN_ERROR(line_number, ERROR_NO_DINOSAUR);
    
    char direction[10];
    if (sscanf(line, "DIG %s", direction) != 1) {
        RETURN_ERROR(line_number, ERROR_DIG_FORMAT);
    }
    
    if (!digHole(game, direction)) {
        RETURN_ERROR(line_number, ERROR_DIRECTION);
    }
    return 1;
}

int processMOUND(struct GameState *game, const char *line, int line_number, int start_command_found) {
    if (!start_command_found) RETURN_ERROR(line_number, ERROR_NO_DINOSAUR);
    
    char direction[10];
    if (sscanf(line, "MOUND %s", direction) != 1) {
        RETURN_ERROR(line_number, ERROR_MOUND_FORMAT);
    }
    
    if (!makeMound(game, direction)) {
        RETURN_ERROR(line_number, ERROR_DIRECTION);
    }
    return 1;
}

int processJUMP(struct GameState *game, const char *line, int line_number, int start_command_found) {
    if (!start_command_found) RETURN_ERROR(line_number, ERROR_NO_DINOSAUR);
    
    char direction[10];
    int distance;
    if (sscanf(line, "JUMP %s %d", direction, &distance) != 2) {
        RETURN_ERROR(line_number, ERROR_JUMP_FORMAT);
    }
    
    if (distance <= 0) {
        RETURN_ERROR(line_number, ERROR_JUMP_DISTANCE);
    }
    
    if (!jumpDino(game, direction, distance)) {
        RETURN_ERROR(line_number, ERROR_DIRECTION);
    }
    return 1;
}

int processGROW(struct GameState *game, const char *line, int line_number, int start_command_found) {
    if (!start_command_found) RETURN_ERROR(line_number, ERROR_NO_DINOSAUR);
    
    char direction[10];
    if (sscanf(line, "GROW %s", direction) != 1) {
        RETURN_ERROR(line_number, ERROR_GROW_FORMAT);
    }
    
    if (!growTree(game, direction)) {
        RETURN_ERROR(line_number, ERROR_DIRECTION);
    }
    return 1;
}

int processCHOP(struct GameState *game, const char *line, int line_number, int start_command_found) {
    if (!start_command_found) RETURN_ERROR(line_number, ERROR_NO_DINOSAUR);
    
    char direction[10];
    if (sscanf(line, "CHOP %s", direction) != 1) {
        RETURN_ERROR(line_number, ERROR_CHOP_FORMAT);
    }
    
    if (!chopTree(game, direction)) {
        RETURN_ERROR(line_number, ERROR_DIRECTION);
    }
    return 1;
}

int processMAKE(struct GameState *game, const char *line, int line_number, int start_command_found) {
    if (!start_command_found) RETURN_ERROR(line_number, ERROR_NO_DINOSAUR);
    
    char direction[10];
    if (sscanf(line, "MAKE %s", direction) != 1) {
        RETURN_ERROR(line_number, ERROR_MAKE_FORMAT);
    }
    
    if (!makeStone(game, direction)) {
        RETURN_ERROR(line_number, ERROR_DIRECTION);
    }
    return 1;
}

int processPUSH(struct GameState *game, const char *line, int line_number, int start_command_found) {
    if (!start_command_found) RETURN_ERROR(line_number, ERROR_NO_DINOSAUR);
    
    char direction[10];
    if (sscanf(line, "PUSH %s", direction) != 1) {
        RETURN_ERROR(line_number, ERROR_PUSH_FORMAT);
    }
    
    if (!pushStone(game, direction)) {
        RETURN_ERROR(line_number, ERROR_DIRECTION);
    }
    return 1;
}