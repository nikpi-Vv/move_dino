#ifndef UTILS_H
#define UTILS_H

// Функции для работы со строками
void cleanString(char *line);
int isEmptyLine(const char *line);

// Функции для работы с консолью
void clearConsole();
void sleepMs(int milliseconds);

#endif