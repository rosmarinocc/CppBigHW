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
#define NORMAL_STATE       1
#define RELATED_STATE      2
#define SELECTED_STATE     3
#define BONUS              10

struct elem {
	int key;
	int state;//1:原始状态;2:关联状态（选中区域）;3:选中状态（1次回车后）
};

/***************************函数01:获取行列数*********************************/
int input_rc(char ch)
{
	int k;
	int start_x, start_y, input_x, input_y;
	cct_getxy(start_x, start_y);
	if (ch == 'r')
		cout << "请输入行数(8-10):   ";
	else
		cout << "请输入列数(8-10):   ";
	cct_getxy(input_x, input_y);
	while (1) {
		cin >> k;
		if (cin.good() && k >= 8 && k <= 10)
			break;
		else {
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "输入不合法请重新输入" << endl;

			if (ch == 'r') {
				cct_gotoxy(start_x, start_y);
				cout << "请输入行数(8-10):               ";
				cct_gotoxy(input_x, input_y);
			}
			else {
				cct_gotoxy(start_x, start_y);
				cout << "请输入列数(8-10):               ";
				cct_gotoxy(input_x, input_y);
			}
			continue;
		}
	}
	return k;
}
/***************************函数02:产生一个0~N-1的随机数*********************************/
int RAND(int N)//功能:产生一个0~N-1的随机数
{
	return rand() % N;
}
/***************************函数03:内部数组初始化*********************************/
void init_array(elem star[12][12], int row_num, int col_num)//注意内部数组首元素为star[1][1]
{
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 12; j++) {
			star[i][j].key = 0;
			star[i][j].state = 1;
		}

	for (int i = 1; i <= row_num; i++)
		for (int j = 1; j <= col_num; j++)
			star[i][j].key = RAND(5) + 1;

}
/***************************函数04:箭头移动选择*********************************/
void arrow_move(elem star[][12], int keycode2, int row_num, int col_num, int& row_now, int& col_now)
{
	//已经保证rc1的值非0,注意这里传入的row和col为内部数组rc,即MRow+1
	star[row_now + 1][col_now + 1].state = NORMAL_STATE;//取消当前高光
	int i = 1;
	switch (keycode2) {
		case KB_ARROW_UP://向上
			if (star[row_now][col_now + 1].key == 0) //如果上面的元素为0,返回最底部
				row_now = row_num - 1;
			else
				row_now--;
			break;

		case KB_ARROW_DOWN://向下
			if (row_now + 1 == row_num) {
				for (int i = 1; i <= row_num; i++)
					if (star[i][col_now + 1].key != 0) {
						row_now = i - 1;
						break;
					}
			}//已经在最底部,则找到该列最顶部的非零元
			else
				row_now++;
			break;

		case KB_ARROW_LEFT://向左
			while (star[row_now + 1][(col_now + col_num - i) % col_num + 1].key == 0)
				i++;
			col_now = (col_now + col_num - i) % col_num;
			break;

		case KB_ARROW_RIGHT://向右
			while (star[row_now + 1][(col_now + col_num + i) % col_num + 1].key == 0)
				i++;
			col_now = (col_now + col_num + i) % col_num;
			break;
	}
}
/***************************函数05:方块全局绘制*********************************/
void draw_all_block(const CONSOLE_GRAPHICS_INFO* const starCGI, const int row_num, const int col_num, elem star[12][12], const BLOCK_DISPLAY_INFO* const bdi_normal, const BLOCK_DISPLAY_INFO* const bdi_selected, const BLOCK_DISPLAY_INFO* const bdi_related)
{
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (star[i + 1][j + 1].state == NORMAL_STATE)//正常
				gmw_draw_block(starCGI, i, j, star[i + 1][j + 1].key, bdi_normal);
			else if (star[i + 1][j + 1].state == SELECTED_STATE)//选中
				gmw_draw_block(starCGI, i, j, star[i + 1][j + 1].key, bdi_selected);
			else if (star[i + 1][j + 1].state == RELATED_STATE)//关联
				gmw_draw_block(starCGI, i, j, star[i + 1][j + 1].key, bdi_related);
		}
	}
}
/***************************函数06:查找周边相等的方块,置关联状态*********************************/
void search_sur(elem star[][12], int row, int col, int& cnt) {
	if (star[row][col].key == 0 || star[row][col].state != NORMAL_STATE)
		return;
	else {
		star[row][col].state = RELATED_STATE;
		cnt++;
	}
	if (star[row - 1][col].key == star[row][col].key)
		search_sur(star, row - 1, col, cnt);
	if (star[row + 1][col].key == star[row][col].key)
		search_sur(star, row + 1, col, cnt);
	if (star[row][col - 1].key == star[row][col].key)
		search_sur(star, row, col - 1, cnt);
	if (star[row][col + 1].key == star[row][col].key)
		search_sur(star, row, col + 1, cnt);
}
/***************************函数07:回退所有状态,置普通状态*********************************/
void reset_all(elem star[][12], int row_num, int col_num)
{
	for (int i = 1; i <= row_num; i++)
		for (int j = 1; j <= col_num; j++)
			star[i][j].state = NORMAL_STATE;
}
/***************************函数08:进行一次消除*********************************/
bool all_zero(elem star[][12], int row_num, int j)
{
	/*第j列是否为全0列*/
	for (int i = 1; i <= row_num; i++)
		if (star[i][j].key != 0)
			return false;
	return true;
}

