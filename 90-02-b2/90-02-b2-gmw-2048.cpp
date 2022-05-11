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

#define   BASEGOAL          512  
#define   TOUP					1
#define   TODOWN				2
#define   TOLEFT				3
#define   TORIGHT				4
/****************************函数01：获得游戏参数**************************************/
void get_parameter(int& row_num, int& col_num, int& target, int& delay)
{
	int temp, start_x, start_y;
	cout << "游戏2048参数设置" << endl;
	/*行*/
	cout << "请输入行数[4-8]:";
	cct_getxy(start_x, start_y);
	while (1) {
		cin >> temp;
		if (cin.good() && 4 <= temp && temp <= 8) {
			row_num = temp;
			break;
		}
		else {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(start_x, start_y, ' ', 0, 7, 20);
			cct_gotoxy(start_x, start_y);
		}
	}
	/*列*/
	cout << "请输入列数[4-10]:";
	cct_getxy(start_x, start_y);
	while (1) {
		cin >> temp;
		if (cin.good() && 4 <= temp && temp <= 10) {
			col_num = temp;
			break;
		}
		else {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(start_x, start_y, ' ', 0, 7, 20);
			cct_gotoxy(start_x, start_y);
		}
	}
	/*目标分数*/
	int goal;

	if (row_num * col_num <= 20)
		goal = BASEGOAL;
	else if (row_num * col_num <= 25)
		goal = 2 * BASEGOAL;
	else if (row_num * col_num <= 36)
		goal = 4 * BASEGOAL;
	else if (row_num * col_num <= 49)
		goal = 8 * BASEGOAL;
	else
		goal = 16 * BASEGOAL;

	cout << "请输入目标分数[" << goal << '/' << 2 * goal << '/' << 4 * goal << '/' << 8 * goal << "]:";
	cct_getxy(start_x, start_y);
	while (1) {
		cin >> temp;
		if (cin.good() && temp == goal || temp == 2 * goal || temp == 4 * goal || temp == 8 * goal) {
			target = temp;
			break;
		}
		else {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(start_x, start_y, ' ', 0, 7, 20);
			cct_gotoxy(start_x, start_y);
		}
	}
	/*延迟*/
	cout << "请输入动画延迟[0-5]:";
	cct_getxy(start_x, start_y);
	while (1) {
		cin >> temp;
		if (cin.good() && 0 <= temp && temp <= 5) {
			delay = BLOCK_MOVED_DELAY_MS + 10 * temp;
			break;
		}
		else {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(start_x, start_y, ' ', 0, 7, 20);
			cct_gotoxy(start_x, start_y);
		}
	}
}


/********************************函数02:生成0~N-1随机数*************************************************/
int RAND(int N)
{
	return rand() % N;
}
/****************************函数03：在空白区域生成一个新的2**************************************/
bool array_generate(int a[8][10], int row_num, int col_num)
{
	/*在空白随机区域产生一个新的2,如果没有空位了返回false,否则返回true*/
	int cnt = 0;
	for (int i = 0; i < row_num; i++)
		for (int j = 0; j < col_num; j++)
			if (a[i][j] != 0)
				cnt++;
	if (cnt == row_num * col_num)//全满
		return false;
	else {
		while (1)
		{
			int rand_i = RAND(row_num);
			int rand_j = RAND(col_num);
			if (a[rand_i][rand_j] == 0) {//空位
				a[rand_i][rand_j] = 2;
				break;
			}
		}
		return true;
	}
}
/****************************函数04：全局方块绘制**************************************/
void draw_all_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_num, const int col_num, int a[8][10], const BLOCK_DISPLAY_INFO* const bdi_normal)
{
	for (int i = 0; i < row_num; i++)
		for (int j = 0; j < col_num; j++)
			gmw_draw_block(pCGI, i, j, a[i][j], bdi_normal);
}

