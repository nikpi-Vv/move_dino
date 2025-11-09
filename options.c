#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "options.h"

void parseOptions(int argc, char *argv[], struct ProgramOptions *options) {
    // Установим значения по умолчанию
    options->display_enabled = 1;
    options->save_enabled = 1;
    options->interval = 1.0;  // 1 секунда по умолчанию

    // Пропускаем первые 3 аргумента (program_name, input.txt, output.txt)
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "no-display") == 0) {
            options->display_enabled = 0;
        }
        else if (strcmp(argv[i], "no-save") == 0) {
            options->save_enabled = 0;
        }
        else if (strcmp(argv[i], "interval") == 0) {
            if (i + 1 < argc) {
                options->interval = atof(argv[i + 1]);
                i++;  // Пропускаем следующий аргумент (число)
            }
        }
    }
}