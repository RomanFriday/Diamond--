#ifndef _MOVING_H
#define _MOVING_H

#include "declarartions.h"
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

#endif //_MOVING_H