/*1852877 �ƿ� ��껈�*/
#include <iostream>
#include "90-02-b4.h"

using namespace std;

const char* items[3] = { "1.�ַ�ģʽ��ʾ", "2.LEDģʽ��ʾ(������Ч��ʾ)", "0.�˳�" };



int main()
{
	while (1) {
		init_para();
		cct_cls();
		cout << "��ѡ��:" << endl;
		char mode = display_menu(items, 3);
		if (mode == '0')
			break;
		else if (mode == '1') {
			cout << mode << endl;
			mode_char();
		}

		else if (mode == '2') {
			cout << mode << endl;

			mode_led();
		}
	}
}