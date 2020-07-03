#ifndef _MOVING_H
#define _MOVING_H

#include "declarations.h"
#include "is_something.h"
#include "stones_and_diamonds.h"

// ����� �� ����� �������
int can_i_move_right(direction dir, s_map* map, int X, int Y);

// ����� �� ����� ������
int can_i_move_left(direction dir, s_map* map, int X, int Y);

// ����� �� ����� �����
int can_i_move_up(direction dir, s_map* map, int X, int Y);

// ����� �� ����� ����
int can_i_move_down(direction dir, s_map* map, int X, int Y);

// ����� �� ����� �� ���������� �����������
int can_i_move(direction dir, s_map* map, int X, int Y);

// ����������� ������ ������
int move_right(s_map *map, s_player *player, s_q_stone *q_stone);

// ����������� ������ �����
int move_left(s_map *map, s_player *player, s_q_stone *q_stone);

// ����������� ������ �����
int move_up(s_map *map, s_player *player);

// ����������� ������ ����
int move_down(s_map *map, s_player *player);

// �������� ����� ������
int move_enemy_right(s_map* map, s_enemy* enemy);

// �������� ����� �����
int move_enemy_left(s_map* map, s_enemy* enemy);

// �������� ����� �����
int move_enemy_up(s_map* map, s_enemy* enemy);

// �������� ����� ����
int move_enemy_down(s_map* map, s_enemy* enemy);

// �������� ������
int move_every_enemy(s_map* map, s_enemy** first_enemy);

#endif //_MOVING_H