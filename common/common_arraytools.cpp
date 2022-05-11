/*1852877 �ƿ� ��껈�*/
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <windows.h>
#include "../include/cmd_console_tools.h"
using namespace std;

/**********����01:�����****/
int RAND(int N)//����:����һ��0~N-1�������
{
	return rand() % N;
}

/***********����02:�ж��ַ�����**************/
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

/************����03:���Ʋ�����ɫ�麬�зֽ��ߵĿ���********************************/
void draw_borderlines(int row, int col, int block_w, int block_h, int start_x, int start_y)//����:������,ɫ�鳤��,������ʼ����
{
	cct_gotoxy(start_x, start_y);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	int rownum = 1 + (1 + block_h) * row;
	int colnum = 1 + (1 + block_w / 2) * col;//��һ���ַ����(2)��Ϊһ���л�����λ
	for (int line = 0; line < rownum; line++) {
		cct_gotoxy(start_x, start_y + line);
		//����
		if (line == 0) {
			for (int i = 0; i < colnum; i++) {
				if (i == 0)
					cout << "�X";
				else if (i == colnum - 1)
					cout << "�[";
				else {
					if (i % (block_w / 2 + 1) == 0) {
						cout << "�j";
						Sleep(1);
					}
					else
						cout << "�T";
				}
			}
		}
		//ĩ��
		else if (line == rownum - 1) {
			for (int i = 0; i < colnum; i++) {
				if (i == 0)
					cout << "�^";
				else if (i == colnum - 1)
					cout << "�a";
				else {
					if (i % (block_w / 2 + 1) == 0) {
						cout << "�m";
						Sleep(1);
					}
					else
						cout << "�T";
				}
			}
		}
		//�м���
		else {
			if (line % (1 + block_h) == 0) {
				for (int i = 0; i < colnum; i++) {
					if (i == 0)
						cout << "�d";
					else if (i == colnum - 1)
						cout << "�g";
					else {
						if (i % (block_w / 2 + 1) == 0) {
							cout << "�p";
							Sleep(1);
						}
						else
							cout << "�T";
					}
				}
			}
			else {
				for (int i = 0; i < colnum; i++) {
					if (i % (block_w / 2 + 1) == 0) {
						cout << "�U";
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