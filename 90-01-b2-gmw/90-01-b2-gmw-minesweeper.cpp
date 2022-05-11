/*1852877 �ƿ� ��껈�*/
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
	int open;//0:�ر�/1:��/-1:���
};

/********************************����01:�Ѷ�ѡ��˵�*************************************************/
void choose_menu(int& row_num, int& col_num, int& mine_num)//����:ѡ���Ѷ�
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
/********************************����02:����0~N-1�����*************************************************/
int RAND(int N)
{
	return rand() % N;
}
/********************************����03:������Χ�˸����ӵ�����*************************************************/
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
/********************************����04:�ڲ������ʼ��*************************************************/
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
			if (a[i][j].key == 9)//��ԪΪ����
				continue;
			else
				a[i][j].key = check_sur(i, j, a);
		}
	}
}

/********************************����05:�����������*************************************************/
void DFS(int row, int col, int row_num, int col_num, mine a[18][32])
{
	a[row][col].open = OPEN;
	if (a[row][col].key == 0)
		for (int i = row - 1; i <= row + 1; i++)
			for (int j = col - 1; j <= col + 1; j++)
				if (a[i][j].open == CLOSE && (i >= 1 && i <= row_num) && (j >= 1 && j <= col_num))
					DFS(i, j, row_num, col_num, a);
}

