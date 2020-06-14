//
//    !!!ВНИМАНИЕ!!!
// Для работы с программой установите размеры буфера экрана: Ширина = 100, Высота = 75.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <Windows.h>
#include <time.h>
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
#define BORDER_SIZE 2
#define BORDER_CHAR '%'
#define ESC 27
#define MAX(x,y) (x)>(y) ? (x) : (y);
#define MIN(x,y) (x)<(y) ? (x) : (y);

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


// нарисовать строку, состоящую только из символов c. Если is_new_line!=0, печатает перевод на новую строку.
void print_line(char c, int size, int is_new_line)
{
	for(int i=0; i<size; i++)
		printf("%c", c);
	if(is_new_line)
		printf("\n");
}

// нарисовать игрока на отображаемой части карты
int print_player(COORD screen_pos, s_player pl, int i, int j)
{
	if(pl.pos.X == j && pl.pos.Y == i)
	{
		printf("%c", pl.ch);
		return 1;
	}
	return 0;
}

// нарисовать клетку карты (трава, стена или камень)
void print_cell(s_map map, int i, int j)
{
	printf("%c", map.matr[i][j].ch);
}

// рисование части карты.
// На вход подаются карта и координаты начала отображения карты
void print_map(s_map map, COORD screen_pos, s_player player/*, s_enemies First*/)
{
	// размеры выводимого экрана крты
	int size_X = MIN(MAX_MAP_SCREEN_X, map.size.X);
	int size_Y = MIN(MAX_MAP_SCREEN_Y, map.size.Y);
	// верхняя строка границы выводимого экрана крты
	for(int i=0; i<BORDER_SIZE; i++)
		print_line(BORDER_CHAR, BORDER_SIZE*2+size_X, 1);
	for(int i=0; i<size_Y; i++)
	{
		// левый край
		print_line(BORDER_CHAR, BORDER_SIZE, 0);
		// символы карты
		for(int j=0; j<size_X; j++)
		{ 
			if(print_player(screen_pos, player, i, j))
				continue;
			print_cell(map, i, j);
		}
		// левый край
		print_line(BORDER_CHAR, BORDER_SIZE, 1);
	}
	// нижняя строка границы экрана
	for(int i=0; i<BORDER_SIZE; i++)
		print_line(BORDER_CHAR, BORDER_SIZE*2+size_X, 1);
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
int create_map_characters(s_txt_name txt_name, s_map *map)
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
// создание s_cell - матрицы размерами m на n
int create_s_cell_matrix(s_cell ***matrix, int m, int n)
{
	if( !((*matrix)=(s_cell**)malloc(sizeof(s_cell*)*m)) )
		return err(RAM_IS_OVER);
	for(int i=0; i<m; i++)
		if( !((*matrix)[i]=(s_cell*)malloc(sizeof(s_cell)*n)) )
			return err(RAM_IS_OVER);
	return 1;
}
// проверка принадлежности координат игрока карте
int is_player_on_map(s_player *player, int m, int n)
{
	if(player->pos.X<0 || player->pos.Y<0)
		return 0;
	if(player->pos.X>=n || player->pos.Y>=m)
		return 0;
	return 1;
}
// перенести игрока на карту - матрицу
int set_player_on_matr(s_map *map, s_conformity conformity, s_player *player)
{
	if( !is_player_on_map(player, map->size.Y, map->size.X) )
		return err(INCORRECT_VALUE);
	map->matr[player->pos.Y][player->pos.X].pl = player; 
	map->matr[player->pos.Y][player->pos.X].ch = type_p_grass;
	map->characters[player->pos.Y*map->size.X+player->pos.X] = player->ch;
	map->matr[player->pos.Y][player->pos.X].color = conformity.backgrownd << 4 | conformity.grass;
	map->colors[player->pos.Y*map->size.X+player->pos.X] = player->color;
	return 1;
}
// создание карты-матрицы
int create_map_matr(s_map *map, s_conformity conformity, s_player *player /*,s_empty *First*/)
{
	if( !create_s_cell_matrix(&(map->matr), map->size.Y, map->size.X) )
		return 0;
	for(int i=0; i<map->size.Y; i++)
	{
		for(int j=0; j<map->size.X; j++)
		{
			map->matr[i][j].ch = map->characters[map->size.X*i+j];
			map->matr[i][j].color = map->colors[map->size.X*i+j];
			map->matr[i][j].dir = left;
			map->matr[i][j].pl = NULL;
			map->matr[i][j].en = NULL;
		}
	}
	if(!set_player_on_matr(map, conformity, player))
		return err(INCORRECT_VALUE);
	/*if(!set_enemies_on_matr())
		return 0;*/
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
int create_all_colors(s_txt_name txt_name, s_conformity *type_colors)
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
// задание начальных параметров игрока
int create_player( s_txt_name txt_name, s_player* player) 
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
	int2str(level, txt_level+6, MAX_TXT_NAME-11);// -11 = ( 1 символ на '\0', 4 на ".txt", 6 на "level_" )
	strcat(txt_level, ".txt");
	FILE *flevel = fopen(txt_level, "r"); // открыли level_*.txt для чтения
	if(!flevel)
		return err(FILE_NOT_FOUND);
	for(char *p = txt_name->conformity, cnt=0; cnt<COUNT_TXT_NAME; cnt++, p+=MAX_TXT_NAME)
	{
		// считываю строку из файла
		if( !(fgets(p, MAX_TXT_NAME, flevel)) )
		{
			fclose(flevel);
			return err(NO_ENOUGH_DATA);
		}
		// удаляю перенос строки
		for(int i=0; p[i]!='\0'; i++)
			if(p[i]=='\n')
				p[i]=0;
	}
	return 1;
}
// выбор (?) уровня
int get_level(int *level)
{
	// если сохранять статистику, то подключить текстовый файл
	// а также изменить s_txt_name и соответствено COUNT_TXT_NAME
	*level = 1;
	return 1;
}
// подготовка - взятие информации из файлов, создание карты
int preparation(int *level, s_map *map, s_conformity *conformity, s_player *player/* ,s_enemy **first_enemy*/)
{

	// ОТКУДА ВЗЯТЬ УРОВЕНЬ???
	if(!get_level(level))
		return 0;
	// названия нужных файлов
	s_txt_name txt_name={0,0,0};
	if(!get_txt_name(1, &txt_name))
		return 0;
	if(!create_all_colors(txt_name, conformity))
		return 0;
	if( !create_map_characters(txt_name, map) )
		return 0;
	if( !create_map_colors(map, *conformity) )
	{
		free(map->characters);
		return 0;
	}
	if( !map_characters_to_print(map) )
	{
		free(map->characters);
		free(map->colors);
		return 0;
	}
	if( !create_player(txt_name, player))
	{
		free(map->characters);
		free(map->colors);
		return 0;
	}
	if( !create_map_matr(map, *conformity, player))
	{
		free(map->characters);
		free(map->colors);
		return 0;
	}
	map->matr[player->pos.Y][player->pos.X].pl = player;// установка игрока на карту
	return 1;
}

// вернёт 1, если данная клетка - трава, 0 - если не трава, или край карты
int is_grass(s_map *map, int X, int Y)
{
	if(X<0 || X>=map->size.X)
		return 0;
	if(Y<0 || Y>=map->size.Y)
		return 0;
	if(map->matr[Y][X].ch == type_p_grass || map->matr[Y][X].ch==type_grass)
		return 1;
	return 0;
}

// создание элемента очереди камней
s_stone* create_stone(s_cell *info, int X, int Y)
{
	s_stone *new_stone = (s_stone*)malloc(sizeof(s_stone));
	if(!new_stone)
		return (s_stone*)err(RAM_IS_OVER);
	// заполнение полей
	new_stone->ch = info->ch;
	new_stone->color = info->color;
	new_stone->next = NULL;
	new_stone->pos.X = X;
	new_stone->pos.Y = Y;
	return new_stone;
}

// добавление в конец
int add_stone_in_end(s_q_stone *q_stone, s_stone *stone)
{
	// проверка на существование
	if(!stone)
		return err(NO_ENOUGH_DATA);
	// очередь пуста
	if(!q_stone->head)
	{
		q_stone->head = q_stone->tail = stone;
		return 1;
	}
	// очередь не пуста
	q_stone->tail->next = stone;
	q_stone->tail = stone;
	return 1;
}

// рекурсивное добавление в очередь
int rec_add_in_q(s_q_stone *q_stone, s_map *map, int X, int Y)
{
	if(X<0 || X>=map->size.X)
		return 1; // край карты-не рассматриваем
	if(Y<0 || Y>=map->size.Y)
		return 1; // край карты-не рассматриваем
	if(map->matr[Y][X].ch != type_p_stone)
		return 1; // это не камень - вышли из шага рекурсии
	if( ! (is_grass(map, X, Y+1) || // можно упасть вниз
		is_grass(map, X-1, Y)&&is_grass(map, X-1, Y+1)&&map->matr[Y+1][X].ch == type_p_stone || // можно упасть влево и камень стоит на камне
		is_grass(map, X+1, Y)&&is_grass(map, X+1, Y+1)&&map->matr[Y+1][X].ch == type_p_stone) ) // можно упасть вправо и камень стоит на камне
		return 1; // камню некуда упасть - вышли из шага рекурсии
	if(!add_stone_in_end(q_stone, create_stone(map->matr[Y]+X, X, Y)))
		return 0;
	map->matr[Y][X].ch = type_p_grass;
	if(!rec_add_in_q(q_stone,map,X,Y-1))
		return 0;
	if(!rec_add_in_q(q_stone,map,X-1,Y-1))
		return 0;
	if(!rec_add_in_q(q_stone,map,X+1,Y-1))
		return 0;
	return 1;
}

// удаление из очереди камней, достигших низа, с начала, пока не встретитяс свободный камень
void del_from_q_stone(s_q_stone *q_stone, s_map *map)
{
	while(q_stone->head)
	{
		int X = q_stone->head->pos.X, Y = q_stone->head->pos.Y;
		// снизу свободно
		if(is_grass(map, X, Y+1))
			return;
		// слева свободно
		if(is_grass(map, X-1, Y)&&is_grass(map, X-1, Y+1)&&map->matr[Y+1][X].ch == type_p_stone)
			return;
		// справа свободно
		if(is_grass(map, X+1, Y)&&is_grass(map, X+1, Y+1)&&map->matr[Y+1][X].ch == type_p_stone)
			return;
		s_stone *cur = q_stone->head; // запомнили для освобождения памяти
		map->matr[Y][X].ch = type_p_stone; // нарисовали на карте
		q_stone->head = cur->next; // убрали из очереди
		free(cur); // освобождение памяти
	}
	// в очереди ничего нет
	q_stone->head = q_stone->tail = NULL;
}

// очистка всей очереди
void q_stone_clear(s_q_stone *q_stone)
{
	while(q_stone->head)
	{
		s_stone *cur = q_stone->head;
		q_stone->head = cur->next;
		free(cur);
	}
	q_stone->head = q_stone->tail = NULL;
}

// смещение всех камней на 1 шаг
void move_stone(s_q_stone *q_stone, s_map *map)
{
	for(s_stone *cur=q_stone->head; cur; cur=cur->next)
	{
		SHORT *X = &(cur->pos.X), *Y = &(cur->pos.Y);
		// снизу свободно
		if( is_grass(map, *X, *Y+1) && !map->matr[*Y+1][*X].pl )
		{
			map->matr[*Y][*X].ch = type_p_grass;
			cur->pos.Y++;
			map->matr[*Y][*X].ch = type_p_stone;
			continue;
		}
		// слева свободно
		if( is_grass(map, *X-1, *Y)&&is_grass(map, *X-1, *Y+1)&&map->matr[*Y+1][*X].ch == type_p_stone && !map->matr[*Y+1][*X].pl )
		{
			map->matr[*Y][*X].ch = type_p_grass;
			cur->pos.X--;
			cur->pos.Y++;
			map->matr[*Y][*X].ch = type_p_stone;
			continue;
		}
		// справа свободно
		if( is_grass(map, *X+1, *Y)&&is_grass(map, *X+1, *Y+1)&&map->matr[*Y+1][*X].ch == type_p_stone && !map->matr[*Y+1][*X].pl )
		{
			map->matr[*Y][*X].ch = type_p_grass;
			cur->pos.X++;
			cur->pos.Y++;
			map->matr[*Y][*X].ch = type_p_stone;
			continue;
		}
	}
}


int main()
{
	printf("Diamond-- by Alex, Evgen, POMAH.\n");
	system("pause");
	int level=0;
	COORD screen_pos={0,0};
	DWORD dw=0;
	s_map map={0,};
	s_conformity conformity;
	s_txt_name txt_name={0,0,0};
	s_player player = { {0,0},0,0 };
	s_q_stone q_stone = {0,0};
	preparation(&level, &map, &conformity, &player);
	print_map(map, screen_pos, player);
	system("pause");
	int d=clock();
	char c=0;
	while(c!=ESC)
	{
		if(_kbhit())
			{
				if( (c=_getch()) == ESC )
					break;
				while(_kbhit())
					char cccp=getch();
				switch(c)
				{
				case 'w':
					if(player.pos.Y>0)
					{
						map.matr[player.pos.Y][player.pos.X].pl=NULL;
						player.pos.Y--;
						map.matr[player.pos.Y][player.pos.X].pl=&player;
					}
					break;
				case 's':
					if(player.pos.Y<map.size.Y-1)
					{
						map.matr[player.pos.Y][player.pos.X].pl=NULL;
						player.pos.Y++;
						map.matr[player.pos.Y][player.pos.X].pl=&player;
					}
					break;
				case 'a':
					if(player.pos.X>0)
					{
						map.matr[player.pos.Y][player.pos.X].pl=NULL;
						player.pos.X--;
						map.matr[player.pos.Y][player.pos.X].pl=&player;
					}
					break;
				case 'd':
					if(player.pos.X<map.size.X-1)
					{
						map.matr[player.pos.Y][player.pos.X].pl=NULL;
						player.pos.X++;
						map.matr[player.pos.Y][player.pos.X].pl=&player;
					}
					break;
				case '*':
					rec_add_in_q(&q_stone, &map, player.pos.X, player.pos.Y-2);
					break;
				default:break;
				};
			}
		if(clock()-d>250)
		{
			d = clock();
			move_stone(&q_stone, &map);
			del_from_q_stone(&q_stone, &map);
			system("cls");
			print_map(map, screen_pos, player);
		}
	}
	for(s_stone *cur = q_stone.head; cur; cur=cur->next)
		printf("\n%d %d",cur->pos.X, cur->pos.Y);
	system("pause");
	free(map.characters);
	free(map.colors);
	for(int i=0; i<map.size.Y; i++)
		free(map.matr[i]);
	free(map.matr);
	q_stone_clear(&q_stone);
	return 0;
}
