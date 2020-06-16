#ifndef _IS_SOMETHING_H
#define _IS_SOMETHING_H

#include "declarartions.h"

// принадлежит ли клетка карте
int is_on_map(s_map *map, int X, int Y);

// клетка - трава
int is_grass(s_map *map, int X, int Y);

// клетка - камень
int is_stone(s_map *map, int X, int Y);

// клетка - кусты
int is_bush(s_map *map, int X, int Y);

// клетка - стена
int is_wall(s_map *map, int X, int Y);

// клетка - выход
int is_exit(s_map *map, int X, int Y);

// клетка - сохранение
int is_checkpoint(s_map *map, int X, int Y);

#endif //_IS_SOMETHING_H