/*1852877 计科 赵昊堃*/
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
		const char* s[] = { "A.命令行找出可消除项并标识","B.命令行完成一次消除（分步骤显示）","C.命令行完成一关（分步骤显示）",
		"D.伪图形界面下用鼠标选择一个色块（无分隔线）","E.伪图形界面下用鼠标选择一个色块（有分隔线）",
		"F.伪图形界面完成一次消除（分步骤）","G.伪图形界面完整版","Q.退出" };
		char ch = display_menu(s, sizeof(s) / sizeof(char*));
		cout << ch << endl;
		if (ch == 'Q')
			return 0;
		else
			choose_menu(ch);
	}
}
