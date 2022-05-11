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

#define   BASEGOAL          512  
#define   TOUP					1
#define   TODOWN				2
#define   TOLEFT				3
#define   TORIGHT				4
/****************************����01�������Ϸ����**************************************/
void get_parameter(int& row_num, int& col_num, int& target, int& delay)
{
	int temp, start_x, start_y;
	cout << "��Ϸ2048��������" << endl;
	/*��*/
	cout << "����������[4-8]:";
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
	/*��*/
	cout << "����������[4-10]:";
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
	/*Ŀ�����*/
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

	cout << "������Ŀ�����[" << goal << '/' << 2 * goal << '/' << 4 * goal << '/' << 8 * goal << "]:";
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
	/*�ӳ�*/
	cout << "�����붯���ӳ�[0-5]:";
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


/********************************����02:����0~N-1�����*************************************************/
int RAND(int N)
{
	return rand() % N;
}
/****************************����03���ڿհ���������һ���µ�2**************************************/
bool array_generate(int a[8][10], int row_num, int col_num)
{
	/*�ڿհ�����������һ���µ�2,���û�п�λ�˷���false,���򷵻�true*/
	int cnt = 0;
	for (int i = 0; i < row_num; i++)
		for (int j = 0; j < col_num; j++)
			if (a[i][j] != 0)
				cnt++;
	if (cnt == row_num * col_num)//ȫ��
		return false;
	else {
		while (1)
		{
			int rand_i = RAND(row_num);
			int rand_j = RAND(col_num);
			if (a[rand_i][rand_j] == 0) {//��λ
				a[rand_i][rand_j] = 2;
				break;
			}
		}
		return true;
	}
}
/****************************����04��ȫ�ַ������**************************************/
void draw_all_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_num, const int col_num, int a[8][10], const BLOCK_DISPLAY_INFO* const bdi_normal)
{
	for (int i = 0; i < row_num; i++)
		for (int j = 0; j < col_num; j++)
			gmw_draw_block(pCGI, i, j, a[i][j], bdi_normal);
}

