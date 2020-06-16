#ifndef _STONES_AND_DIAMONDS_H
#define _STONES_AND_DIAMONDS_H

#include "declarartions.h"
#include "is_something.h"
#include "moving.h"
#include "utilits.h"
#include <stdio.h>
#include <malloc.h>

// создание элемента очереди камней
s_stone* create_stone(s_stone *info);

// добавление в конец
int add_stone_in_end(s_q_stone *q_stone, s_stone *stone);

// если есть камень в очереди, вернёт указатель на него
s_stone* stone_in_q(s_q_stone *q_stone, int X, int Y);

// рекурсивное добавление в очередь
int rec_add_in_q(s_q_stone *q_stone, s_map *map, int X, int Y);

// удаление из очереди камней, достигших низа, с начала, пока не встретится свободный камень
void del_from_q_stone(s_q_stone *q_stone, s_map *map);

// очистка всей очереди
void q_stone_clear(s_q_stone *q_stone);

// добавление крисалла игроку и изменение на карте
int player_get_diamond(s_player *player, s_map *map);

// удалить элемент очереди
void del_1_stone(s_q_stone *q_stone, s_stone *stone);

// передвижение камней, если снизу свободно
int move_stone_down(s_q_stone *q_stone, s_map *map, s_player *player, s_stone *cur, short *X, short *Y);

// передвижение камней, если снизу-слева свободно
int move_stone_down_left(s_q_stone *q_stone, s_map *map, s_player *player, s_stone *cur, short *X, short *Y);

// передвижение камней, если снизу-справа свободно
int move_stone_down_right(s_q_stone *q_stone, s_map *map, s_player *player, s_stone *cur, short *X, short *Y);

// смещение всех камней на 1 шаг
void move_stone(s_q_stone *q_stone, s_map *map, s_player *player);

// толкаем камень. движется только камень.
int push_stone(s_map *map, direction dir, s_player *player, s_q_stone *q_stone);

// запустить процесс добавления камней вокруг игрока
void add_in_q_around_player(s_map *map, s_player *player, s_q_stone *q_stone);

// создать список камней, совпадающий значениями с текущим
int copy_q_stone(s_q_stone *q_stone_1, s_q_stone *q_stone_2);

#endif//_STONES_AND_DIAMONDS_H