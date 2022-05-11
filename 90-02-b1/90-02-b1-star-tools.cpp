/*1852877 �ƿ� ��껈�*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include "90-02-b1-star.h"
#include "../include/common_arraytools.h"
using namespace std;

///////////////////////////////////////////����01:����:��ʾ���벢��ȡ������//////////////////////////
int input_rc(char ch) {
	int k;
	int start_x, start_y, input_x, input_y;
	cct_getxy(start_x, start_y);
	if (ch == 'r')
		cout << "����������(8-10):   ";
	else
		cout << "����������(8-10):   ";
	cct_getxy(input_x, input_y);
	while (1) {
		cin >> k;
		if (cin.good() && k >= 8 && k <= 10)
			break;
		else {
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "���벻�Ϸ�����������" << endl;

			if (ch == 'r') {
				cct_gotoxy(start_x, start_y);
				cout << "����������(8-10):               ";
				cct_gotoxy(input_x, input_y);
			}
			else {
				cct_gotoxy(start_x, start_y);
				cout << "����������(8-10):               ";
				cct_gotoxy(input_x, input_y);
			}
			continue;
		}
	}
	return k;
}
//////////////////////////////////////////����02:����:�˵���ѡ�񲢵�����Ӧ�ķ���///////////////////////
void choose_menu(char ch)
{
	cct_cls();
	cct_setconsoleborder(60, 40, 60, 9000);
	RC rc;
	rc.row = input_rc('r');
	rc.column = input_rc('c');
	switch (ch) {
		case 'A':
			funA(rc);
			break;
		case 'B':
			funB(rc);
			break;
		case 'C':
			funC(rc);
		case 'D':
			funD(rc);
			break;
		case 'E':
			funE(rc);
			break;
		case 'F':
			funF(rc);
			break;
		case 'G':
			funG(rc);
			break;
	}
}
///////////////////////////////////////////////����03:����:�������ʼ��//////////////////////////////////
void init_array(RC rc, elem star[12][12])//ע���ڲ�������Ԫ��Ϊstar[1][1]
{
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 12; j++) {
			star[i][j].key = 0;
			star[i][j].state = 1;
		}

	for (int i = 1; i <= rc.row; i++)
		for (int j = 1; j <= rc.column; j++)
			star[i][j].key = RAND(5) + 1;

}
/*****************************************����04:����:���ݲ���չʾ��������µ�����************************/
void show_array(RC rc, elem star[12][12], int state)
{
	int row = rc.row;
	int col = rc.column;
	cct_setcolor();
	cout << endl;
	switch (state) {
		case 0:
			cout << "\n��ǰ����:" << endl;
			break;
		case 1:
			cout << "\n���ҽ������:" << endl;
			break;
		case 2:
			cout << "\n��ǰ����(��ͬɫ��ʶ):" << endl;
			break;
		case 3:
			cout << "\n��ֵͬ�鲢�������(��ͬɫ��ʶ):" << endl;
			break;
		case 4:
			cout << "\n����������" << endl;
			break;
	}
	cout << "  |";
	for (int i = 0; i < col; i++) {
		cout << "  " << i;
	}
	cout << endl;
	cout << "--+";
	for (int i = 0; i < 3 * col + 2; i++)
		cout << '-';
	cout << endl;
	for (int i = 1; i <= row; i++) {//ע���ڲ�������Ԫ��Ϊstar[1][1]
		cout << char('A' + i - 1) << " |";
		for (int j = 1; j <= col; j++) {
			switch (state) {
				//��ǰ����&����������
				case 0:
				case 4://key=0ȫ������
					if (star[i][j].key == 0) {
						cout << "  ";
						cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
						cout << star[i][j].key;
						cct_setcolor();
					}
					else
						cout << "  " << star[i][j].key;
					break;
					//���ҽ������
				case 1://��ʾ0��*
					if (star[i][j].state == 2)
						cout << "  *";
					else
						cout << "  0";
					break;
					//��ǰ����(��ͬɫ��ʶ)&��ֵͬ�鲢�������(��ͬɫ��ʶ)
				case 2:
				case 3://*������ʾ
					if (star[i][j].state == 2 || star[i][j].state == 3) {
						cout << "  ";
						cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
						cout << star[i][j].key;
						cct_setcolor();
					}
					else
						cout << "  " << star[i][j].key;
					break;
			}
		}
		cout << endl;
	}
}
////////////////////////////////////////////����05:����:���&���ܺϷ�������(����Ϸ�)/////////////////
RC command_pos(RC rc, elem star[12][12])
{
	cout << "\n������ĸ+������ʽ[����c2]����������꣺";
	char ch1, ch2;
	RC rc1 = { 0,0 }, pos = { 0,0 };
	cct_getxy(pos.row, pos.column);
	while (1) {
		//////////////cin�����ж�////////////////
		cin >> ch1 >> ch2;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "��������,����������";
			cct_showstr(pos.row, pos.column, "                        ");
			cct_gotoxy(pos.row, pos.column);
			continue;
		}
		/////////////����Խ���ж�///////////////
		bool flag1 = true, flag2 = true;
		char c1 = isType(ch1, "small") ? ch1 - 'a' + 'A' : ch1;//���ch1��Сд��ĸ,תΪ��дͳһ�ж�
		if (!(isType(c1, "capital") && c1 <= 'A' + rc.row - 1))
			flag1 = false;
		if (!(isType(ch2, "number") && ch2 <= '0' + rc.column - 1))
			flag2 = false;
		if (flag1 && flag2) {
			rc1.row = c1 - 'A' + 1;
			rc1.column = ch2 - '0' + 1;
			cout << "����Ϊ" << c1 << "��" << ch2 << "��            " << endl;
			break;
		}
		else {
			cout << "��������,����������";
			cct_showstr(pos.row, pos.column, "                        ");
			cct_gotoxy(pos.row, pos.column);
			continue;
		}

	}
	return rc1;
}
/////////////////////����06:����:����ܱ�(��������)��ͬkey��Ԫ��////////////////////////////////
void search_sur(elem star[][12], int row, int col, int* pcnt, int execute) {
	if (execute == 0) {//����ģʽ,�ҵ��������Ϊstate=2
		if (star[row][col].key == 0 || star[row][col].state != 1)
			return;
		else {
			star[row][col].state = 2;
			(*pcnt)++;
		}
	}
	else if (execute == 2) {//����ģʽ,���������Ķ���Ϊstate=2&&key=0
		if (star[row][col].key == 0 || star[row][col].state != 2)
			return;
		else {
			star[row][col].state = 3;
			(*pcnt)++;
		}
	}
	if (star[row - 1][col].key == star[row][col].key)
		search_sur(star, row - 1, col, pcnt, execute);
	if (star[row + 1][col].key == star[row][col].key)
		search_sur(star, row + 1, col, pcnt, execute);
	if (star[row][col - 1].key == star[row][col].key)
		search_sur(star, row, col - 1, pcnt, execute);
	if (star[row][col + 1].key == star[row][col].key)
		search_sur(star, row, col + 1, pcnt, execute);
}
//////////////////////����07:����:���&���ܺϷ�������(�Ƿ�Ϊ��,�Ƿ�����)////////////////
bool command_checksur(RC rc1, elem star[][12])
{
	int cnt = 0, * pcnt = &cnt;
	bool legal = true;
	if (star[rc1.row][rc1.column].key == 0) {
		cout << "�����������λ��Ϊ0,����������" << endl;
		legal = false;
	}
	else {
		search_sur(star, rc1.row, rc1.column, pcnt, 0);
		if (cnt == 1) {
			star[rc1.row][rc1.column].state = 1;//������,��Ҫ����״ֵ̬
			cout << "�����������λ����������ֵͬ,����������" << endl;
			legal = false;
		}

	}
	return legal;
}
//////////////////////����08:����:���end///////////////////////////////////////
void check_end()
{
	while (1) {
		cout << "\n��С�����,����End����...";
		char s[20];
		cin >> s;
		if (strcmp(_strlwr(s), "end") == 0)
			break;
		else
			cout << "�������,����������" << endl;
	}
}
//////////////////////����09:����:��������&&ִ����������ͽ���һ���˻�״̬�Ĳ���///////////////////////////////
int traverse_array(RC rc, elem star[][12], const char* s)//clear:�����Һ�Yȷ�Ϻ��Ԫ����0;reset������state=3��Ԫ����1;count:�������·���Ԫ�ظ���
{
	int count = 0;
	for (int i = 1; i <= rc.row; i++)
		for (int j = 1; j <= rc.column; j++)
			if (strcmp(s, "clear") == 0) {
				if (star[i][j].state == 3)
					star[i][j].key = 0;
			}
			else if (strcmp(s, "resetstate") == 0) {
				star[i][j].state = 1;
			}
			else {
				if (star[i][j].key != 0)
					count++;
			}
	return count;
}
/////////////////////����10:����:ʵ�������������Ʋ���/////////////////////////////////////////////////
bool is_allzero(elem star[][12], int row, int col_no)////��װ��һ������:�����к�����������,�����Ƿ�Ϊȫ����
{
	int	zerocnt = 0;
	for (int i = 1; i <= row; i++)
		if (star[i][col_no].key == 0)
			zerocnt++;
	if (zerocnt == row)//����ȫ����,�����ƶ�
		return true;
	else
		return false;
}

