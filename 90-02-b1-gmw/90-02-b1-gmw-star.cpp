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
#define NORMAL_STATE       1
#define RELATED_STATE      2
#define SELECTED_STATE     3
#define BONUS              10

struct elem {
	int key;
	int state;//1:ԭʼ״̬;2:����״̬��ѡ������;3:ѡ��״̬��1�λس���
};

/***************************����01:��ȡ������*********************************/
int input_rc(char ch)
{
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
/***************************����02:����һ��0~N-1�������*********************************/
int RAND(int N)//����:����һ��0~N-1�������
{
	return rand() % N;
}
/***************************����03:�ڲ������ʼ��*********************************/
void init_array(elem star[12][12], int row_num, int col_num)//ע���ڲ�������Ԫ��Ϊstar[1][1]
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
/***************************����04:��ͷ�ƶ�ѡ��*********************************/
void arrow_move(elem star[][12], int keycode2, int row_num, int col_num, int& row_now, int& col_now)
{
	//�Ѿ���֤rc1��ֵ��0,ע�����ﴫ���row��colΪ�ڲ�����rc,��MRow+1
	star[row_now + 1][col_now + 1].state = NORMAL_STATE;//ȡ����ǰ�߹�
	int i = 1;
	switch (keycode2) {
		case KB_ARROW_UP://����
			if (star[row_now][col_now + 1].key == 0) //��������Ԫ��Ϊ0,������ײ�
				row_now = row_num - 1;
			else
				row_now--;
			break;

		case KB_ARROW_DOWN://����
			if (row_now + 1 == row_num) {
				for (int i = 1; i <= row_num; i++)
					if (star[i][col_now + 1].key != 0) {
						row_now = i - 1;
						break;
					}
			}//�Ѿ�����ײ�,���ҵ���������ķ���Ԫ
			else
				row_now++;
			break;

		case KB_ARROW_LEFT://����
			while (star[row_now + 1][(col_now + col_num - i) % col_num + 1].key == 0)
				i++;
			col_now = (col_now + col_num - i) % col_num;
			break;

		case KB_ARROW_RIGHT://����
			while (star[row_now + 1][(col_now + col_num + i) % col_num + 1].key == 0)
				i++;
			col_now = (col_now + col_num + i) % col_num;
			break;
	}
}
/***************************����05:����ȫ�ֻ���*********************************/
void draw_all_block(const CONSOLE_GRAPHICS_INFO* const starCGI, const int row_num, const int col_num, elem star[12][12], const BLOCK_DISPLAY_INFO* const bdi_normal, const BLOCK_DISPLAY_INFO* const bdi_selected, const BLOCK_DISPLAY_INFO* const bdi_related)
{
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (star[i + 1][j + 1].state == NORMAL_STATE)//����
				gmw_draw_block(starCGI, i, j, star[i + 1][j + 1].key, bdi_normal);
			else if (star[i + 1][j + 1].state == SELECTED_STATE)//ѡ��
				gmw_draw_block(starCGI, i, j, star[i + 1][j + 1].key, bdi_selected);
			else if (star[i + 1][j + 1].state == RELATED_STATE)//����
				gmw_draw_block(starCGI, i, j, star[i + 1][j + 1].key, bdi_related);
		}
	}
}
/***************************����06:�����ܱ���ȵķ���,�ù���״̬*********************************/
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
/***************************����07:��������״̬,����ͨ״̬*********************************/
void reset_all(elem star[][12], int row_num, int col_num)
{
	for (int i = 1; i <= row_num; i++)
		for (int j = 1; j <= col_num; j++)
			star[i][j].state = NORMAL_STATE;
}
/***************************����08:����һ������*********************************/
bool all_zero(elem star[][12], int row_num, int j)
{
	/*��j���Ƿ�Ϊȫ0��*/
	for (int i = 1; i <= row_num; i++)
		if (star[i][j].key != 0)
			return false;
	return true;
}

