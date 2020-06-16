#ifndef _PREPARATION_H
#define _PREPARATION_H

#define _CRT_SECURE_NO_WARNINGS
#include "declarartions.h"
#include "is_something.h"
#include "utilits.h"
#include <stdio.h>

// создание карты из текстового файла
int create_map_characters(s_txt_name txt_name, s_map *map);

// перенести игрока на карту - матрицу
int set_player_on_matr(s_map *map, s_all_colors all_colors, s_player *player);

// создание карты-матрицы
int create_map_matr(s_map *map, s_all_colors all_colors, s_player *player /*,s_empty *First*/);

// создание соответствия типу клетки с цветом из текстового файла
int create_all_colors(s_txt_name txt_name, s_all_colors *all_colors);

// создание массива цветов карты
int create_map_colors(s_map *map, s_all_colors all_colors);

// задание начальных параметров игрока
int create_player( s_txt_name txt_name, s_player* player);

// записать в txt_name имена текстовых файлов, используемых на данном уровне
int get_txt_name(int level, s_txt_name *txt_name);

// выбор (?) уровня
int get_level(int *level);

// подготовка - взятие информации из файлов, создание карты
int preparation(int *level, s_map *map, s_all_colors *all_colors, s_player *player, s_map *save_map/* ,s_enemy **first_enemy*/);

#endif //_PREPARATION_H