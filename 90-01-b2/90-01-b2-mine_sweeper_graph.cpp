/*1852877 计科 赵昊堃*/
#include "90-01-b2-mine_sweeper.h"
#include "../include/cmd_console_tools.h"
#include "../include/common_arraytools.h"
#include <iostream>
using namespace std;
//////////////////////////////////////////////////////////////////////////////////
void draw_table(int row, int col, mine a[18][32]) {

	for (int i = 1; i <= col; i++) {
		cct_gotoxy(6 * i - 1, 1);
		cout << i - 1;
	}
	for (int i = 1; i <= row; i++) {
		cct_gotoxy(0, 1 + 3 * i);
		cout << (char)('A' + i - 1);
	}
	draw_borderlines(row, col, 4, 2, 2, 2);
}
void draw_square(int row, int col, mine a[18][32])
{

	for (int r = 1; r <= row; r++) {
		for (int c = 1; c <= col; c++) {
			cct_gotoxy(4 + 6 * (c - 1), 3 + 3 * (r - 1));
			if (a[r][c].open == 0) {
				cct_setcolor(12, a[r][c].key);
				cout << "    ";
				cct_gotoxy(4 + 6 * (c - 1), 4 + 3 * (r - 1));
				cout << "    ";
			}
			else if (a[r][c].open == 1) {
				cct_setcolor(14, a[r][c].key);
				cout << "    ";
				cct_gotoxy(4 + 6 * (c - 1), 4 + 3 * (r - 1));
				if (a[r][c].key == 0)
					cout << "    ";
				else if (a[r][c].key == 9)
					cout << " " << '*' << "  ";
				else
					cout << " " << a[r][c].key << "  ";
			}
			else if (a[r][c].open == -1) {
				cct_setcolor(10, 7);
				cout << "    ";
				cct_gotoxy(4 + 6 * (c - 1), 4 + 3 * (r - 1));
				cout << " " << '#' << "  ";
			}
			cct_setcolor();
		}
	}
}

