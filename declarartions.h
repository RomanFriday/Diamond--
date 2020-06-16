#ifndef _DECLARATIONS_H
#define _DECLARATIONS_H

// размеры экрана
#define MAX_SCREEN_X 100
#define MAX_SCREEN_Y 75
// размеры экрана карты : на экран выводится не вся карта, а лишь её часть
#define MAX_MAP_SCREEN_X 14 // 14
#define MAX_MAP_SCREEN_Y 8 // 8
// коды ошибок
#define RAM_IS_OVER 0
#define FILE_NOT_FOUND 404
#define NO_ENOUGH_DATA 2
#define INCORRECT_VALUE 3
// дополнительно:
#define COUNT_ALL_COLORS 6
#define COUNT_TXT_NAME 3
#define MAX_TXT_NAME 66
#define MAX_STR_LENGHT 21
#define SCREEN_REFRESH_RATE 250
#define BORDER_SIZE 1
#define BORDER_CHAR '+'
// коды спец. кнопок
#define ESC 27
#define DEL 83 // -32 83
// макросы
#define MAX(x,y) (x)>(y) ? (x) : (y)
#define MIN(x,y) (x)<(y) ? (x) : (y)

#include <Windows.h>

// типы клеток
enum type_cell
{
		type_bush = 'b', // кусты: клетка, удержавающая врагов и камни. При попадании игрока на кусты, кусты ломаются и превращаются в траву.
		type_p_bush = 176, // символ b на экране 
		type_exit = 'x', // выход
		type_p_exit = '#',
		type_grass = 'g', // свободная клетка
		type_p_grass = ' ',
		type_stone = 's', // камень
		type_p_stone = '0',
		type_wall = 'w', // стена
		type_p_wall = 219,
		type_diamond = '*', // кристал
		type_p_diamond = 4,
		type_checkpoint = 'c', // клетка сохранения
		type_p_checkpoint = 253
		// враги и персонаж накладываются на карту
};
// направление
enum direction
{
	up = 'w',
	left = 'a',
	down = 's',
	right = 'd'
};
// цвета в консоли
enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};
// соответствие тип клетки - цвет клетки
typedef struct _s_all_colors
{
	unsigned short background,
	  bush,
	  exit,
	  grass,
	  stone,
	  wall;
	//при изменении изменить COUNT_ALL_COLORS и confirm.txt
} s_all_colors;
// структура игрока (может быть дополнена)
typedef struct _s_player
{
	char ch; // символ игрока
	unsigned short	color; // цвет игрока
	COORD pos; // позиция на карте
	int diamonds, lives;
	
} s_player;
// структура врага
typedef struct _s_enemy
{
	COORD pos;
	char ch; // символ врга
	unsigned short	color; // цвет врага
	direction d; // изначальное направление
	struct _enemy *next, *prev; // указатели на следуюющего и предыдущего
} s_enemy;
// структура камня
typedef struct _s_stone
{
	char ch; // символ камня
	unsigned short	color; // цвет камня
	COORD pos; // позиция камня
	_s_stone *next; // указатель на следующий
} s_stone;
// структура очереди камней
typedef struct _s_q_stone
{
	s_stone *head, // начало очереди
		*tail; // конец очереди
} s_q_stone;
// структура - клетка карты
typedef struct _s_cell
{
	char ch; // символ
	char color; // цвет клетки
	direction dir; // направление изначального движения
	s_enemy *en; // указатель на врага (если на данной клетке враг)
	s_player *pl; // указатель на игрока (если на данной клетке игрок)
} s_cell;
// структура всей карты
typedef struct _s_map
{
	COORD size;
	s_cell **matr;
	char *characters;
	unsigned short *colors;
	int diamonds;
} s_map;
// сбор всех текстовых файлов (используется, например, при разных уровнях)
typedef struct _s_txt_name
{
	// при добавлении и удалении изменить COUNT_TXT_NAME
	char all_colors[MAX_TXT_NAME];
	char map[MAX_TXT_NAME];
	char player[MAX_TXT_NAME];
} s_txt_name;


#endif //_DECLARATIONS_H