#include "preparation.h"

// создание карты из текстового файла
int create_map_characters(s_txt_name txt_name, s_map *map)
{
	FILE *fmap = fopen(txt_name.map, "r");
	if(!fmap)
	return err(FILE_NOT_FOUND);
	// считывание размеров карты
	if (fscanf(fmap, "%d%d", &map->size.X, &map->size.Y) < 2)
		return err(NO_ENOUGH_DATA);
	if (map->size.X < 1 || map->size.Y < 1)
		return err(INCORRECT_VALUE);
	char c = 0;
	int i=0;
	if( !(map->characters = (char*)malloc( map->size.X*map->size.Y*sizeof(char) )) )
	{
		fclose(fmap);
		return err(RAM_IS_OVER);
	}
	while( (c=fgetc(fmap)) != EOF ) // пока не конец файла
	{
		if( (map->characters[i] = c) == '\n' )
			continue;
		i++;
		if( i==map->size.X * map->size.Y ) // карта считана полностью
			break;
	}
	if( i<map->size.X * map->size.Y )
	{
		free(map->characters);
		fclose(fmap);
		return err(NO_ENOUGH_DATA);
	}
	c=fgetc(fmap); // считывание переноса строки
	if(!fscanf(fmap, "%d", &(map->diamonds)))
	{
		free(map->characters);
		fclose(fmap);
		return err(NO_ENOUGH_DATA);
	}
	fclose(fmap);
	return 1;
}

// перенести игрока на карту - матрицу
int set_player_on_matr(s_map *map, s_all_colors all_colors, s_player *player)
{
	if( !is_on_map(map, player->pos.X, player->pos.Y) )
		return err(INCORRECT_VALUE);
	map->matr[player->pos.Y][player->pos.X].pl = player; 
	map->matr[player->pos.Y][player->pos.X].ch = type_p_checkpoint;
	map->characters[player->pos.Y*map->size.X+player->pos.X] = player->ch;
	map->matr[player->pos.Y][player->pos.X].color = all_colors.background << 4 | all_colors.grass;
	map->colors[player->pos.Y*map->size.X+player->pos.X] = player->color;
	return 1;
}

// создание карты-матрицы
int create_map_matr(s_map *map, s_all_colors all_colors, s_player *player /*,s_empty *First*/)
{
	if( !create_s_cell_matrix(&(map->matr), map->size.Y, map->size.X) )
		return 0;
	for(int i=0; i<map->size.Y; i++)
	{
		for(int j=0; j<map->size.X; j++)
		{
			map->matr[i][j].ch = map->characters[map->size.X*i+j];
			map->matr[i][j].color = map->colors[map->size.X*i+j];
			map->matr[i][j].dir = left;
			map->matr[i][j].pl = NULL;
			map->matr[i][j].en = NULL;
		}
	}
	if(!set_player_on_matr(map, all_colors, player))
		return err(INCORRECT_VALUE);
	/*if(!set_enemies_on_matr())
		return 0;*/
	return 1;
}

// создание соответстви€ типу клетки с цветом из текстового файла
int create_all_colors(s_txt_name txt_name, s_all_colors *all_colors)
{
	FILE *fconf = fopen(txt_name.all_colors, "r");
	if(!fconf)
		return err(FILE_NOT_FOUND);
	unsigned short *p = &(all_colors->background);// первое по счЄту поле структуры
	// заполнение полей all_colors
	for(int i=0; i<COUNT_ALL_COLORS; i++, p++) // p++ - перейти к следующему полю структуры s_all_colors
		if(!str2color_from_file(p, &fconf))
			return 0*fclose(fconf); // вернуть ноль и закрыть файл
	fclose(fconf);
	return 1;
}

// создание массива цветов карты
int create_map_colors(s_map *map, s_all_colors all_colors)
{
	if( !(map->colors = (unsigned short*)malloc( sizeof(unsigned short)*map->size.X*map->size.Y )) )
		return err(RAM_IS_OVER);
	for(int i=0; i<map->size.X*map->size.Y; i++)
	{
		switch(int(map->characters[i]))
		{
		case type_bush:
			map->colors[i] = all_colors.background << 4 | all_colors.bush;
			break;
		case type_exit:
			map->colors[i] = all_colors.background << 4 | all_colors.exit;
			break;
		case type_grass:
			map->colors[i] = all_colors.background << 4 | all_colors.grass;
			break;
		case type_stone:
			map->colors[i] = all_colors.background << 4 | all_colors.stone;
			break;
		case type_wall:
			map->colors[i] = all_colors.background << 4 | all_colors.wall;
			break;
		default:
			map->colors[i] = 0;
		};
	}
	return 1;
}

