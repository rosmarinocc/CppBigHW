/*1852877 计科 赵昊堃*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include "90-02-b1-star.h"
#include "../include/common_arraytools.h"
using namespace std;

///////////////////////////////////////////函数01:功能:提示输入并获取行列数//////////////////////////
int input_rc(char ch) {
	int k;
	int start_x, start_y, input_x, input_y;
	cct_getxy(start_x, start_y);
	if (ch == 'r')
		cout << "请输入行数(8-10):   ";
	else
		cout << "请输入列数(8-10):   ";
	cct_getxy(input_x, input_y);
	while (1) {
		cin >> k;
		if (cin.good() && k >= 8 && k <= 10)
			break;
		else {
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "输入不合法请重新输入" << endl;

			if (ch == 'r') {
				cct_gotoxy(start_x, start_y);
				cout << "请输入行数(8-10):               ";
				cct_gotoxy(input_x, input_y);
			}
			else {
				cct_gotoxy(start_x, start_y);
				cout << "请输入列数(8-10):               ";
				cct_gotoxy(input_x, input_y);
			}
			continue;
		}
	}
	return k;
}
//////////////////////////////////////////函数02:功能:菜单项选择并调用相应的方法///////////////////////
void choose_menu(char ch)
{
	cct_cls();
	cct_setconsoleborder(60, 40, 60, 9000);
	RC rc;
	rc.row = input_rc('r');
	rc.column = input_rc('c');
	switch (ch) {
		case 'A':
			funA(rc);
			break;
		case 'B':
			funB(rc);
			break;
		case 'C':
			funC(rc);
		case 'D':
			funD(rc);
			break;
		case 'E':
			funE(rc);
			break;
		case 'F':
			funF(rc);
			break;
		case 'G':
			funG(rc);
			break;
	}
}
///////////////////////////////////////////////函数03:功能:给数组初始化//////////////////////////////////
void init_array(RC rc, elem star[12][12])//注意内部数组首元素为star[1][1]
{
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 12; j++) {
			star[i][j].key = 0;
			star[i][j].state = 1;
		}

	for (int i = 1; i <= rc.row; i++)
		for (int j = 1; j <= rc.column; j++)
			star[i][j].key = RAND(5) + 1;

}
/*****************************************函数04:功能:根据参数展示各种情况下的数组************************/
void show_array(RC rc, elem star[12][12], int state)
{
	int row = rc.row;
	int col = rc.column;
	cct_setcolor();
	cout << endl;
	switch (state) {
		case 0:
			cout << "\n当前数组:" << endl;
			break;
		case 1:
			cout << "\n查找结果数组:" << endl;
			break;
		case 2:
			cout << "\n当前数组(不同色标识):" << endl;
			break;
		case 3:
			cout << "\n相同值归并后的数组(不同色标识):" << endl;
			break;
		case 4:
			cout << "\n下落后的数组" << endl;
			break;
	}
	cout << "  |";
	for (int i = 0; i < col; i++) {
		cout << "  " << i;
	}
	cout << endl;
	cout << "--+";
	for (int i = 0; i < 3 * col + 2; i++)
		cout << '-';
	cout << endl;
	for (int i = 1; i <= row; i++) {//注意内部数组首元素为star[1][1]
		cout << char('A' + i - 1) << " |";
		for (int j = 1; j <= col; j++) {
			switch (state) {
				//当前数组&下落后的数组
				case 0:
				case 4://key=0全部高亮
					if (star[i][j].key == 0) {
						cout << "  ";
						cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
						cout << star[i][j].key;
						cct_setcolor();
					}
					else
						cout << "  " << star[i][j].key;
					break;
					//查找结果数组
				case 1://显示0和*
					if (star[i][j].state == 2)
						cout << "  *";
					else
						cout << "  0";
					break;
					//当前数组(不同色标识)&相同值归并后的数组(不同色标识)
				case 2:
				case 3://*高亮显示
					if (star[i][j].state == 2 || star[i][j].state == 3) {
						cout << "  ";
						cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
						cout << star[i][j].key;
						cct_setcolor();
					}
					else
						cout << "  " << star[i][j].key;
					break;
			}
		}
		cout << endl;
	}
}
////////////////////////////////////////////函数05:功能:检测&接受合法的行列(输入合法)/////////////////
RC command_pos(RC rc, elem star[12][12])
{
	cout << "\n请以字母+数字形式[例：c2]输入矩阵坐标：";
	char ch1, ch2;
	RC rc1 = { 0,0 }, pos = { 0,0 };
	cct_getxy(pos.row, pos.column);
	while (1) {
		//////////////cin出错判断////////////////
		cin >> ch1 >> ch2;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "输入有误,请重新输入";
			cct_showstr(pos.row, pos.column, "                        ");
			cct_gotoxy(pos.row, pos.column);
			continue;
		}
		/////////////行列越界判断///////////////
		bool flag1 = true, flag2 = true;
		char c1 = isType(ch1, "small") ? ch1 - 'a' + 'A' : ch1;//如果ch1是小写字母,转为大写统一判断
		if (!(isType(c1, "capital") && c1 <= 'A' + rc.row - 1))
			flag1 = false;
		if (!(isType(ch2, "number") && ch2 <= '0' + rc.column - 1))
			flag2 = false;
		if (flag1 && flag2) {
			rc1.row = c1 - 'A' + 1;
			rc1.column = ch2 - '0' + 1;
			cout << "输入为" << c1 << "行" << ch2 << "列            " << endl;
			break;
		}
		else {
			cout << "输入有误,请重新输入";
			cct_showstr(pos.row, pos.column, "                        ");
			cct_gotoxy(pos.row, pos.column);
			continue;
		}

	}
	return rc1;
}
/////////////////////函数06:功能:检查周边(上下左右)相同key的元素////////////////////////////////
void search_sur(elem star[][12], int row, int col, int* pcnt, int execute) {
	if (execute == 0) {//查找模式,找到后对象设为state=2
		if (star[row][col].key == 0 || star[row][col].state != 1)
			return;
		else {
			star[row][col].state = 2;
			(*pcnt)++;
		}
	}
	else if (execute == 2) {//消除模式,本次消除的对象为state=2&&key=0
		if (star[row][col].key == 0 || star[row][col].state != 2)
			return;
		else {
			star[row][col].state = 3;
			(*pcnt)++;
		}
	}
	if (star[row - 1][col].key == star[row][col].key)
		search_sur(star, row - 1, col, pcnt, execute);
	if (star[row + 1][col].key == star[row][col].key)
		search_sur(star, row + 1, col, pcnt, execute);
	if (star[row][col - 1].key == star[row][col].key)
		search_sur(star, row, col - 1, pcnt, execute);
	if (star[row][col + 1].key == star[row][col].key)
		search_sur(star, row, col + 1, pcnt, execute);
}
//////////////////////函数07:功能:检测&接受合法的行列(是否为零,是否连续)////////////////
bool command_checksur(RC rc1, elem star[][12])
{
	int cnt = 0, * pcnt = &cnt;
	bool legal = true;
	if (star[rc1.row][rc1.column].key == 0) {
		cout << "输入矩阵坐标位置为0,请重新输入" << endl;
		legal = false;
	}
	else {
		search_sur(star, rc1.row, rc1.column, pcnt, 0);
		if (cnt == 1) {
			star[rc1.row][rc1.column].state = 1;//无连续,需要回退状态值
			cout << "输入矩阵坐标位置无连续相同值,请重新输入" << endl;
			legal = false;
		}

	}
	return legal;
}
//////////////////////函数08:功能:检测end///////////////////////////////////////
void check_end()
{
	while (1) {
		cout << "\n本小题结束,输入End继续...";
		char s[20];
		cin >> s;
		if (strcmp(_strlwr(s), "end") == 0)
			break;
		else
			cout << "输入错误,请重新输入" << endl;
	}
}
//////////////////////函数09:功能:遍历数组&&执行消除置零和结束一步退回状态的操作///////////////////////////////
int traverse_array(RC rc, elem star[][12], const char* s)//clear:将查找后并Y确认后的元素置0;reset将所有state=3的元素置1;count:计算余下非零元素个数
{
	int count = 0;
	for (int i = 1; i <= rc.row; i++)
		for (int j = 1; j <= rc.column; j++)
			if (strcmp(s, "clear") == 0) {
				if (star[i][j].state == 3)
					star[i][j].key = 0;
			}
			else if (strcmp(s, "resetstate") == 0) {
				star[i][j].state = 1;
			}
			else {
				if (star[i][j].key != 0)
					count++;
			}
	return count;
}
/////////////////////函数10:功能:实现数组下落左移操作/////////////////////////////////////////////////
bool is_allzero(elem star[][12], int row, int col_no)////封装成一个函数:输入列号与行列数据,返回是否为全零行
{
	int	zerocnt = 0;
	for (int i = 1; i <= row; i++)
		if (star[i][col_no].key == 0)
			zerocnt++;
	if (zerocnt == row)//出现全零行,集体移动
		return true;
	else
		return false;
}

