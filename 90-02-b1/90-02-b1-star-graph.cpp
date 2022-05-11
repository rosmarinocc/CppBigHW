/*1852877 �ƿ� ��껈�*/
#include <iostream>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include "../include/common_arraytools.h"
#include "90-02-b1-star.h"
using namespace std;

/********************************************����01:����:���Ʊ߿�,plus�汾�����зֽ���************************************************/
void draw_border(RC rc)
{
	cct_cls();
	cct_setconsoleborder(7 + BLOCK_W * rc.column, 12 + BLOCK_H * rc.row, 7 + BLOCK_W * rc.column, 9000);
	cout << "��Ļ��ǰ����Ϊ:" << 12 + BLOCK_H * rc.row << "��" << 7 + BLOCK_W * rc.column << "��" << endl;
	cout << ' ';
	for (int i = 0; i < rc.column; i++)
		cout << "     " << i;
	cout << endl;
	cout << "  ";
	int start_x, start_y;//��¼�߿����ʼλ��
	cct_getxy(start_x, start_y);
	cct_gotoxy(0, start_y - 1);
	for (int i = 0; i < rc.row; i++) {
		cout << '\n' << endl;
		cout << '\n' << char('A' + i);
	}
	cct_gotoxy(start_x, start_y);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	for (int line = 0; line < 2 + BLOCK_H * rc.row; line++) {
		cct_gotoxy(start_x, start_y + line);
		if (line == 0) {
			cout << "�X";
			for (int i = 1; i <= BLOCK_W / 2 * rc.column; i++) {
				if (i % 3 == 0)
					Sleep(1);//��ͣ����
				cout << "�T";
			}
			cout << "�[";
		}
		else if (line == 1 + BLOCK_H * rc.row) {
			cout << "�^";
			for (int i = 1; i <= BLOCK_W / 2 * rc.column; i++) {
				if (i % 3 == 0)
					Sleep(1);//��ͣ����
				cout << "�T";
			}
			cout << "�a";
		}
		else {
			cout << "�U";
			for (int i = 1; i <= BLOCK_W / 2 * rc.column; i++) {
				if (i % 3 == 0)
					Sleep(1);//��ͣ����
				cout << "  ";
			}
			cout << "�U";
		}

	}
	cct_setcolor();
}

void draw_borderplus(RC rc)//��дΪ��������?��������,block�ĳ���,��ʼ����
{
	cct_cls();
	cct_setconsoleborder(7 + (2 + BLOCK_W) * rc.column, 12 + (1 + BLOCK_H) * rc.row, 7 + (2 + BLOCK_W) * rc.column, 9000);
	cout << "��Ļ��ǰ����Ϊ:" << 12 + (1 + BLOCK_H) * rc.row << "��" << 7 + (2 + BLOCK_W) * rc.column << "��" << endl;
	for (int i = 0; i < rc.column; i++)
		cout << "       " << i;
	cout << endl;
	cout << "   ";
	int start_x, start_y;//��¼�߿����ʼλ��
	cct_getxy(start_x, start_y);
	cct_gotoxy(0, start_y - 2);
	for (int i = 0; i < rc.row; i++) {
		cout << '\n' << endl;
		cout << '\n' << endl;
		cout << char('A' + i);
	}
	draw_borderlines(rc.row, rc.column, BLOCK_W, BLOCK_H, start_x, start_y);
}

