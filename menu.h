#ifndef _MENU_H
#define _MENU_H

#include "utilits.h"
#include "preparation.h"
#include "print.h"
#include <Windows.h>
#include <time.h>

// великая победа
int great_victory(s_map *map, s_player *player);

// выбрать уровень из доступных для прохождения
int choose_passed_level(int *level);

// играть в выбранный уровень (доступный для прохождения)
int play_choose_level(int *level, 
	s_map *map,
	s_all_colors *all_colors,
	s_player *player,
	COORD *screen_pos,
	s_q_stone *q_stone,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone);

// играть: продолжить игру. вернёт 0, если возникла ошибка
int play_continue_game(int *level, 
	s_map *map,
	s_all_colors *all_colors,
	s_player *player,
	COORD *screen_pos,
	s_q_stone *q_stone,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone);

// выбор команды меню. если ничего не выбрано, вернёт -1
int choose_menu_commands(commands *menu_commands);

// выполняется команда меню. вернёт 0, если произошла ошибка.
int menu(int *level,
	s_map *map,
	s_all_colors *all_colors,
	s_player *player,
	COORD *screen_pos,
	s_q_stone *q_stone,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone);

// выполнить действия над клеткой, на которой стоит игрок - взять алмаз, стереть куст...
int player_on_cell(s_map *map,
	s_player *player,
	s_q_stone *q_stone, 
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone);

// нажатие клавиши
int press_bottom(s_map *map,
	s_player *player,
	s_q_stone *q_stone,
	s_map *save_map,
	s_player *save_player,
	s_q_stone *save_q_stone);

// процесс игры. игра завершается, когда игрок наступит на клетку выхода или нажмёт ESC
int game_process(s_map *map,
	s_player *player, 
	COORD *screen_pos, 
	s_q_stone *q_stone, 
	s_map *save_map, 
	s_player *save_player, 
	s_q_stone *save_q_stone);

#endif // _MENU_H