#include "menu.h"

// великая победа
int great_victory(s_map *map, s_player *player, int is_change_statistic)
{
	// игрок не на выходе - не великая победа
	if(!is_exit(map, player->pos.X, player->pos.Y))
		return 0;
	// игрок собрал не все кристаллы - не великая победа
	if(map->diamonds > 0)
		return 0;
	int level = 1;
	if(!get_level_from_file(&level))
		return 1;
	// не надо изменять статистику игрока
	if(!is_change_statistic)
		return 1;
	level = MIN(level+1, MAX_LEVEL);
	FILE *fuser = fopen(USER_NAME, "w");
	if(!fuser)
		return err(FILE_NOT_FOUND);
	fprintf(fuser, "%d\n", level);
	fclose(fuser);
	print_great_victory();
	return 1;
}

// выбрать уровень из доступных для прохождения
int choose_passed_level(int *level)
{
	 // выбираем уровень. начинаем с 1
	// при действиях прибавляется 1, так что cur_level = [1;MAX_LEVEL]
	int cur_level = 1;
	char bottom = 0; // кнопка
	while(1)
	{
		system("cls");
		special_bottom(&bottom);
		switch(bottom)
		{
		case up:
			// вычитание по модулю [1;MAX_LEVEL]
			cur_level = cur_level-1 < 1 ? MAX_LEVEL : cur_level-1 ;
			break;
		case down:
			// сложение по модулю [1:MAX_LEVEL]
			cur_level = cur_level+1 > MAX_LEVEL ? 1 : cur_level+1 ;
			break;
		case ENTER:
			if(cur_level>*level)
				break;
			return *level = cur_level;
		case ESC:
			return -1; // вышли без выбора уровня
		default:
			break;
		}
			while(_kbhit()) // пока не отпущена кнопка, считывать как одно нажатие
			_getch();
		print_choose_level(*level, cur_level);
		bottom = _getch();
	}
	return *level;
}

// играть в выбранный уровень (доступный для прохождения)
int play_choose_level(int *level, 
	s_map *map,
	s_all_colors *all_colors,
	s_player *player,
	COORD *screen_pos,
	s_q_stone *q_stone,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone)
{
	if(!get_level_from_file(level))
		return 0;
	if(choose_passed_level(level)==-1) // нажали ESC - выйти из функции
		return 1;
	// подготовка к игре - открытие файлов, выбор (?) уровня
	if(!preparation(level, map, all_colors, player, save_map))
		return 0;
	// процесс проходения карты
	game_process(map, player, screen_pos, q_stone, save_map, save_player, save_q_stone);
	// вывести сообщение о завершении игры
	if(!great_victory(map, player, 0))
		game_over();
	// освобождение использованной памяти
	free_all(map, save_map, q_stone, save_q_stone);
	return 1;
}

// играть: продолжить игру. вернёт 0, если возникла ошибка
int play_continue_game(int *level, 
	s_map *map,
	s_all_colors *all_colors,
	s_player *player,
	COORD *screen_pos,
	s_q_stone *q_stone,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone)
{
	if(!get_level_from_file(level))
		return 0;
	// подготовка к игре - открытие файлов, выбор (?) уровня
	if(!preparation(level, map, all_colors, player, save_map))
		return 0;
	// процесс проходения карты
	game_process(map, player, screen_pos, q_stone, save_map, save_player, save_q_stone);
	// вывести сообщение о завершении игры
	if(!great_victory(map, player, 1))
		game_over();
	// освобождение использованной памяти
	free_all(map, save_map, q_stone, save_q_stone);
	return 1;
}

// выбор команды меню. если ничего не выбрано, вернёт -1
int choose_menu_commands(commands *menu_commands)
{
	int cur = continue_game;
	char bottom = 0; // кнопка
	do
	{
		system("cls");
		special_bottom(&bottom);
		switch(bottom)
		{
		case up:
			// вычитание по модулю COUNT_MENU_COMMANDS
			cur = (cur - 1 + COUNT_MENU_COMMANDS) % COUNT_MENU_COMMANDS;
			break;
		case down:
			// сложение по модулю COUNT_MENU_COMMANDS
			cur = (cur + 1 + COUNT_MENU_COMMANDS) % COUNT_MENU_COMMANDS;
			break;
		case ENTER:
			return cur;
		default:
			break;
		}
		while(_kbhit()) // пока не отпущена кнопка, считывать как одно нажатие
			_getch();
		print_choose_menu_commands(menu_commands, (commands)cur);
	}while( (bottom = _getch()) != ESC );
	return exit_game; // нажата ESC - вышли из игры
}