/********************************************����02:����:����ɫ��,plus�汾�����зֽ���************************************************/
void draw_block(RC rc, elem star[][12], RC rc1, int highlight)
{
	//highlight=0,�޸���;=1,����rc1��Ӧ����;=2����rc1��Χ��������
	for (int i = 1; i <= rc.row; i++)
		for (int j = 1; j <= rc.column; j++) {
			cct_gotoxy(4 + BLOCK_W * (j - 1), 3 + BLOCK_H * (i - 1));
			if (star[i][j].key == 0) {
				cct_setcolor(COLOR_HWHITE, COLOR_HWHITE);
				cout << "      ";
				cct_gotoxy(4 + BLOCK_W * (j - 1), 4 + BLOCK_H * (i - 1));
				cout << "      ";
				cct_gotoxy(4 + BLOCK_W * (j - 1), 5 + BLOCK_H * (i - 1));
				cout << "      ";
			}
			else {
				if (highlight == 0) {
					cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "�X�T�[";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 4 + BLOCK_H * (i - 1));
					cout << "�U��U";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 5 + BLOCK_H * (i - 1));
					cout << "�^�T�a";
				}
				else if (highlight == 1) {
					if (i == rc1.row && j == rc1.column)
						cct_setcolor(star[i][j].key, COLOR_HWHITE);
					else
						cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "�X�T�[";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 4 + BLOCK_H * (i - 1));
					cout << "�U��U";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 5 + BLOCK_H * (i - 1));
					cout << "�^�T�a";
				}
				else if (highlight == 2) {//��Ҫ����ǰ��֤������������ַ,�����������κμ��
					if (star[i][j].state == 2 || star[i][j].state == 3)
						cct_setcolor(star[i][j].key, COLOR_HWHITE);
					else
						cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "�X�T�[";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 4 + BLOCK_H * (i - 1));
					cout << "�U��U";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 5 + BLOCK_H * (i - 1));
					cout << "�^�T�a";
				}
			}
		}
	cct_setcolor();
}

void draw_blockplus(RC rc, elem star[][12], RC rc1, int highlight)
{
	//highlight=0,�޸���;=1,����rc1��Ӧ����;=2����rc1��Χ��������
	for (int i = 1; i <= rc.row; i++)
		for (int j = 1; j <= rc.column; j++) {
			cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 3 + (1 + BLOCK_H) * (i - 1));
			if (star[i][j].key == 0) {
				cct_setcolor(COLOR_HWHITE, COLOR_HWHITE);
				cout << "      ";
				cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 4 + (1 + BLOCK_H) * (i - 1));
				cout << "      ";
				cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 5 + (1 + BLOCK_H) * (i - 1));
				cout << "      ";
			}
			else {
				if (highlight == 0) {
					cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "�X�T�[";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 4 + (1 + BLOCK_H) * (i - 1));
					cout << "�U��U";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 5 + (1 + BLOCK_H) * (i - 1));
					cout << "�^�T�a";
				}
				else if (highlight == 1) {
					if (i == rc1.row && j == rc1.column)
						cct_setcolor(star[i][j].key, COLOR_HWHITE);
					else
						cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "�X�T�[";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 4 + (1 + BLOCK_H) * (i - 1));
					cout << "�U��U";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 5 + (1 + BLOCK_H) * (i - 1));
					cout << "�^�T�a";
				}
				else if (highlight == 2) {//��Ҫ����ǰ��֤������������ַ,�����������κμ��
					if (star[i][j].state == 2 || star[i][j].state == 3)
						cct_setcolor(star[i][j].key, COLOR_HWHITE);
					else
						cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "�X�T�[";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 4 + (1 + BLOCK_H) * (i - 1));
					cout << "�U��U";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 5 + (1 + BLOCK_H) * (i - 1));
					cout << "�^�T�a";
				}
			}
		}
	cct_setcolor();
}

