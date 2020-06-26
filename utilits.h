#ifndef _UTILITS_H
#define _UTILITS_H

#include "declarations.h"
#include "moving.h"
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

// вывод сообщения по коду ошибки. всега возвращает 0
int err(int type);

// перевод символов карты из типа ввода в тип вывода
int type_char_to_print(char *c);

// перевод всех символов карты из типа ввода в тип вывода
int map_characters_to_print(s_map *map);

// перевод строки цвета в чиловое значение цвета
int str2color(char *str);

// перевод строки цвета в чиловое значение цвета, строка берётся из файла
int str2color_from_file(unsigned short *parametr, FILE **fin);

// реверс числа
int reverse_int(int n);

// перевод числа в строку
void int2str(int n, char str[], int lenght);

// перевод стрелочек в направление
int special_bottom(char *bottom);

// копирование map (карты одного размера!)
int copy_map(s_map *map1, s_map *map2);

// сохраниться на чекпоинте
int save_on_checkpoint(s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone);

// перейти по сохранению и уменьшить player->lifes на 1
int go_to_checkpoint(s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone);

// выполнить команду по нажатой клавише
void command(char bottom, s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone);

// создание s_cell - матрицы размерами m на n
int create_s_cell_matrix(s_cell ***matrix, int m, int n);

// положение экрана относительно игрока
void screen_position(COORD *screen_pos, s_player *player, s_map *map);

// освободить память матрицы с типом s_cell. передать количество строк
int free_s_cell_matrix(s_cell ***matrix, int size_of_strings);

// очистить всю память, что занимали
int free_all(s_map *map, s_map *save_map, s_q_stone *q_stone, s_q_stone *save_q_stone);

#endif //_UTILITS_H