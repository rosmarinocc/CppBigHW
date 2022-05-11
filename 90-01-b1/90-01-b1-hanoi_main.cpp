/*1852877 �ƿ� ��껈�*/
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "90-01-b1-hanoi.h"
#include "../include/common_menu.h"
using namespace std;

/* -----------------------------------------

	 ���ļ����ܣ�
	1����main����
	2����ʼ����Ļ
	3�����ò˵�������hanoi_menu.cpp�У�������ѡ��
	4������ѡ����ò˵������Ӧ��ִ�к�����hanoi_multiple_solutions.cpp�У�

	 ���ļ�Ҫ��
	1����������ȫ�ֱ��������ⲿȫ�ֺ;�̬ȫ����const��#define�������Ʒ�Χ�ڣ�
	2����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	3���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------- */

   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ�
	 ���������
	 �� �� ֵ��
	 ˵    ����
   ***************************************************************************/
int main()
{
	cct_setconsoleborder(120, 40, 120, 9000);
	/* demo������ִ�д˾䣬��cmd��������Ϊ40��x120�У����������120�У�����9000�У���cmd�����Ҳ���д�ֱ�����ˣ�*/
	const char* s[] = { "1.������" ,"2.������(������¼)" ,"3.�ڲ�������ʾ(����)" ,"4.�ڲ�������ʾ(���� + ����)" ,
	"5.ͼ�ν� - Ԥ�� - ������Բ��" ,"6.ͼ�ν� - Ԥ�� - ����ʼ���ϻ�n������" , "7.ͼ�ν� - Ԥ�� - ��һ���ƶ�" ,
	"8.ͼ�ν� - �Զ��ƶ��汾" , "9.ͼ�ν� - ��Ϸ��" ,"0.�˳�" };

	while (1)
	{
		cct_cls();
		while (1) {
			char ch = display_menu(s, sizeof(s)/sizeof(char*));
			cout << ch << endl;
			cout << endl;
			if (ch == '0')
				return 0;
			else
			{
				switch (ch)
				{
					case '1':
						method1();
						break;
					case '2':
						method2();
						break;
					case '3':
						method3();
						break;
					case '4':
						method4();
						break;
					case '5':
						method5();
						break;
					case '6':
						method6();
						break;
					case '7':
						method7();
						break;
					case '8':
						method8();
						break;
					case '9':
						method9();
						break;
				}
				cout << endl;
				cout << endl;
				cout << "���س�������";
			}
			while (_getch() != '\r')
				continue;
		}
	}
}