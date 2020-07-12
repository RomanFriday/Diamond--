#include "utilits.h"

// ����� ��������� �� ���� ������. ������ ���������� 0
int err(int type)
{
	switch(type)
	{
	case RAM_IS_OVER:
		printf("\nRAM is over!\n");
		break;
	case FILE_NOT_FOUND:
		printf("\nError 404: file not found\n");
		break;
	case NO_ENOUGH_DATA:
		printf("\nNo enough data...\n");
		break;
	case INCORRECT_VALUE:
		printf("\nIncorrect value\n");
		break;
	default: return 0;
	}
	system("pause");
	return 0;
}

// ������� �������� ����� �� ���� ����� � ��� ������
int type_char_to_print(char *c)
{
	switch((int(*c)+256)%256)
	{
	case type_bush:
		*c = type_p_bush;
		return 1;
	case type_exit:
		*c = type_p_exit;
		return 1;
	case type_grass:
		*c = type_p_grass;
		return 1;
	case type_stone:
		*c = type_p_stone;
		return 1;
	case type_wall:
		*c = type_p_wall;
		return 1;
	case type_diamond:
		*c = type_p_diamond;
		return 1;
	case type_checkpoint:
		*c = type_p_checkpoint;
		return 1;
	default:
		return err(INCORRECT_VALUE);
	};
	return 0;
}

// ������� ���� �������� ����� �� ���� ����� � ��� ������
int map_characters_to_print(s_map *map)
{
	for(int i=0; i<map->size.X*map->size.Y; i++)
		if( !type_char_to_print( &(map->characters[i]) ) )
			return 0;
	return 1;
}

// ������� ������ ����� � ������� �������� �����
int str2color(char *str)
{
	if(!strcmp(str, "Black\n"))
		return Black;
	if(!strcmp(str, "Blue\n"))
		return Blue;
	if(!strcmp(str, "Green\n"))
		return Green;
	if(!strcmp(str, "Cyan\n"))
		return Cyan;
	if(!strcmp(str, "Red\n"))
		return Red;
	if(!strcmp(str, "Magenta\n"))
		return Magenta;
	if(!strcmp(str, "Brown\n"))
		return Brown;
	if(!strcmp(str, "LightGray\n"))
		return LightGray;
	if(!strcmp(str, "DarkGray\n"))
		return DarkGray;
	if(!strcmp(str, "LightBlue\n"))
		return LightBlue;
	if(!strcmp(str, "LightGreen\n"))
		return LightGreen;
	if(!strcmp(str, "LightRed\n"))
		return LightRed;
	if(!strcmp(str, "LightCyan\n"))
		return LightCyan;
	if(!strcmp(str, "LightMagenta\n"))
		return LightMagenta;
	if(!strcmp(str, "Yellow\n"))
		return Yellow;
	if(!strcmp(str, "White\n"))
		return White;
	return err(INCORRECT_VALUE);
}

// ������� ������ ����� � ������� �������� �����, ������ ������ �� �����
int str2color_from_file(unsigned short *parametr, FILE **fin)
{
	char str_f[MAX_STR_LENGHT]={0,}; // ���� ��������� ����� � ���� ������
	if(!fgets(str_f, MAX_STR_LENGHT, *fin))
	{
		fclose(*fin);
		return err(NO_ENOUGH_DATA);
	}
	*parametr = str2color(str_f);
	return 1; // ���� ��������� ����� �� ���� �������
}

// ������ �����
int reverse_int(int n)
{
	int rev = 0;
	for(int temp=n>0 ? n : -n; temp>0; temp/=10)
		rev = rev*10+(temp%10);
	return n>0 ? rev : -rev;
}

// ������� ����� � ������
void int2str(int n, char str[], int lenght)
{
	int i = n>0 ? 0 : 1,
		rev = reverse_int(n>0 ? n : -n);
	if(i)
		str[0] = '-';
	for(; i<lenght && rev>0; i++, rev/=10)
		str[i]=rev%10+'0';
	return;
}

// ������� ��������� � �����������
int special_bottom(char *bottom)
{
	if(*bottom == -32 || *bottom == 224)
		*bottom = _getch();
	else
		return 0;
	switch(*bottom)
	{
	case 72:
		*bottom = up;
		break;
	case 80:
		*bottom = down;
		break;
	case 75:
		*bottom = left;
		break;
	case 77:
		*bottom = right;
		break;
	case DEL:
		*bottom = DEL;
		break;
	default: 0;
	}
	return 1;
}

// ����������� map (����� ������ �������!)
int copy_map(s_map *map1, s_map *map2)
{
	if(!map1->matr || !map2->matr)
		return 0;
	map2->size = map1->size;
	map2->diamonds = map1->diamonds;
	for(int i=0; i<map1->size.Y; i++)
		for(int j=0; j<map1->size.X; j++)
			map2->matr[i][j] = map1->matr[i][j];
	return 1;
}

