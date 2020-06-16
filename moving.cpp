#include "moving.h"

// можно ли пойти направо
int can_i_move_right(direction dir, s_map* map, int X, int Y)
{
	if (!is_on_map(map, X + 1, Y))
		return 0;
	switch ((map->matr[Y][X + 1].ch+256)%256)
	{
	case type_bush: case type_exit: case type_grass: case type_diamond: case type_checkpoint:
	case type_p_bush: case type_p_exit: case type_p_grass: case type_p_diamond: case type_p_checkpoint:
		return 1;
	case type_stone: case type_p_stone:
		if (is_grass(map, X + 2, Y))
			return 1;
	default: 0;
	}
	return 0;
}

// можно ли пойти налево
int can_i_move_left(direction dir, s_map* map, int X, int Y)
{
	if (!is_on_map(map, X - 1, Y))
		return 0;
	switch ((map->matr[Y][X-1].ch+256)%256)
	{
	case type_bush: case type_exit: case type_grass: case type_diamond: case type_checkpoint:
	case type_p_bush: case type_p_exit: case type_p_grass: case type_p_diamond: case type_p_checkpoint:
		return 1;
	case type_stone: case type_p_stone:
		if (is_grass(map, X - 2, Y))
			return 1;
	default: 0;
	}
	return 0;
}

// можно ли пойти вверх
int can_i_move_up(direction dir, s_map* map, int X, int Y)
{
	if (!is_on_map(map, X, Y - 1))
		return 0;
	switch ((map->matr[Y-1][X].ch+256)%256)
	{
	case type_bush: case type_exit: case type_grass: case type_diamond: case type_checkpoint:
	case type_p_bush: case type_p_exit: case type_p_grass: case type_p_diamond: case type_p_checkpoint:
		return 1;
	default: 0;
	}
	return 0;
}

// можно ли пойти вниз
int can_i_move_down(direction dir, s_map* map, int X, int Y)
{
	if (!is_on_map(map, X, Y + 1))
		return 0;
	switch ((map->matr[Y+1][X].ch+256)%256)
	{
	case type_bush: case type_exit: case type_grass: case type_diamond: case type_checkpoint:
	case type_p_bush: case type_p_exit: case type_p_grass: case type_p_diamond: case type_p_checkpoint:
		return 1;
	default: 0;
	}
	return 0;
}

// можно ли пойти по указанному направлению
int can_i_move(direction dir, s_map* map, int X, int Y)
{
	switch (dir)
	{
	case right:
		return can_i_move_right(dir, map, X, Y);
	case left:
		return can_i_move_left(dir, map, X, Y);
	case up:
		return can_i_move_up(dir, map, X, Y);
	case down:
		return can_i_move_down(dir, map, X, Y);
	}
	return 0;
}

// передвинуть игрока вправо
int move_right(s_map *map, s_player *player, s_q_stone *q_stone)
{
	if(!can_i_move(right, map, player->pos.X, player->pos.Y))
		return 0;
	push_stone(map, right, player, q_stone);
	map->matr[player->pos.Y][player->pos.X].pl = NULL;
	player->pos.X++;
	map->matr[player->pos.Y][player->pos.X].pl = player;
	return 1;
}

// передвинуть игрока влево
int move_left(s_map *map, s_player *player, s_q_stone *q_stone)
{
	if(!can_i_move(left, map, player->pos.X, player->pos.Y))
		return 0;
	push_stone(map, left, player, q_stone);
	map->matr[player->pos.Y][player->pos.X].pl = NULL;
	player->pos.X--;
	map->matr[player->pos.Y][player->pos.X].pl = player;
	return 1;
}

// передвинуть игрока вверх
int move_up(s_map *map, s_player *player)
{
	if(!can_i_move(up, map, player->pos.X, player->pos.Y))
		return 0;
	map->matr[player->pos.Y][player->pos.X].pl = NULL;
	player->pos.Y--;
	map->matr[player->pos.Y][player->pos.X].pl = player;
	return 1;
}

// передвинуть игрока вниз
int move_down(s_map *map, s_player *player)
{
	if(!can_i_move(down, map, player->pos.X, player->pos.Y))
		return 0;
	map->matr[player->pos.Y][player->pos.X].pl = NULL;
	player->pos.Y++;
	map->matr[player->pos.Y][player->pos.X].pl = player;
	return 1;
}