int middle_zero_v(elem star[][12], int row_num, int j, int& cnt)
{
	/*第j列是否存在中间0(悬空元素)?         事先保证非全零行
	返回首个悬空元素的行号,如果不存在返回-1                */
	for (int i = row_num; i >= 2; i--) {
		if (star[i][j].key == 0)
			cnt++;
		if (star[i][j].key == 0 && star[i - 1][j].key != 0)
			return i - 1;
	}
	return -1;
}
int middle_zero_h(elem star[][12], int row_num, int col_num, int& cnt)
{
	/*底部是否存在中间0(悬空元素)? 决定是否平移
	返回首个悬空元素的行号,如果不存在返回-1                */
	for (int j = 1; j <= col_num - 1; j++) {
		if (star[row_num][j].key == 0)
			cnt++;
		if (star[row_num][j].key == 0 && star[row_num][j + 1].key != 0)
			return j + 1;
	}
	return -1;


}
void clean_sur(const CONSOLE_GRAPHICS_INFO* const starCGI, elem star[][12], int row_num, int col_num, int& cnt, const BLOCK_DISPLAY_INFO* bdi_normal)
{
	/*消除工作*/
	for (int i = 1; i <= row_num; i++)
		for (int j = 1; j <= col_num; j++)
			if (star[i][j].state == SELECTED_STATE || star[i][j].state == RELATED_STATE) {
				star[i][j].key = 0;
				gmw_draw_block(starCGI, i - 1, j - 1, 0, bdi_normal);
				cnt++;
			}

	reset_all(star, row_num, col_num);

	/*下落工作*/
	for (int j = 1; j <= col_num; j++) {
		if (!all_zero(star, row_num, j)) {//全零列无下落的必要
			while (1) {
				int zero_num = 0;
				int res_row = middle_zero_v(star, row_num, j, zero_num);//首个悬空元素的行号(内部数组的行号)
				if (res_row != -1) {//存在悬空元素
					for (int i = res_row; i >= 1; i--) {
						if (star[i][j].key != 0) {
							star[i + zero_num][j].key = star[i][j].key;
							gmw_move_block(starCGI, i - 1, j - 1, star[i][j].key, 0, bdi_normal, UP_TO_DOWN, zero_num);
							star[i][j].key = 0;
						}
					}
				}
				else
					break;
			}
		}
	}

	/*平移工作*/
	for (int j = 1; j <= col_num; j++) {
		while (1) {
			int zero_num = 0;
			int res_col = middle_zero_h(star, row_num, col_num, zero_num);
			if (res_col != -1) {//存在悬空元素
				for (int j = res_col; j <= col_num; j++) {
					for (int i = row_num; i >= 1; i--) {//整列移动
						if (star[i][j].key != 0) {
							star[i][j - zero_num].key = star[i][j].key;
							gmw_move_block(starCGI, i - 1, j - 1, star[i][j].key, 0, bdi_normal, RIGHT_TO_LEFT, zero_num);
							star[i][j].key = 0;
						}

					}
				}
			}
			else
				break;
		}
	}
}
/***************************函数09:检查游戏是否结束*********************************/
bool check_gameover(elem star[][12], int row_num, int col_num)//gameover返回0代表未结束,返回1代表结束
{
	reset_all(star, row_num, col_num);
	for (int i = 1; i <= row_num; i++)
		for (int j = 1; j <= col_num; j++) {
			if (star[i][j].key != 0) {
				int cnt = 0;
				search_sur(star, i, j, cnt);
				if (cnt > 1) {
					reset_all(star, row_num, col_num);
					return false;
				}
			}
		}
	reset_all(star, row_num, col_num);
	return true;
}

