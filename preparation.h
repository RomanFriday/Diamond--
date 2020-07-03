#ifndef _PREPARATION_H
#define _PREPARATION_H

#define _CRT_SECURE_NO_WARNINGS
#include "declarations.h"
#include "enemys.h"
#include "is_something.h"
#include "utilits.h"
#include <stdio.h>

// �������� ����� �� ���������� �����
int create_map_characters(s_txt_name txt_name, s_map *map);

// ��������� ������ �� ����� - �������
int set_player_on_matr(s_map *map, s_all_colors all_colors, s_player *player);

// �������� �����-�������
int create_map_matr(s_map *map, s_all_colors all_colors, s_player *player, s_enemy** first_enemy);

// �������� ������������ ���� ������ � ������ �� ���������� �����
int create_all_colors(s_txt_name txt_name, s_all_colors *all_colors);

// �������� ������� ������ �����
int create_map_colors(s_map *map, s_all_colors all_colors);

// ������� ��������� ���������� ������
int create_player( s_txt_name txt_name, s_player* player);

// �������� � txt_name ����� ��������� ������, ������������ �� ������ ������
int get_txt_name(int level, s_txt_name *txt_name);

// ����� ������� �� �����
int get_level_from_file(int *level);

// ���������� - ������ ���������� �� ������, �������� �����
int preparation(int *level, s_map *map, s_all_colors *all_colors, s_player *player, s_map *save_map, s_enemy **first_enemy);

#endif //_PREPARATION_H