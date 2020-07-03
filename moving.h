#ifndef _MOVING_H
#define _MOVING_H

#include "declarations.h"
#include "is_something.h"
#include "stones_and_diamonds.h"

// можно ли пойти направо
int can_i_move_right(direction dir, s_map* map, int X, int Y);

// можно ли пойти налево
int can_i_move_left(direction dir, s_map* map, int X, int Y);

// можно ли пойти вверх
int can_i_move_up(direction dir, s_map* map, int X, int Y);

// можно ли пойти вниз
int can_i_move_down(direction dir, s_map* map, int X, int Y);

// можно ли пойти по указанному направлению
int can_i_move(direction dir, s_map* map, int X, int Y);

// передвинуть игрока вправо
int move_right(s_map *map, s_player *player, s_q_stone *q_stone);

// передвинуть игрока влево
int move_left(s_map *map, s_player *player, s_q_stone *q_stone);

// передвинуть игрока вверх
int move_up(s_map *map, s_player *player);

// передвинуть игрока вниз
int move_down(s_map *map, s_player *player);

// движение врага вправо
int move_enemy_right(s_map* map, s_enemy* enemy);

// движение врага влево
int move_enemy_left(s_map* map, s_enemy* enemy);

// движение врага вверх
int move_enemy_up(s_map* map, s_enemy* enemy);

// движение врага вниз
int move_enemy_down(s_map* map, s_enemy* enemy);

// движение врагов
int move_every_enemy(s_map* map, s_enemy** first_enemy);

#endif //_MOVING_H