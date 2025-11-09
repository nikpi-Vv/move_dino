#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include "utils.h"

// Функция для полной очистки строки (включая BOM)
void cleanString(char *line) {
    int len = strlen(line);
    
    // Убираем символы новой строки
    if (len > 0 && line[len-1] == '\n') {
        line[len-1] = 0;
        len--;
    }
    if (len > 0 && line[len-1] == '\r') {
        line[len-1] = 0;
        len--;
    }
    
    // Убираем комментарии (все что после //)
    char *comment = strstr(line, "//");
    if (comment != NULL) {
        *comment = 0;
        len = strlen(line);
    }
    
    // Убираем пробелы в начале и конце
    char *start = line;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    
    int new_len = strlen(start);
    while (new_len > 0 && isspace((unsigned char)start[new_len-1])) {
        start[new_len-1] = 0;
        new_len--;
    }
    
    if (start != line) {
        memmove(line, start, new_len + 1);
    }
}

// Функция проверки пустой строки
int isEmptyLine(const char *line) {
    if (line == NULL) return 1;
    
    for (const char *p = line; *p; p++) {
        if (!isspace((unsigned char)*p)) {
            return 0;
        }
    }
    return 1;
}

// Очистка консоли
void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Задержка в миллисекундах
void sleepMs(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}