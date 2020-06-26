//
//    !!!ВНИМАНИЕ!!!
// Для работы с программой установите размеры буфера экрана: Ширина = 100, Высота = 75.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <Windows.h>
#include <time.h>

#include "declarations.h"
#include "is_something.h"
#include "moving.h"
#include "preparation.h"
#include "print.h"
#include "stones_and_diamonds.h"
#include "utilits.h"

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
		command(bottom, map, player, q_stone, save_map, save_player, save_q_stone);	
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

int main()
{
	printf("Diamond-- by Alex, Evgen, POMAH.\n");
	system("pause");
	// объявление переменных, участвующих в игре
	int level = 0, count_diamonds = 0;
	COORD screen_pos = {0,0};
	DWORD dw = 0;
	s_map map = {0,}, save_map = {0,};
	s_all_colors all_colors;
	s_txt_name txt_name = {0,0,0};
	s_player player = { 0,0,{0,0},0,0 }, save_player = { 0,0,{0,0},0,0};
	s_q_stone q_stone = {0,0}, save_q_stone = {0,0};
	// подготовка к игре - открытие файлов, выбор (?) уровня
	if(!preparation(&level, &map, &all_colors, &player, &save_map))
		return 0;
	// процесс проходения карты
	game_process(&map, &player, &screen_pos, &q_stone, &save_map, &save_player, &save_q_stone);
	// вывести сообщение о завершении игры
	if(!great_victory(&map, &player))
		game_over();
	// освобождение использованной памяти
	free_all(&map, &save_map, &q_stone, &save_q_stone);
	return 0;
}
