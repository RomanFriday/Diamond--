#ifndef _ENEMYS_H
#define _ENEMYS_H

#include <stdio.h>
#include "declarations.h"
#include "utilits.h"

//�������� ������ �����
s_enemy* create_enemy(s_enemy* info);

//���������� �����
int add_in_head(s_enemy* new_enemy, s_enemy** first_enemy);

//�������� ������ ������
int create_enemys(s_txt_name txt_name, s_enemy** first_enemy);

//�������� �����
int delete_enemy(s_enemy* enemy, s_enemy** first_enemy);

//�������� ���� ������
int delete_all_enemys(s_enemy** first_enemy);

//������ �����
int death_enemy(s_enemy* enemy, s_enemy** first_enemy, s_map* map);

//������ ������� �����
void death_every_enemy(s_enemy** first_enemy, s_map* map);

// ��������� ����� �� �����
int set_enenemys(s_map* map, s_enemy** first_enemy);

// ������� ������ ������, ����������� ���������� � �������
int copy_enemys(s_enemy** first_enemy_1, s_enemy** first_enemy_2);

#endif //_ENEMYS_H