/*1852877 �ƿ� ��껈�*/
#include <iostream>
#include <cstring>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include "90-02-b1-star.h"
using namespace std;

void funA(RC rc)//rc:����������Ŀ,rc1:����λ�õ���������
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

		cout << "\n������������,���س�������һ������...";
		while (1)
			if (_getche() == '\r')
				break;
		if (check_gameover(rc, star)) {
			show_array(rc, star, 0);
			cct_setcolor(COLOR_HYELLOW, COLOR_RED);
			cout << "\nʣ��" << traverse_array(rc, star, "count") << "������������,���ؽ���";
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
	cout << "��ͷ��/����ƶ�,�س���/�������ѡ�񲢽���";
	Mouse_and_Key(rc, star, rc1);
	cct_gotoxy(0, pos_y);
	cout << "ѡ����" << (char)(rc1.row - 1 + 'A') << "��" << rc1.column - 1 << "��" << "                             ";
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
	cout << "��ͷ��/����ƶ�,�س���/�������ѡ�񲢽���";
	Mouse_and_Key(rc, star, rc1, 1);
	cct_gotoxy(0, pos_y);
	cout << "ѡ����" << (char)(rc1.row - 1 + 'A') << "��" << rc1.column - 1 << "��" << "                             ";
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
	cout << "���κϳɽ���,��C/�������������һ�εĺϳ�                 ";
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
			cout << "ʣ��" << rest << "������������,���ؽ���,�س���һ��          ";
			score += BONUS * (rc.row * rc.column - rest);
			cct_gotoxy(0, 0);
			cout << "���ε÷�:" << BONUS * (rc.row * rc.column - rest) << "      �ܵ÷�:" << score << "      ";
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