void show_graph(mine a[18][32], int row, int col)
{
	draw_table(row, col, a);
	draw_square(row, col, a);
	cout << endl;
}
/////////////////////////////////////////////////////////////////////////////////
void mouse_click(int row, int col, char level, mine a[18][32])
{
	cout << endl;
	cout << endl;
	cout << "【当前光标】";
	int MX, MY, MAction, keycode1, keycode2, x1, y1;
	bool legal;
	cct_getxy(x1, y1);
	cct_enable_mouse();
	while (cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2) == 0)//鼠标事件
	{
		legal = 0;
		if (MAction == MOUSE_ONLY_MOVED || MAction == MOUSE_LEFT_BUTTON_CLICK) {
			for (int r = 1; r <= row; r++)
				for (int c = 1; c <= col; c++)
					if ((4 + 6 * (c - 1)) <= MX && MX <= (7 + 6 * (c - 1)) && (3 + 3 * (r - 1)) <= MY && MY <= (4 + 3 * (r - 1))) {
						legal = 1;
						cct_gotoxy(x1, y1);
						cout << (char)(r - 1 + 'A') << " 行" << c - 1 << " 列";
						if (MAction == MOUSE_LEFT_BUTTON_CLICK && legal == 1) {
							while (a[r][c].key == 9) {
								init_array(a, level);
							}
							DFS_traverse(r, c, a, level);
							draw_square(row, col, a);
							return;
						}
					}
			if (legal == 0) {
				cct_gotoxy(x1, y1);
				cout << "位置非法";
			}
		}
	}

}
///////////////////////////////////////////////////////////////
void check_mouse(int row, int col)
{
	cout << endl;
	cout << endl;
	cout << "【当前光标】";
	int MX, MY, MAction, keycode1, keycode2, x1, y1;
	bool legal;
	cct_getxy(x1, y1);
	cct_enable_mouse();
	while (cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2) == 0)//鼠标事件
	{
		legal = 0;
		if (MAction == MOUSE_LEFT_BUTTON_CLICK)
			return;
		else if (MAction == MOUSE_ONLY_MOVED) {
			for (int r = 1; r <= row; r++)
				for (int c = 1; c <= col; c++)
					if ((4 + 6 * (c - 1)) <= MX && MX <= (7 + 6 * (c - 1)) && (3 + 3 * (r - 1)) <= MY && MY <= (4 + 3 * (r - 1))) {
						legal = 1;
						cct_gotoxy(x1, y1);
						cout << (char)(r - 1 + 'A') << " 行" << c - 1 << " 列";
					}
			if (legal == 0) {
				cct_gotoxy(x1, y1);
				cout << "位置非法";
			}
		}
	}
}
int get_num(char level)
{
	if (level == '1')
		return 10;
	else if (level == '2')
		return 40;
	else
		return 99;
}
void GAME(int row, int col, char level, mine a[18][32], int mode, time_t start_time)
{
	int remain = get_num(level);
	if (mode == 9)
		cout << "ESC退出，空格显示时间" << endl;
	else if (mode == 8)
		cout << "ESC退出" << endl;
	show_graph(a, row, col);
	cout << endl;
	cout << endl;
	cout << "【当前光标】";
	int MX, MY, MAction, keycode1, keycode2, x1, y1;
	bool legal;
	cct_getxy(x1, y1);
	cct_enable_mouse();
	int time = 0;
	bool win = 0;
	while (1) {
		legal = 0;
		if (cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2) == 1)//键盘事件
			if (keycode1 == 27)
				return;
			else if (keycode1 == 32 && mode == 9) {
				cct_gotoxy(0, 0);
				cout << "当前时间：" << double(clock() - start_time) / CLOCKS_PER_SEC << "秒," << "ESC退出，空格显示时间           " << endl;
			}
		if (MAction == MOUSE_ONLY_MOVED || MAction == MOUSE_LEFT_BUTTON_CLICK || MAction == MOUSE_RIGHT_BUTTON_CLICK) {
			for (int r = 1; r <= row; r++)
				for (int c = 1; c <= col; c++)
					if ((4 + 6 * (c - 1)) <= MX && MX <= (7 + 6 * (c - 1)) && (3 + 3 * (r - 1)) <= MY && MY <= (4 + 3 * (r - 1))) {
						legal = 1;
						cct_gotoxy(x1, y1);
						cout << (char)(r - 1 + 'A') << " 行" << c - 1 << " 列";
						if (MAction == MOUSE_LEFT_BUTTON_CLICK && legal == 1) {
							time++;
							while (a[r][c].key == 9 && time == 1)
								init_array(a, level);
							if (a[r][c].open == 0) {
								if (a[r][c].key == 9) {
									a[r][c].open = 1;
									draw_square(row, col, a);
									cout << endl;
									cout << endl;
									cout << endl;
									cout << endl;
									cout << "游戏结束，你输了" << endl;
									if (mode == 9) {
										cct_gotoxy(0, 0);
										cout << "当前时间：" << double(clock() - start_time) / CLOCKS_PER_SEC << "秒," << "ESC退出，空格显示时间" << endl;
									}
									return;
								}
								DFS_traverse(r, c, a, level);
								draw_square(row, col, a);
							}
						}
						else if (MAction == MOUSE_RIGHT_BUTTON_CLICK && legal == 1) {
							if (a[r][c].open == 0) {
								a[r][c].open = -1;
								draw_square(row, col, a);
								remain--;
							}
							else if (a[r][c].open == -1) {
								a[r][c].open = 0;
								draw_square(row, col, a);
								remain++;
							}

						}
					}
			if (legal == 0) {
				cct_gotoxy(x1, y1);
				cout << "位置非法";
			}
			if (mode == 9 && (MAction == MOUSE_LEFT_BUTTON_CLICK || MAction == MOUSE_RIGHT_BUTTON_CLICK)) {
				cct_gotoxy(0, 0);
				if (remain < 0)
					cout << "剩余雷数：" << 0 << "个，" << "ESC退出，空格显示时间               " << endl;
				else
					cout << "剩余雷数：" << remain << "个，" << "ESC退出，空格显示时间               " << endl;

			}
		}
		if (check_win(a, level)) {
			draw_square(row, col, a);
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			cout << "游戏结束，你赢了" << endl;
			if (mode == 9) {
				cct_gotoxy(0, 0);
				cout << "当前时间：" << double(clock() - start_time) / CLOCKS_PER_SEC << "秒," << "ESC退出，空格显示时间" << endl;
			}
			return;
		}
	}
}