/***************************游戏主体*********************************/
void game_star(CONSOLE_GRAPHICS_INFO* starCGI, elem star[][12], int row_num, int col_num, int& score)
{
	char temp[40] = { 0 };
	init_array(star, row_num, col_num);

	gmw_set_rowcol(starCGI, row_num, col_num);
	/*画框架*/
	gmw_draw_frame(starCGI);
	/* 上状态栏显示内容 */
	sprintf(temp, "屏幕当前设置为:%d行%d列", starCGI->lines, starCGI->cols);
	gmw_status_line(starCGI, TOP_STATUS_LINE, temp);
	/* 下状态栏显示内容 */
	gmw_status_line(starCGI, LOWER_STATUS_LINE, "箭头键/鼠标移动,回车键/单击左键选择");

	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1,  COLOR_BLUE,   COLOR_BLACK,  NULL},	//数字1的“正常”状态
		{2,  COLOR_GREEN,  COLOR_BLACK,  NULL},
		{3,  COLOR_CYAN,   COLOR_BLACK,  NULL},
		{4,  COLOR_RED,    COLOR_BLACK,  NULL},
		{5,  COLOR_PINK,   COLOR_BLACK,  NULL},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	const BLOCK_DISPLAY_INFO bdi_related[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1,  COLOR_BLUE,   COLOR_WHITE,  NULL},	//数字1的“可选择+关联”状态
		{2,  COLOR_GREEN,  COLOR_WHITE,  NULL},
		{3,  COLOR_CYAN,   COLOR_WHITE,  NULL},
		{4,  COLOR_RED,    COLOR_WHITE,  NULL},
		{5,  COLOR_PINK,   COLOR_WHITE,  NULL},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{1, COLOR_BLUE,   COLOR_HWHITE, NULL},	//数字1的“选中”状态
		{2, COLOR_GREEN,  COLOR_HWHITE, NULL},
		{3, COLOR_CYAN,   COLOR_HWHITE, NULL},
		{4, COLOR_RED,    COLOR_HWHITE, NULL},
		{5, COLOR_PINK,   COLOR_HWHITE, NULL},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};

	int row_now = row_num - 1, col_now = 0;//记录当前选中块的行列值,初始设置为左下角
	star[row_now + 1][col_now + 1].state = RELATED_STATE;


	int MRow, MCol, MAction, keycode1, keycode2;

	cct_enable_mouse();

	while (1) {
		draw_all_block(starCGI, row_num, col_num, star, bdi_normal, bdi_selected, bdi_related);
		int event = gmw_read_keyboard_and_mouse(starCGI, MAction, MRow, MCol, keycode1, keycode2, true);
		/*键盘事件*/
		if (event == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 0x0D) {//回车键
				if (star[row_now + 1][col_now + 1].state == RELATED_STATE && star[row_now + 1][col_now + 1].key != 0) {
					int cnt = 0;
					star[row_now + 1][col_now + 1].state = NORMAL_STATE;//search之前需要暂时设为普通
					search_sur(star, row_now + 1, col_now + 1, cnt);
					if (cnt == 1) {//无连续方块
						reset_all(star, row_num, col_num);//回退状态
						star[row_now + 1][col_now + 1].state = RELATED_STATE;
						gmw_status_line(starCGI, LOWER_STATUS_LINE, "箭头/鼠标移动,回车/单击左键选择/,Q/单击右键结束", "周围无相同值");
					}
					else
						star[row_now + 1][col_now + 1].state = SELECTED_STATE;
				}
				else if (star[row_now + 1][col_now + 1].state == SELECTED_STATE) {
					int cnt = 0;
					clean_sur(starCGI, star, row_num, col_num, cnt, bdi_normal);
					score += cnt * BONUS;
					sprintf(temp, "本次得分: %d , 总得分: %d", cnt * BONUS, score);
					gmw_status_line(starCGI, TOP_STATUS_LINE, temp);
					row_now = row_num - 1, col_now = 0;
					//是否结束
					if (check_gameover(star, row_num, col_num)) {//游戏结束
						int rest = 0;
						for (int i = 1; i <= row_num; i++)
							for (int j = 1; j <= col_num; j++)
								if (star[i][j].key != 0)
									rest++;
						/*上状态栏*/
						score += BONUS * (row_num * col_num - rest);
						sprintf(temp, "剩余%d个不可消除项,本关结束,回车下一关", rest);
						gmw_status_line(starCGI, TOP_STATUS_LINE, temp);
						/*下状态栏*/
						sprintf(temp, "奖励得分: %d , 总得分: %d", BONUS * (row_num * col_num - rest), score);
						gmw_status_line(starCGI, LOWER_STATUS_LINE, temp);
						return;
					}
				}
			}
			else if (keycode1 == 0xe0) {//方向键
				reset_all(star, row_num, col_num);
				arrow_move(star, keycode2, row_num, col_num, row_now, col_now);//修改选中色块的RC值
				star[row_now + 1][col_now + 1].state = RELATED_STATE;//重新设置
			}
			else if (keycode1 == 'Q' || keycode1 == 'q') {//Q
				exit(0);//游戏结束
			}
		}
		/*鼠标事件*/
		else if (event == CCT_MOUSE_EVENT) {
			row_now = MRow;
			col_now = MCol;
			if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
				if (star[row_now + 1][col_now + 1].state == RELATED_STATE && star[row_now + 1][col_now + 1].key != 0) {
					int cnt = 0;
					star[row_now + 1][col_now + 1].state = NORMAL_STATE;//search之前需要暂时设为普通
					search_sur(star, row_now + 1, col_now + 1, cnt);
					if (cnt == 1) {//无连续方块
						reset_all(star, row_num, col_num);//回退状态
						star[row_now + 1][col_now + 1].state = RELATED_STATE;
						gmw_status_line(starCGI, LOWER_STATUS_LINE, "箭头/鼠标移动,回车/单击左键选择/,Q/单击右键结束", "周围无相同值");
					}
					else
						star[row_now + 1][col_now + 1].state = SELECTED_STATE;
				}
				else if (star[row_now + 1][col_now + 1].state == SELECTED_STATE) {
					int cnt = 0;
					clean_sur(starCGI, star, row_num, col_num, cnt, bdi_normal);
					score += cnt * BONUS;
					sprintf(temp, "本次得分: %d , 总得分: %d", cnt * BONUS, score);
					gmw_status_line(starCGI, TOP_STATUS_LINE, temp);
					row_now = row_num - 1, col_now = 0;
					//是否结束
					if (check_gameover(star, row_num, col_num)) {//游戏结束
						int rest = 0;
						for (int i = 1; i <= row_num; i++)
							for (int j = 1; j <= col_num; j++)
								if (star[i][j].key != 0)
									rest++;
						/*上状态栏*/
						score += BONUS * (row_num * col_num - rest);
						sprintf(temp, "剩余%d个不可消除项,本关结束,回车下一关", rest);
						gmw_status_line(starCGI, TOP_STATUS_LINE, temp);
						/*下状态栏*/
						sprintf(temp, "奖励得分: %d , 总得分: %d", BONUS * (row_num * col_num - rest), score);
						gmw_status_line(starCGI, LOWER_STATUS_LINE, temp);
						return;
					}
				}
			}
			else if (MAction == MOUSE_ONLY_MOVED) {
				if (star[row_now + 1][col_now + 1].key == 0)
					gmw_status_line(starCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");
				else
				{
					reset_all(star, row_num, col_num);
					star[row_now + 1][col_now + 1].state = RELATED_STATE;//重新设置
				}
			}
			else if (MAction == MOUSE_RIGHT_BUTTON_CLICK) {
				exit(0);//游戏退出
			}
		}



	}
}
int main()
{
	srand(unsigned int(time(0)));

	CONSOLE_GRAPHICS_INFO starCGI; //声明一个CGI变量

	/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
	gmw_init(&starCGI);
	gmw_set_color(&starCGI, COLOR_BLACK, COLOR_WHITE);			//整个窗口颜色
	gmw_set_font(&starCGI, "新宋体", 24);						//字体
	gmw_set_frame_style(&starCGI, 6, 3, true);					//游戏主区域风格：每个色块宽6高3，有分隔线
	gmw_set_frame_color(&starCGI, COLOR_WHITE, COLOR_BLACK);	//游戏主区域颜色
	gmw_set_block_border_switch(&starCGI, true);						//小色块带边框
	gmw_set_rowno_switch(&starCGI, true);						//显示行号
	gmw_set_colno_switch(&starCGI, true);						//显示列标
	gmw_set_status_line_switch(&starCGI, TOP_STATUS_LINE, true);    //开启上状态栏
	gmw_set_status_line_color(&starCGI, TOP_STATUS_LINE);
	gmw_set_status_line_color(&starCGI, LOWER_STATUS_LINE);
	gmw_set_status_line_switch(&starCGI, LOWER_STATUS_LINE, true);	//开启下状态栏
	int score = 0;
	elem star[12][12];
	int row_num = input_rc('r');
	int col_num = input_rc('c');
	char temp[40] = { 0 };
	while (1) {
		game_star(&starCGI, star, row_num, col_num, score);

		while (1)
			if (_getche() == '\r')
				break;
	}
}