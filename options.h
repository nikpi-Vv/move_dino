#ifndef OPTIONS_H
#define OPTIONS_H

struct ProgramOptions {
    int display_enabled;    // 1 = показывать поле, 0 = нет
    int save_enabled;       // 1 = сохранять в файл, 0 = нет  
    double interval;        // задержка между обновлениями в секундах
};

// Функция для разбора аргументов командной строки
void parseOptions(int argc, char *argv[], struct ProgramOptions *options);

#endif