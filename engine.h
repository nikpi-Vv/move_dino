// engine.h
#ifndef ENGINE_H
#define ENGINE_H

#define MAX_RECURSION_DEPTH 10

#include "options.h"
#include "game.h" 

int run_engine(const char *input_filename, const char *output_filename, struct ProgramOptions *options, struct GameState *game);

#endif // ENGINE_H
