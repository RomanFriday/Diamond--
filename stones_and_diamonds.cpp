#include "stones_and_diamonds.h"


// �������� �������� ������� ������
s_stone* create_stone(s_stone *info)
{
	s_stone *new_stone = (s_stone*)malloc(sizeof(s_stone));
	if(!new_stone)
		return (s_stone*)err(RAM_IS_OVER);
	// ���������� �����
	new_stone->ch = info->ch;
	new_stone->color = info->color;
	new_stone->pos = info->pos;
	new_stone->next = NULL;
	return new_stone;
}

// ���������� � �����
int add_stone_in_end(s_q_stone *q_stone, s_stone *stone)
{
	// �������� �� �������������
	if(!stone)
		return err(NO_ENOUGH_DATA);
	// ������� �����
	if(!q_stone->head)
	{
		q_stone->head = q_stone->tail = stone;
		return 1;
	}
	// ������� �� �����
	q_stone->tail->next = stone;
	q_stone->tail = stone;
	return 1;
}

// ���� ���� ������ � �������, ����� ��������� �� ����
s_stone* stone_in_q(s_q_stone *q_stone, int X, int Y)
{
	for(s_stone *cur=q_stone->head; cur; cur=cur->next)
		if(cur->pos.X == X && cur->pos.Y == Y)
			return cur;
	return NULL;
}

// ����������� ���������� � �������
int rec_add_in_q(s_q_stone *q_stone, s_map *map, int X, int Y)
{
	if(X<0 || X>=map->size.X)
		return 1; // ���� �����-�� �������������
	if(Y<0 || Y>=map->size.Y)
		return 1; // ���� �����-�� �������������
	if((map->matr[Y][X].ch+256)%256 != type_p_stone && (map->matr[Y][X].ch+256)%256 != type_p_diamond )
		return 1; // ��� �� ������ � �� �������� - ����� �� ���� ��������
	if( !(is_grass(map, X, Y+1) || // ����� ������ ����
		is_grass(map, X-1, Y)&&
			is_grass(map, X-1, Y+1)&&
			((map->matr[Y][X].ch+256)%256 == type_p_stone || (map->matr[Y][X].ch+256)%256 == type_p_diamond)|| // ����� ������ ����� � ������ ����� �� �����
		is_grass(map, X+1, Y)&&
			is_grass(map, X+1, Y+1)&&
			((map->matr[Y][X].ch+256)%256 == type_p_stone || (map->matr[Y][X].ch+256)%256 == type_p_diamond)) ) // ����� ������ ������ � ������ ����� �� �����
		return 1; // ����� ������ ������ - ����� �� ���� ��������
	if(stone_in_q(q_stone, X, Y)) // ������ ��� ��������� � �������
		return 1;
	s_stone temp = {(map->matr[Y][X].ch+256)%256, map->matr[Y][X].color, {X, Y}, NULL}; // ��� ����������
	if(!add_stone_in_end(q_stone, create_stone(&temp)))
		return 0;
	map->matr[Y][X].ch = type_p_grass; // ����� �� ������ ������ ����
	if(!rec_add_in_q(q_stone,map,X,Y-1))
		return 0;
	if(!rec_add_in_q(q_stone,map,X-1,Y-1))
		return 0;
	if(!rec_add_in_q(q_stone,map,X+1,Y-1))
		return 0;
	return 1;
}

// �������� �� ������� ������, ��������� ����, � ������, ���� �� ���������� ��������� ������
void del_from_q_stone(s_q_stone *q_stone, s_map *map)
{
	while(q_stone->head)
	{
		int X = q_stone->head->pos.X, Y = q_stone->head->pos.Y;
		// ����� ��������
		if(is_grass(map, X, Y+1))
			if(map->matr[Y+1][X].pl)
				if(q_stone->head->ch == type_p_diamond)
					return;
				else; // ��� �� ����� �� ����� �����
			else // ��� ������ ����� ��� ������
				return;
		// ������ �� �����
		if( map->matr[Y+1][X].ch == type_p_stone || map->matr[Y+1][X].ch == type_p_diamond )
		{
			// ����� ��������
			if(is_grass(map, X-1, Y)&&is_grass(map, X-1, Y+1))
				if(!map->matr[Y+1][X-1].pl) // ����� ������ ���
					if(map->matr[Y][X-1].pl) // ����� �����-�����
						if(q_stone->head->ch == type_p_diamond)
								return;
							else; // ��� �� �����, �� ����� �����
						else // ��� ������ ����� ��� ������
							return;
			// ������ ��������
			if(is_grass(map, X+1, Y)&&is_grass(map, X+1, Y+1))
				if(!map->matr[Y+1][X+1].pl) // ������ ������ ���
					if(map->matr[Y][X+1].pl) // ����� ������-�����
						if(q_stone->head->ch == type_p_diamond)
								return;
							else; // ��� �� �����, �� ����� �����
						else // ��� ������ ����� ��� ������
							return;
		}
		s_stone *cur = q_stone->head; // ��������� ��� ������������ ������
		map->matr[Y][X].ch = cur->ch; // ���������� �� �����
		q_stone->head = cur->next; // ������ �� �������
		free(cur); // ������������ ������
	}
	// � ������� ������ ���
	q_stone->head = q_stone->tail = NULL;
}

