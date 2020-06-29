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
#include "menu.h"
#include "moving.h"
#include "preparation.h"
#include "print.h"
#include "stones_and_diamonds.h"
#include "utilits.h"


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
	menu(&level, &map, &all_colors, &player, &screen_pos, &q_stone, &save_map, &save_player, &save_q_stone);
	
	return 0;
}