/****************************函数05：exchange**************************************/
void exchange(int a[8][10], int row1, int col1, int row2, int col2, bool& generate)
{
	if (row1 == row2 && col1 == col2)
		return;//排除自身交换
	int temp = a[row1][col1];
	a[row1][col1] = a[row2][col2];
	a[row2][col2] = temp;
	generate = true;
	return;
}
/****************************函数06：归位**************************************/
void position(int a[8][10], int row_num, int col_num, int type, bool& generate, const CONSOLE_GRAPHICS_INFO* const pCGI, const BLOCK_DISPLAY_INFO* const bdi_normal)
{
	if (type == TOUP) {
		for (int j = 0; j < col_num; j++) {//依次检查每一列
			for (int i = 0; i < row_num; i++) //从上到下检查每一行
				if (a[i][j] != 0) {//只操作非零元素

					bool all_zero = true;
					/*全零*/
					for (int t = i - 1; t >= 0; t--)
						if (a[t][j] != 0) {
							all_zero = false;
							break;
						}
					if (all_zero) {
						gmw_move_block(pCGI, i, j, a[i][j], 0, bdi_normal, DOWN_TO_UP, i);
						exchange(a, i, j, 0, j, generate);

					}

					else {
						for (int k = i - 1; k >= 0; k--)
							if (a[k][j] != 0) {
								gmw_move_block(pCGI, i, j, a[i][j], 0, bdi_normal, DOWN_TO_UP, i - (k + 1));
								exchange(a, i, j, k + 1, j, generate);

								break;
							}
					}
				}
		}
	}
	else if (type == TODOWN) {
		for (int j = 0; j < col_num; j++) {//依次检查每一列
			for (int i = row_num - 1; i >= 0; i--) //从下到上检查每一行
				if (a[i][j] != 0) {//只操作非零元素

					bool all_zero = true;
					/*全零*/
					for (int t = i + 1; t < row_num; t++)
						if (a[t][j] != 0) {
							all_zero = false;
							break;
						}
					if (all_zero) {
						gmw_move_block(pCGI, i, j, a[i][j], 0, bdi_normal, UP_TO_DOWN, row_num - 1 - i);
						exchange(a, i, j, row_num - 1, j, generate);

					}

					else {
						for (int k = i + 1; k < row_num; k++)
							if (a[k][j] != 0) {
								gmw_move_block(pCGI, i, j, a[i][j], 0, bdi_normal, UP_TO_DOWN, k - 1 - i);
								exchange(a, i, j, k - 1, j, generate);
								break;
							}
					}
				}
		}
	}
	else if (type == TOLEFT) {
		for (int i = 0; i < row_num; i++) {//依次检查每一行
			for (int j = 0; j < col_num; j++) //从左往右
				if (a[i][j] != 0) {//只操作非零元素

					bool all_zero = true;
					/*全零*/
					for (int t = j - 1; t >= 0; t--)
						if (a[i][t] != 0) {
							all_zero = false;
							break;
						}
					if (all_zero) {
						gmw_move_block(pCGI, i, j, a[i][j], 0, bdi_normal, RIGHT_TO_LEFT, j);
						exchange(a, i, j, i, 0, generate);
					}

					else {
						for (int k = j - 1; k >= 0; k--)
							if (a[i][k] != 0) {
								gmw_move_block(pCGI, i, j, a[i][j], 0, bdi_normal, RIGHT_TO_LEFT, j - (k + 1));
								exchange(a, i, j, i, k + 1, generate);
								break;
							}
					}
				}
		}
	}
	else if (type == TORIGHT) {
		for (int i = 0; i < row_num; i++) {//依次检查每一行
			for (int j = col_num - 1; j >= 0; j--) //从右往左检查每一行
				if (a[i][j] != 0) {//只操作非零元素

					bool all_zero = true;
					/*全零*/
					for (int t = j + 1; t < col_num; t++)
						if (a[i][t] != 0) {
							all_zero = false;
							break;
						}
					if (all_zero) {
						gmw_move_block(pCGI, i, j, a[i][j], 0, bdi_normal, LEFT_TO_RIGHT, col_num - 1 - j);
						exchange(a, i, j, i, col_num - 1, generate);
					}

					else {
						for (int k = j + 1; k < col_num; k++)
							if (a[i][k] != 0) {
								gmw_move_block(pCGI, i, j, a[i][j], 0, bdi_normal, LEFT_TO_RIGHT, k - 1 - j);
								exchange(a, i, j, i, k - 1, generate);
								break;
							}
					}
				}
		}
	}
}
/****************************函数07：判断结束**************************************/
bool check_sur(int temp_a[10][12], int row_no, int col_no)
{
	//false代表周围没有可合并项
	if (temp_a[row_no][col_no] == temp_a[row_no + 1][col_no])
		return true;
	else if (temp_a[row_no][col_no] == temp_a[row_no - 1][col_no])
		return true;
	else if (temp_a[row_no][col_no] == temp_a[row_no][col_no + 1])
		return true;
	else if (temp_a[row_no][col_no] == temp_a[row_no][col_no - 1])
		return true;

	return false;

}
bool game_over(int a[8][10], int row_num, int col_num)
{
	//还没全满
	for (int i = 0; i < row_num; i++)
		for (int j = 0; j < col_num; j++)
			if (a[i][j] == 0)
				return false;
	//已经全部满了
	int temp_a[10][12] = { 0 };
	for (int i = 0; i < row_num; i++)
		for (int j = 0; j < col_num; j++) {
			temp_a[i + 1][j + 1] = a[i][j];
		}

	for (int i = 1; i <= row_num; i++)
		for (int j = 1; j <= col_num; j++) {
			if (check_sur(temp_a, i, j))//还有合并项
				return false;
		}
	return true;

}
/****************************游戏主体**************************************/
void game_2048(CONSOLE_GRAPHICS_INFO* pCGI, int& max_score)
{


	int a[8][10] = { 0 };
	char temp[100] = { 0 };
	int row_num, col_num, target, delay;
	int score = 0;//计分器
	get_parameter(row_num, col_num, target, delay);

	gmw_set_rowcol(pCGI, row_num, col_num);
	/*设置显示数组*/
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{2, COLOR_HYELLOW, COLOR_BLACK, NULL},
		{4, COLOR_HRED, COLOR_BLACK, NULL},
		{8, COLOR_GREEN, COLOR_BLACK, NULL},
		{16, COLOR_RED, COLOR_BLACK, NULL},
		{32, COLOR_HGREEN, COLOR_BLACK, NULL},
		{64, COLOR_HYELLOW, COLOR_BLACK, NULL},
		{128, COLOR_HRED, COLOR_BLACK, NULL},
		{256, COLOR_HWHITE, COLOR_BLACK, NULL},
		{512, COLOR_HBLACK, COLOR_BLACK, NULL},
		{1024, COLOR_HPINK, COLOR_BLACK, NULL},
		{2048, COLOR_WHITE, COLOR_BLACK, NULL},
		{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
		{8192, COLOR_PINK, COLOR_BLACK, NULL},
		{16384, COLOR_HBLUE, COLOR_BLACK, NULL},
		{32768, COLOR_HCYAN, COLOR_BLACK, NULL},
		{65536, COLOR_HGREEN, COLOR_BLACK, NULL},
		{131072, COLOR_HPINK, COLOR_BLACK, NULL},  //如果开心，还可以继续加
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};

	/*画框架*/
	gmw_draw_frame(pCGI);
	/*初始化:产生第一个2*/
	array_generate(a, row_num, col_num);

	/* 上状态栏显示内容,同时开始计时 */
	time_t start_time = clock();
	sprintf(temp, "目标:%d 分数:%d 最高:%d 时间:%d (R:重玩 Q:退出)", target, score, max_score, int(clock() - start_time) / CLOCKS_PER_SEC);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
	/*设置延时*/
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, delay);
	/*禁用鼠标*/
	cct_disable_mouse();
	/*读取键盘*/
	while (1) {
		draw_all_block(pCGI, row_num, col_num, a, bdi_normal);
		int MAction, MRow, MCol, keycode1, keycode2;
		bool generate = false;
		int event = gmw_read_keyboard_and_mouse(pCGI, MAction, MRow, MCol, keycode1, keycode2, false);
		if (event == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 0xe0) {//方向键
				switch (keycode2) {
					case KB_ARROW_UP:
						/***上***/
						/*归位*/
						position(a, row_num, col_num, TOUP, generate, pCGI, bdi_normal);
						/*合并*/
						for (int j = 0; j < col_num; j++) {//依次检查每一列
							for (int i = 1; i < row_num; i++) //从上到下检查每一行
								if (a[i][j] == a[i - 1][j]) {
									a[i - 1][j] *= 2;
									score += a[i - 1][j];
									a[i][j] = 0;
									generate = true;
								}
						}
						/*再归位*/
						position(a, row_num, col_num, TOUP, generate, pCGI, bdi_normal);
						break;

					case KB_ARROW_DOWN:
						/***下***/
						/*归位*/
						position(a, row_num, col_num, TODOWN, generate, pCGI, bdi_normal);
						/*合并*/
						for (int j = 0; j < col_num; j++) {//依次检查每一列
							for (int i = row_num - 1; i >= 1; i--)
								if (a[i][j] == a[i - 1][j]) {
									a[i][j] *= 2;
									score += a[i][j];
									a[i - 1][j] = 0;
									generate = true;
								}
						}
						/*再归位*/
						position(a, row_num, col_num, TODOWN, generate, pCGI, bdi_normal);
						break;

					case KB_ARROW_LEFT://左
						/*归位*/
						position(a, row_num, col_num, TOLEFT, generate, pCGI, bdi_normal);
						/*合并*/
						for (int i = 0; i < row_num; i++) {
							for (int j = 0; j < col_num - 1; j++)
								if (a[i][j] == a[i][j + 1]) {
									a[i][j] *= 2;
									score += a[i][j];
									a[i][j + 1] = 0;
									generate = true;
								}
						}
						/*归位*/
						position(a, row_num, col_num, TOLEFT, generate, pCGI, bdi_normal);
						break;

					case KB_ARROW_RIGHT://右
						/*归位*/
						position(a, row_num, col_num, TORIGHT, generate, pCGI, bdi_normal);
						/*合并*/
						for (int i = 0; i < row_num; i++) {
							for (int j = col_num - 1; j > 0; j--)
								if (a[i][j] == a[i][j - 1]) {
									a[i][j] *= 2;
									score += a[i][j];
									a[i][j - 1] = 0;
									generate = true;
								}
						}
						/*归位*/
						position(a, row_num, col_num, TORIGHT, generate, pCGI, bdi_normal);
						break;
				}
				if (generate) {
					array_generate(a, row_num, col_num);
					if (score > max_score)
						max_score = score;
					sprintf(temp, "目标:%d 分数:%d 最高:%d 时间:%d (R:重玩 Q:退出)", target, score, max_score, int(clock() - start_time) / CLOCKS_PER_SEC);
					gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
				}
				if (game_over(a, row_num, col_num)) {
					sprintf(temp, "游戏失败!最终分数:%d 时间:%d(R:重玩 Q:退出)", score, int(clock() - start_time) / CLOCKS_PER_SEC);
					gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
					Sleep(1000);
					return;
				}
			}
			else if (keycode1 == 'Q' || keycode1 == 'q')
				exit(0);
			else if (keycode1 == 'R' || keycode1 == 'r')
				return;
		}

	}
}

int main()
{
	srand(unsigned int(time(0)));


	CONSOLE_GRAPHICS_INFO G2048_CGI; //声明一个CGI变量

	/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
	gmw_init(&G2048_CGI);

	gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);			//整个窗口颜色
	gmw_set_font(&G2048_CGI, "新宋体", 16);						//字体
	gmw_set_frame_style(&G2048_CGI, 10, 5, true);					//游戏主区域风格：每个色块宽10高5，有分隔线【数字色块带边框，宽度为10(放最多6位数字)，高度为5(为了保持色块为方形)】
	gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);	//游戏主区域颜色
	gmw_set_block_border_switch(&G2048_CGI, true);				//小色块带边框
	gmw_set_rowno_switch(&G2048_CGI, false);						//关闭行号
	gmw_set_colno_switch(&G2048_CGI, false);						//关闭列标
	gmw_set_status_line_switch(&G2048_CGI, TOP_STATUS_LINE, true);    //开启上状态栏
	gmw_set_status_line_color(&G2048_CGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(&G2048_CGI, LOWER_STATUS_LINE, false);	//关闭下状态栏

	int max_score = 0;
	while (1) {
		cct_setcolor();
		cct_cls();
		game_2048(&G2048_CGI, max_score);

	}
}