#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "enemys.h"
#include "declarations.h"
#include "utilits.h"
#include "is_something.h"

//�������� ������ �����
s_enemy* create_enemy(s_enemy* info)
{
	s_enemy* new_enemy = (s_enemy*)malloc(sizeof(s_enemy));
	if (!new_enemy)
		return (s_enemy*)err(RAM_IS_OVER);
	new_enemy->d = info->d;
	new_enemy->ch = info->ch;
	new_enemy->color = info->color;
	new_enemy->pos = info->pos;
	new_enemy->next = NULL;
	return new_enemy;
}

//���������� �����
int add_in_head(s_enemy* new_enemy, s_enemy **first_enemy)
{
	if(new_enemy==0)
		return 0;
	if (!first_enemy)
	{
		*first_enemy = new_enemy;
		return 1;
	}
	new_enemy->next = *first_enemy;
	*first_enemy = new_enemy;
	return 1;
}

//�������� ������ ������
int create_enemys(s_txt_name txt_name, s_enemy** first_enemy)
{
	s_enemy temp = {0,};
	FILE* fenemy = fopen(txt_name.enemy, "r");
	if (!fenemy)
		return err(FILE_NOT_FOUND);
	// �� ������ ������ ������ ���
	if (fgetc(fenemy) == '.')
			return 1;
	while (1)
	{
		if ( !fscanf(fenemy, "%d", &(temp.pos.X)) || !fscanf(fenemy, "%d", &(temp.pos.Y)) )
		{
			fclose(fenemy);
			return err(NO_ENOUGH_DATA) * delete_all_enemys(first_enemy);
		}
		fgetc(fenemy);
		if (!fscanf(fenemy, "%c", &(temp.ch)))
		{
			fclose(fenemy);
			return err(NO_ENOUGH_DATA) * delete_all_enemys(first_enemy);
		}
		fgetc(fenemy);
		if (!fscanf(fenemy, "%c", &(temp.d)))
		{
			fclose(fenemy);
			return err(NO_ENOUGH_DATA)*delete_all_enemys(first_enemy);
		}if ( !add_in_head(create_enemy(&temp), first_enemy) )
		{
			fclose(fenemy);
			return delete_all_enemys(first_enemy);
		}
		fgetc(fenemy);
		if (fgetc(fenemy) == '.')
			break;
		
	}
	fclose(fenemy);
	return 1;
	}

//�������� �����
int delete_enemy(s_enemy* enemy, s_enemy** first_enemy)
{
	if (enemy == *first_enemy)
	{
		*first_enemy = (*first_enemy)->next;
		free(enemy);
		return 1;
	}
	s_enemy* prev = *first_enemy;
	while (prev && prev->next != enemy)
		prev = prev->next; // ������� ���������� �������
	if (!prev)
		return 0; // ������ ������� �� ��������� � �������
	prev->next = enemy->next;
	free(enemy);
	return 1;
}

//�������� ���� ������. ������ ���������� ����
int delete_all_enemys(s_enemy** first_enemy)
{
	if(!first_enemy)
		return 0;
	while (*first_enemy!=NULL)
		delete_enemy(*first_enemy, first_enemy);
	return 0;
}

//������ �����
int death_enemy(s_enemy* enemy, s_enemy** first_enemy, s_map* map)
{
	// ���� ������ ��� �������� ������ �������
	if (
		(
			((is_stone(map, enemy->pos.X - 1, enemy->pos.Y) || is_diamond(map, enemy->pos.X - 1, enemy->pos.Y)) &&
			(is_stone(map, enemy->pos.X - 1, enemy->pos.Y - 1) || is_diamond(map, enemy->pos.X - 1, enemy->pos.Y - 1)))
		&&
			(!is_grass(map, enemy->pos.X - 2, enemy->pos.Y) || !is_grass(map, enemy->pos.X - 2, enemy->pos.Y - 1) ||
				is_player(map, enemy->pos.X - 2, enemy->pos.Y)

				|| is_player(map, enemy->pos.X - 2, enemy->pos.Y - 1))
		)
			&&
		 (is_grass(map, enemy->pos.X, enemy->pos.Y-1) && !is_player(map, enemy->pos.X, enemy->pos.Y-1) && !is_enemy(map, enemy->pos.X, enemy->pos.Y - 1)))
	{
		map->matr[enemy->pos.Y][enemy->pos.X].en = NULL;
		delete_enemy(enemy, first_enemy);
		return 1;
	}
	if (is_stone(map, enemy->pos.X, enemy->pos.Y-1)||is_diamond(map, enemy->pos.X, enemy->pos.Y-1))
	{
		map->matr[enemy->pos.Y][enemy->pos.X].en = NULL;
		delete_enemy(enemy, first_enemy);
		return 1;
	}
	if ((is_stone(map, enemy->pos.X + 1, enemy->pos.Y) || is_diamond(map, enemy->pos.X + 1, enemy->pos.Y)) &&
		(is_stone(map, enemy->pos.X + 1, enemy->pos.Y - 1) || is_diamond(map, enemy->pos.X + 1, enemy->pos.Y - 1)) &&
		is_grass(map, enemy->pos.X, enemy->pos.Y - 1) && !is_player(map, enemy->pos.X, enemy->pos.Y - 1) && !is_enemy(map, enemy->pos.X, enemy->pos.Y - 1))
	{
		map->matr[enemy->pos.Y][enemy->pos.X].en = NULL;
		delete_enemy(enemy, first_enemy);
		return 1;
	}
	return 0;
}

//������ ������� �����
void death_every_enemy(s_enemy** first_enemy, s_map* map)
{
	s_enemy* cur_next=NULL;
	for (s_enemy* cur = *first_enemy; cur; cur = cur_next)
	{
		cur_next= cur->next;
		death_enemy(cur, first_enemy, map);
	}
}

// ��������� ����� �� �����
int set_enenemys(s_map* map, s_enemy** first_enemy)
{
	for (s_enemy* cur = *first_enemy; cur; cur = cur->next)
	{
		if (!is_on_map(map, cur->pos.X, cur->pos.Y))
			return 0;
		map->matr[cur->pos.Y][cur->pos.X].en = cur;
	}
	return 1;
}

// ������� ������ ������, ����������� ���������� � �������
int copy_enemys(s_enemy** first_enemy_1, s_enemy** first_enemy_2)
{
	for (s_enemy* cur = *first_enemy_1; cur; cur = cur->next)
		if (!add_in_head(create_enemy(cur), first_enemy_2))
			return 0;
	return 1;
}
