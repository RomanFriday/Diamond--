//
//    !!!ВНИМАНИЕ!!!
// Для работы с программой установите размеры буфера экрана: Ширина = 100, Высота = 75.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
// размеры экрана
#define MAX_SCREEN_X 100
#define MAX_SCREEN_Y 75
// размеры экрана карты : на экран выводится не вся крта, а лишь её часть
#define MAX_MAP_SCREEN_X 55
#define MAX_MAP_SCREEN_Y 34
#define COUNT_CONFORMITY_TYPES 6

// коды ошибок
#define RAM_IS_OVER 0
#define FILE_NOT_FOUND 404
#define NO_ENOUGH_DATA 2
#define INCORRECT_VALUE 3

// типы клеток
enum type_cell
{
		type_bush = 'b', // клетка, удержавающая врагов и камни
		type_p_bush = 176, // символ b на экране 
		type_exit = 'x', // выход
		type_p_exit = 253, // жучок бегает 
		type_grass = 'g', // свободная клетка
		type_p_grass = 219,
		type_stone = 's', // камень
		type_p_stone = 'O',
		type_wall = 'w', // стена
		type_p_wall = 178,
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
typedef struct _s_conformity
{
	unsigned short backgrownd,
	  bush,
	  exit,
	  grass,
	  stone,
	  wall;
	//при изменении изменить COUNT_CONFORMITY_TYPES и confirm.txt
} s_conformity;
// структура игрока (может быть дополнена)
typedef struct _s_player
{
	COORD pos; // позиция на карте
	char ch; // символ игрока
	unsigned short	color; // цвет фона и цвет игрока
} s_player;
// структура врага
typedef struct _s_enemy
{
	COORD pos;
	char ch; // символ врга
	unsigned short	color; // цвет фона и цвет игрока
	direction d; //
	struct _enemy *next, *prev;
} s_enemy;
// структура камня
typedef struct _s_stone
{
	COORD pos; // позиция камня
	char ch; // символ камня
	unsigned short	color; // цвет камня
	_s_stone *next; // указатель на следующий
} s_stone;
// структура очереди камней
typedef struct _s_q_stone
{
	s_stone *head, // начало очереди
		*tail; // конец очереди
} s_q_stone;
// структура клетка карты
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
} s_map;
// вывод сообщения по коду ошибки. всега возвращает 0
int err(int type)
{
	// вывод в стандартный буфер вывода (в консоль)
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	switch(type)
	{
	case RAM_IS_OVER:
		printf("\nRAM is over!\n");
		break;
	case FILE_NOT_FOUND:
		printf("\nError 404: file not found");
		break;
	case NO_ENOUGH_DATA:
		printf("\nNo enough data...");
		break;
	case INCORRECT_VALUE:
		printf("\nIncorrect value");
		break;
	default: return 0;
	}
	system("pause");
	return 0;
}

HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

// рисование части карты.
// На вход подаются координаты начала отображения карты, карта(символы+цвета), координаты игрока, список координат врагов
void print_map(s_map map, COORD screen_pos)
{
	DWORD dw=0; // число записей на экран
	COORD now={0,0};
	int size_X = MAX_MAP_SCREEN_X > map.size.X ? map.size.X : MAX_MAP_SCREEN_X;
	int size_Y = MAX_MAP_SCREEN_Y > map.size.Y ? map.size.Y : MAX_MAP_SCREEN_Y;
	SetConsoleActiveScreenBuffer(hConsole); // установка активного буфера экрана
	// прорисовка карты без игрока и врагов
	for(; now.Y < size_Y; now.Y++)
	{
		WriteConsoleOutputCharacter(hConsole, // дескриптор буфера экрана
			map.characters + (screen_pos.Y + now.Y)*size_X, // строка символов
			map.size.X, // длина строки
			now, // координаты начала строки на консоли
			&dw); // количество записей
		WriteConsoleOutputAttribute(
			hConsole, // дескриптор экранного буфера
			map.colors + (screen_pos.Y + now.Y)*size_X, // строка цветов
			map.size.X, // длина строки
			now, // координаты начала строки на консоли
			&dw);// количество записей
	}
	return;
}

// создание карты из текстового файла
int create_map(char *txt_name, s_map map)
{
	FILE *fmap = fopen(txt_name, "r");
	if(!fmap)
		return err(FILE_NOT_FOUND);
	if( fscanf(fmap, "%d%d", &map.size.X, &map.size.Y) < 2 )
		return err(NO_ENOUGH_DATA);
	if( map.size.X<1 || map.size.Y<1 )
		return err(INCORRECT_VALUE);
	char c=0;
	int i=0;
	if( !(map.characters = (char*)malloc( map.size.X*map.size.Y*sizeof(char) )) )
	{
		fclose(fmap);
		return err(RAM_IS_OVER);
	}
	while( (c=fgetc(fmap)) != EOF ) // пока не конец файла
	{
		if( (map.characters[i] = c) == '\n' )
			continue;
		i++;
		if( i==map.size.X * map.size.Y ) // карта считана полностью
			break;
	}
	fclose(fmap);
	if( i<map.size.X * map.size.Y )
	{
		free(map.characters);
		return err(NO_ENOUGH_DATA);
	}
	return 1;
}