// ����������� �� ���������
int save_on_checkpoint(s_map* map, s_player* player, s_q_stone* q_stone, s_enemy** first_enemy,
					s_map* save_map, s_player* save_player, s_q_stone* save_q_stone, s_enemy** save_first_enemy)
{
	copy_map(map, save_map);
	*save_player = *player;
	q_stone_clear(save_q_stone); // ������� ������ ��������
	if(!copy_q_stone(q_stone, save_q_stone))
		return 0;
	delete_all_enemys(save_first_enemy);
	if (!copy_enemys(first_enemy, save_first_enemy))
		return 0;
	if(!set_enenemys(map, first_enemy))
		return 0;
	return 1;
}

// ������� �� ���������� � ��������� player->lives �� 1
int go_to_checkpoint(s_map *map, s_player *player, s_q_stone *q_stone, s_enemy **first_enemy,
					s_map *save_map, s_player *save_player, s_q_stone *save_q_stone, s_enemy **save_first_enemy)
{
	copy_map(save_map, map);
	*player = *save_player;
	q_stone_clear(q_stone); // ������� ������ ��������
	if(!copy_q_stone(save_q_stone, q_stone))
		return 0;
	delete_all_enemys(first_enemy);
	if (!copy_enemys(save_first_enemy, first_enemy))
		return 0;
	if(!set_enenemys(map, first_enemy))
		return 0;
	player->lives--;
	return 1;
}

// ��������� ������� �� ������� �������
void command_in_game(char bottom, s_map* map, s_player* player, s_q_stone* q_stone, s_enemy** first_enemy,
			s_map* save_map, s_player* save_player, s_q_stone* save_q_stone, s_enemy** save_first_enemy)
{
	switch(bottom)
	{
	case up:
		move_up(map, player);
		return;
	case down:
		move_down(map, player);
		return;
	case right:
		move_right(map, player, q_stone);
		return;
	case left:
		move_left(map, player, q_stone);
		return;
	case DEL:
		go_to_checkpoint(map, player, q_stone, first_enemy,
						save_map, save_player, save_q_stone, save_first_enemy);
		return;
	default: return;
	}
}

// �������� s_cell - ������� ��������� m �� n
int create_s_cell_matrix(s_cell ***matrix, int m, int n)
{
	if( !((*matrix)=(s_cell**)malloc(sizeof(s_cell*)*m)) )
		return err(RAM_IS_OVER);
	for(int i=0; i<m; i++)
		if( !((*matrix)[i]=(s_cell*)malloc(sizeof(s_cell)*n)) )
			return err(RAM_IS_OVER) * free_s_cell_matrix(matrix, i);
	return 1;
}

// ��������� ������ ������������ ������
void screen_position(COORD *screen_pos, s_player *player, s_map *map)
{
	// ������� ���������� ������ ����
	int size_X = MIN(MAX_MAP_SCREEN_X, map->size.X);
	int size_Y = MIN(MAX_MAP_SCREEN_Y, map->size.Y);
	COORD pos = {player->pos.X-(size_X/2), player->pos.Y-(size_Y/2)};
	if(pos.X>=map->size.X-size_X)
		pos.X = map->size.X-size_X;
	if(pos.Y>=map->size.Y-size_Y)
		pos.Y = map->size.Y-size_Y;
	if(pos.X<0)
		pos.X = 0;
	if(pos.Y<0)
		pos.Y = 0;
	*screen_pos = pos;
}

// ���������� ������ ������� � ����� s_cell. �������� ���������� �����. ������ ���������� 0
int free_s_cell_matrix(s_cell ***matrix, int size_of_strings)
{
	for(int i=0; i<size_of_strings; i++)
	{
		free((*matrix)[i]);
		(*matrix)[i] = NULL;
	}
	free(*matrix);
	*matrix=NULL;
	return 0;
}

// �������� ��� ������, ��� ��������. ���������� 0
int free_all(s_map *map, s_map *save_map, s_q_stone *q_stone, s_q_stone *save_q_stone,
	s_enemy** first_enemy, s_enemy** save_first_enemy)
{
	q_stone_clear(q_stone);
	q_stone_clear(save_q_stone);
	delete_all_enemys(first_enemy);
	delete_all_enemys(save_first_enemy);
	if(map->characters)
		free(map->characters);
	if(map->colors)
		free(map->colors);
	if(map->matr)
		free_s_cell_matrix(&map->matr, map->size.Y);
	if(save_map->matr)
		free_s_cell_matrix(&save_map->matr, save_map->size.Y);
	return 0;
}

// ������ ����� ���� - � ���� �������� �������� �������� ������ = 1
// ����� 0, ���� ���� �� ������
int new_user()
{
	FILE *fuser = fopen(USER_NAME, "w");
	if(!fuser)
		return err(FILE_NOT_FOUND);
	fprintf(fuser, "1\n");
	fclose(fuser);
	return 1;
}


