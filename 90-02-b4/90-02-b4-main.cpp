/*1852877 计科 赵昊堃*/
#include <iostream>
#include "90-02-b4.h"

using namespace std;

const char* items[3] = { "1.字符模式显示", "2.LED模式显示(包含特效演示)", "0.退出" };



int main()
{
	while (1) {
		init_para();
		cct_cls();
		cout << "请选择:" << endl;
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