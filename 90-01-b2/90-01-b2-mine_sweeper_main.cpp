/*1852877 �ƿ� ��껈�*/
#include <iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "90-01-b2-mine_sweeper.h"
#include "../include/common_menu.h"
using namespace std;


int main()
{
	srand((unsigned int)(time(0)));
	cct_setconsoleborder(200, 80, 120, 9000);
	const char* s[] = { "1.ѡ���ѶȲ���ʾ�ڲ�����" ,"2.�����ʼλ�ò���ʾ���򿪵ĳ�ʼ����","3.�ڲ����������" ,
	"4.�ڲ����������棨��ǡ�����ʱ�䣩" ,"5.����αͼ�λ���ܲ���ʾ�ڲ�����" , "6.������λ�úͺϷ��ԣ���������˳���",
	"7.���ѡ���ʼλ�ò���ʾ���򿪵ĳ�ʼ����" ,"8.αͼ�ν��������", "9.αͼ�ν���������" ,"0.�˳���Ϸ" };
	while (1)
	{
		cct_cls();
		char ch = display_menu(s, sizeof(s)/sizeof(char*));
		if (ch == '0')
			return 0;
		else {
			choose_menu(ch);
			cout << endl;
			cout << "���س�����..." << endl;
			while (_getch() != '\r')
				continue;
		}
	}
}
