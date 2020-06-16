#ifndef _UTILITS_H
#define _UTILITS_H

#include "declarartions.h"
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
int pointer2direction(char *bottom);

// выполнить команду по нажатой клавише
void command(char bottom, s_map *map, s_player *player, s_q_stone *q_stone);

// создание s_cell - матрицы размерами m на n
int create_s_cell_matrix(s_cell ***matrix, int m, int n);


// положение экрана относительно игрока
void screen_position(COORD *screen_pos, s_player *player, s_map *map);

#endif //_UTILITS_H