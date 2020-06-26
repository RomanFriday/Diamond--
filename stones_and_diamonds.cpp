#include "stones_and_diamonds.h"


// создание элемента очереди камней
s_stone* create_stone(s_stone *info)
{
	s_stone *new_stone = (s_stone*)malloc(sizeof(s_stone));
	if(!new_stone)
		return (s_stone*)err(RAM_IS_OVER);
	// заполнение полей
	new_stone->ch = info->ch;
	new_stone->color = info->color;
	new_stone->pos = info->pos;
	new_stone->next = NULL;
	return new_stone;
}

// добавление в конец
int add_stone_in_end(s_q_stone *q_stone, s_stone *stone)
{
	// проверка на существование
	if(!stone)
		return err(NO_ENOUGH_DATA);
	// очередь пуста
	if(!q_stone->head)
	{
		q_stone->head = q_stone->tail = stone;
		return 1;
	}
	// очередь не пуста
	q_stone->tail->next = stone;
	q_stone->tail = stone;
	return 1;
}

// если есть камень в очереди, вернёт указатель на него
s_stone* stone_in_q(s_q_stone *q_stone, int X, int Y)
{
	for(s_stone *cur=q_stone->head; cur; cur=cur->next)
		if(cur->pos.X == X && cur->pos.Y == Y)
			return cur;
	return NULL;
}

// рекурсивное добавление в очередь
int rec_add_in_q(s_q_stone *q_stone, s_map *map, int X, int Y)
{
	if(X<0 || X>=map->size.X)
		return 1; // край карты-не рассматриваем
	if(Y<0 || Y>=map->size.Y)
		return 1; // край карты-не рассматриваем
	if(map->matr[Y][X].ch != type_p_stone && map->matr[Y][X].ch != type_p_diamond )
		return 1; // это не камень и не кристалл - вышли из шага рекурсии
	if( !(is_grass(map, X, Y+1) || // можно упасть вниз
		is_grass(map, X-1, Y)&&
			is_grass(map, X-1, Y+1)&&
			(map->matr[Y+1][X].ch == type_p_stone || map->matr[Y+1][X].ch == type_p_diamond)|| // можно упасть влево и камень стоит на камне
		is_grass(map, X+1, Y)&&
			is_grass(map, X+1, Y+1)&&
			(map->matr[Y+1][X].ch == type_p_stone || map->matr[Y+1][X].ch == type_p_diamond)) ) // можно упасть вправо и камень стоит на камне
		return 1; // камню некуда упасть - вышли из шага рекурсии
	if(stone_in_q(q_stone, X, Y)) // камень уже участвует в падении
		return 1;
	s_stone temp = {map->matr[Y][X].ch, map->matr[Y][X].color, {X, Y}, NULL}; // для информации
	if(!add_stone_in_end(q_stone, create_stone(&temp)))
		return 0;
	map->matr[Y][X].ch = type_p_grass; // будто бы данный камень упал
	if(!rec_add_in_q(q_stone,map,X,Y-1))
		return 0;
	if(!rec_add_in_q(q_stone,map,X-1,Y-1))
		return 0;
	if(!rec_add_in_q(q_stone,map,X+1,Y-1))
		return 0;
	return 1;
}

// удаление из очереди камней, достигших низа, с начала, пока не встретится свободный камень
void del_from_q_stone(s_q_stone *q_stone, s_map *map)
{
	while(q_stone->head)
	{
		int X = q_stone->head->pos.X, Y = q_stone->head->pos.Y;
		// снизу свободно
		if(is_grass(map, X, Y+1))
			if(map->matr[Y+1][X].pl)
				if(q_stone->head->ch == type_p_diamond)
					return;
				else; // это не алмаз но стоит игрок
			else // это клетка травы без игрока
				return;
		// камень на камне
		if( map->matr[Y+1][X].ch == type_p_stone || map->matr[Y+1][X].ch == type_p_diamond )
		{
			// слева свободно
			if(is_grass(map, X-1, Y)&&is_grass(map, X-1, Y+1))
				if(!map->matr[Y+1][X-1].pl) // слева игрока нет
					if(map->matr[Y][X-1].pl) // игрок слева-снизу
						if(q_stone->head->ch == type_p_diamond)
								return;
							else; // это не алмаз, но стоит игрок
						else // это клетка травы без игрока
							return;
			// справа свободно
			if(is_grass(map, X+1, Y)&&is_grass(map, X+1, Y+1))
				if(!map->matr[Y+1][X+1].pl) // справа игрока нет
					if(map->matr[Y][X+1].pl) // игрок справа-снизу
						if(q_stone->head->ch == type_p_diamond)
								return;
							else; // это не алмаз, но стоит игрок
						else // это клетка травы без игрока
							return;
		}
		s_stone *cur = q_stone->head; // запомнили для освобождения памяти
		map->matr[Y][X].ch = cur->ch; // нарисовали на карте
		q_stone->head = cur->next; // убрали из очереди
		free(cur); // освобождение памяти
	}
	// в очереди ничего нет
	q_stone->head = q_stone->tail = NULL;
}

// очистка всей очереди
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

// добавление крисалла игроку и изменение на карте
int player_get_diamond(s_player *player, s_map *map)
{
	int X=player->pos.X, Y=player->pos.Y;
	if(map->matr[Y][X].ch == type_p_diamond)
	{
		player->diamonds++;
		map->diamonds--;
		map->matr[Y][X].ch = type_p_grass;
	}
	return 1;
}

