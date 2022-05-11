/*1852877 计科 赵昊堃*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

#define CLOSE_STATE     -1
#define MARK_STATE      -2
#define CLOSE            0
#define OPEN             1
#define MARK            -1

struct mine {
	int key;
	int open;//0:关闭/1:打开/-1:标记
};

/********************************函数01:难度选择菜单*************************************************/
void choose_menu(int& row_num, int& col_num, int& mine_num)//功能:选择难度
{
	cct_cls();
	cout << "请选择难度:" << endl;
	cout << "  1.初级( 9*9  - 10颗雷)" << endl;
	cout << "  2.中级(16*16 - 40颗雷)" << endl;
	cout << "  3.高级(16*30 - 99颗雷)" << endl;
	cout << "请输入[1..3]:";
	char level;

	while (level = _getch()) {
		if (!(level == '1' || level == '2' || level == '3'))
			continue;
		else
			break;
	}

	switch (level) {
		case '1':
			row_num = 9;
			col_num = 9;
			mine_num = 10;
			break;
		case '2':
			row_num = 16;
			col_num = 16;
			mine_num = 40;
			break;
		case '3':
			row_num = 16;
			col_num = 30;
			mine_num = 99;
			break;
	}
}
/********************************函数02:生成0~N-1随机数*************************************************/
int RAND(int N)
{
	return rand() % N;
}
/********************************函数03:查找周围八个格子的雷数*************************************************/
int check_sur(int row, int col, mine a[18][32])
{
	int cnt = 0;
	for (int i = row - 1; i <= row + 1; i++) {
		for (int j = col - 1; j <= col + 1; j++) {
			if (i == row && j == col)
				continue;
			if (a[i][j].key == 9)
				cnt++;
		}
	}
	return cnt;
}
/********************************函数04:内部数组初始化*************************************************/
void init_array(mine a[18][32], int row_num, int col_num, int mine_num)
{

	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 32; j++) {
			a[i][j].key = 0;
			a[i][j].open = 0;
		}
	}

	for (int i = 0; i < mine_num; i++)
	{
		int r = 1 + RAND(row_num);
		int c = 1 + RAND(col_num);
		if (a[r][c].key == 9)
		{
			i--;
			continue;
		}
		else
			a[r][c].key = 9;
	}
	for (int i = 1; i <= row_num; i++)
	{
		for (int j = 1; j <= col_num; j++)
		{
			if (a[i][j].key == 9)//此元为地雷
				continue;
			else
				a[i][j].key = check_sur(i, j, a);
		}
	}
}

/********************************函数05:深度优先搜索*************************************************/
void DFS(int row, int col, int row_num, int col_num, mine a[18][32])
{
	a[row][col].open = OPEN;
	if (a[row][col].key == 0)
		for (int i = row - 1; i <= row + 1; i++)
			for (int j = col - 1; j <= col + 1; j++)
				if (a[i][j].open == CLOSE && (i >= 1 && i <= row_num) && (j >= 1 && j <= col_num))
					DFS(i, j, row_num, col_num, a);
}