// ������� ���� �������
void q_stone_clear(s_q_stone *q_stone)
{
	if(!q_stone)
		return;
	while(q_stone->head)
	{
		s_stone *cur = q_stone->head;
		q_stone->head = cur->next;
		free(cur);
	}
	q_stone->head = q_stone->tail = NULL;
}

// ���������� �������� ������ � ��������� �� �����
int player_get_diamond(s_player *player, s_map *map)
{
	int X=player->pos.X, Y=player->pos.Y;
	if((map->matr[Y][X].ch+256)%256 == type_p_diamond)
	{
		player->diamonds++;
		map->diamonds--;
		map->matr[Y][X].ch = type_p_grass;
	}
	return 1;
}

// ������� ������� �������
void del_1_stone(s_q_stone *q_stone, s_stone *stone)
{
	if(!stone)
		return; // ������ �������
	if(stone == q_stone->head)
	{
		if(q_stone->tail == stone) // � ������� ����� ���� �������
			q_stone->head = q_stone->tail = NULL;
		else
			q_stone->head = stone->next;
		free(stone);
		return;
	}
	s_stone *prev = q_stone->head;
	while(prev && prev->next != stone)
		prev = prev->next; // ������� ���������� �������
	if(!prev)
		return; // ������ ������� �� ��������� � �������
	if(stone==q_stone->tail) // ���� ������� ��������� ������ - ������� q_stone->tail
		q_stone->tail = prev;
	prev->next = stone->next;
	free(stone);
}

// ������������ ������, ���� ����� ��������
int move_stone_down(s_q_stone *q_stone, s_map *map, s_player *player, s_stone *cur, short *X, short *Y)
{
	if(is_grass(map, *X, *Y+1)) // ����� �����
	{
		if(map->matr[*Y+1][*X].pl) // ����� �����
		{
			if((map->matr[*Y][*X].ch+256)%256==type_p_diamond) // ������ - ��������
			{
				map->matr[*Y][*X].ch = type_p_grass;
				(*Y)++;
				map->matr[*Y][*X].ch = type_p_diamond;
				player_get_diamond(player, map);
				del_1_stone(q_stone, cur);
			}
		}
		else // ����� �����
		{
			map->matr[*Y][*X].ch = type_p_grass;
			cur->pos.Y++;
			map->matr[*Y][*X].ch = cur->ch;
		}
		return 1;
	}
	return 0;
}

// ������������ ������, ���� �����-����� ��������
int move_stone_down_left(s_q_stone *q_stone, s_map *map, s_player *player, s_stone *cur, short *X, short *Y)
{
	if((map->matr[*Y+1][*X].ch+256)%256 == type_p_stone || (map->matr[*Y+1][*X].ch+256)%256 == type_p_diamond) // ������ �� �����
		if(is_grass(map, int(*X)-1, int(*Y)) && !map->matr[*Y][*X-1].pl) // ����� ������ ���
		{
			if(is_grass(map, *X-1, *Y+1)) // �����-����� �����
			{
				if(map->matr[*Y+1][*X-1].pl) // �����-����� �����
				{
					if((map->matr[*Y][*X].ch+256)%256 == type_p_diamond) // ������ - ��������
					{
						map->matr[*Y][*X].ch = type_p_grass;
						(*Y)++;
						(*X)--;
						map->matr[*Y][*X].ch = type_p_diamond;
						player_get_diamond(player, map);
						del_1_stone(q_stone, cur);
					}
				}
				else // �����-����� ����� ������
				{
					map->matr[*Y][*X].ch = type_p_grass;
					cur->pos.Y++;
					cur->pos.X--;
					map->matr[*Y][*X].ch = cur->ch;
				}
				return 1;
			}
		}
	return 0;
}

