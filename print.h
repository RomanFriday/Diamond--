#ifndef _PRINT_H
#define _PRINT_H

#define _CRT_SECURE_NO_WARNINGS
#include "declarations.h"
#include "is_something.h"
#include "utilits.h"
#include <stdio.h>

// нарисовать строку, состоящую только из символов c. Если is_new_line!=0, печатает перевод на новую строку.
void print_line(char c, int size, int is_new_line);

// нарисовать игрока на отображаемой части карты
int print_player(COORD *screen_pos, s_player *pl, int i, int j);

// нарисовать клетку карты (трава, стена или камень)
void print_cell(s_map *map, int i, int j);

// рисование части карты.
// На вход подаются карта и координаты начала отображения карты
void print_map(s_map *map, COORD *screen_pos, s_player *player/*, s_enemies First*/);

// великая победа - вывод на экран
int print_great_victory();

// завершение игры
int game_over();

// вывести команду меню
void print_menu_command(commands cur);

// вывести на экран команды меню.
// размеры не передаются, т.к. указаны в declarations.h как константы.
// cur - номер текущей команды
void print_choose_menu_commands(commands menu_commands[], commands cur);

#endif // _PRINT_H