/********************************************����03:����:������λ�ò�ת��Ϊ���������*************************************************/
bool convert_to_rc(elem star[][12], RC rc, RC& rc1, int MX, int MY, bool boundary = 0)//��������ڵ�����ת��Ϊ������ʽ,���ж��Ƿ�Ϊ�Ϸ�����
{
	bool legal = false;
	if (!boundary) {
		for (int i = 1; i <= rc.row; i++)
			for (int j = 1; j <= rc.column; j++)
				if (4 + BLOCK_W * (j - 1) <= MX && MX <= 4 + BLOCK_W * j
					&& 3 + BLOCK_H * (i - 1) <= MY && MY <= 3 + BLOCK_H * i) {
					legal = true;
					rc1.row = i;
					rc1.column = j;
				}
	}
	else {
		for (int i = 1; i <= rc.row; i++)
			for (int j = 1; j <= rc.column; j++)
				if (5 + (2 + BLOCK_W) * (j - 1) <= MX && MX <= 5 + (2 + BLOCK_W) * (j - 1) + BLOCK_W
					&& 3 + (1 + BLOCK_H) * (i - 1) <= MY && MY <= 3 + (1 + BLOCK_H) * (i - 1) + BLOCK_H) {
					legal = true;
					rc1.row = i;
					rc1.column = j;
				}
	}

	if (star[rc1.row][rc1.column].key == 0)
		legal = false;
	return legal;
}
/********************************************����04:����:��������ƶ������뷽���,ʵ��ѡ��ɫ�����������ɫ������ֵ*************************************************************/
void check_arrow(elem star[][12], int keycode2, RC rc, RC& rc1) //���ݼ�ֵ2�ж����������ƶ�,�߽緵��
{
	//�Ѿ���֤rc1��ֵ��0
	int i = 1;
	switch (keycode2) {
		case KB_ARROW_UP://����
			if (star[rc1.row - 1][rc1.column].key == 0) //��������Ԫ��Ϊ0,������ײ�
				rc1.row = rc.row;
			else
				rc1.row--;
			break;

		case KB_ARROW_DOWN://����
			if (rc1.row == rc.row) {
				for (int i = 1; i <= rc.row; i++)
					if (star[i][rc1.column].key != 0) {
						rc1.row = i;
						break;
					}
			}//�Ѿ�����ײ�,���ҵ���������ķ���Ԫ
			else
				rc1.row++;
			break;

		case KB_ARROW_LEFT://����
			while (star[rc1.row][(rc1.column - 1 + rc.column - i) % rc.column + 1].key == 0)
				i++;
			rc1.column = (rc1.column - 1 + rc.column - i) % rc.column + 1;
			break;

		case KB_ARROW_RIGHT://����
			while (star[rc1.row][(rc1.column - 1 + rc.column + i) % rc.column + 1].key == 0)
				i++;
			rc1.column = (rc1.column - 1 + rc.column + i) % rc.column + 1;
			break;

	}
}
void Mouse_and_Key(RC rc, elem star[][12], RC& rc1, bool boundary)//rc1��¼����Ŀ���
{
	//boundary=0�޷ֽ������,=1�зֽ������
	int pos_y;
	void (*drawblk)(RC, elem[][12], RC, int);//ָ������ָ��
	if (!boundary) {
		pos_y = 4 + BLOCK_H * rc.row;
		drawblk = draw_block;
	}
	else {
		pos_y = 4 + (1 + BLOCK_H) * rc.row;
		drawblk = draw_blockplus;
	}
	drawblk(rc, star, { rc1.row,rc1.column }, 1);//���޸�,��Ϊ���ҳ���󲻻��˻�1,1
	int MX, MY, MAction, keycode1, keycode2;
	RC odd_rc1 = rc1;
	cct_enable_mouse();
	while (1) {
		//�����¼�
		if (cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2) == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 0xe0) {//�����
				check_arrow(star, keycode2, rc, rc1);
				drawblk(rc, star, { rc1.row,rc1.column }, 1);
				cct_gotoxy(0, pos_y);
				cout << "[��ǰ����]" << (char)(rc1.row - 1 + 'A') << "��" << rc1.column - 1 << "��" << "                              ";
			}
			if (keycode1 == 0x0D) //�س�
				return;

		}

		//����¼�
		else {
			if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
				if (convert_to_rc(star, rc, rc1, MX, MY, boundary) == true)
					return;

			}
			if (MAction == MOUSE_ONLY_MOVED) {
				bool legal = convert_to_rc(star, rc, rc1, MX, MY, boundary);
				if (odd_rc1.row != rc1.row || odd_rc1.column != rc1.column) {//��ֹ΢Сλ����ɵĶ���
					if (!legal) {
						cct_gotoxy(0, pos_y);
						cout << "[��ǰ���]λ�÷Ƿ�                               ";
					}
					else {
						drawblk(rc, star, { rc1.row,rc1.column }, 1);
						cct_gotoxy(0, pos_y);
						cout << "[��ǰ���]" << (char)(rc1.row - 1 + 'A') << "��" << rc1.column - 1 << "��" << "                              ";
					}
				}
			}


		}
		odd_rc1 = rc1;
	}
}

