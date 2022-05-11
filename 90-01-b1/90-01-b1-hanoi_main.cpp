/*1852877 计科 赵昊堃*/
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "90-01-b1-hanoi.h"
#include "../include/common_menu.h"
using namespace std;

/* -----------------------------------------

	 本文件功能：
	1、放main函数
	2、初始化屏幕
	3、调用菜单函数（hanoi_menu.cpp中）并返回选项
	4、根据选项调用菜单各项对应的执行函数（hanoi_multiple_solutions.cpp中）

	 本文件要求：
	1、不允许定义全局变量（含外部全局和静态全集，const及#define不在限制范围内）
	2、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	3、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------- */

   /***************************************************************************
	 函数名称：
	 功    能：
	 输入参数：
	 返 回 值：
	 说    明：
   ***************************************************************************/
int main()
{
	cct_setconsoleborder(120, 40, 120, 9000);
	/* demo中首先执行此句，将cmd窗口设置为40行x120列（缓冲区宽度120列，行数9000行，即cmd窗口右侧带有垂直滚动杆）*/
	const char* s[] = { "1.基本解" ,"2.基本解(步数记录)" ,"3.内部数组显示(横向)" ,"4.内部数组显示(纵向 + 横向)" ,
	"5.图形解 - 预备 - 画三个圆柱" ,"6.图形解 - 预备 - 在起始柱上画n个盘子" , "7.图形解 - 预备 - 第一次移动" ,
	"8.图形解 - 自动移动版本" , "9.图形解 - 游戏版" ,"0.退出" };

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
				cout << "按回车键继续";
			}
			while (_getch() != '\r')
				continue;
		}
	}
}