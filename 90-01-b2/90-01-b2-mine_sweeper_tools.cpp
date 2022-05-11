/*1852877 �ƿ� ��껈�*/
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <conio.h>
#include "90-01-b2-mine_sweeper.h"
#include "../include/cmd_console_tools.h"
#include "../include/common_arraytools.h"
using namespace std;
////////////////////////////////////////////////
void choose_menu(char ch)//����:ѡ��˵���
{
	cct_cls();
	cout << "��ѡ���Ѷ�:" << endl;
	cout << "  1.����( 9*9  - 10����)" << endl;
	cout << "  2.�м�(16*16 - 40����)" << endl;
	cout << "  3.�߼�(16*30 - 99����)" << endl;
	cout << "������[1..3]:";
	char level;
	while (level = _getch()) {
		if (!(level == '1' || level == '2' || level == '3'))
			continue;
		else
			break;
	}
	switch (ch) {
		case '1':
			fun1(level);
			break;
		case '2':
			fun2(level);
			break;
		case '3':
			fun3(level);
			break;
		case '4':
			fun4(level);
			break;
		case '5':
			fun5(level);
			break;
		case '6':
			fun6(level);
			break;
		case '7':
			fun7(level);
			break;
		case '8':
			fun8(level);
			break;
		case '9':
			fun9(level);
			break;
	}
}
////////////////////////////////////////////////
int getrow(char level)
{
	if (level == '1')
		return 9;
	else
		return 16;
}
int getcol(char level)
{

	if (level == '1')
		return 9;
	else if (level == '2')
		return 16;
	else
		return 30;
}
int getnum(char level)
{
	if (level == '1')
		return 10;
	else if (level == '2')
		return 40;
	else
		return 99;
}

