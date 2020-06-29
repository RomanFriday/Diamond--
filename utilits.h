#ifndef _UTILITS_H
#define _UTILITS_H

#include "declarations.h"
#include "print.h"
#include "moving.h"
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

// ����� ��������� �� ���� ������. ������ ���������� 0
int err(int type);

// ������� �������� ����� �� ���� ����� � ��� ������
int type_char_to_print(char *c);

// ������� ���� �������� ����� �� ���� ����� � ��� ������
int map_characters_to_print(s_map *map);

// ������� ������ ����� � ������� �������� �����
int str2color(char *str);

// ������� ������ ����� � ������� �������� �����, ������ ������ �� �����
int str2color_from_file(unsigned short *parametr, FILE **fin);

// ������ �����
int reverse_int(int n);

// ������� ����� � ������
void int2str(int n, char str[], int lenght);

// ������� ��������� � �����������
int special_bottom(char *bottom);

// ����������� map (����� ������ �������!)
int copy_map(s_map *map1, s_map *map2);

// ����������� �� ���������
int save_on_checkpoint(s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone);

// ������� �� ���������� � ��������� player->lifes �� 1
int go_to_checkpoint(s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone);

// ��������� ������� �� ������� �������
void command_in_game(char bottom, s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone);

// �������� s_cell - ������� ��������� m �� n
int create_s_cell_matrix(s_cell ***matrix, int m, int n);

// ��������� ������ ������������ ������
void screen_position(COORD *screen_pos, s_player *player, s_map *map);

// ���������� ������ ������� � ����� s_cell. �������� ���������� �����
int free_s_cell_matrix(s_cell ***matrix, int size_of_strings);

// �������� ��� ������, ��� ��������
int free_all(s_map *map, s_map *save_map, s_q_stone *q_stone, s_q_stone *save_q_stone);

// ������ ����� ���� - � ���� �������� �������� �������� ������ = 1
// ����� 0, ���� ���� �� ������
int new_user();

#endif //_UTILITS_H