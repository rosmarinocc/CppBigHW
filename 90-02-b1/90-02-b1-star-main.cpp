/*1852877 �ƿ� ��껈�*/
#include <iostream>
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include "90-02-b1-star.h"
using namespace std;



int main()
{
	srand((unsigned int)(time(0)));
	while (1) {
		cct_cls();
		const char* s[] = { "A.�������ҳ����������ʶ","B.���������һ���������ֲ�����ʾ��","C.���������һ�أ��ֲ�����ʾ��",
		"D.αͼ�ν����������ѡ��һ��ɫ�飨�޷ָ��ߣ�","E.αͼ�ν����������ѡ��һ��ɫ�飨�зָ��ߣ�",
		"F.αͼ�ν������һ���������ֲ��裩","G.αͼ�ν���������","Q.�˳�" };
		char ch = display_menu(s, sizeof(s) / sizeof(char*));
		cout << ch << endl;
		if (ch == 'Q')
			return 0;
		else
			choose_menu(ch);
	}
}
