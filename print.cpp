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
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if(pl->pos.X == j && pl->pos.Y == i)
	{
		col(Green, pl->color);
		printf("%c", pl->ch);
		col(Black, White);
		return 1;
	}
	return 0;
	
}

// нарисовать врага
int print_enemy(s_map* map, int X, int Y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (map->matr[Y][X].en)
	{
		col(Green, LightRed);
		switch(map->matr[Y][X].en->d)
		{
		case up:
			printf("%c", 24);
			col(Black, White);
			return 1;
		case down:
			printf("%c", 25);
			col(Black, White);
			return 1;
		case left:
			printf("%c", 27);
			col(Black, White);
			return 1;
		case right:
			printf("%c", 26);
			col(Black, White);
			return 1;
		default:
			return 0;
		}
	}
	return 0;
}

// нарисовать клетку карты (трава, стена или камень)
void print_cell(s_map *map, int i, int j)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch((map->matr[i][j].ch+256)%256)
	{
	case type_p_grass:
		col(Green, Green);
		break;
	case type_p_bush:
		col(Green, Brown);
		break;
	case type_p_stone:
		col(Green, LightGray);
		break;
	case type_p_diamond:
		col(Green, LightCyan);
		break;
	case type_p_wall:
		col(LightGray, DarkGray);
		break;
	case type_p_exit:
		col(Green, White);
	case type_p_checkpoint:
		col(Green, Blue);
		break;
	default: 
		break;
	}
	printf("%c", map->matr[i][j].ch);
	col(Black, White);
}

// рисование части карты.
// На вход подаются карта и координаты начала отображения карты
void print_map(s_map *map, COORD *screen_pos, s_player *player, s_enemy** first_enemy)
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
			if (print_enemy(map, j, i))
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

// великая победа - вывод на экран
int print_great_victory()
{
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

// вывести на экран уровни для прохождения, или X, если уровень недоступен
void print_choose_level(int max_passed_level, int cur_level)
{
	for(int i=1; i<=MAX_LEVEL; i++)
	{
		printf("\n %c ", i==cur_level ? 26 : ' '); // если это текущий уровень, показываем стрелкой
		if(i<=max_passed_level)
			printf("%d", i);
		else
			printf("XXX"); // уровень недоступен для прохождения
		printf(" %c\n", i==cur_level ? 27 : ' '); // если это текущий уровень, показываем стрелкой
	}
}

// вывести команду меню
void print_menu_command(commands cur)
{
	switch(cur)
	{
	case continue_game:
		printf("continue game");
		break;
	case new_game:
		printf("new game");
		break;
	case choose_level:
		printf("choose level");
		break;
	case exit_game:
		printf("exit game");
		break;
	default : 
		err(INCORRECT_VALUE);
		break;
	}
	return;
}

// вывести на экран команды меню.
// размеры не передаются, т.к. указаны в declarations.h как константы.
// cur - номер текущей команды
void print_choose_menu_commands(commands menu_commands[], commands cur)
{
	for(int i=0; i<COUNT_MENU_COMMANDS; i++)
	{
		printf("\n %c ", i==cur ? 26 : ' '); // если это текущая команда, показываем стрелкой
		print_menu_command((commands)i);
		printf(" %c\n", i==cur ? 27 : ' '); // если это текущая команда, показываем стрелкой
	}
}