void fall_array(RC rc, elem star[][12])
{
	cout << "\n���س����������������...";
	while (1)
		if (_getche() == '\r')
			break;

	for (int j = 1; j <= rc.column; j++) {//�����ƶ�:�߼���
		int temp[12] = { 0 }, * p = temp, * q = temp;
		for (int i = rc.row; i >= 1; i--)
			if (star[i][j].key != 0)
				*p++ = star[i][j].key;
		for (int i = rc.row; i >= 1; i--)
			star[i][j].key = *q++;
	}

	//ֻ�г���ȫ����&&�Ҳ������з�ȫ���е�ʱ��,�Ž��������ƶ�

	while (1) {
		bool out = true;
		for (int j = 1; j <= rc.column; j++)
			if (is_allzero(star, rc.row, j) && !is_allzero(star, rc.row, j + 1))
				out = false;
		if (out == true)
			break;
		else {
			for (int j = 1; j <= rc.column; j++)
				if (is_allzero(star, rc.row, j) && !is_allzero(star, rc.row, j + 1)) {
					//�������Ʋ���:ʵ�����ý�����Ϊ����,��ȫ���н��������Ҳ�
					for (int i = 1; i <= rc.row; i++) {
						star[i][j].key = star[i][j + 1].key;
						star[i][j + 1].key = 0;
					}
				}
		}
	}

}