/******************************����:����:���������or�س�������,�ƿ������ȡ������***********************************************/
bool check_to_clear(RC rc, elem star[][12], RC& rc1, int& score)//��ʱ�����Ѿ�ȷ���Ϸ�
{//����һ��boolֵ,�����Ƿ�����,�Ӷ������Ƿ��������
	RC odd_rc1 = rc1;
	while (1) {
		int MX, MY, MAction, keycode1, keycode2;
		//�����¼�
		if (cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2) == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 13) {//���̰��»س�
				int cnt = 0, * pcnt = &cnt;
				search_sur(star, rc1.row, rc1.column, pcnt, 2);
				score += BONUS * cnt;
				traverse_array(rc, star, "clear");
				draw_block(rc, star);
				cct_gotoxy(0, 0);
				cout << "���ε÷�:" << BONUS * cnt << "      �ܵ÷�:" << score << "      ";
				traverse_array(rc, star, "resetstate");
				return true;
			}
			else if (keycode1 == 0xe0) {//���̰��·����
				traverse_array(rc, star, "resetstate");
				check_arrow(star, keycode2, rc, rc1);//�����·�����������ƶ�,ֻ��ȡ��ѡ��,�ڶ��β��ƶ�
				return false;
			}
		}
		//����¼�
		else {//ֻ�зǶ����ƶ��͵��������˳�ѭ��
			if (MAction == MOUSE_LEFT_BUTTON_CLICK) {//��갴�����
				int cnt = 0, * pcnt = &cnt;
				search_sur(star, rc1.row, rc1.column, pcnt, 2);
				score += BONUS * cnt;
				traverse_array(rc, star, "clear");
				draw_block(rc, star);
				cct_gotoxy(0, 0);
				cout << "���ε÷�:" << BONUS * cnt << "      �ܵ÷�:" << score << "      ";
				traverse_array(rc, star, "resetstate");
				return true;
			}
			if (MAction == MOUSE_ONLY_MOVED) {
				bool legal = convert_to_rc(star, rc, rc1, MX, MY);
				if (odd_rc1.row != rc1.row || odd_rc1.column != rc1.column) {//����ƶ��Ƕ���
					traverse_array(rc, star, "resetstate");
					return false;
				}

			}
		}
	}
}

/*******************************����:����:ʵ��ȷ����������������������Ƶȹ���************************************************************/
void delete_sur(elem star[][12], RC rc1, int key)//��ͼ�ν���ǿ����ֲ��ʹ������Щ��ª,������дһ��,��ʾ���Ѿ�ȷ��Ҫ������,��rc1��ΧԪ��ָ���ǵȴ�����
{
	if (star[rc1.row][rc1.column].key != key || star[rc1.row][rc1.column].state == -1)
		return;
	else {
		star[rc1.row][rc1.column].key = 0;
		star[rc1.row][rc1.column].state = -1;//��ʾ�Ѿ�������
		delete_sur(star, { rc1.row - 1,rc1.column }, key);
		delete_sur(star, { rc1.row + 1,rc1.column }, key);
		delete_sur(star, { rc1.row ,rc1.column - 1 }, key);
		delete_sur(star, { rc1.row ,rc1.column + 1 }, key);
	}
}
void find_pos(elem star[][12], RC rc, int col, int oddrow, int& dstrow) {
	for (int i = rc.row; i > oddrow; i--)
		if (star[i][col].key == 0)
		{
			dstrow = i;//�ڱ��е�i���ҵ��˵�һ����λ
			return;
		}
}
void dynamic_exchange(elem star[][12], RC rc, int col, int oddrow, int dstrow)//ʵ��star[oddrow][col]��star[dstrow][col]�Ķ�̬����
{
	for (int i = oddrow; i < dstrow; i++) {
		int temp = star[i][col].key;
		star[i][col].key = star[i + 1][col].key;
		star[i + 1][col].key = temp;
		draw_block(rc, star);
		Sleep(30);
	}
}
void fall_graph(elem star[][12], RC rc)//ͼ�ν�������,���ʵ�ֶ���Ч��?��Ҫ����state=-1��key=0��������?��״̬�޹�
{
	//���²���
	for (int j = 1; j <= rc.column; j++)
		for (int i = rc.row; i >= 1; i--) {
			if (star[i][j].key != 0) {
				int dstrow = i;
				find_pos(star, rc, j, i, dstrow);//��һ���ҵ��·���һ����λ��/û�ҵ�dstrow������Ȼͨ��
				if (i != dstrow)
					dynamic_exchange(star, rc, j, i, dstrow);
			}
		}
	//���Ҳ���
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
						draw_block(rc, star);
						Sleep(10);
					}
				}
		}
	}
}

