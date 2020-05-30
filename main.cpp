//
//    !!!ВНИМАНИЕ!!!
// Для работы с программой установите размеры буфера экрана: Ширина = 100, Высота = 75.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
// размеры экрана
#define MAX_SCREEN_X 100
#define MAX_SCREEN_Y 75
// размеры экрана карты : на экран выводится не вся карта, а лишь её часть
#define MAX_MAP_SCREEN_X 42
#define MAX_MAP_SCREEN_Y 26
// коды ошибок
#define RAM_IS_OVER 0
#define FILE_NOT_FOUND 404
#define NO_ENOUGH_DATA 2
#define INCORRECT_VALUE 3
// дополнительно:
#define COUNT_CONFORMITY 6
#define COUNT_TXT_NAME 3
#define MAX_TXT_NAME 66
#define ESC 27

// типы клеток
enum type_cell
{
		type_bush = 'b', // клетка, удержавающая врагов и камни
		type_p_bush = 176, // символ b на экране 
		type_exit = 'x', // выход
		type_p_exit = '#', // жучок бегает 
		type_grass = 'g', // свободная клетка
		type_p_grass = 'X',
		type_stone = 's', // камень
		type_p_stone = '0',
		type_wall = 'w', // стена
		type_p_wall = 177,
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
// сбор всех текстовых файлов (используется, например, при разных уровнях)
typedef struct _s_txt_name
{
	// при добавлении и удалении изменить COUNT_TXT_NAME
	char conformity[MAX_TXT_NAME];
	char map[MAX_TXT_NAME];
	char player[MAX_TXT_NAME];
} s_txt_name;

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

// дескриптор объявляю здесь. чтобы не таскать его за собой ПОСТОЯНО
HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);


// нарисовать игрока на отображаемой части карты
void print_player_on_screen(COORD screen_pos, s_player pl)
{
	COORD now = {pl.pos.X-screen_pos.X, pl.pos.Y-screen_pos.Y}; // координаты игрока на экране
	DWORD dw = 0; // число записей на экран
	char ch[2]={0,}; // строка для вывода символа игрока
	ch[0] = pl.ch;
	WORD color[2]={0,}; // строка для вывода цвета фона и символа игрока
	color[0] = pl.color;
	if( now.X >= 0 && now.X < MAX_MAP_SCREEN_X)
		if(now.Y >= 0 && now.Y < MAX_MAP_SCREEN_Y)
		{
			WriteConsoleOutputCharacter(hConsole, // дескриптор буфера экрана
				ch, // строка символов
				1, // длина строки
				now, // координаты начала строки на консоли
				&dw); // количество записей
			WriteConsoleOutputAttribute(
				hConsole, // дескриптор экранного буфера
				color, // строка цветов
				1, // длина строки
				now, // координаты начала строки на консоли
				&dw);// количество записей
		}
	return;
}



// рисование части карты.
// На вход подаются карта и координаты начала отображения карты
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
			map.characters + (screen_pos.Y + now.Y)*map.size.X, // строка символов
			map.size.X, // длина строки
			now, // координаты начала строки на консоли
			&dw); // количество записей
		WriteConsoleOutputAttribute(
			hConsole, // дескриптор экранного буфера
			map.colors + (screen_pos.Y + now.Y)*map.size.X, // строка цветов
			map.size.X, // длина строки
			now, // координаты начала строки на консоли
			&dw);// количество записей	
	}
	return;
}

// перевод символов карты из типа ввода в тип вывод
int type_char_to_print(char *c)
{
	switch(int(*c))
	{
	case type_bush:
		*c = type_p_bush;
		return 1;
	case type_exit:
		*c = type_p_exit;
		return 1;
	case type_grass:
		*c = type_p_grass;
		return 1;
	case type_stone:
		*c = type_p_stone;
		return 1;
	case type_wall:
		*c = type_p_wall;
		return 1;
	default:
		return err(INCORRECT_VALUE);
	};
	return 0;
}

// перевод всех символов карты из типа ввода в тип вывода
int map_characters_to_print(s_map *map)
{
	for(int i=0; i<map->size.X*map->size.Y; i++)
		if( !type_char_to_print( &(map->characters[i]) ) )
			return 0;
	return 1;
}