/********************************函数06:全局方块重画*************************************************/
void draw_all_block(const CONSOLE_GRAPHICS_INFO* const mineCGI, const int row_num, const int col_num, mine a[18][32], const BLOCK_DISPLAY_INFO* const bdi_normal)
{
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (a[i + 1][j + 1].open == 0)//关闭
				gmw_draw_block(mineCGI, i, j, CLOSE_STATE, bdi_normal);
			else if (a[i + 1][j + 1].open == 1)//打开
				gmw_draw_block(mineCGI, i, j, a[i + 1][j + 1].key, bdi_normal);
			else if (a[i + 1][j + 1].open == -1)//标记
				gmw_draw_block(mineCGI, i, j, MARK_STATE, bdi_normal);
		}
	}
}
/********************************函数07:检测是否通关*************************************************/
bool check_win(mine a[18][32], int row_num, int col_num, int mine_num)
{
	bool win = 1;
	int cnt = 0;
	for (int i = 1; i <= row_num; i++)
		for (int j = 1; j <= col_num; j++)
			if (a[i][j].open == OPEN)
				cnt++;
	if (cnt == row_num * col_num - mine_num)
		return 1;
	return 0;
}
/********************************游戏主体*************************************************/
void game_mine(CONSOLE_GRAPHICS_INFO* mineCGI)
{
	mine a[18][32];
	int row_num, col_num, mine_num;

	choose_menu(row_num, col_num, mine_num);

	time_t start_time = clock();

	init_array(a, row_num, col_num, mine_num);
	gmw_set_rowcol(mineCGI, row_num, col_num);
	/*画框架*/
	gmw_draw_frame(mineCGI);
	/* 上状态栏显示内容 */
	gmw_status_line(mineCGI, TOP_STATUS_LINE, "ESC退出,空格显示时间");
	/* 下状态栏显示内容 */
	gmw_status_line(mineCGI, LOWER_STATUS_LINE, "[当前光标]");

	const BLOCK_DISPLAY_INFO bdi_normal[] = {
				{BDI_VALUE_BLANK, COLOR_HYELLOW, -1, NULL},  //0不显示，用空格填充即可
				{1, COLOR_HYELLOW, 1, NULL},
				{2, COLOR_HYELLOW, 2, NULL},
				{3, COLOR_HYELLOW, 3, NULL},
				{4, COLOR_HYELLOW, 4, NULL},
				{5, COLOR_HYELLOW, 5, NULL},
				{6, COLOR_HYELLOW, 6, NULL},
				{7, COLOR_HYELLOW, 7, NULL},
				{8, COLOR_HYELLOW, 8, NULL},
				{9, COLOR_HYELLOW, 9, "*"},
				{CLOSE_STATE, COLOR_HRED, COLOR_HRED, NULL},
				{MARK_STATE, COLOR_HGREEN, COLOR_WHITE,"#"},
				{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	/*初始填充画色块:全部关闭   *特别注意内部数组行列从1开始,而工具函数从0开始*/
	for (int i = 0; i < row_num; i++)
		for (int j = 0; j < col_num; j++)
			gmw_draw_block(mineCGI, i, j, CLOSE_STATE, bdi_normal);

	int MRow, MCol, MAction, keycode1, keycode2;
	int time = 0;//记录是否为第一次
	bool win = false;//是否胜利
	int remain = mine_num;//记录剩余地雷数

	cct_enable_mouse();
	while (1) {
		int event = gmw_read_keyboard_and_mouse(mineCGI, MAction, MRow, MCol, keycode1, keycode2, true);
		/*键盘事件*/
		if (event == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 27)
				return;//退出
			else if (keycode1 == 32) {
				char msg[40] = { 0 };
				sprintf(msg, "当前时间:%d秒,ESC退出,空格显示时间", int(clock() - start_time) / CLOCKS_PER_SEC);
				gmw_status_line(mineCGI, TOP_STATUS_LINE, msg);
			}
		}
		if (event == CCT_MOUSE_EVENT) {
			if (MAction == MOUSE_LEFT_BUTTON_CLICK) {//左击打开
				time++;
				/*处理首次死亡*/
				while (a[MRow + 1][MCol + 1].key == 9 && time == 1)
					init_array(a, row_num, col_num, mine_num);

				/*一般情况*/
				if (a[MRow + 1][MCol + 1].open == CLOSE) {
					if (a[MRow + 1][MCol + 1].key == 9) {//踩雷
						a[MRow + 1][MCol + 1].open = OPEN;
						gmw_draw_block(mineCGI, MRow, MCol, a[MRow + 1][MCol + 1].key, bdi_normal);
						/*上状态栏*/
						char msg[40] = { 0 };
						sprintf(msg, "当前时间:%d秒,ESC退出,空格显示时间", int(clock() - start_time) / CLOCKS_PER_SEC);
						gmw_status_line(mineCGI, TOP_STATUS_LINE, msg);
						/*下状态栏*/
						gmw_status_line(mineCGI, LOWER_STATUS_LINE, "游戏结束,你输了");
						return;//退出
					}
					DFS(MRow + 1, MCol + 1, row_num, col_num, a);
					/*画色块*/
					draw_all_block(mineCGI, row_num, col_num, a, bdi_normal);
				}
			}
			else if (MAction == MOUSE_RIGHT_BUTTON_CLICK) {
				if (a[MRow + 1][MCol + 1].open == CLOSE) {
					a[MRow + 1][MCol + 1].open = MARK;
					draw_all_block(mineCGI, row_num, col_num, a, bdi_normal);
					remain--;
				}
				else if (a[MRow + 1][MCol + 1].open == MARK) {
					a[MRow + 1][MCol + 1].open = CLOSE;
					draw_all_block(mineCGI, row_num, col_num, a, bdi_normal);
					remain++;
				}
			}

			/*鼠标单击上状态栏信息改变*/
			if (MAction == MOUSE_LEFT_BUTTON_CLICK || MAction == MOUSE_RIGHT_BUTTON_CLICK) {
				/*上状态栏*/
				int dis_remain = (remain < 0) ? 0 : remain;
				char msg[40] = { 0 };
				sprintf(msg, "剩余雷数:%d个,ESC退出,空格显示时间", dis_remain);
				gmw_status_line(mineCGI, TOP_STATUS_LINE, msg);
			}
		}
		/*检测是否获胜*/
		if (check_win(a, row_num, col_num, mine_num)) {
			draw_all_block(mineCGI, row_num, col_num, a, bdi_normal);
			/*上状态栏*/
			char msg[40] = { 0 };
			sprintf(msg, "当前时间:%d秒,ESC退出,空格显示时间", int(clock() - start_time) / CLOCKS_PER_SEC);
			gmw_status_line(mineCGI, TOP_STATUS_LINE, msg);
			/*下状态栏*/
			gmw_status_line(mineCGI, LOWER_STATUS_LINE, "游戏结束,你赢了");
			return;//退出
		}
	}
}



int main()
{
	srand(unsigned int(time(0)));

	CONSOLE_GRAPHICS_INFO mineCGI; //声明一个CGI变量

	/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
	gmw_init(&mineCGI);
	gmw_set_color(&mineCGI, COLOR_BLACK, COLOR_WHITE);			//整个窗口颜色
	gmw_set_font(&mineCGI, "新宋体", 24);						//字体
	gmw_set_frame_style(&mineCGI, 4, 2, true);					//游戏主区域风格：每个色块宽4高2，有分隔线
	gmw_set_frame_color(&mineCGI, COLOR_WHITE, COLOR_BLACK);	//游戏主区域颜色
	gmw_set_block_border_switch(&mineCGI);						//小色块带边框
	gmw_set_rowno_switch(&mineCGI, true);						//显示行号
	gmw_set_colno_switch(&mineCGI, true);						//显示列标
	gmw_set_status_line_switch(&mineCGI, TOP_STATUS_LINE, true);    //开启上状态栏
	gmw_set_status_line_color(&mineCGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(&mineCGI, LOWER_STATUS_LINE, true);	//开启下状态栏
	gmw_set_status_line_color(&mineCGI, LOWER_STATUS_LINE);
	while (1) {
		game_mine(&mineCGI);
		cout << "按回车继续..." << endl;
		while (_getch() != '\r')
			continue;
	}
}