// ������������ ������, ���� �����-������ ��������
int move_stone_down_right(s_q_stone *q_stone, s_map *map, s_player *player, s_stone *cur, short *X, short *Y)
{
	if((map->matr[*Y+1][*X].ch+256)%256 == type_p_stone || (map->matr[*Y+1][*X].ch+256)%256 == type_p_diamond) // ������ �� �����
		if(is_grass(map, int(*X)+1, int(*Y)) && !map->matr[*Y][*X+1].pl) // ������ ������ ���
			if(is_grass(map, *X+1, *Y+1)) // ������-����� �����
			{
				if(map->matr[*Y+1][*X+1].pl) // ������-����� �����
				{
					if((map->matr[*Y][*X].ch+256)%256 == type_p_diamond) // ������ - ��������
					{
						map->matr[*Y][*X].ch = type_p_grass;
						(*Y)++;
						(*X)++;
						map->matr[*Y][*X].ch = type_p_diamond;
						player_get_diamond(player, map);
						del_1_stone(q_stone, cur);
					}
				}
				else // ������-����� ����� ������
				{
					map->matr[*Y][*X].ch = type_p_grass;
					cur->pos.Y++;
					cur->pos.X++;
					map->matr[*Y][*X].ch = cur->ch;
				}
				return 1;
			}
	return 0;
}

// �������� ���� ������ �� 1 ���
void move_stone(s_q_stone *q_stone, s_map *map, s_player *player)
{
	s_stone *cur=q_stone->head, *next = q_stone->head ? q_stone->head->next : NULL;
	for(;cur;cur=next, next=cur?cur->next:NULL)
	{
		short *X = &(cur->pos.X), *Y = &(cur->pos.Y);
		// ����� ��������
		if(move_stone_down(q_stone, map, player, cur, X, Y))
			continue;
		// ����� ��������
		if(move_stone_down_left(q_stone, map, player, cur, X, Y))
			continue;
		// ������ ��������
		if(move_stone_down_right(q_stone, map, player, cur, X, Y))
			continue;
	}
}

// ������� ������. �������� ������ ������.
int push_stone(s_map *map, direction dir, s_player *player, s_q_stone *q_stone)
{
	s_stone *stone = NULL; // ��������� �� ������ � ������� ������
	switch(dir)
	{
	case left:
		if(!is_stone(map, player->pos.X-1, player->pos.Y)) // ��� �� ������ - �������
			return 0;
		if(!can_i_move( dir, map, player->pos.X, player->pos.Y)) // ������ ������ ����������� - �������
			return 0;
		stone = stone_in_q(q_stone, player->pos.X-1, player->pos.Y);
		map->matr[player->pos.Y][player->pos.X-1].ch = type_p_grass;
		map->matr[player->pos.Y][player->pos.X-2].ch = type_p_stone;
		if(stone)
			stone->pos.X--;
		break;
	case right:
		if(!is_stone(map, player->pos.X+1, player->pos.Y)) // ��� �� ������ - �������
			return 0;
		if(!can_i_move( dir, map, player->pos.X, player->pos.Y)) // ������ ������ ����������� - �������
			return 0;
		stone = stone_in_q(q_stone, player->pos.X+1, player->pos.Y);
		map->matr[player->pos.Y][player->pos.X+1].ch = type_p_grass;
		map->matr[player->pos.Y][player->pos.X+2].ch = type_p_stone;
		if(stone)
			stone->pos.X++;
		break;
	default: return 0;
	}
	return 1;
}

// ��������� ������� ���������� ������ ������ ������. ����� 0, ���� ��������� ������.
int add_in_q_around_player(s_map *map, s_player *player, s_q_stone *q_stone)
{
	for(int X=player->pos.X-2; X<=player->pos.X+2; X++)
		for(int Y=player->pos.Y-2; Y<=player->pos.Y+1; Y++)
			if(!rec_add_in_q(q_stone, map, X, Y))
				return 0;
	for(s_stone *cur=q_stone->head; cur; cur=cur->next)
		map->matr[cur->pos.Y][cur->pos.X].ch = cur->ch;
	return 1;
}

// ������� ������ ������, ����������� ���������� � �������
int copy_q_stone(s_q_stone *q_stone_1, s_q_stone *q_stone_2)
{
	for(s_stone *cur = q_stone_1->head; cur; cur=cur->next)
		if( !add_stone_in_end(q_stone_2, create_stone(cur)) )
			return 0;
	return 1;
}
