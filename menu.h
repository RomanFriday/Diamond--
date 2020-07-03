#ifndef _MENU_H
#define _CRT_SECURE_NO_WARNINGS
#define _MENU_H

#include "declarations.h"
#include "utilits.h"
#include "preparation.h"
#include "print.h"
#include <Windows.h>
#include <time.h>

// ������� ������
int great_victory(s_map *map, s_player *player);

// ������� ������� �� ��������� ��� �����������
int choose_passed_level(int *level);

// ������ � ��������� ������� (��������� ��� �����������)
int play_choose_level(int *level, 
	s_map *map,
	s_all_colors *all_colors,
	s_player *player,
	COORD *screen_pos,
	s_q_stone *q_stone,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone);

// ������: ���������� ����. ����� 0, ���� �������� ������
int play_continue_game(int *level, 
	s_map *map,
	s_all_colors *all_colors,
	s_player *player,
	COORD *screen_pos,
	s_q_stone *q_stone,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone);

// ����� ������� ����. ���� ������ �� �������, ����� -1
int choose_menu_commands(commands *menu_commands);

// ����������� ������� ����. ����� 0, ���� ��������� ������.
int menu(int *level,
	s_map *map,
	s_all_colors *all_colors,
	s_player *player,
	COORD *screen_pos,
	s_q_stone *q_stone,
	s_enemy **first_enemy,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone,
	s_enemy **save_first_enemy);

// ��������� �������� ��� �������, �� ������� ����� ����� - ����� �����, ������� ����...
int player_on_cell(s_map *map,
	s_player *player,
	s_q_stone *q_stone,
	s_enemy **first_enemy,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone,
	s_enemy **save_first_enemy);

// ������� �������
int press_bottom(s_map *map,
	s_player *player,
	s_q_stone *q_stone,
	s_enemy **first_enemy,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone,
	s_enemy **save_first_enemy);

// ������� ����. ���� �����������, ����� ����� �������� �� ������ ������ ��� ����� ESC
int game_process(s_map *map,
	s_player *player, 
	COORD *screen_pos, 
	s_q_stone *q_stone,
	s_enemy **first_enemy,
	s_map *save_map, 
	s_player *save_player, 
	s_q_stone *save_q_stone,
	s_enemy **save_first_enemy);

#endif // _MENU_H