/********************************����06:ȫ�ַ����ػ�*************************************************/
void draw_all_block(const CONSOLE_GRAPHICS_INFO* const mineCGI, const int row_num, const int col_num, mine a[18][32], const BLOCK_DISPLAY_INFO* const bdi_normal)
{
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (a[i + 1][j + 1].open == 0)//�ر�
				gmw_draw_block(mineCGI, i, j, CLOSE_STATE, bdi_normal);
			else if (a[i + 1][j + 1].open == 1)//��
				gmw_draw_block(mineCGI, i, j, a[i + 1][j + 1].key, bdi_normal);
			else if (a[i + 1][j + 1].open == -1)//���
				gmw_draw_block(mineCGI, i, j, MARK_STATE, bdi_normal);
		}
	}
}
/********************************����07:����Ƿ�ͨ��*************************************************/
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
/********************************��Ϸ����*************************************************/
void game_mine(CONSOLE_GRAPHICS_INFO* mineCGI)
{
	mine a[18][32];
	int row_num, col_num, mine_num;

	choose_menu(row_num, col_num, mine_num);

	time_t start_time = clock();

	init_array(a, row_num, col_num, mine_num);
	gmw_set_rowcol(mineCGI, row_num, col_num);
	/*�����*/
	gmw_draw_frame(mineCGI);
	/* ��״̬����ʾ���� */
	gmw_status_line(mineCGI, TOP_STATUS_LINE, "ESC�˳�,�ո���ʾʱ��");
	/* ��״̬����ʾ���� */
	gmw_status_line(mineCGI, LOWER_STATUS_LINE, "[��ǰ���]");

	const BLOCK_DISPLAY_INFO bdi_normal[] = {
				{BDI_VALUE_BLANK, COLOR_HYELLOW, -1, NULL},  //0����ʾ���ÿո���伴��
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
				{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	/*��ʼ��仭ɫ��:ȫ���ر�   *�ر�ע���ڲ��������д�1��ʼ,�����ߺ�����0��ʼ*/
	for (int i = 0; i < row_num; i++)
		for (int j = 0; j < col_num; j++)
			gmw_draw_block(mineCGI, i, j, CLOSE_STATE, bdi_normal);

	int MRow, MCol, MAction, keycode1, keycode2;
	int time = 0;//��¼�Ƿ�Ϊ��һ��
	bool win = false;//�Ƿ�ʤ��
	int remain = mine_num;//��¼ʣ�������

	cct_enable_mouse();
	while (1) {
		int event = gmw_read_keyboard_and_mouse(mineCGI, MAction, MRow, MCol, keycode1, keycode2, true);
		/*�����¼�*/
		if (event == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 27)
				return;//�˳�
			else if (keycode1 == 32) {
				char msg[40] = { 0 };
				sprintf(msg, "��ǰʱ��:%d��,ESC�˳�,�ո���ʾʱ��", int(clock() - start_time) / CLOCKS_PER_SEC);
				gmw_status_line(mineCGI, TOP_STATUS_LINE, msg);
			}
		}
		if (event == CCT_MOUSE_EVENT) {
			if (MAction == MOUSE_LEFT_BUTTON_CLICK) {//�����
				time++;
				/*�����״�����*/
				while (a[MRow + 1][MCol + 1].key == 9 && time == 1)
					init_array(a, row_num, col_num, mine_num);

				/*һ�����*/
				if (a[MRow + 1][MCol + 1].open == CLOSE) {
					if (a[MRow + 1][MCol + 1].key == 9) {//����
						a[MRow + 1][MCol + 1].open = OPEN;
						gmw_draw_block(mineCGI, MRow, MCol, a[MRow + 1][MCol + 1].key, bdi_normal);
						/*��״̬��*/
						char msg[40] = { 0 };
						sprintf(msg, "��ǰʱ��:%d��,ESC�˳�,�ո���ʾʱ��", int(clock() - start_time) / CLOCKS_PER_SEC);
						gmw_status_line(mineCGI, TOP_STATUS_LINE, msg);
						/*��״̬��*/
						gmw_status_line(mineCGI, LOWER_STATUS_LINE, "��Ϸ����,������");
						return;//�˳�
					}
					DFS(MRow + 1, MCol + 1, row_num, col_num, a);
					/*��ɫ��*/
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

			/*��굥����״̬����Ϣ�ı�*/
			if (MAction == MOUSE_LEFT_BUTTON_CLICK || MAction == MOUSE_RIGHT_BUTTON_CLICK) {
				/*��״̬��*/
				int dis_remain = (remain < 0) ? 0 : remain;
				char msg[40] = { 0 };
				sprintf(msg, "ʣ������:%d��,ESC�˳�,�ո���ʾʱ��", dis_remain);
				gmw_status_line(mineCGI, TOP_STATUS_LINE, msg);
			}
		}
		/*����Ƿ��ʤ*/
		if (check_win(a, row_num, col_num, mine_num)) {
			draw_all_block(mineCGI, row_num, col_num, a, bdi_normal);
			/*��״̬��*/
			char msg[40] = { 0 };
			sprintf(msg, "��ǰʱ��:%d��,ESC�˳�,�ո���ʾʱ��", int(clock() - start_time) / CLOCKS_PER_SEC);
			gmw_status_line(mineCGI, TOP_STATUS_LINE, msg);
			/*��״̬��*/
			gmw_status_line(mineCGI, LOWER_STATUS_LINE, "��Ϸ����,��Ӯ��");
			return;//�˳�
		}
	}
}



int main()
{
	srand(unsigned int(time(0)));

	CONSOLE_GRAPHICS_INFO mineCGI; //����һ��CGI����

	/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
	gmw_init(&mineCGI);
	gmw_set_color(&mineCGI, COLOR_BLACK, COLOR_WHITE);			//����������ɫ
	gmw_set_font(&mineCGI, "������", 24);						//����
	gmw_set_frame_style(&mineCGI, 4, 2, true);					//��Ϸ��������ÿ��ɫ���4��2���зָ���
	gmw_set_frame_color(&mineCGI, COLOR_WHITE, COLOR_BLACK);	//��Ϸ��������ɫ
	gmw_set_block_border_switch(&mineCGI);						//Сɫ����߿�
	gmw_set_rowno_switch(&mineCGI, true);						//��ʾ�к�
	gmw_set_colno_switch(&mineCGI, true);						//��ʾ�б�
	gmw_set_status_line_switch(&mineCGI, TOP_STATUS_LINE, true);    //������״̬��
	gmw_set_status_line_color(&mineCGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(&mineCGI, LOWER_STATUS_LINE, true);	//������״̬��
	gmw_set_status_line_color(&mineCGI, LOWER_STATUS_LINE);
	while (1) {
		game_mine(&mineCGI);
		cout << "���س�����..." << endl;
		while (_getch() != '\r')
			continue;
	}
}