void fall_array(RC rc, elem star[][12])
{
	cout << "\n按回车进行数组下落操作...";
	while (1)
		if (_getche() == '\r')
			break;

	for (int j = 1; j <= rc.column; j++) {//上下移动:逻辑简单
		int temp[12] = { 0 }, * p = temp, * q = temp;
		for (int i = rc.row; i >= 1; i--)
			if (star[i][j].key != 0)
				*p++ = star[i][j].key;
		for (int i = rc.row; i >= 1; i--)
			star[i][j].key = *q++;
	}

	//只有出现全零列&&右侧相邻列非全零列的时候,才进行左右移动

	while (1) {
		bool out = true;
		for (int j = 1; j <= rc.column; j++)
			if (is_allzero(star, rc.row, j) && !is_allzero(star, rc.row, j + 1))
				out = false;
		if (out == true)
			break;
		else {
			for (int j = 1; j <= rc.column; j++)
				if (is_allzero(star, rc.row, j) && !is_allzero(star, rc.row, j + 1)) {
					//集体左移操作:实际上用交换更为贴切,将全零列交换到最右侧
					for (int i = 1; i <= rc.row; i++) {
						star[i][j].key = star[i][j + 1].key;
						star[i][j + 1].key = 0;
					}
				}
		}
	}

}

