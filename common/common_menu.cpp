/*1852877 �ƿ� ��껈�*/
#include <iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
using namespace std;

//display_menu:����:����˵�ѡ������û�����ĺϷ��ַ�(�����ִ�Сд,��Ϊ��ĸͳһ���ش�д)
char display_menu(const char* items[], int itemsnum)
{
	int len_max = 0;
	for (int i = 0; i < itemsnum; i++)//��ȡ�ַ�����󳤶�
		if (strlen(items[i]) > (unsigned)len_max)
			len_max = strlen(items[i]);

	for (int i = 0; i < len_max + 2; i++)//��ӡ�˵�
		cout << '-';
	cout << endl;
	for (int i = 0; i < itemsnum; i++)
		cout << items[i] << endl;
	for (int i = 0; i < len_max + 2; i++)
		cout << '-';
	cout << endl;
	cout << "[��ѡ��] :";

	while (1)//ѡ��˵���
	{
		char ch = _getch();
		bool flag = false;
		for (int i = 0; i < itemsnum; i++)
			if (ch == items[i][0] ||
				items[i][0] >= 'a' && items[i][0] <= 'z' && (items[i][0] - ch == 'a' - 'A') ||
				items[i][0] >= 'A' && items[i][0] <= 'Z' && (items[i][0] - ch == 'A' - 'a'))//�˵�����ж�
				flag = true;
		if (flag == true) {
			if (ch >= 'a' && ch <= 'z')//����Сд,ͬһ���ش�д
				return ch + 'A' - 'a';
			else
				return ch;
		}
		else
			continue;
	}
}

//����:��ȡ���ֲ���,������ʾ��,���������½�,Ĭ��ֵ,�Լ����޸Ĳ���������,��ѡ������ʾ������Χ��:ʹ��Ĭ��ֵ/����ѭ��
void get_dig_para(const char* prompt, int Min, int Max, int& para, bool use_default, int Default)
{
	int start_x, start_y;
	int cols, lines, buffer_cols, buffer_lines;
	cct_getconsoleborder(cols, lines, buffer_cols, buffer_lines);
	cout << prompt;
	cct_getxy(start_x, start_y);
	while (1) {
		cct_gotoxy(start_x, start_y);
		cct_showch(start_x, start_y, ' ', 0, 7, cols - start_x);
		cct_gotoxy(start_x, start_y);
		cin >> para;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1024, '\n');
			continue;
		}
		if (Min <= para && para <= Max)
			break;
		if (use_default) {
			para = Default;
			break;
		}
	}
	cout << endl;
}

//����:���س�����
void wait_for_enter()
{
	cout << endl << "���س�������";
	while (_getch() != '\r')
		;
	cout << endl << endl;
}