/************************************����11:����:����봦��Y/N/Q*******************8888*****************/
int command_clear(RC rc, RC rc1, elem star[][12], int& score)//int &score��¼�ܷ�
{
	cout << "��ȷ���Ƿ�������������(Y/N/Q):";
	char cmd;
	int cnt = 0, * pcnt = &cnt;
	while (1) {
		cmd = _getche();
		if (cmd == 'Y' || cmd == 'y' || cmd == 'N' || cmd == 'n' || cmd == 'Q' || cmd == 'q')
			break;
	}

	if (cmd == 'Y' || cmd == 'y')
	{
		search_sur(star, rc1.row, rc1.column, pcnt, 2);
		traverse_array(rc, star, "clear");
		show_array(rc, star, 2);
		score += BONUS * cnt;
		cout << "���ε÷�:" << BONUS * cnt << "  �ܵ÷�:" << score << endl;
		traverse_array(rc, star, "resetstate");
		fall_array(rc, star);
		show_array(rc, star, 4);
		return 1;

	}
	else if (cmd == 'N' || cmd == 'n') {
		traverse_array(rc, star, "resetstate");
		return 1;
	}


	else {//q Q
		traverse_array(rc, star, "resetstate");
		return 0;
	}
}
/**********************************����12:����:����Ƿ����޿ɼ���������Ԫ��(��Ϸ�Ƿ����)*************************/
bool check_gameover(RC rc, elem star[][12])//gameover����0����δ����,����1�������
{
	for (int i = 1; i <= rc.row; i++)
		for (int j = 1; j <= rc.column; j++) {
			if (star[i][j].key != 0) {
				int cnt = 0, * pcnt = &cnt;
				search_sur(star, i, j, pcnt);
				if (cnt > 1) {
					traverse_array(rc, star, "resetstate");
					return 0;
				}
			}
		}
	traverse_array(rc, star, "resetstate");
	return 1;
}