// выполняется команда меню. вернёт 0, если произошла ошибка.
int menu(int *level, 
	s_map *map,
	s_all_colors *all_colors,
	s_player *player,
	COORD *screen_pos,
	s_q_stone *q_stone,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone)
{
	commands menu_commands[COUNT_MENU_COMMANDS] = {continue_game,new_game,choose_level,exit_game};
	while(1) // пока не вышли из игры
	{
		system("color 07");
		int comand = choose_menu_commands(menu_commands);
		switch(comand)
		{
		case continue_game:
			if(!play_continue_game(level, map, all_colors, player, screen_pos, q_stone, save_map, save_player, save_q_stone))
				return 0;
			break;
		case new_game:
			if(!new_user())
				return 0;
			if(!play_continue_game(level, map, all_colors, player, screen_pos, q_stone, save_map, save_player, save_q_stone))
				return 0;
			break;
		case choose_level:
			if(!play_choose_level(level, map, all_colors, player, screen_pos, q_stone, save_map, save_player, save_q_stone))
				return 0;
			break;
		case exit_game:
			return 1;
		default:
			return err(INCORRECT_VALUE);
		}
	}
	return 1;
}


// выполнить действия над клеткой, на которой стоит игрок - взять алмаз, стереть куст...
int player_on_cell(s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone)
{
	if(is_exit(map, player->pos.X, player->pos.Y))
		return -1;
	if(is_checkpoint(map, player->pos.X, player->pos.Y))
		if(!save_on_checkpoint(map, player, q_stone, save_map, save_player, save_q_stone))
			return -1;
	if(is_bush(map, player->pos.X, player->pos.Y))
		map->matr[player->pos.Y][player->pos.X].ch = type_p_grass;
	player_get_diamond(player, map);
	del_1_stone(q_stone, stone_in_q(q_stone, player->pos.X, player->pos.Y));
	return 1;
}

// нажатие клавиши
int press_bottom(s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone)
{
	char bottom = 0; // нажатая кнопка
	if(_kbhit()) //если нажали клавишу
	{
		if((bottom =_getch())==ESC) // считать клавишу. проверить на досрочный выход
			return 1;
		special_bottom(&bottom); // если нажали стрелку, перевести её в направление
		while(_kbhit()) // пока не отпущена кнопка, считывать как одно нажатие
			_getch();
		command_in_game(bottom, map, player, q_stone, save_map, save_player, save_q_stone);	
	}
	if(player->lives<0)
		return 1;
	if(player_on_cell(map, player, q_stone, save_map, save_player, save_q_stone)==-1) // выход
		return 1;
	if(!add_in_q_around_player(map, player, q_stone))
		return 1; // нехватка памяти - выходим
	return 0;
}

// процесс игры. игра завершается, когда игрок наступит на клетку выхода или нажмёт ESC
int game_process(s_map *map, s_player *player, COORD *screen_pos, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone)
{
	int now_time = clock(); // текущее время
	while(1)
	{
		// обработка нажатия клавиши
		if(press_bottom(map, player, q_stone, save_map, save_player, save_q_stone)) // если игрок на выходе - выйти из функции
			return 1;
		if(clock()-now_time>250) // обновление раз в четверть секунды
		{
			screen_position(screen_pos, player, map); // обновить позицию прорисовки экрана
			now_time = clock(); // обновить время
			move_stone(q_stone, map, player); // передвинуть камни
			del_from_q_stone(q_stone, map); // удалить камни, не участвующие в падении
			system("cls"); // обновить экран
			print_map(map, screen_pos, player); // нарисовать карту
			printf("\nDiamonds = %d / %d\nLives = %d", player->diamonds, map->diamonds, player->lives); // статистика
		}
	}
	return 0; // нажали ESC - досрочное завершение программы
}