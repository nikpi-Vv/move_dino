#ifndef OBJECTS_H
#define OBJECTS_H

#include "game.h"

// Базовые объекты
void paintCell(struct GameState *game, char color);
int digHole(struct GameState *game, const char *direction);
int makeMound(struct GameState *game, const char *direction);

// Деревья (&)
int growTree(struct GameState *game, const char *direction);
int chopTree(struct GameState *game, const char *direction);

// Камни (@)
int makeStone(struct GameState *game, const char *direction);
int pushStone(struct GameState *game, const char *direction);

#endif