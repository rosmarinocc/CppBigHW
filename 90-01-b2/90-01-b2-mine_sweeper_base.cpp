/*1852877 计科 赵昊堃*/
#include "90-01-b2-mine_sweeper.h"
#include "../include/cmd_console_tools.h"
#include <ctime>
#include <iostream>
using namespace std;
void fun1(char level)
{
	mine a[18][32];
	init_array(a, level);
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 32; j++) {
			a[i][j].open = 1;//打开
		}
	}
	cct_cls();
	show_array(a, level, 0);
}
void fun2(char level)
{
	mine a[18][32];
	init_array(a, level);
	cct_cls();
	show_array(a, level, 1);
	bool gameover = 0, exit = 0;
	int time = 1;
	open_array(a, level, gameover, exit, time);
}
void fun3(char level)
{
	mine a[18][32];
	init_array(a, level);
	cct_cls();
	show_array(a, level, 1);
	game(a, level, 0);
}
void fun4(char level)
{
	time_t start_time = clock();
	mine a[18][32];
	init_array(a, level);
	cct_cls();
	show_array(a, level, 1);
	game(a, level, 1, start_time);

}
void fun5(char level)
{
	mine a[18][32];
	init_array(a, level);
	int row = getrow(level);
	int col = getcol(level);
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 32; j++) {
			a[i][j].open = 1;//打开
		}
	}
	cct_cls();
	show_graph(a, row, col);
}
void fun6(char level)
{
	mine a[18][32];
	init_array(a, level);
	int row = getrow(level);
	int col = getcol(level);
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 32; j++) {
			a[i][j].open = 1;//打开
		}
	}
	cct_cls();
	show_graph(a, row, col);
	check_mouse(row, col);

}
void fun7(char level)
{
	mine a[18][32];
	init_array(a, level);
	int row = getrow(level);
	int col = getcol(level);
	cct_cls();
	show_graph(a, row, col);
	mouse_click(row, col, level, a);
	cout << endl;
	cout << endl;
	cout << endl;

}
void fun8(char level)
{
	mine a[18][32];
	time_t start_time = clock();
	init_array(a, level);
	int row = getrow(level);
	int col = getcol(level);
	cct_cls();
	GAME(row, col, level, a, 8);
	cout << endl;
	cout << endl;
	cout << endl;

}
void fun9(char level)
{
	mine a[18][32];
	time_t start_time = clock();
	init_array(a, level);
	int row = getrow(level);
	int col = getcol(level);
	cct_cls();
	GAME(row, col, level, a, 9);
	cout << endl;
	cout << endl;
	cout << endl;

}