/////////////////////////////////////////////////////////////////////
int check_sur(int row, int col, mine a[18][32])//������Χ�˸����ӵ�����
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
//////////////////////////////////////////////////////////////////
void init_array(mine a[18][32], char level)//����һ����ά����
{

	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 32; j++) {
			a[i][j].key = 0;
			a[i][j].open = 0;
		}
	}
	int row = getrow(level);
	int col = getcol(level);
	int num = getnum(level);
	for (int i = 0; i < num; i++)
	{
		int r = 1 + RAND(row);
		int c = 1 + RAND(col);
		if (a[r][c].key == 9)
		{
			i--;
			continue;
		}
		else
			a[r][c].key = 9;
	}
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			if (a[i][j].key == 9)//��ԪΪ����
				continue;
			else
				a[i][j].key = check_sur(i, j, a);
		}
	}
}
///////////////////////////////////////////////////////////
void show_array(mine a[18][32], char level, bool color)
{
	int row = getrow(level);
	int col = getcol(level);
	cct_setcolor();
	cout << endl;
	cout << "�ڲ����飺" << endl;
	cout << "  |";
	for (int i = 1; i <= col; i++) {
		if (i >= 10) {
			char c = (char)('a' + i - 10);
			cout << c << ' ';
		}
		else
			cout << i << ' ';
	}
	cout << endl;
	cout << "--+";
	for (int i = 0; i < 2 * col + 2; i++)
		cout << '-';
	cout << endl;
	for (int i = 1; i <= row; i++) {
		cout << char('A' + i - 1) << " |";
		for (int j = 1; j <= col; j++) {
			if (a[i][j].open == 0)
				cout << 'X' << " ";
			else if (a[i][j].open == -1) {
				cct_setcolor(4, 7);
				cout << 'X';
				cct_setcolor();
				cout << " ";
			}
			else if (a[i][j].key == 9)
				cout << '*' << " ";
			else {
				if (color)
					cct_setcolor(14, a[i][j].key);
				cout << a[i][j].key;
				cct_setcolor();
				cout << " ";
			}

		}
		cout << endl;
	}
}
/// /////////////////////////////////////////////////////////////////
int getx(int col) {
	return 2 * col + 1;
}//������ת��x����
int gety(int row) {
	return 2 + row;
}//������ת��y����
//////////////////////////////////////////////////////////////////////
void DFS(int row, int col, int rowmax, int colmax, mine a[18][32])
{
	a[row][col].open = 1;
	if (a[row][col].key == 0)
		for (int i = row - 1; i <= row + 1; i++)
			for (int j = col - 1; j <= col + 1; j++)
				if (a[i][j].open == 0 && (i >= 1 && i <= rowmax) && (j >= 1 && j <= colmax))
					DFS(i, j, rowmax, colmax, a);
}
void DFS_traverse(int row, int col, mine a[18][32], char level)
{
	int rowmax = getrow(level);
	int colmax = getcol(level);
	DFS(row, col, rowmax, colmax, a);
}
//////////////////////////////////////////////////////////////////////
bool check_win(mine a[18][32], char level) {
	int row = getrow(level);
	int col = getcol(level);
	bool win = 1;
	int cnt = 0;
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			if (a[i][j].open == 1)
				cnt++;
	if (cnt == row * col - getnum(level))//���˵���ȫ��������
		return 1;
	return 0;
}
//////////////////////////////////////////////////////////////////////
void open_array(mine a[18][32], char level, bool& gameover, bool& exit, int& time, bool special, time_t start_time)
{
	int row = getrow(level);
	int col = getcol(level);
	cout << endl;
	if (special) {
		cout << "��������˵����& - ��Ϸ������ʱ��(���ַ����ɣ�����Ҫ������)" << endl;
		cout << "              ! - ��Ǹ�����Ϊ��(����!E3)" << endl;
		cout << "              # - ȡ�����      (����#E3)" << endl;
	}
	cout << "�������λ�õ��������꣨���к��У��ϸ����ִ�Сд������G1/Af����Q/q�˳�����";
	char ch1, ch2;
	int r, c;
	if (!special)
	{
		while (ch1 = _getch()) {
			if (ch1 == 'Q' || ch1 == 'q')
			{
				exit = 1;
				return;
			}
			else if (!('A' <= ch1 && ch1 <= 'A' + row - 1))
				continue;
			else {
				putchar(ch1);
				break;
			}
		}
		while (ch2 = _getch()) {
			if (col >= 9) {
				if (!('1' <= ch2 && ch2 <= '9' || 'a' <= ch2 && ch2 <= 'a' + (col - 10)))
					continue;
				else {
					putchar(ch2);
					break;
				}
			}
			else {
				if (!('1' <= ch2 && ch2 <= '9'))
					continue;
				else {
					putchar(ch2);
					break;
				}
			}
		}
	}
	else {
		char ch0;
		while (ch0 = _getch()) {
			if (ch0 == 'Q' || ch0 == 'q') {
				exit = 1;
				return;
			}

			else if (ch0 == '&') {
				putchar(ch0);
				cout << endl;
				cout << "������ʱ�䣺" << double(clock() - start_time) / CLOCKS_PER_SEC << "��" << endl;
				time--;//û�в��ײ��ƴ���
				return;
			}
			else if (ch0 == '!') {
				putchar(ch0);
				while (ch1 = _getch()) {
					if (!('A' <= ch1 && ch1 <= 'A' + row - 1))
						continue;
					else {
						putchar(ch1);
						break;
					}
				}
				while (ch2 = _getch()) {
					if (col >= 9) {
						if (!('1' <= ch2 && ch2 <= '9' || 'a' <= ch2 && ch2 <= 'a' + (col - 10)))
							continue;
						else {
							putchar(ch2);
							break;
						}
					}
					else {
						if (!('1' <= ch2 && ch2 <= '9'))
							continue;
						else {
							putchar(ch2);
							break;
						}
					}
				}
				r = ch1 - 'A' + 1;
				if (ch2 >= 'a')
					c = ch2 - 'a' + 10;
				else
					c = ch2 - '0';
				if (a[r][c].open == 0)//û�д�ʱ�ſ��Ա��
					a[r][c].open = -1;
				show_array(a, level, 1);
				time--;//û�в��ײ��ƴ���
				return;
			}
			else if (ch0 == '#') {
				putchar(ch0);
				while (ch1 = _getch()) {
					if (!('A' <= ch1 && ch1 <= 'A' + row - 1))
						continue;
					else {
						putchar(ch1);
						break;
					}
				}
				while (ch2 = _getch()) {
					if (col >= 9) {
						if (!('1' <= ch2 && ch2 <= '9' || 'a' <= ch2 && ch2 <= 'a' + (col - 10)))
							continue;
						else {
							putchar(ch2);
							break;
						}
					}
					else {
						if (!('1' <= ch2 && ch2 <= '9'))
							continue;
						else {
							putchar(ch2);
							break;
						}
					}
				}
				r = ch1 - 'A' + 1;
				if (ch2 >= 'a')
					c = ch2 - 'a' + 10;
				else
					c = ch2 - '0';
				if (a[r][c].open == -1) //û�д�ʱ�ſ��Ա��
					a[r][c].open = 0;
				show_array(a, level, 1);
				time--;//û�в��ײ��ƴ���
				return;
			}
			else if (!('A' <= ch0 && ch0 <= 'A' + row - 1))
				continue;
			else {
				putchar(ch0);
				ch1 = ch0;
				break;
			}
		}
		while (ch2 = _getch()) {
			if (col >= 9) {
				if (!('1' <= ch2 && ch2 <= '9' || 'a' <= ch2 && ch2 <= 'a' + (col - 10)))
					continue;
				else {
					putchar(ch2);
					break;
				}
			}
			else {
				if (!('1' <= ch2 && ch2 <= '9'))
					continue;
				else {
					putchar(ch2);
					break;
				}
			}
		}
	}

	r = ch1 - 'A' + 1;
	if (ch2 >= 'a')
		c = ch2 - 'a' + 10;
	else
		c = ch2 - '0';
	if (time == 1) {//�״ν���ı�������
		while (a[r][c].key == 9) {
			init_array(a, level);
		}
	}
	cout << endl;
	if (a[r][c].open == 0) {
		DFS_traverse(r, c, a, level);
		if (a[r][c].key == 9)
			gameover = 1;
	}
	show_array(a, level, 1);

}
///////////////////////////////////////////////////////////////
void game(mine a[18][32], char level, bool special, time_t start_time)
{
	bool gameover = 0;
	bool win = 0;
	bool exit = 0;
	int time = 1;
	while (!gameover && !win && !exit)
	{
		open_array(a, level, gameover, exit, time, special, start_time);
		time++;
		win = check_win(a, level);
	}
	if (gameover == 1) {
		cout << endl;
		cout << "�����ˣ���Ϸ������" << endl;
		return;
	}
	if (win == 1) {
		cout << endl;
		cout << "��ϲʤ������Ϸ������" << endl;
		return;
	}
}