// перевод строки цвета в чиловое значение цвета
int str2color(char *str)
{
	if(!strcmp(str, "Black\n"))
		return Black;
	if(!strcmp(str, "Blue\n"))
		return Blue;
	if(!strcmp(str, "Green\n"))
		return Green;
	if(!strcmp(str, "Cyan\n"))
		return Cyan;
	if(!strcmp(str, "Red\n"))
		return Red;
	if(!strcmp(str, "Magenta\n"))
		return Magenta;
	if(!strcmp(str, "Brown\n"))
		return Brown;
	if(!strcmp(str, "LightGray\n"))
		return LightGray;
	if(!strcmp(str, "DarkGray\n"))
		return DarkGray;
	if(!strcmp(str, "LightBlue\n"))
		return LightBlue;
	if(!strcmp(str, "LightGreen\n"))
		return LightGreen;
	if(!strcmp(str, "LightRed\n"))
		return LightRed;
	if(!strcmp(str, "LightCyan\n"))
		return LightCyan;
	if(!strcmp(str, "LightMagenta\n"))
		return LightMagenta;
	if(!strcmp(str, "Yellow\n"))
		return Yellow;
	if(!strcmp(str, "White\n"))
		return White;
	return err(INCORRECT_VALUE);
}

// перевод строки цвета в чиловое значение цвета, строка берётся из файла
int str2color_from_file(unsigned short *parametr, FILE **fin)
{
	char str_f[21]={0,}; // цвет переднего плана в виде строки
	if(!fgets(str_f, 20, *fin))
	{
		fclose(*fin);
		return err(NO_ENOUGH_DATA);
	}
	return *parametr = str2color(str_f); // цвет переднего плана по коду консоли
}

// создание соответствия типу клетки с цветом из текстового файла
int create_type_colors(char *txt_conformity, s_conformity *type_colors)
{
	char str_b[20]={0,}, str_f[20]={0,}; // строка цвета фона и переднего плана
	FILE *fconf = fopen(txt_conformity, "r");
	if(!fconf)
		return err(FILE_NOT_FOUND);
	unsigned short *p = &(type_colors->backgrownd);
	// заполнение полей type_colors
	for(int i=0; i<COUNT_CONFORMITY_TYPES; i++, p++) // p++ - перейти к следующему полю структуры s_conformity
		if(!str2color_from_file(p, &fconf))
			return 0*fclose(fconf); // вернуть ноль и закрыть файл
	fclose(fconf);
	return 1;
}

// создание массива цветов карты
int create_map_colors(s_map map, s_conformity type_colors)
{
	if( !(map.colors = (unsigned short*)malloc( sizeof(unsigned short)*map_size_X*map_size_Y )) )
		return err(RAM_IS_OVER);
	for(int i=0; i<map.size.X*map.size.Y; i++)
	{
		switch(int(map.characters[i]))
		{
		case type_bush:
			map_colors[i] = type_colors.bush;
			break;
		case type_exit_map:
			map_colors[i] = type_colors.exit;
			break;
		case type_grass:
			map_colors[i] = type_colors.grass;
			break;
		case type_stone:
			map_colors[i] = type_colors.stone;
			break;
		case type_wall:
			map_colors[i] = type_colors.wall;
			break;
		default:
			map_colors[i] = 0;
		};
	}
	return map_colors;
}



void main()
{
	printf("Hello from POMAH\n");
	int map_size_X, map_size_Y;
	char *map;
	unsigned short *map_colors;
	conformity type_colors;
	char conf_txt[]="conformity.txt";
	if(!create_type_colors(conf_txt, &type_colors))
		return;
	unsigned short *p = &type_colors.backgrownd;
	for(int i=0; i<COUNT_CONFORMITY_TYPES; i++, *p+=sizeof(unsigned short))
		printf("\n%d", *p);
	char map_txt[]="map.txt";
	if( !(map=create_map(map_txt, &map_size_X, &map_size_Y)) )
		return;
	printf("\n");
	for(int i=0; i<map_size_Y; i++)
	{
		for(int j=0; j<map_size_X; j++)
			printf(" %3c", map[i*map_size_X+j]);
		printf("\n");
	}
	if(! (map_colors = create_map_colors(map, map_size_X, map_size_Y, type_colors)) )
	{
		free(map);
		return;
	}
	for(int i=0; i<map_size_Y; i++)
	{
		for(int j=0; j<map_size_X; j++)
			printf(" %3d", map_colors[i*map_size_X+j]);
		printf("\n");
	}
	system("pause");
	COORD screen_pos={0,0};

	player pl={{10,6},0,0};
	print_map(map, map_colors, map_size_X, map_size_Y, screen_pos, pl, &screen_pos);
	system("pause");
	free(map);
	free(map_colors);
	return;
}