// удалить элемент очереди
void del_1_stone(s_q_stone *q_stone, s_stone *stone)
{
	if(!stone)
		return; // нечего удалять
	if(stone == q_stone->head)
	{
		if(q_stone->tail == stone) // в очереди всего один элемент
			q_stone->head = q_stone->tail = NULL;
		else
			q_stone->head = stone->next;
		free(stone);
		return;
	}
	s_stone *prev = q_stone->head;
	while(prev && prev->next != stone)
		prev = prev->next; // находим предыдущий элемент
	if(!prev)
		return; // данный элемент не находится в очереди
	if(stone==q_stone->tail) // елси удаляем последний камень - смещаем q_stone->tail
		q_stone->tail = prev;
	prev->next = stone->next;
	free(stone);
}

// передвижение камней, если снизу свободно
int move_stone_down(s_q_stone *q_stone, s_map *map, s_player *player, s_stone *cur, short *X, short *Y)
{
	if(is_grass(map, *X, *Y+1)) // снизу трава
	{
		if(map->matr[*Y+1][*X].pl) // снизу игрок
		{
			if(map->matr[*Y][*X].ch==type_p_diamond) // камень - кристалл
			{
				map->matr[*Y][*X].ch = type_p_grass;
				(*Y)++;
				map->matr[*Y][*X].ch = type_p_diamond;
				player_get_diamond(player, map);
				del_1_stone(q_stone, cur);
			}
		}
		else // снизу пусто
		{
			map->matr[*Y][*X].ch = type_p_grass;
			cur->pos.Y++;
			map->matr[*Y][*X].ch = cur->ch;
		}
		return 1;
	}
	return 0;
}

// передвижение камней, если снизу-слева свободно
int move_stone_down_left(s_q_stone *q_stone, s_map *map, s_player *player, s_stone *cur, short *X, short *Y)
{
	if(map->matr[*Y+1][*X].ch == type_p_stone || map->matr[*Y+1][*X].ch == type_p_diamond) // камень на камне
		if(is_grass(map, int(*X)-1, int(*Y)) && !map->matr[*Y][*X-1].pl) // слева ничего нет
		{
			if(is_grass(map, *X-1, *Y+1)) // слева-снизу трава
			{
				if(map->matr[*Y+1][*X-1].pl) // слева-снизу игрок
				{
					if(map->matr[*Y][*X].ch == type_p_diamond) // камень - кристалл
					{
						map->matr[*Y][*X].ch = type_p_grass;
						(*Y)++;
						(*X)--;
						map->matr[*Y][*X].ch = type_p_diamond;
						player_get_diamond(player, map);
						del_1_stone(q_stone, cur);
					}
				}
				else // слева-снизу пусто клетка
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

// передвижение камней, если снизу-справа свободно
int move_stone_down_right(s_q_stone *q_stone, s_map *map, s_player *player, s_stone *cur, short *X, short *Y)
{
	if(map->matr[*Y+1][*X].ch == type_p_stone || map->matr[*Y+1][*X].ch == type_p_diamond) // камень на камне
		if(is_grass(map, int(*X)+1, int(*Y)) && !map->matr[*Y][*X+1].pl) // справа ничего нет
			if(is_grass(map, *X+1, *Y+1)) // справа-снизу трава
			{
				if(map->matr[*Y+1][*X+1].pl) // справа-снизу игрок
				{
					if(map->matr[*Y][*X].ch == type_p_diamond) // камень - кристалл
					{
						map->matr[*Y][*X].ch = type_p_grass;
						(*Y)++;
						(*X)++;
						map->matr[*Y][*X].ch = type_p_diamond;
						player_get_diamond(player, map);
						del_1_stone(q_stone, cur);
					}
				}
				else // справа-снизу пусто клетка
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

// смещение всех камней на 1 шаг
void move_stone(s_q_stone *q_stone, s_map *map, s_player *player)
{
	s_stone *cur=q_stone->head, *next = q_stone->head ? q_stone->head->next : NULL;
	for(;cur;cur=next, next=cur?cur->next:NULL)
	{
		short *X = &(cur->pos.X), *Y = &(cur->pos.Y);
		// снизу свободно
		if(move_stone_down(q_stone, map, player, cur, X, Y))
			continue;
		// слева свободно
		if(move_stone_down_left(q_stone, map, player, cur, X, Y))
			continue;
		// справа свободно
		if(move_stone_down_right(q_stone, map, player, cur, X, Y))
			continue;
	}
}

// толкаем камень. движется только камень.
int push_stone(s_map *map, direction dir, s_player *player, s_q_stone *q_stone)
{
	s_stone *stone = NULL; // указатель на камень в очереди камней
	switch(dir)
	{
	case left:
		if(!is_stone(map, player->pos.X-1, player->pos.Y)) // это не камень - выходим
			return 0;
		if(!can_i_move( dir, map, player->pos.X, player->pos.Y)) // камень нельзя передвинуть - выходим
			return 0;
		stone = stone_in_q(q_stone, player->pos.X-1, player->pos.Y);
		map->matr[player->pos.Y][player->pos.X-1].ch = type_p_grass;
		map->matr[player->pos.Y][player->pos.X-2].ch = type_p_stone;
		if(stone)
			stone->pos.X--;
		break;
	case right:
		if(!is_stone(map, player->pos.X+1, player->pos.Y)) // это не камень - выходим
			return 0;
		if(!can_i_move( dir, map, player->pos.X, player->pos.Y)) // камень нельзя передвинуть - выходим
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

// запустить процесс добавления камней вокруг игрока. вернёт 0, если нехватает памяти.
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

// создать список камней, совпадающий значениями с текущим
int copy_q_stone(s_q_stone *q_stone_1, s_q_stone *q_stone_2)
{
	for(s_stone *cur = q_stone_1->head; cur; cur=cur->next)
		if( !add_stone_in_end(q_stone_2, create_stone(cur)) )
			return 0;
	return 1;
}
