#include <stdio.h>
#include <Windows.h>

enum tupe_cell
{
		grass = 'g',// свободная клетка
		bush = 'b',// клетка, удержавающая врагов и камни
		stone = 's',// камень
		wall = 'w',// стена
		// враги и персонаж накладываются на карту
};

void main()
{
	printf("Hello from POMAH\n");
	system("pause");
	return;
}
