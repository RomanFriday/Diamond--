#ifndef _PRINT_H
#define _PRINT_H

#define _CRT_SECURE_NO_WARNINGS
#include "declarartions.h"
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

// великая победа
int great_victory(s_map *map, s_player *player);

// завершение игры
int game_over();

#endif // _PRINT_H