/*1852877 计科 赵昊堃*/
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <windows.h>
#include "../include/cmd_console_tools.h"
using namespace std;

/**********函数01:随机数****/
int RAND(int N)//功能:产生一个0~N-1的随机数
{
	return rand() % N;
}

/***********函数02:判断字符类型**************/
bool isType(char ch, const char* s)
{
	if (strcmp(s, "capital") == 0)
		return (ch >= 'A' && ch <= 'Z') ? true : false;
	else if (strcmp(s, "small") == 0)
		return (ch >= 'a' && ch <= 'z') ? true : false;
	else if (strcmp(s, "number") == 0)
		return (ch >= '0' && ch <= '9') ? true : false;
	else
		return false;
}

/************函数03:绘制不包括色块含有分界线的框线********************************/
void draw_borderlines(int row, int col, int block_w, int block_h, int start_x, int start_y)//参数:行列数,色块长宽,框线起始坐标
{
	cct_gotoxy(start_x, start_y);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	int rownum = 1 + (1 + block_h) * row;
	int colnum = 1 + (1 + block_w / 2) * col;//将一个字符宽度(2)视为一个列基本单位
	for (int line = 0; line < rownum; line++) {
		cct_gotoxy(start_x, start_y + line);
		//首行
		if (line == 0) {
			for (int i = 0; i < colnum; i++) {
				if (i == 0)
					cout << "╔";
				else if (i == colnum - 1)
					cout << "╗";
				else {
					if (i % (block_w / 2 + 1) == 0) {
						cout << "╦";
						Sleep(1);
					}
					else
						cout << "═";
				}
			}
		}
		//末行
		else if (line == rownum - 1) {
			for (int i = 0; i < colnum; i++) {
				if (i == 0)
					cout << "╚";
				else if (i == colnum - 1)
					cout << "╝";
				else {
					if (i % (block_w / 2 + 1) == 0) {
						cout << "╩";
						Sleep(1);
					}
					else
						cout << "═";
				}
			}
		}
		//中间行
		else {
			if (line % (1 + block_h) == 0) {
				for (int i = 0; i < colnum; i++) {
					if (i == 0)
						cout << "╠";
					else if (i == colnum - 1)
						cout << "╣";
					else {
						if (i % (block_w / 2 + 1) == 0) {
							cout << "╬";
							Sleep(1);
						}
						else
							cout << "═";
					}
				}
			}
			else {
				for (int i = 0; i < colnum; i++) {
					if (i % (block_w / 2 + 1) == 0) {
						cout << "║";
						Sleep(1);
					}
					else
						cout << "  ";
				}
			}
		}
	}
	cct_setcolor();
}