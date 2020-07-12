#ifndef _PRINT_H
#define _PRINT_H

#define _CRT_SECURE_NO_WARNINGS
#include "declarations.h"
#include "is_something.h"
#include "utilits.h"
#include <stdio.h>

#define col(back, fore) SetConsoleTextAttribute(hConsole, (WORD) (((back) << 4) | (fore)))

// ���������� ������, ��������� ������ �� �������� c. ���� is_new_line!=0, �������� ������� �� ����� ������.
void print_line(char c, int size, int is_new_line);

// ���������� ������ �� ������������ ����� �����
int print_player(COORD *screen_pos, s_player *pl, int i, int j);

// ���������� �����
int print_enemy(s_map* map, int X, int Y);

// ���������� ������ ����� (�����, ����� ��� ������)
void print_cell(s_map *map, int i, int j);

// ��������� ����� �����.
// �� ���� �������� ����� � ���������� ������ ����������� �����
void print_map(s_map *map, COORD *screen_pos, s_player *player, s_enemy** first_enemy);

// ������� ������ - ����� �� �����
int print_great_victory();

// ���������� ����
int game_over();

// ������� �� ����� ������ ��� �����������, ��� X, ���� ������� ����������
void print_choose_level(int max_passed_level, int cur_level);

// ������� ������� ����
void print_menu_command(commands cur);

// ������� �� ����� ������� ����.
// ������� �� ����������, �.�. ������� � declarations.h ��� ���������.
// cur - ����� ������� �������
void print_choose_menu_commands(commands menu_commands[], commands cur);

#endif // _PRINT_H