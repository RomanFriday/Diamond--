//
//    !!!ВНИМАНИЕ!!!
// Для работы с программой установите размеры буфера экрана: Ширина = 100, Высота = 75.

#include <stdio.h>
#include <Windows.h>
// размеры экрана
#define MAX_SCREEN_X 100
#define MAX_SCREEN_Y 75
// размеры экрана карты : на экран выводится не вся крта, а лишь её часть
#define MAX_MAP_SCREEN_X 55
#define MAX_MAP_SCREEN_Y 34

enum tupe_cell
{
		grass = 'g',// свободная клетка
		bush = 'b',// клетка, удержавающая врагов и камни
		stone = 's',// камень
		wall = 'w',// стена
		// враги и персонаж накладываются на карту
};

HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
int b=SetConsoleActiveScreenBuffer(hConsole);

// рисование части карты.
// На вход подаются координаты начала отображения карты, карта(символы+цвета), координаты игрока, список координат врагов
int print_map(char *map, COORD pos, unsigned short *map_colors, COORD player, COORD *enemies)
{
	DWORD dw=0; // число записей на экран
	for(COORD now={0,0}; now.Y < MAX_MAP_SCREEN_Y; now.Y++)
	{
		WriteConsoleOutputCharacter(hConsole, // дескриптор буфера экрана
			map + (pos.Y + now.Y)*MAX_MAP_SCREEN_X, // строка 
			MAX_MAP_SCREEN_X, // длина строки
			now, // координаты начала строки на консоли
			&dw); // количество записей
	}
	/* рисование играка */
	/* рисование врагов */
	return 0;
}

void main()
{
	printf("Hello from POMAH\n");
	system("pause");
	return;
}
