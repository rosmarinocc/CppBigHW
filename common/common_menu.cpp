/*1852877 计科 赵昊堃*/
#include <iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
using namespace std;

//display_menu:功能:输出菜单选项并返回用户输入的合法字符(不区分大小写,若为字母统一返回大写)
char display_menu(const char* items[], int itemsnum)
{
	int len_max = 0;
	for (int i = 0; i < itemsnum; i++)//获取字符串最大长度
		if (strlen(items[i]) > (unsigned)len_max)
			len_max = strlen(items[i]);

	for (int i = 0; i < len_max + 2; i++)//打印菜单
		cout << '-';
	cout << endl;
	for (int i = 0; i < itemsnum; i++)
		cout << items[i] << endl;
	for (int i = 0; i < len_max + 2; i++)
		cout << '-';
	cout << endl;
	cout << "[请选择] :";

	while (1)//选择菜单项
	{
		char ch = _getch();
		bool flag = false;
		for (int i = 0; i < itemsnum; i++)
			if (ch == items[i][0] ||
				items[i][0] >= 'a' && items[i][0] <= 'z' && (items[i][0] - ch == 'a' - 'A') ||
				items[i][0] >= 'A' && items[i][0] <= 'Z' && (items[i][0] - ch == 'A' - 'a'))//菜单序号判断
				flag = true;
		if (flag == true) {
			if (ch >= 'a' && ch <= 'z')//输入小写,同一返回大写
				return ch + 'A' - 'a';
			else
				return ch;
		}
		else
			continue;
	}
}

//功能:获取数字参数,输入提示语,闭区间上下界,默认值,以及待修改参数的引用,可选参数表示超出范围后:使用默认值/继续循环
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

//功能:按回车继续
void wait_for_enter()
{
	cout << endl << "按回车键继续";
	while (_getch() != '\r')
		;
	cout << endl << endl;
}