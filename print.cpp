#include "print.h"

// нарисовать строку, состоящую только из символов c. Если is_new_line!=0, печатает перевод на новую строку.
void print_line(char c, int size, int is_new_line)
{
	for(int i=0; i<size; i++)
		printf("%c", c);
	if(is_new_line)
		printf("\n");
}

// нарисовать игрока на отображаемой части карты
int print_player(COORD *screen_pos, s_player *pl, int i, int j)
{
	if(pl->pos.X == j && pl->pos.Y == i)
	{
		printf("%c", pl->ch);
		return 1;
	}
	return 0;
}

// нарисовать клетку карты (трава, стена или камень)
void print_cell(s_map *map, int i, int j)
{
	printf("%c", map->matr[i][j].ch);
}

// рисование части карты.
// На вход подаются карта и координаты начала отображения карты
void print_map(s_map *map, COORD *screen_pos, s_player *player/*, s_enemies First*/)
{
	// размеры выводимого экрана крты
	int size_X = MIN(MAX_MAP_SCREEN_X, map->size.X);
	int size_Y = MIN(MAX_MAP_SCREEN_Y, map->size.Y);
	// верхняя строка границы выводимого экрана крты
	for(int i=0; i<BORDER_SIZE; i++)
		print_line(BORDER_CHAR, BORDER_SIZE*2+size_X, 1);
	for(int i=screen_pos->Y; i<size_Y+screen_pos->Y; i++)
	{
		// левый край
		print_line(BORDER_CHAR, BORDER_SIZE, 0);
		// символы карты
		for(int j=screen_pos->X; j<size_X+screen_pos->X; j++)
		{ 
			if(print_player(screen_pos, player, i, j))
				continue;
			print_cell(map, i, j);
		}
		// левый край
		print_line(BORDER_CHAR, BORDER_SIZE, 1);
	}
	// нижняя строка границы экрана
	for(int i=0; i<BORDER_SIZE; i++)
		print_line(BORDER_CHAR, BORDER_SIZE*2+size_X, 1);
}

// великая победа
int great_victory(s_map *map, s_player *player)
{
	// игрок не на выходе - не великая победа
	if(!is_exit(map, player->pos.X, player->pos.Y))
		return 0;
	// игрок собрал не все кристаллы - не великая победа
	if(map->diamonds != 0)
		return 0;
	system("cls");
	system("color 9E");
	FILE *f = fopen("great_victory.txt", "r");
	if(!f)
		return err(FILE_NOT_FOUND);
	char c;
	while( (c=fgetc(f))!= EOF )
		printf("%c", c);
	fclose(f);
	system("pause");
	return 1;
}

// завершение игры
int game_over()
{
	system("cls");
	system("color 04");
	FILE *f = fopen("game_over.txt", "r");
	if(!f)
		return err(FILE_NOT_FOUND);
	char c;
	while( (c=fgetc(f))!= EOF )
		printf("%c", c);
	fclose(f);
	system("pause");
	return 1;
}
