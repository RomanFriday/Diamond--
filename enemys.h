#ifndef _ENEMYS_H
#define _ENEMYS_H

#include <stdio.h>
#include "declarations.h"
#include "utilits.h"

//создание одного врага
s_enemy* create_enemy(s_enemy* info);

//добавление врага
int add_in_head(s_enemy* new_enemy, s_enemy** first_enemy);

//создание списка врагов
int create_enemys(s_txt_name txt_name, s_enemy** first_enemy);

//удаление врага
int delete_enemy(s_enemy* enemy, s_enemy** first_enemy);

//удаление всех врагов
int delete_all_enemys(s_enemy** first_enemy);

//смерть врага
int death_enemy(s_enemy* enemy, s_enemy** first_enemy, s_map* map);

//смерть каждого врага
void death_every_enemy(s_enemy** first_enemy, s_map* map);

// установка врага на карту
int set_enenemys(s_map* map, s_enemy** first_enemy);

// создать список камней, совпадающий значениями с текущим
int copy_enemys(s_enemy** first_enemy_1, s_enemy** first_enemy_2);

#endif //_ENEMYS_H