// engine.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "game.h"
#include "command_proc.h"
#include "errors.h"
#include "options.h"
#include "utils.h"

static int recursion_depth = 0;

int run_engine(const char *input_filename, const char *output_filename, 
                struct ProgramOptions *options, struct GameState *game){
    
    // Открываем файл
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file) {
        printf("ERROR: Cannot open file %s\n", input_filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int success = 1;

    // Если это первый уровень (главный запуск) — флаги сбрасываем
    static int recursion_level = 0;
    static int size_command_found = 0;
    static int start_command_found = 0;

    if (recursion_level == 0) {
        size_command_found = 0;
        start_command_found = 0;
    }

    recursion_level++;

    int line_number = 0;
    while (fgets(line, sizeof(line), input_file) && success) {
        line_number++;

        // Сохраняем оригинальную строку для проверки пробелов
        char original_line[MAX_LINE_LENGTH];
        strncpy(original_line, line, sizeof(original_line)-1);
        original_line[sizeof(original_line)-1] = '\0';

        // Проверяем левые пробелы в оригинальной строке ДО очистки
        if (original_line[0] == ' ' || original_line[0] == '\t') {
            printf("ERROR line %d: %s\n", line_number, ERROR_LEADING_SPACES);
            success = 0;
            break;
        }

        // Очищаем строку
        cleanString(line);

        // Пропускаем пустые строки
        if (isEmptyLine(line)) {
            continue;
        }

        // Защита от слишком длинной строки
        if ((int)strlen(line) >= MAX_LINE_LENGTH - 1) {
            printf("ERROR line %d: %s\n", line_number, ERROR_LINE_TOO_LONG);
            success = 0;
            break;
        }

        printf("Line %d: '%s'\n", line_number, line);

        // Обрабатываем команды
        if (strncmp(line, "SIZE", 4) == 0) {
            success = processSIZE(game, line, line_number, &size_command_found);
            if (success) {
                initField(game);
            }
        } else if (strncmp(line, "START", 5) == 0) {
            success = processSTART(game, line, line_number, size_command_found, &start_command_found);
        } else if (strncmp(line, "MOVE", 4) == 0) {
            success = processMOVE(game, line, line_number, start_command_found);
        } else if (strncmp(line, "PAINT", 5) == 0) {
            success = processPAINT(game, line, line_number, start_command_found);
        } else if (strncmp(line, "DIG", 3) == 0) {
            success = processDIG(game, line, line_number, start_command_found);
        } else if (strncmp(line, "MOUND", 5) == 0) {
            success = processMOUND(game, line, line_number, start_command_found);
        } else if (strncmp(line, "JUMP", 4) == 0) {
            success = processJUMP(game, line, line_number, start_command_found);
        } else if (strncmp(line, "GROW", 4) == 0) {
            success = processGROW(game, line, line_number, start_command_found);
        } else if (strncmp(line, "CHOP", 4) == 0) {
            success = processCHOP(game, line, line_number, start_command_found);
        } else if (strncmp(line, "MAKE", 4) == 0) {
            success = processMAKE(game, line, line_number, start_command_found);
        } else if (strncmp(line, "PUSH", 4) == 0) {
            success = processPUSH(game, line, line_number, start_command_found);
        } else if (strncmp(line, "LOAD", 4) == 0) {
            if (line_number != 1) {
                printf("ERROR line %d: LOAD must be the first command\n", line_number);
                success = 0;
                break;
            }

            char filename[100];
            if (sscanf(line, "LOAD %99s", filename) != 1) {
                printf("ERROR line %d: Invalid LOAD format\n", line_number);
                success = 0;
                break;
            }

            FILE *load_file = fopen(filename, "r");
            if (!load_file) {
                printf("ERROR line %d: Cannot open %s\n", line_number, filename);
                success = 0;
                break;
            }

            // Считываем поле построчно
            char buffer[MAX_LINE_LENGTH];
            int y = 0;
            while (fgets(buffer, sizeof(buffer), load_file) && y < MAX_HEIGHT) {
                int len = strlen(buffer);
                if (buffer[len - 1] == '\n') buffer[len - 1] = '\0';
                for (int x = 0; x < len && x < MAX_WIDTH; x++) {
                    char c = buffer[x];
                    game->field[y][x].type = (c == '_' || c == '#' || c == '%' ||
                                            c == '^' || c == '&' || c == '@') ? c : '_';
                    game->field[y][x].color = (c >= 'a' && c <= 'z') ? c : '_';
                    if (c == '#') {
                        game->dino_x = x;
                        game->dino_y = y;
                    }
                }
                y++;
            }
            fclose(load_file);
            game->height = y;
            game->width = strlen(buffer);
            printf("Field loaded from %s (%dx%d)\n", filename, game->width, game->height);
        } else if (strncmp(line, "EXEC", 4) == 0) {
            char filename[100];
            if (sscanf(line, "EXEC %99s", filename) != 1) {
                printf("ERROR line %d: Invalid EXEC format\n", line_number);
                success = 0;
                break;
            }

            if (recursion_depth >= MAX_RECURSION_DEPTH) {
                printf("ERROR line %d: Maximum recursion depth reached (limit %d)\n",
                    line_number, MAX_RECURSION_DEPTH);
                success = 0;
                break;
            }

            recursion_depth++;
            printf("Executing nested file: %s\n", filename);

            // рекурсивный вызов того же движка
            int result = run_engine(filename, output_filename, options, game);
            recursion_depth--;

            if (result != 0) {
                printf("ERROR line %d: EXEC failed on file %s\n", line_number, filename);
                success = 0;
                break;
            }
        } else if (strncmp(line, "IF CELL", 7) == 0) {
            int x, y;
            char symbol;
            char subcommand[MAX_LINE_LENGTH];
            if (sscanf(line, "IF CELL %d %d IS %c THEN %[^\n]", &x, &y, &symbol, subcommand) != 4) {
                printf("ERROR line %d: Invalid IF syntax\n", line_number);
                success = 0;
                break;
            }

            if (x < 0 || x >= game->width || y < 0 || y >= game->height) {
                printf("ERROR line %d: Coordinates out of range in IF\n", line_number);
                success = 0;
                break;
            }

            char current = (game->field[y][x].type != '_') ? game->field[y][x].type : game->field[y][x].color;

            if (current == symbol) {
                printf("Condition met at (%d, %d): '%c' == '%c' -> executing '%s'\n",
                    x, y, current, symbol, subcommand);

                // Создаем временный файл для подкоманды
                recursion_depth++;
                FILE *tmp = fopen("temp_subcommand.txt", "w");
                fprintf(tmp, "%s\n", subcommand);
                fclose(tmp);
                int result = run_engine("temp_subcommand.txt", output_filename, options, game);
                recursion_depth--;
                remove("temp_subcommand.txt");
                
                if (result != 0) {
                    success = 0;
                    break;
                }
            } else {
                printf("Condition not met at (%d, %d): found '%c', expected '%c'\n", x, y, current, symbol);
            }
        } else {
            printf("ERROR line %d: Unknown command '%s'\n", line_number, line);
            success = 0;
            break;
        }

        // Визуализация для выполненных команд
        if (success && options->display_enabled && !isEmptyLine(line)) {
            clearConsole();
            printf("=== Field State (Line %d: %s) ===\n", line_number, line);
            printField(game);
            sleepMs((int)(options->interval * 1000));
        }
    }

    fclose(input_file);

    // Завершаем работу движка: сохраняем поле, если успешно
    if (success) {
        if (options->save_enabled) {
            saveField(game, output_filename);
            printf("Field saved to: %s\n", output_filename);
        } else {
            printf("Save disabled (no-save option)\n");
        }
        return 0;
    } else {
        return 1;
    }
}