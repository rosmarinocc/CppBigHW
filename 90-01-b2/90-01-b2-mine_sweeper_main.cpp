/*1852877 计科 赵昊堃*/
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
	const char* s[] = { "1.选择难度并显示内部数组" ,"2.输入初始位置并显示被打开的初始区域","3.内部数组基础版" ,
	"4.内部数组完整版（标记、运行时间）" ,"5.画出伪图形化框架并显示内部数据" , "6.检测鼠标位置和合法性（左键单击退出）",
	"7.鼠标选择初始位置并显示被打开的初始区域" ,"8.伪图形界面基础版", "9.伪图形界面完整版" ,"0.退出游戏" };
	while (1)
	{
		cct_cls();
		char ch = display_menu(s, sizeof(s)/sizeof(char*));
		if (ch == '0')
			return 0;
		else {
			choose_menu(ch);
			cout << endl;
			cout << "按回车继续..." << endl;
			while (_getch() != '\r')
				continue;
		}
	}
}
