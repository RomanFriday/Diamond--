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

// структура игрока (может быть дополнена)
typedef struct _player
{
	COORD pos; // позиция на карте
	LPCSTR ch; // символ игрока
	WORD *color; // цвет фона и цвет игрока
} player;

// ошибки. всега возвращает 0
int ERR(int type)
{
	switch(type)
	{
	case 0:
		printf("\nRAM is over!\n");
		return 0;
	default: return 0;
	}
	return 0;
}


HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//int b=SetConsoleActiveScreenBuffer(hConsole);

// нарисовать игрока на отображаемой части карты
void print_player_on_screen(COORD screen_pos, player pl)
{
	COORD now = {pl.pos.X-screen_pos.X, pl.pos.Y-screen_pos.Y}; // координаты игрока на экране
	DWORD dw = 0; // число записей на экран
	if( now.X >= 0 && now.X < MAX_MAP_SCREEN_X)
		if(now.Y >= 0 && now.Y < MAX_MAP_SCREEN_Y)
		{
			WriteConsoleOutputCharacter(hConsole, // дескриптор буфера экрана
				pl.ch, // строка символов
				1, // длина строки
				now, // координаты начала строки на консоли
				&dw); // количество записей
			WriteConsoleOutputAttribute(
				hConsole, // дескриптор экранного буфера
				pl.color, // строка цветов
				1, // длина строки
				now, // координаты начала строки на консоли
				&dw);// количество записей
		}
	return;
}

// рисование части карты.
// На вход подаются координаты начала отображения карты, карта(символы+цвета), координаты игрока, список координат врагов
void print_map(char *map,  unsigned short *map_colors, COORD screen_pos, player pl, COORD *enemies)
{
	DWORD dw=0; // число записей на экран
	COORD now={0,0};
	// прорисовка карты без игрока и врагов
	for(; now.Y < MAX_MAP_SCREEN_Y; now.Y++)
	{
		WriteConsoleOutputCharacter(hConsole, // дескриптор буфера экрана
			map + (screen_pos.Y + now.Y)*MAX_MAP_SCREEN_X, // строка символов
			MAX_MAP_SCREEN_X, // длина строки
			now, // координаты начала строки на консоли
			&dw); // количество записей
		WriteConsoleOutputAttribute(
			hConsole, // дескриптор экранного буфера
			map_colors+(screen_pos.Y + now.Y)*MAX_MAP_SCREEN_X, // строка цветов
			MAX_MAP_SCREEN_X, // длина строки
			now, // координаты начала строки на консоли
			&dw);// количество записей
	}
	// рисование игрока, если он находится на отображаемой части экрана
	print_player_on_screen(screen_pos, pl);
	/* рисование врагов */
	return;
}

char* create_map(char *txt_name, int *map_size_X, int *map_size_Y)
{
	FILE *fmap = fopen(txt_name, "r");
	if(!fmap)
		return NULL;
	fscanf(fmap, "%d%d", map_size_Y, map_size_X);
	char *map;
	if( !(map = (char*)malloc( (*map_size_X)*(*map_size_Y)*sizeof(char) )) )
		return (char*)ERR(0);
	for(int i=0; i<(*map_size_Y)*(*map_size_X); i++)
		if( (map[i] = fgetc(fmap)) == '\n' )
			map[i] = fgetc(fmap);
	return map;
}

void main()
{
	printf("Hello from POMAH\n");
	int map_size_X, map_size_Y;
	char *map;
	if( !(map=create_map("map.txt", &map_size_X, &map_size_Y)) )
	{
	system("pause");
	return;
	}
	printf("\n");
	for(int i=0; i<map_size_Y; i++)
	{
		for(int j=0; j<map_size_X; j++)
			printf("%c", map[i*map_size_X+j]);
		printf("\n");
	}
	system("pause");
	return;
}
