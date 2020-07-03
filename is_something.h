#ifndef _IS_SOMETHING_H
#define _IS_SOMETHING_H

#include "declarations.h"

// ����������� �� ������ �����
int is_on_map(s_map *map, int X, int Y);

// ������ - �����
int is_grass(s_map *map, int X, int Y);

// ������ - ������
int is_stone(s_map *map, int X, int Y);

// ������ - ������
int is_diamond(s_map* map, int X, int Y);

// ������ - �����
int is_bush(s_map *map, int X, int Y);

// ������ - �����
int is_wall(s_map *map, int X, int Y);

// ������ - �����
int is_exit(s_map *map, int X, int Y);

// ������ - ����������
int is_checkpoint(s_map *map, int X, int Y);

// �� ������ - �����
int is_player(s_map* map, int X, int Y);

// �� ������ - ����
int is_enemy(s_map* map, int X, int Y);

#endif //_IS_SOMETHING_H