/****************************����05��exchange**************************************/
void exchange(int a[8][10], int row1, int col1, int row2, int col2, bool& generate)
{
	if (row1 == row2 && col1 == col2)
		return;//�ų�������
	int temp = a[row1][col1];
	a[row1][col1] = a[row2][col2];
	a[row2][col2] = temp;
	generate = true;
	return;
}
/****************************����06����λ**************************************/
void position(int a[8][10], int row_num, int col_num, int type, bool& generate, const CONSOLE_GRAPHICS_INFO* const pCGI, const BLOCK_DISPLAY_INFO* const bdi_normal)
{
	if (type == TOUP) {
		for (int j = 0; j < col_num; j++) {//���μ��ÿһ��
			for (int i = 0; i < row_num; i++) //���ϵ��¼��ÿһ��
				if (a[i][j] != 0) {//ֻ��������Ԫ��

					bool all_zero = true;
					/*ȫ��*/
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
		for (int j = 0; j < col_num; j++) {//���μ��ÿһ��
			for (int i = row_num - 1; i >= 0; i--) //���µ��ϼ��ÿһ��
				if (a[i][j] != 0) {//ֻ��������Ԫ��

					bool all_zero = true;
					/*ȫ��*/
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
		for (int i = 0; i < row_num; i++) {//���μ��ÿһ��
			for (int j = 0; j < col_num; j++) //��������
				if (a[i][j] != 0) {//ֻ��������Ԫ��

					bool all_zero = true;
					/*ȫ��*/
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
		for (int i = 0; i < row_num; i++) {//���μ��ÿһ��
			for (int j = col_num - 1; j >= 0; j--) //����������ÿһ��
				if (a[i][j] != 0) {//ֻ��������Ԫ��

					bool all_zero = true;
					/*ȫ��*/
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
/****************************����07���жϽ���**************************************/
bool check_sur(int temp_a[10][12], int row_no, int col_no)
{
	//false������Χû�пɺϲ���
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
	//��ûȫ��
	for (int i = 0; i < row_num; i++)
		for (int j = 0; j < col_num; j++)
			if (a[i][j] == 0)
				return false;
	//�Ѿ�ȫ������
	int temp_a[10][12] = { 0 };
	for (int i = 0; i < row_num; i++)
		for (int j = 0; j < col_num; j++) {
			temp_a[i + 1][j + 1] = a[i][j];
		}

	for (int i = 1; i <= row_num; i++)
		for (int j = 1; j <= col_num; j++) {
			if (check_sur(temp_a, i, j))//���кϲ���
				return false;
		}
	return true;

}
/****************************��Ϸ����**************************************/
void game_2048(CONSOLE_GRAPHICS_INFO* pCGI, int& max_score)
{


	int a[8][10] = { 0 };
	char temp[100] = { 0 };
	int row_num, col_num, target, delay;
	int score = 0;//�Ʒ���
	get_parameter(row_num, col_num, target, delay);

	gmw_set_rowcol(pCGI, row_num, col_num);
	/*������ʾ����*/
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
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
		{131072, COLOR_HPINK, COLOR_BLACK, NULL},  //������ģ������Լ�����
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};

	/*�����*/
	gmw_draw_frame(pCGI);
	/*��ʼ��:������һ��2*/
	array_generate(a, row_num, col_num);

	/* ��״̬����ʾ����,ͬʱ��ʼ��ʱ */
	time_t start_time = clock();
	sprintf(temp, "Ŀ��:%d ����:%d ���:%d ʱ��:%d (R:���� Q:�˳�)", target, score, max_score, int(clock() - start_time) / CLOCKS_PER_SEC);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
	/*������ʱ*/
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, delay);
	/*�������*/
	cct_disable_mouse();
	/*��ȡ����*/
	while (1) {
		draw_all_block(pCGI, row_num, col_num, a, bdi_normal);
		int MAction, MRow, MCol, keycode1, keycode2;
		bool generate = false;
		int event = gmw_read_keyboard_and_mouse(pCGI, MAction, MRow, MCol, keycode1, keycode2, false);
		if (event == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 0xe0) {//�����
				switch (keycode2) {
					case KB_ARROW_UP:
						/***��***/
						/*��λ*/
						position(a, row_num, col_num, TOUP, generate, pCGI, bdi_normal);
						/*�ϲ�*/
						for (int j = 0; j < col_num; j++) {//���μ��ÿһ��
							for (int i = 1; i < row_num; i++) //���ϵ��¼��ÿһ��
								if (a[i][j] == a[i - 1][j]) {
									a[i - 1][j] *= 2;
									score += a[i - 1][j];
									a[i][j] = 0;
									generate = true;
								}
						}
						/*�ٹ�λ*/
						position(a, row_num, col_num, TOUP, generate, pCGI, bdi_normal);
						break;

					case KB_ARROW_DOWN:
						/***��***/
						/*��λ*/
						position(a, row_num, col_num, TODOWN, generate, pCGI, bdi_normal);
						/*�ϲ�*/
						for (int j = 0; j < col_num; j++) {//���μ��ÿһ��
							for (int i = row_num - 1; i >= 1; i--)
								if (a[i][j] == a[i - 1][j]) {
									a[i][j] *= 2;
									score += a[i][j];
									a[i - 1][j] = 0;
									generate = true;
								}
						}
						/*�ٹ�λ*/
						position(a, row_num, col_num, TODOWN, generate, pCGI, bdi_normal);
						break;

					case KB_ARROW_LEFT://��
						/*��λ*/
						position(a, row_num, col_num, TOLEFT, generate, pCGI, bdi_normal);
						/*�ϲ�*/
						for (int i = 0; i < row_num; i++) {
							for (int j = 0; j < col_num - 1; j++)
								if (a[i][j] == a[i][j + 1]) {
									a[i][j] *= 2;
									score += a[i][j];
									a[i][j + 1] = 0;
									generate = true;
								}
						}
						/*��λ*/
						position(a, row_num, col_num, TOLEFT, generate, pCGI, bdi_normal);
						break;

					case KB_ARROW_RIGHT://��
						/*��λ*/
						position(a, row_num, col_num, TORIGHT, generate, pCGI, bdi_normal);
						/*�ϲ�*/
						for (int i = 0; i < row_num; i++) {
							for (int j = col_num - 1; j > 0; j--)
								if (a[i][j] == a[i][j - 1]) {
									a[i][j] *= 2;
									score += a[i][j];
									a[i][j - 1] = 0;
									generate = true;
								}
						}
						/*��λ*/
						position(a, row_num, col_num, TORIGHT, generate, pCGI, bdi_normal);
						break;
				}
				if (generate) {
					array_generate(a, row_num, col_num);
					if (score > max_score)
						max_score = score;
					sprintf(temp, "Ŀ��:%d ����:%d ���:%d ʱ��:%d (R:���� Q:�˳�)", target, score, max_score, int(clock() - start_time) / CLOCKS_PER_SEC);
					gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
				}
				if (game_over(a, row_num, col_num)) {
					sprintf(temp, "��Ϸʧ��!���շ���:%d ʱ��:%d(R:���� Q:�˳�)", score, int(clock() - start_time) / CLOCKS_PER_SEC);
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


	CONSOLE_GRAPHICS_INFO G2048_CGI; //����һ��CGI����

	/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
	gmw_init(&G2048_CGI);

	gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);			//����������ɫ
	gmw_set_font(&G2048_CGI, "������", 16);						//����
	gmw_set_frame_style(&G2048_CGI, 10, 5, true);					//��Ϸ��������ÿ��ɫ���10��5���зָ��ߡ�����ɫ����߿򣬿��Ϊ10(�����6λ����)���߶�Ϊ5(Ϊ�˱���ɫ��Ϊ����)��
	gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);	//��Ϸ��������ɫ
	gmw_set_block_border_switch(&G2048_CGI, true);				//Сɫ����߿�
	gmw_set_rowno_switch(&G2048_CGI, false);						//�ر��к�
	gmw_set_colno_switch(&G2048_CGI, false);						//�ر��б�
	gmw_set_status_line_switch(&G2048_CGI, TOP_STATUS_LINE, true);    //������״̬��
	gmw_set_status_line_color(&G2048_CGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(&G2048_CGI, LOWER_STATUS_LINE, false);	//�ر���״̬��

	int max_score = 0;
	while (1) {
		cct_setcolor();
		cct_cls();
		game_2048(&G2048_CGI, max_score);

	}
}