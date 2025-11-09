// movdino.c — минимальный main
#include <stdio.h>
#include <stdlib.h>
#include "options.h"
#include "engine.h"
#include "game.h" 

int main(int argc, char *argv[]) {
    printf("=== MOVDINO PROGRAM ===\n");
    
    // Инициализируем игру
    struct GameState game;
    game.width = 0;
    game.height = 0;
    game.dino_x = -1;
    game.dino_y = -1;

    // Проверяем аргументы
    if (argc < 3) {
        printf("Usage: %s input.txt output.txt [options]\n", argv[0]);
        printf("Options:\n");
        printf("  interval N    - delay between updates in seconds\n");
        printf("  no-display    - disable field visualization\n");
        printf("  no-save       - disable saving to output file\n");
        return 1;
    }

    // Разбираем опции
    struct ProgramOptions options;
    parseOptions(argc, argv, &options);

    // Проверка минимального интервала
    if (options.interval < 0.1) {
        options.interval = 0.1;
    }

    printf("Input file: %s\n", argv[1]);
    printf("Output file: %s\n", argv[2]);
    printf("Display: %s\n", options.display_enabled ? "enabled" : "disabled");
    printf("Save: %s\n", options.save_enabled ? "enabled" : "disabled");
    printf("Interval: %.1f seconds\n", options.interval);

    // Запускаем движок (engine) — обработчик файла/команд
    int result = run_engine(argv[1], argv[2], &options, &game);

    if (result == 0) {
        printf("\nSUCCESS: All commands executed!\n");
    } else {
        printf("\nERROR: Program finished with error (code %d)\n", result);
    }

    return (result == 0) ? 0 : 1;
}