// создание карты из текстового файла
int create_map(s_txt_name txt_name, s_map *map)
{
	FILE *fmap = fopen(txt_name.map, "r");
	if(!fmap)

	return err(FILE_NOT_FOUND);
	if (fscanf(fmap, "%d%d", &map->size.X, &map->size.Y) < 2)
		return err(NO_ENOUGH_DATA);//НЕПРАВИЛЬНО БЛЯДЬ
	if (map->size.X < 1 || map->size.Y < 1)
		return err(INCORRECT_VALUE);
	char c = 0;

	int i=0;
	if( !(map->characters = (char*)malloc( map->size.X*map->size.Y*sizeof(char) )) )
	{
		fclose(fmap);
		return err(RAM_IS_OVER);
	}
	while( (c=fgetc(fmap)) != EOF ) // пока не конец файла
	{
		if( (map->characters[i] = c) == '\n' )
			continue;
		i++;
		if( i==map->size.X * map->size.Y ) // карта считана полностью
			break;
	}
	fclose(fmap);
	if( i<map->size.X * map->size.Y )
	{
		free(map->characters);
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
	*parametr = str2color(str_f);
	return 1; // цвет переднего плана по коду консоли
}

// создание соответствия типу клетки с цветом из текстового файла
int create_type_colors(s_txt_name txt_name, s_conformity *type_colors)
{
	FILE *fconf = fopen(txt_name.conformity, "r");
	if(!fconf)
		return err(FILE_NOT_FOUND);
	unsigned short *p = &(type_colors->backgrownd);
	// заполнение полей type_colors
	for(int i=0; i<COUNT_CONFORMITY; i++, p++) // p++ - перейти к следующему полю структуры s_conformity
		if(!str2color_from_file(p, &fconf))
			return 0*fclose(fconf); // вернуть ноль и закрыть файл
	fclose(fconf);
	return 1;
}

// создание массива цветов карты
int create_map_colors(s_map *map, s_conformity type_colors)
{
	if( !(map->colors = (unsigned short*)malloc( sizeof(unsigned short)*map->size.X*map->size.Y )) )
		return err(RAM_IS_OVER);
	for(int i=0; i<map->size.X*map->size.Y; i++)
	{
		switch(int(map->characters[i]))
		{
		case type_bush:
			map->colors[i] = type_colors.backgrownd << 4 | type_colors.bush;
			break;
		case type_exit:
			map->colors[i] = type_colors.backgrownd << 4 | type_colors.exit;
			break;
		case type_grass:
			map->colors[i] = type_colors.backgrownd << 4 | type_colors.grass;
			break;
		case type_stone:
			map->colors[i] = type_colors.backgrownd << 4 | type_colors.stone;
			break;
		case type_wall:
			map->colors[i] = type_colors.backgrownd << 4 | type_colors.wall;
			break;
		default:
			map->colors[i] = 0;
		};
	}
	return 1;
}
int create_player(s_player* player, s_txt_name txt_name) 
{
		FILE* fplayer = fopen(txt_name.player, "r");
		if (!fplayer)
			return err(FILE_NOT_FOUND);
		if (!fscanf(fplayer, "%d%d", &(player->pos.X), &(player->pos.Y)))
		{
			fclose(fplayer);
			return err(NO_ENOUGH_DATA);
		}
			if (!fscanf(fplayer, "%d", &(player->ch)))
			{
				fclose(fplayer);
				return err(NO_ENOUGH_DATA);
			}
		char exstr[MAX_TXT_NAME];
		fgetc(fplayer);
		if (!fgets(exstr, MAX_TXT_NAME, fplayer))
		{
			fclose(fplayer);
			return err(NO_ENOUGH_DATA);
		}
	player->color=str2color(exstr);
		fclose(fplayer);
		return 1;
	

}

// реверс числа
int reverse_int(int n)
{
	int rev = 0;
	for(int temp=n>0 ? n : -n; temp>0; temp/=10)
		rev = rev*10+(temp%10);
	return n>0 ? rev : -rev;
}

// перевод числа в строку
void int2str(int n, char str[], int lenght)
{
	int i = n>0 ? 0 : 1,
		rev = reverse_int(n>0 ? n : -n);
	if(i)
		str[0] = '-';
	for(; i<lenght && rev>0; i++, rev/=10)
		str[i]=rev%10+'0';
	return;
}

// записать в txt_name имена текстовых файлов, используемых на данном уровне
int get_txt_name(int level, s_txt_name *txt_name)
{
	char txt_level[MAX_TXT_NAME]={'l','e','v','e','l','_',0};
	int2str(level, txt_level+6, MAX_TXT_NAME-11);// 1 символ на \0, 4 на .txt, 6 на level_
	strcat(txt_level, ".txt");
	FILE *flevel = fopen(txt_level, "r"); // открыли level_*.txt для чтения
	if(!flevel)
		return err(FILE_NOT_FOUND);
	for(char *p = txt_name->conformity, cnt=0; cnt<COUNT_TXT_NAME; cnt++, p+=MAX_TXT_NAME)
	{
		// считываю строку из файла
		if( !(fgets(p, MAX_TXT_NAME, flevel)) )
			return err(NO_ENOUGH_DATA);
		// удаляю перенос строки
		for(int i=0; p[i]!='\0'; i++)
			if(p[i]=='\n')
				p[i]=0;
	}
	return 1;
}


int main()
{
	printf("Diamond-- by Alex, Evgen, POMAH.\n");
	s_map map={0,};
	s_conformity type_colors;
	s_txt_name txt_name={0,0,0};
	COORD screen_pos={0,0};
	get_txt_name(1, &txt_name);
	if(!create_type_colors(txt_name, &type_colors))
		return 0;
	unsigned short *p = &type_colors.backgrownd;
	if( !create_map(txt_name, &map) )
		return 0;
	printf("\n");
	if( !create_map_colors(&map, type_colors) )
	{
		free(map.characters);
		return 0;
	}
	if( !map_characters_to_print(&map) )
	{
		system("pause");
		free(map.characters);
		free(map.colors);
		return 0;
	}
	system("pause");
    s_player pl = { {0,0},0,0 };
	if( !create_player(&pl, txt_name))
	{
		system("pause");
		free(map.characters);
		free(map.colors);
		return 0;
	}
	if (pl.pos.X < map.size.X && pl.pos.X >= 0 && pl.pos.Y < map.size.Y && pl.pos.Y >= 0 && int(map.characters[pl.pos.Y * map.size.X + pl.pos.X]) == type_p_grass)
	{
		map.characters[pl.pos.Y * map.size.X + pl.pos.X] = pl.ch;
		map.colors[pl.pos.Y * map.size.X + pl.pos.X] = pl.color;
	}
	print_map(map, screen_pos);
	system("pause");
	free(map.characters);
	free(map.colors);
	return 0;
}