int middle_zero_v(elem star[][12], int row_num, int j, int& cnt)
{
	/*��j���Ƿ�����м�0(����Ԫ��)?         ���ȱ�֤��ȫ����
	�����׸�����Ԫ�ص��к�,��������ڷ���-1                */
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
	/*�ײ��Ƿ�����м�0(����Ԫ��)? �����Ƿ�ƽ��
	�����׸�����Ԫ�ص��к�,��������ڷ���-1                */
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
	/*��������*/
	for (int i = 1; i <= row_num; i++)
		for (int j = 1; j <= col_num; j++)
			if (star[i][j].state == SELECTED_STATE || star[i][j].state == RELATED_STATE) {
				star[i][j].key = 0;
				gmw_draw_block(starCGI, i - 1, j - 1, 0, bdi_normal);
				cnt++;
			}

	reset_all(star, row_num, col_num);

	/*���乤��*/
	for (int j = 1; j <= col_num; j++) {
		if (!all_zero(star, row_num, j)) {//ȫ����������ı�Ҫ
			while (1) {
				int zero_num = 0;
				int res_row = middle_zero_v(star, row_num, j, zero_num);//�׸�����Ԫ�ص��к�(�ڲ�������к�)
				if (res_row != -1) {//��������Ԫ��
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

	/*ƽ�ƹ���*/
	for (int j = 1; j <= col_num; j++) {
		while (1) {
			int zero_num = 0;
			int res_col = middle_zero_h(star, row_num, col_num, zero_num);
			if (res_col != -1) {//��������Ԫ��
				for (int j = res_col; j <= col_num; j++) {
					for (int i = row_num; i >= 1; i--) {//�����ƶ�
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
/***************************����09:�����Ϸ�Ƿ����*********************************/
bool check_gameover(elem star[][12], int row_num, int col_num)//gameover����0����δ����,����1�������
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

/***************************��Ϸ����*********************************/
void game_star(CONSOLE_GRAPHICS_INFO* starCGI, elem star[][12], int row_num, int col_num, int& score)
{
	char temp[40] = { 0 };
	init_array(star, row_num, col_num);

	gmw_set_rowcol(starCGI, row_num, col_num);
	/*�����*/
	gmw_draw_frame(starCGI);
	/* ��״̬����ʾ���� */
	sprintf(temp, "��Ļ��ǰ����Ϊ:%d��%d��", starCGI->lines, starCGI->cols);
	gmw_status_line(starCGI, TOP_STATUS_LINE, temp);
	/* ��״̬����ʾ���� */
	gmw_status_line(starCGI, LOWER_STATUS_LINE, "��ͷ��/����ƶ�,�س���/�������ѡ��");

	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1,  COLOR_BLUE,   COLOR_BLACK,  NULL},	//����1�ġ�������״̬
		{2,  COLOR_GREEN,  COLOR_BLACK,  NULL},
		{3,  COLOR_CYAN,   COLOR_BLACK,  NULL},
		{4,  COLOR_RED,    COLOR_BLACK,  NULL},
		{5,  COLOR_PINK,   COLOR_BLACK,  NULL},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	const BLOCK_DISPLAY_INFO bdi_related[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1,  COLOR_BLUE,   COLOR_WHITE,  NULL},	//����1�ġ���ѡ��+������״̬
		{2,  COLOR_GREEN,  COLOR_WHITE,  NULL},
		{3,  COLOR_CYAN,   COLOR_WHITE,  NULL},
		{4,  COLOR_RED,    COLOR_WHITE,  NULL},
		{5,  COLOR_PINK,   COLOR_WHITE,  NULL},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
		{1, COLOR_BLUE,   COLOR_HWHITE, NULL},	//����1�ġ�ѡ�С�״̬
		{2, COLOR_GREEN,  COLOR_HWHITE, NULL},
		{3, COLOR_CYAN,   COLOR_HWHITE, NULL},
		{4, COLOR_RED,    COLOR_HWHITE, NULL},
		{5, COLOR_PINK,   COLOR_HWHITE, NULL},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};

	int row_now = row_num - 1, col_now = 0;//��¼��ǰѡ�п������ֵ,��ʼ����Ϊ���½�
	star[row_now + 1][col_now + 1].state = RELATED_STATE;


	int MRow, MCol, MAction, keycode1, keycode2;

	cct_enable_mouse();

	while (1) {
		draw_all_block(starCGI, row_num, col_num, star, bdi_normal, bdi_selected, bdi_related);
		int event = gmw_read_keyboard_and_mouse(starCGI, MAction, MRow, MCol, keycode1, keycode2, true);
		/*�����¼�*/
		if (event == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 0x0D) {//�س���
				if (star[row_now + 1][col_now + 1].state == RELATED_STATE && star[row_now + 1][col_now + 1].key != 0) {
					int cnt = 0;
					star[row_now + 1][col_now + 1].state = NORMAL_STATE;//search֮ǰ��Ҫ��ʱ��Ϊ��ͨ
					search_sur(star, row_now + 1, col_now + 1, cnt);
					if (cnt == 1) {//����������
						reset_all(star, row_num, col_num);//����״̬
						star[row_now + 1][col_now + 1].state = RELATED_STATE;
						gmw_status_line(starCGI, LOWER_STATUS_LINE, "��ͷ/����ƶ�,�س�/�������ѡ��/,Q/�����Ҽ�����", "��Χ����ֵͬ");
					}
					else
						star[row_now + 1][col_now + 1].state = SELECTED_STATE;
				}
				else if (star[row_now + 1][col_now + 1].state == SELECTED_STATE) {
					int cnt = 0;
					clean_sur(starCGI, star, row_num, col_num, cnt, bdi_normal);
					score += cnt * BONUS;
					sprintf(temp, "���ε÷�: %d , �ܵ÷�: %d", cnt * BONUS, score);
					gmw_status_line(starCGI, TOP_STATUS_LINE, temp);
					row_now = row_num - 1, col_now = 0;
					//�Ƿ����
					if (check_gameover(star, row_num, col_num)) {//��Ϸ����
						int rest = 0;
						for (int i = 1; i <= row_num; i++)
							for (int j = 1; j <= col_num; j++)
								if (star[i][j].key != 0)
									rest++;
						/*��״̬��*/
						score += BONUS * (row_num * col_num - rest);
						sprintf(temp, "ʣ��%d������������,���ؽ���,�س���һ��", rest);
						gmw_status_line(starCGI, TOP_STATUS_LINE, temp);
						/*��״̬��*/
						sprintf(temp, "�����÷�: %d , �ܵ÷�: %d", BONUS * (row_num * col_num - rest), score);
						gmw_status_line(starCGI, LOWER_STATUS_LINE, temp);
						return;
					}
				}
			}
			else if (keycode1 == 0xe0) {//�����
				reset_all(star, row_num, col_num);
				arrow_move(star, keycode2, row_num, col_num, row_now, col_now);//�޸�ѡ��ɫ���RCֵ
				star[row_now + 1][col_now + 1].state = RELATED_STATE;//��������
			}
			else if (keycode1 == 'Q' || keycode1 == 'q') {//Q
				exit(0);//��Ϸ����
			}
		}
		/*����¼�*/
		else if (event == CCT_MOUSE_EVENT) {
			row_now = MRow;
			col_now = MCol;
			if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
				if (star[row_now + 1][col_now + 1].state == RELATED_STATE && star[row_now + 1][col_now + 1].key != 0) {
					int cnt = 0;
					star[row_now + 1][col_now + 1].state = NORMAL_STATE;//search֮ǰ��Ҫ��ʱ��Ϊ��ͨ
					search_sur(star, row_now + 1, col_now + 1, cnt);
					if (cnt == 1) {//����������
						reset_all(star, row_num, col_num);//����״̬
						star[row_now + 1][col_now + 1].state = RELATED_STATE;
						gmw_status_line(starCGI, LOWER_STATUS_LINE, "��ͷ/����ƶ�,�س�/�������ѡ��/,Q/�����Ҽ�����", "��Χ����ֵͬ");
					}
					else
						star[row_now + 1][col_now + 1].state = SELECTED_STATE;
				}
				else if (star[row_now + 1][col_now + 1].state == SELECTED_STATE) {
					int cnt = 0;
					clean_sur(starCGI, star, row_num, col_num, cnt, bdi_normal);
					score += cnt * BONUS;
					sprintf(temp, "���ε÷�: %d , �ܵ÷�: %d", cnt * BONUS, score);
					gmw_status_line(starCGI, TOP_STATUS_LINE, temp);
					row_now = row_num - 1, col_now = 0;
					//�Ƿ����
					if (check_gameover(star, row_num, col_num)) {//��Ϸ����
						int rest = 0;
						for (int i = 1; i <= row_num; i++)
							for (int j = 1; j <= col_num; j++)
								if (star[i][j].key != 0)
									rest++;
						/*��״̬��*/
						score += BONUS * (row_num * col_num - rest);
						sprintf(temp, "ʣ��%d������������,���ؽ���,�س���һ��", rest);
						gmw_status_line(starCGI, TOP_STATUS_LINE, temp);
						/*��״̬��*/
						sprintf(temp, "�����÷�: %d , �ܵ÷�: %d", BONUS * (row_num * col_num - rest), score);
						gmw_status_line(starCGI, LOWER_STATUS_LINE, temp);
						return;
					}
				}
			}
			else if (MAction == MOUSE_ONLY_MOVED) {
				if (star[row_now + 1][col_now + 1].key == 0)
					gmw_status_line(starCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");
				else
				{
					reset_all(star, row_num, col_num);
					star[row_now + 1][col_now + 1].state = RELATED_STATE;//��������
				}
			}
			else if (MAction == MOUSE_RIGHT_BUTTON_CLICK) {
				exit(0);//��Ϸ�˳�
			}
		}



	}
}
int main()
{
	srand(unsigned int(time(0)));

	CONSOLE_GRAPHICS_INFO starCGI; //����һ��CGI����

	/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
	gmw_init(&starCGI);
	gmw_set_color(&starCGI, COLOR_BLACK, COLOR_WHITE);			//����������ɫ
	gmw_set_font(&starCGI, "������", 24);						//����
	gmw_set_frame_style(&starCGI, 6, 3, true);					//��Ϸ��������ÿ��ɫ���6��3���зָ���
	gmw_set_frame_color(&starCGI, COLOR_WHITE, COLOR_BLACK);	//��Ϸ��������ɫ
	gmw_set_block_border_switch(&starCGI, true);						//Сɫ����߿�
	gmw_set_rowno_switch(&starCGI, true);						//��ʾ�к�
	gmw_set_colno_switch(&starCGI, true);						//��ʾ�б�
	gmw_set_status_line_switch(&starCGI, TOP_STATUS_LINE, true);    //������״̬��
	gmw_set_status_line_color(&starCGI, TOP_STATUS_LINE);
	gmw_set_status_line_color(&starCGI, LOWER_STATUS_LINE);
	gmw_set_status_line_switch(&starCGI, LOWER_STATUS_LINE, true);	//������״̬��
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