/************************************函数11:功能:检测与处理Y/N/Q*******************8888*****************/
int command_clear(RC rc, RC rc1, elem star[][12], int& score)//int &score记录总分
{
	cout << "请确认是否消除高亮区域(Y/N/Q):";
	char cmd;
	int cnt = 0, * pcnt = &cnt;
	while (1) {
		cmd = _getche();
		if (cmd == 'Y' || cmd == 'y' || cmd == 'N' || cmd == 'n' || cmd == 'Q' || cmd == 'q')
			break;
	}

	if (cmd == 'Y' || cmd == 'y')
	{
		search_sur(star, rc1.row, rc1.column, pcnt, 2);
		traverse_array(rc, star, "clear");
		show_array(rc, star, 2);
		score += BONUS * cnt;
		cout << "本次得分:" << BONUS * cnt << "  总得分:" << score << endl;
		traverse_array(rc, star, "resetstate");
		fall_array(rc, star);
		show_array(rc, star, 4);
		return 1;

	}
	else if (cmd == 'N' || cmd == 'n') {
		traverse_array(rc, star, "resetstate");
		return 1;
	}


	else {//q Q
		traverse_array(rc, star, "resetstate");
		return 0;
	}
}
/**********************************函数12:功能:检查是否已无可继续消除的元素(游戏是否结束)*************************/
bool check_gameover(RC rc, elem star[][12])//gameover返回0代表未结束,返回1代表结束
{
	for (int i = 1; i <= rc.row; i++)
		for (int j = 1; j <= rc.column; j++) {
			if (star[i][j].key != 0) {
				int cnt = 0, * pcnt = &cnt;
				search_sur(star, i, j, pcnt);
				if (cnt > 1) {
					traverse_array(rc, star, "resetstate");
					return 0;
				}
			}
		}
	traverse_array(rc, star, "resetstate");
	return 1;
}