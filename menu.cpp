#include "menu.h"

// ������� ������
int great_victory(s_map *map, s_player *player, int is_change_statistic)
{
	// ����� �� �� ������ - �� ������� ������
	if(!is_exit(map, player->pos.X, player->pos.Y))
		return 0;
	// ����� ������ �� ��� ��������� - �� ������� ������
	if(map->diamonds > 0)
		return 0;
	int level = 1;
	if(!get_level_from_file(&level))
		return 1;
	// �� ���� �������� ���������� ������
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

// ������� ������� �� ��������� ��� �����������
int choose_passed_level(int *level)
{
	 // �������� �������. �������� � 1
	// ��� ��������� ������������ 1, ��� ��� cur_level = [1;MAX_LEVEL]
	int cur_level = 1;
	char bottom = 0; // ������
	while(1)
	{
		system("cls");
		special_bottom(&bottom);
		switch(bottom)
		{
		case up:
			// ��������� �� ������ [1;MAX_LEVEL]
			cur_level = cur_level-1 < 1 ? MAX_LEVEL : cur_level-1 ;
			break;
		case down:
			// �������� �� ������ [1:MAX_LEVEL]
			cur_level = cur_level+1 > MAX_LEVEL ? 1 : cur_level+1 ;
			break;
		case ENTER:
			if(cur_level>*level)
				break;
			return *level = cur_level;
		case ESC:
			return -1; // ����� ��� ������ ������
		default:
			break;
		}
			while(_kbhit()) // ���� �� �������� ������, ��������� ��� ���� �������
			_getch();
		print_choose_level(*level, cur_level);
		bottom = _getch();
	}
	return *level;
}

// ������ � ��������� ������� (��������� ��� �����������)
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
	if(choose_passed_level(level)==-1) // ������ ESC - ����� �� �������
		return 1;
	// ���������� � ���� - �������� ������, ����� (?) ������
	if(!preparation(level, map, all_colors, player, save_map))
		return 0;
	// ������� ���������� �����
	game_process(map, player, screen_pos, q_stone, save_map, save_player, save_q_stone);
	// ������� ��������� � ���������� ����
	if(!great_victory(map, player, 0))
		game_over();
	// ������������ �������������� ������
	free_all(map, save_map, q_stone, save_q_stone);
	return 1;
}

// ������: ���������� ����. ����� 0, ���� �������� ������
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
	// ���������� � ���� - �������� ������, ����� (?) ������
	if(!preparation(level, map, all_colors, player, save_map))
		return 0;
	// ������� ���������� �����
	game_process(map, player, screen_pos, q_stone, save_map, save_player, save_q_stone);
	// ������� ��������� � ���������� ����
	if(!great_victory(map, player, 1))
		game_over();
	// ������������ �������������� ������
	free_all(map, save_map, q_stone, save_q_stone);
	return 1;
}

// ����� ������� ����. ���� ������ �� �������, ����� -1
int choose_menu_commands(commands *menu_commands)
{
	int cur = continue_game;
	char bottom = 0; // ������
	do
	{
		system("cls");
		special_bottom(&bottom);
		switch(bottom)
		{
		case up:
			// ��������� �� ������ COUNT_MENU_COMMANDS
			cur = (cur - 1 + COUNT_MENU_COMMANDS) % COUNT_MENU_COMMANDS;
			break;
		case down:
			// �������� �� ������ COUNT_MENU_COMMANDS
			cur = (cur + 1 + COUNT_MENU_COMMANDS) % COUNT_MENU_COMMANDS;
			break;
		case ENTER:
			return cur;
		default:
			break;
		}
		while(_kbhit()) // ���� �� �������� ������, ��������� ��� ���� �������
			_getch();
		print_choose_menu_commands(menu_commands, (commands)cur);
	}while( (bottom = _getch()) != ESC );
	return exit_game; // ������ ESC - ����� �� ����
}

// ����������� ������� ����. ����� 0, ���� ��������� ������.
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
	while(1) // ���� �� ����� �� ����
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


// ��������� �������� ��� �������, �� ������� ����� ����� - ����� �����, ������� ����...
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

// ������� �������
int press_bottom(s_map *map, s_player *player, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone)
{
	char bottom = 0; // ������� ������
	if(_kbhit()) //���� ������ �������
	{
		if((bottom =_getch())==ESC) // ������� �������. ��������� �� ��������� �����
			return 1;
		special_bottom(&bottom); // ���� ������ �������, ��������� � � �����������
		while(_kbhit()) // ���� �� �������� ������, ��������� ��� ���� �������
			_getch();
		command_in_game(bottom, map, player, q_stone, save_map, save_player, save_q_stone);	
	}
	if(player->lives<0)
		return 1;
	if(player_on_cell(map, player, q_stone, save_map, save_player, save_q_stone)==-1) // �����
		return 1;
	if(!add_in_q_around_player(map, player, q_stone))
		return 1; // �������� ������ - �������
	return 0;
}

// ������� ����. ���� �����������, ����� ����� �������� �� ������ ������ ��� ����� ESC
int game_process(s_map *map, s_player *player, COORD *screen_pos, s_q_stone *q_stone, s_map *save_map, s_player *save_player, s_q_stone *save_q_stone)
{
	int now_time = clock(); // ������� �����
	while(1)
	{
		// ��������� ������� �������
		if(press_bottom(map, player, q_stone, save_map, save_player, save_q_stone)) // ���� ����� �� ������ - ����� �� �������
			return 1;
		if(clock()-now_time>250) // ���������� ��� � �������� �������
		{
			screen_position(screen_pos, player, map); // �������� ������� ���������� ������
			now_time = clock(); // �������� �����
			move_stone(q_stone, map, player); // ����������� �����
			del_from_q_stone(q_stone, map); // ������� �����, �� ����������� � �������
			system("cls"); // �������� �����
			print_map(map, screen_pos, player); // ���������� �����
			printf("\nDiamonds = %d / %d\nLives = %d", player->diamonds, map->diamonds, player->lives); // ����������
		}
	}
	return 0; // ������ ESC - ��������� ���������� ���������
}