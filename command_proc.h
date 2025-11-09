#ifndef COMMAND_PROC_H
#define COMMAND_PROC_H

#include "game.h"
#include "objects.h"

// Обработчики команд
int processSIZE(struct GameState *game, const char *line, int line_number, int *size_command_found);
int processSTART(struct GameState *game, const char *line, int line_number, int size_command_found, int *start_command_found);
int processMOVE(struct GameState *game, const char *line, int line_number, int start_command_found);
//
int processPAINT(struct GameState *game, const char *line, int line_number, int start_command_found);
int processDIG(struct GameState *game, const char *line, int line_number, int start_command_found);
int processMOUND(struct GameState *game, const char *line, int line_number, int start_command_found);
//
int processJUMP(struct GameState *game, const char *line, int line_number, int start_command_found);
// 
int processGROW(struct GameState *game, const char *line, int line_number, int start_command_found);
int processCHOP(struct GameState *game, const char *line, int line_number, int start_command_found);
int processMAKE(struct GameState *game, const char *line, int line_number, int start_command_found);
int processPUSH(struct GameState *game, const char *line, int line_number, int start_command_found);

#endif