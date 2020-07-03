#ifndef _DECLARATIONS_H
#define _DECLARATIONS_H

// ������� ������
#define MAX_SCREEN_X 100
#define MAX_SCREEN_Y 75
// ������� ������ ����� : �� ����� ��������� �� ��� �����, � ���� � �����
#define MAX_MAP_SCREEN_X 14 // 14
#define MAX_MAP_SCREEN_Y 8 // 8
// ���� ������
#define RAM_IS_OVER 0
#define FILE_NOT_FOUND 404
#define NO_ENOUGH_DATA 2
#define INCORRECT_VALUE 3
// �������������:
#define COUNT_ALL_COLORS 6
#define COUNT_TXT_NAME 4
#define COUNT_MENU_COMMANDS 4
#define MAX_TXT_NAME 66
#define MAX_STR_LENGHT 21
#define MAX_LEVEL 7
#define SCREEN_REFRESH_RATE 250
#define USER_NAME "user.txt"
#define BORDER_SIZE 1
#define BORDER_CHAR '+'
// ���� ����. ������
#define ESC 27
#define DEL 83 // -32 83
#define ENTER 13
// �������
#define MAX(x,y) (x)>(y) ? (x) : (y)
#define MIN(x,y) (x)<(y) ? (x) : (y)

#include <Windows.h>

// ���� ������
enum type_cell
{
		type_bush = 'b', // �����: ������, ������������ ������ � �����. ��� ��������� ������ �� �����, ����� �������� � ������������ � �����.
		type_p_bush = 176, // ������ b �� ������ 
		type_exit = 'x', // �����
		type_p_exit = '#',
		type_grass = 'g', // ��������� ������
		type_p_grass = ' ',
		type_stone = 's', // ������
		type_p_stone = '0',
		type_wall = 'w', // �����
		type_p_wall = 219,
		type_diamond = '*', // �������
		type_p_diamond = 4,
		type_checkpoint = 'c', // ������ ����������
		type_p_checkpoint = 253
		// ����� � �������� ������������� �� �����
};
// �����������
enum direction
{
	up = 'w',
	left = 'a',
	down = 's',
	right = 'd'
};
// ����� � �������
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
// ������������ ��� ������ - ���� ������
typedef struct _s_all_colors
{
	unsigned short background,
	  bush,
	  exit,
	  grass,
	  stone,
	  wall;
	//��� ��������� �������� COUNT_ALL_COLORS � confirm.txt
} s_all_colors;
// ��������� ������ (����� ���� ���������)
typedef struct _s_player
{
	char ch; // ������ ������
	unsigned short	color; // ���� ������
	COORD pos; // ������� �� �����
	int diamonds, lives;
	
} s_player;
// ��������� �����
typedef struct _s_enemy
{
	char ch; // ������ ����
	unsigned short	color; // ���� �����
	direction d; // ����������� �����������
	COORD pos;
	struct _s_enemy *next; // ��������� �� ����������� � �����������
} s_enemy;
// ��������� �����
typedef struct _s_stone
{
	char ch; // ������ �����
	unsigned short	color; // ���� �����
	COORD pos; // ������� �����
	_s_stone *next; // ��������� �� ���������
} s_stone;
// ��������� ������� ������
typedef struct _s_q_stone
{
	s_stone *head, // ������ �������
		*tail; // ����� �������
} s_q_stone;
// ��������� - ������ �����
typedef struct _s_cell
{
	char ch; // ������
	char color; // ���� ������
	direction dir; // ����������� ������������ ��������
	s_enemy *en; // ��������� �� ����� (���� �� ������ ������ ����)
	s_player *pl; // ��������� �� ������ (���� �� ������ ������ �����)
} s_cell;
// ��������� ���� �����
typedef struct _s_map
{
	COORD size;
	s_cell **matr;
	char *characters;
	unsigned short *colors;
	int diamonds;
} s_map;
// ���� ���� ��������� ������ (������������, ��������, ��� ������ �������)
typedef struct _s_txt_name
{
	// ��� ���������� � �������� �������� COUNT_TXT_NAME
	char all_colors[MAX_TXT_NAME];
	char map[MAX_TXT_NAME];
	char player[MAX_TXT_NAME];
	char enemy[MAX_TXT_NAME];
} s_txt_name;
// ������� ����
enum commands
{
	continue_game = 0,
	new_game = 1,
	choose_level = 2,
	exit_game = 3
};

#endif //_DECLARATIONS_H