// задание начальных параметров игрока
int create_player( s_txt_name txt_name, s_player* player) 
{
	FILE* fplayer = fopen(txt_name.player, "r");
	if (!fplayer)
		return err(FILE_NOT_FOUND);
	if (!fscanf(fplayer, "%uh", &(player->pos.X)) || !fscanf(fplayer, "%uh", &(player->pos.Y)))
	{
		fclose(fplayer);
		return err(NO_ENOUGH_DATA);
	}
	if (!fscanf(fplayer, "%d", &(player->ch)))
	{
		fclose(fplayer);
		return err(NO_ENOUGH_DATA);
	}
	char exstr[MAX_TXT_NAME];
	fgetc(fplayer);
	if (!fgets(exstr, MAX_TXT_NAME, fplayer))
	{
		fclose(fplayer);
		return err(NO_ENOUGH_DATA);
	}
	player->color=str2color(exstr);
	if (!fscanf(fplayer, "%d", &(player->lives)))
	{
		fclose(fplayer);
		return err(NO_ENOUGH_DATA);
	}
	player->diamonds=0;
	fclose(fplayer);
	return 1;
}

// записать в txt_name имена текстовых файлов, используемых на данном уровне
int get_txt_name(int level, s_txt_name *txt_name)
{
	char txt_level[MAX_TXT_NAME]={'l','e','v','e','l','_',0};
	int2str(level, txt_level+6, MAX_TXT_NAME-11);// -11 = ( 1 символ на '\0', 4 на ".txt", 6 на "level_" )
	strcat(txt_level, ".txt");
	FILE *flevel = fopen(txt_level, "r"); // открыли level_*.txt дл€ чтени€
	if(!flevel)
		return err(FILE_NOT_FOUND);
	for(char *p = txt_name->all_colors, cnt=0; cnt<COUNT_TXT_NAME; cnt++, p+=MAX_TXT_NAME)
	{
		// считываю строку из файла
		if( !(fgets(p, MAX_TXT_NAME, flevel)) )
		{
			fclose(flevel);
			return err(NO_ENOUGH_DATA);
		}
		// удал€ю перенос строки
		for(int i=0; p[i]!='\0'; i++)
			if(p[i]=='\n')
				p[i]=0;
	}
	return 1;
}

// выбор (?) уровн€
int get_level(int *level)
{
	// если сохран€ть статистику, то подключить текстовый файл
	// а также изменить s_txt_name и соответствено COUNT_TXT_NAME
	*level = 1;
	return 1;
}

// подготовка - вз€тие информации из файлов, создание карты
int preparation(int *level, s_map *map, s_all_colors *all_colors, s_player *player, s_map *save_map/* ,s_enemy **first_enemy*/)
{
	// ќ“ ”ƒј ¬«я“№ ”–ќ¬≈Ќ№???
	if(!get_level(level))
		return 0;
	// названи€ нужных файлов
	s_txt_name txt_name={0,0,0};
	if(!get_txt_name(1, &txt_name))
		return 0;
	if(!create_all_colors(txt_name, all_colors))
		return 0;
	if( !create_map_characters(txt_name, map) )
		return 0;
	if( !create_map_colors(map, *all_colors) )
		return free_all(map, save_map, NULL, NULL);
	if( !map_characters_to_print(map) )
		return free_all(map, save_map, NULL, NULL);
	if( !create_player(txt_name, player))
		return free_all(map, save_map, NULL, NULL);
	if( !create_map_matr(map, *all_colors, player))
		return free_all(map, save_map, NULL, NULL);
	map->matr[player->pos.Y][player->pos.X].pl = player;// установка игрока на карту
	if(!create_s_cell_matrix(&(save_map->matr), map->size.Y, map->size.X))
		return free_all(map, save_map, NULL, NULL);;
	return 1;
}