/*******************************����:����:���or�س�������**************************************************************************************/
void check_to_fall(RC rc, elem star[][12], RC& rc1) //�������Ҫ���Ƕ���
{
	int MX, MY, MAction, keycode1, keycode2;
	int cnt = 0, * pcnt = &cnt;
	while (1) {
		cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);
		//�����¼�
		if (keycode1 == 13) {
			int key = star[rc1.row][rc1.column].key;
			delete_sur(star, rc1, key);
			traverse_array(rc, star, "resetstate");
			fall_graph(star, rc);
			return;
		}

		//����¼�
		if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
			int key = star[rc1.row][rc1.column].key;
			delete_sur(star, rc1, key);
			traverse_array(rc, star, "resetstate");
			fall_graph(star, rc);
			return;
		}
	}
}
/*********************************����:����:����Ӧ��һ��F�Ĵ�������*****************************************************/
void game_once(RC rc, elem star[][12])
{

	RC rc1 = { rc.row,1 };
	int pos_y = 4 + BLOCK_H * rc.row;
	int score = 0;
	int cnt = 0, * pcnt = &cnt;
	cct_gotoxy(0, pos_y);
	cout << "��ͷ��/����ƶ�,�س���/�������ѡ��                ";
	while (1) {

		Mouse_and_Key(rc, star, rc1);//������/�س���rc1
		search_sur(star, rc1.row, rc1.column, pcnt);//����rc1,�ж��Ƿ�����
		if (*pcnt == 1) {
			star[rc1.row][rc1.column].state = 1;
			cct_gotoxy(0, pos_y);
			cout << "��Χ����ֵͬ,��ͷ��/����ƶ�,�س���/��������ϳ�                         ";
			continue;
		}
		else {//ѡ���������
			cct_gotoxy(0, pos_y);
			cout << "��ͷ��/����ƶ�ȡ����ǰѡ��,�س���/��������ϳ�                          ";
			draw_block(rc, star, rc1, 2);
			if (check_to_clear(rc, star, rc1, score) == true)//���س��͵�������ȥ,�Լ��ƿ������ȡ������
			{
				cct_gotoxy(0, pos_y);
				cout << "�ϳ����,�س���/�����������                             ";
				check_to_fall(rc, star, rc1);
				return;
			}
			else
				continue;
		}
	}
}

void GAME(RC rc, elem star[][12], int& score)
{

	RC rc1 = { rc.row,1 };
	int pos_y = 4 + BLOCK_H * rc.row;
	int cnt = 0, * pcnt = &cnt;
	cct_gotoxy(0, pos_y);
	cout << "��ͷ��/����ƶ�,�س���/�������ѡ��                ";
	while (1) {

		Mouse_and_Key(rc, star, rc1);//������/�س���rc1
		search_sur(star, rc1.row, rc1.column, pcnt);//����rc1,�ж��Ƿ�����
		if (*pcnt == 1) {
			star[rc1.row][rc1.column].state = 1;
			cct_gotoxy(0, pos_y);
			cout << "��Χ����ֵͬ,��ͷ��/����ƶ�,�س���/��������ϳ�                         ";
			continue;
		}
		else {//ѡ���������
			cct_gotoxy(0, pos_y);
			cout << "��ͷ��/����ƶ�ȡ����ǰѡ��,�س���/��������ϳ�                          ";
			draw_block(rc, star, rc1, 2);
			if (check_to_clear(rc, star, rc1, score) == true)//���س��͵�������ȥ,�Լ��ƿ������ȡ������
			{
				int key = star[rc1.row][rc1.column].key;
				delete_sur(star, rc1, key);
				traverse_array(rc, star, "resetstate");
				fall_graph(star, rc);
				return;
			}
			else
				continue;
		}
	}
}
