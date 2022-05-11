/*1852877 计科 赵昊堃*/
#pragma once
#include <ctime>
struct mine { 
	int key; 
	int open; //0为关闭，1为打开，-1为标记
};

void choose_menu(char ch);
int getrow(char level);
int getcol(char level);
void init_array(mine a[18][32], char level);//初始化扫雷数组
void show_array(mine a[18][32], char level,bool color);//展示扫雷数组
void open_array(mine a[18][32], char level, bool& gameover,bool& exit, int &time, bool special = 0, time_t start_time = 0);//翻开，包含深度优先搜索
void show_graph(mine a[18][32], int row,int col);
void game(mine a[18][32], char level, bool special,time_t start_time = 0);
void GAME(int row, int col, char level, mine a[18][32], int mode, time_t start_time = 0);
bool check_win(mine a[18][32], char level);
void check_mouse(int row, int col);
void DFS_traverse(int row, int col, mine a[18][32], char level);
void mouse_click(int row, int col, char level, mine a[18][32]);
void fun1(char level);
void fun2(char level);
void fun3(char level);
void fun4(char level);
void fun5(char level);
void fun6(char level);
void fun7(char level);
void fun8(char level);
void fun9(char level);