/*1852877 计科 赵昊堃*/
#include <iostream>
#include <cstring>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include "90-02-b1-star.h"
using namespace std;

void funA(RC rc)//rc:数组行列数目,rc1:输入位置的行列数据
{
	elem star[12][12];
	RC rc1 = { 0,0 };
	init_array(rc, star);
	show_array(rc, star, 0);
	while (1) {
		rc1 = command_pos(rc, star);
		if (command_checksur(rc1, star))
			break;
	}
	show_array(rc, star, 1);
	show_array(rc, star, 2);
	check_end();
}

void funB(RC rc)
{
	elem star[12][12];
	RC rc1 = { 0,0 };
	int score = 0;
	init_array(rc, star);
	show_array(rc, star, 0);
	while (1) {
		rc1 = command_pos(rc, star);
		if (command_checksur(rc1, star))
			break;
	}
	show_array(rc, star, 1);
	show_array(rc, star, 2);
	command_clear(rc, rc1, star, score);
	check_end();
}

void funC(RC rc)
{
	elem star[12][12];
	RC rc1 = { 0,0 };
	int score = 0;
	init_array(rc, star);
	while (1) {
		show_array(rc, star, 0);
		while (1) {
			rc1 = command_pos(rc, star);
			if (command_checksur(rc1, star))
				break;
		}
		show_array(rc, star, 1);
		show_array(rc, star, 2);
		command_clear(rc, rc1, star, score);

		cout << "\n本次消除结束,按回车进行下一次消除...";
		while (1)
			if (_getche() == '\r')
				break;
		if (check_gameover(rc, star)) {
			show_array(rc, star, 0);
			cct_setcolor(COLOR_HYELLOW, COLOR_RED);
			cout << "\n剩余" << traverse_array(rc, star, "count") << "个不可消除项,本关结束";
			cct_setcolor();
			cout << endl;
			break;
		}
	}
	check_end();
}

void funD(RC rc)
{
	elem star[12][12];
	int pos_y = 4 + BLOCK_H * rc.row;
	init_array(rc, star);
	draw_border(rc);
	draw_block(rc, star);
	RC rc1 = { rc.row,1 };
	cct_gotoxy(0, pos_y);
	cout << "箭头键/鼠标移动,回车键/单击左键选择并结束";
	Mouse_and_Key(rc, star, rc1);
	cct_gotoxy(0, pos_y);
	cout << "选中了" << (char)(rc1.row - 1 + 'A') << "行" << rc1.column - 1 << "列" << "                             ";
	cout << '\n' << endl;
	check_end();
}

void funE(RC rc)
{
	elem star[12][12];
	int pos_y = 4 + (1 + BLOCK_H) * rc.row;
	init_array(rc, star);
	draw_borderplus(rc);
	draw_blockplus(rc, star);
	RC rc1 = { rc.row,1 };
	cct_gotoxy(0, pos_y);
	cout << "箭头键/鼠标移动,回车键/单击左键选择并结束";
	Mouse_and_Key(rc, star, rc1, 1);
	cct_gotoxy(0, pos_y);
	cout << "选中了" << (char)(rc1.row - 1 + 'A') << "行" << rc1.column - 1 << "列" << "                             ";
	cout << '\n' << endl;
	check_end();
}

void funF(RC rc)
{
	elem star[12][12];
	int pos_y = 4 + BLOCK_H * rc.row;
	int score = 0;
	init_array(rc, star);
	draw_border(rc);
	draw_block(rc, star);
	game_once(rc, star);
	cct_gotoxy(0, pos_y);
	cout << "本次合成结束,按C/单击左键继续新一次的合成                 ";
	cout << '\n' << endl;
	check_end();
}

void funG(RC rc)
{
	elem star[12][12];
	int pos_y = 4 + BLOCK_H * rc.row;
	int score = 0;
	init_array(rc, star);
	draw_border(rc);
	draw_block(rc, star);
	while (1) {
		GAME(rc, star, score);
		if (check_gameover(rc, star)) {
			cct_gotoxy(0, pos_y);
			int rest = traverse_array(rc, star, "count");
			cout << "剩余" << rest << "个不可消除项,本关结束,回车下一关          ";
			score += BONUS * (rc.row * rc.column - rest);
			cct_gotoxy(0, 0);
			cout << "本次得分:" << BONUS * (rc.row * rc.column - rest) << "      总得分:" << score << "      ";
			while (1)
				if (_getche() == '\r') {
					init_array(rc, star);
					draw_border(rc);
					draw_block(rc, star);
					break;
				}
		}
	}
}