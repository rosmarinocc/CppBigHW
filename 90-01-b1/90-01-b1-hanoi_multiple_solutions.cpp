/*1852877 计科 赵昊堃*/
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include <cstring>
#include "../include/cmd_console_tools.h"
using namespace std;

int cnt = 0;//计步器
int A[10] = { 0 }, B[10] = { 0 }, C[10] = { 0 };//三个一维数组记录圆盘编号
int atop = 0, btop = 0, ctop = 0;//记录圆柱上圆盘数量,即栈顶指针位置
int delay;//延时
/* -----------------------------------------

     本文件功能：
	1、放被 hanoi_main.cpp/hanoi_menu.cpp 中的各函数调用的菜单各项对应的执行函数

     本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------- */


/********************01.栈复原函数:使用完栈之后进行复原栈和计步器********************************************/
void original()
{
	cnt = 0;
	atop = btop = ctop = 0;
	for (int i = 0; i < 10; i++)
	{
		A[i] = 0;
		B[i] = 0;
		C[i] = 0;
	}
}
/********************01.栈复原函数:使用完栈之后进行复原栈和计步器********************************************/


/***************02.参数输入函数:为method方法函数输入起始.目标,中间,层数的参数****************/
void getparameter(int & n, char & src, char & tmp,char & dst,int method)
{
	while (1)
	{
		cout << "请输入汉诺塔的层数(1-10)" << endl;
		cin >> n;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1024, '\n');
			continue;
		}
		if (n >= 1 && n <= 10)
		{
			cin.clear();
			cin.ignore(1024, '\n');
			break;
		}
		cin.clear();
		cin.ignore(1024, '\n');
	}
	while (1)
	{
		cout << "请输入起始柱(A-C)" << endl;
		cin >> src;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1024, '\n');
			continue;
		}
		if (src >= 65 && src <= 67 || src >= 97 && src <= 99)
		{
			cin.clear();
			cin.ignore(1024, '\n');
			break;
		}
		cin.clear();
		cin.ignore(1024, '\n');
	}
	while (1)
	{
		cout << "请输入目标柱(A-C)" << endl;
		cin >> dst;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1024, '\n');
			continue;
		}
		if ((dst >= 65 && dst <= 67 || dst >= 97 && dst <= 99) && (abs(src - dst) != 0) && (abs(src - dst) != 32))
		{
			cin.clear();
			cin.ignore(1024, '\n');
			break;
		}
		cin.clear();
		cin.ignore(1024, '\n');
	}
	if (src == 'A' || src == 'a')
	{
		if (dst == 'B' || dst == 'b')
		{
			src = 'A';
			dst = 'B';
			tmp = 'C';
		}
		else
		{
			src = 'A';
			dst = 'C';
			tmp = 'B';
		}
	}
	else
	{
		if (src == 'B' || src == 'b')
		{
			if (dst == 'A' || dst == 'a')
			{
				src = 'B';
				dst = 'A';
				tmp = 'C';
			}
			else
			{
				src = 'B';
				dst = 'C';
				tmp = 'A';
			}
		}
		else
		{
			if (dst == 'A' || dst == 'a')
			{
				src = 'C';
				dst = 'A';
				tmp = 'B';
			}
			else
			{
				src = 'C';
				dst = 'B';
				tmp = 'A';
			}
		}
	}
	if (method == 4 || method == 8)
	{
		while (1) {
			cout << "请输入移动速度(0 - 5: 0 - 按回车单步演示 1 - 延时最长 5 - 延时最短)" << endl;
			cin >> delay;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(1024, '\n');
				continue;
			}
			if (delay >= 0 && delay <= 5)
				break;
		}
	}

}
/***************02.参数输入函数:为method方法函数输入起始.目标,中间,层数的参数****************/


/***************03.暂停函数:根据delay值不同时长的暂停函数******************************************/
void sleep()
{
	switch (delay)
	{
		case 0:
			while (1) {
				if (_getch() == '\r')
					break;
			}
			break;
		case 1:
			Sleep(400);
			break;
		case 2:
			Sleep(200);
			break;
		case 3:
			Sleep(100);
			break;
		case 4:
			Sleep(50);
			break;
		case 5:
			Sleep(25);
			break;
	}
}
/***************03.暂停函数:根据delay值不同时长的暂停函数******************************************/


/***************04.横向打印汉诺塔函数***********************************************************************/
void print()
{
	cct_setcolor();
	cout << setiosflags(ios::right) << "A:";
	for (int aa = 0; aa < 10; aa++)
	{
		if (A[aa] > 0)
			cout << setw(2) << A[aa];
		else
			cout << setw(2) << " ";
	}
	cout << " B:";
	for (int bb = 0; bb < 10; bb++)
	{
		if (B[bb] > 0)
			cout << setw(2) << B[bb];
		else
			cout << setw(2) << " ";
	}
	cout << " C:";
	for (int cc = 0; cc < 10; cc++)
	{
		if (C[cc] > 0)
			cout << setw(2) << C[cc];
		else
			cout << setw(2) << " ";
	}
	cout << endl;
}
/***************04.横向打印汉诺塔函数***********************************************************************/


/**********************05.纵向打印汉诺塔函数*****************************************************/
void PRINT(int method)
{
	cct_setcolor();
	if (method == 8)
	{
		cct_gotoxy(11, 28);
		cout << "A         B         C" << endl;
		cct_gotoxy(9, 27);
		cout << "=========================" << endl;
		for (int i = 0; i < 10; i++)
		{
			cct_gotoxy(10, 26 - i);
			if (A[i] > 0)
				cout << setiosflags(ios::right) << setw(2) << A[i];
			else
				cout << "  ";
		}
		for (int i = 0; i < 10; i++)
		{
			cct_gotoxy(20, 26 - i);
			if (B[i] > 0)
				cout << setiosflags(ios::right) << setw(2) << B[i];
			else
				cout << "  ";
		}
		for (int i = 0; i < 10; i++)
		{
			cct_gotoxy(30, 26 - i);
			if (C[i] > 0)
				cout << setiosflags(ios::right) << setw(2) << C[i];
			else
				cout << "  ";
		}
	}
	else {
		cct_gotoxy(11, 13);
		cout << "A         B         C" << endl;
		cct_gotoxy(9, 12);
		cout << "=========================" << endl;
		for (int i = 0; i < 10; i++)
		{
			cct_gotoxy(10, 11 - i);
			if (A[i] > 0)
				cout << setiosflags(ios::right) << setw(2) << A[i];
			else
				cout << "  ";
		}
		for (int i = 0; i < 10; i++)
		{
			cct_gotoxy(20, 11 - i);
			if (B[i] > 0)
				cout << setiosflags(ios::right) << setw(2) << B[i];
			else
				cout << "  ";
		}
		for (int i = 0; i < 10; i++)
		{
			cct_gotoxy(30, 11 - i);
			if (C[i] > 0)
				cout << setiosflags(ios::right) << setw(2) << C[i];
			else
				cout << "  ";
		}
	}
}
/**********************05.纵向打印汉诺塔函数*****************************************************/


/************************06.绘画柱子函数***************************************/
void drawpole()//画柱子
{
	const int Y = 15;
	const char ch = ' ';
	int LENGTH = 23;
	const int bg_color = COLOR_HYELLOW;	//背景为亮黄色
	const int fg_color = COLOR_HYELLOW;
	cct_setcursor(CURSOR_INVISIBLE);
	for (int x = 1; x < 66; x = x + 32) {
		cct_showch(x, Y, ch, bg_color, fg_color, LENGTH);//画横向轴
		/* 延时0.1秒 */
		Sleep(30);
		for (int y = 14; 2 < y; y--)//画纵向轴
		{
			cct_showch(x + 11, y, ch, bg_color, fg_color, 1);
			Sleep(20);
		}
	}
}
/************************06.绘画柱子函数***************************************/

/************************07.绘画盘子函数****************************************/
void drawdish(int n, int shift)
{
	int j = 14;
	for (int i = n; i > 0; i--)
	{
		cct_showch(12 - i + 32 * shift, j, ' ', i, j, 2 * i + 1);
		j--;
		Sleep(10);
	}
}
/************************07.绘画盘子函数****************************************/


/************************08.移动盘子的函数***************************************/
void move_singledish(int x1, int y1, int x2, int y2, int move)//实现一个长度为len(传参为2*move+1)的色块(颜色为move)移动1->2
{
	//确定移动色块的颜色=move
	//上移到y=1
	for (int y = y1; y > 0; y--)
	{
		cct_showch(x1, y, ' ', move, move, 2 * move + 1);
		Sleep(35);
		if (y > 1) {
			if (y > 2)
			{
				cct_showch(x1, y, ' ', COLOR_BLACK, COLOR_WHITE, move);
				cct_showch(x1 + move, y, ' ', COLOR_HYELLOW, COLOR_HYELLOW, 1);
				cct_showch(x1+move+1, y, ' ', COLOR_BLACK, COLOR_WHITE, move);
			}
			else
				cct_showch(x1, y, ' ', COLOR_BLACK, COLOR_WHITE, 2 * move + 1);
		}
	}
	//左移/右移到x=x2
	if (x2 - x1 > 0)//右移
	{
		for (int x = x1; x < x2 + 1; x++)
		{
			cct_showch(x, 1, ' ', move, move, 2 * move + 1);
			Sleep(35);
			if (x < x2) {
				cct_showch(x, 1, ' ', COLOR_BLACK, COLOR_WHITE, 2 * move + 1);
			}
		}
	}
	else//左移到x2
	{
		for (int x = x1; x >= x2; x--)
		{
			cct_showch(x, 1, ' ', move, move, 2 * move + 1);
			Sleep(35);
			if (x > x2) {
				cct_showch(x, 1, ' ', COLOR_BLACK, COLOR_WHITE, 2 * move + 1);
			}
		}
	}
	//下移到y2
	for (int y = 1; y <= y2; y++)
	{
		cct_showch(x2, y, ' ', move, move, 2 * move + 1);
		Sleep(35);
		if (y < y2) {
			if (y > 2)
			{
				cct_showch(x2, y, ' ', COLOR_BLACK, COLOR_WHITE, move);
				cct_showch(x2 + move, y, ' ', COLOR_HYELLOW, COLOR_HYELLOW, 1);
				cct_showch(x2 + move + 1, y, ' ', COLOR_BLACK, COLOR_WHITE, move);
			}
			else
				cct_showch(x2, y, ' ', COLOR_BLACK, COLOR_WHITE, 2 * move + 1);
		}
	}
}

void move_dish(char src, char dst, int move)//接受一个始末参数表示从src移动n号盘到dst,move可以表示移动盘子的盘号(对应大小);起始位置由起始位置栈顶指针确定
{
	int x1, y1, x2, y2;
	switch (src)
	{
		case 'A':
			if (dst == 'B')//A->B
			{
				x1 = 12 - move;
				y1 = 14 - atop;
				x2 = 44 - move;
				y2 = 15 - btop;
			}
			else//A->C
			{
				x1 = 12 - move;
				y1 = 14 - atop;
				x2 = 76 - move;
				y2 = 15 - ctop;
			}
			break;
		case 'B':
			if (dst == 'A')//B->A
			{
				x1 = 44 - move;
				y1 = 14 - btop;
				x2 = 12 - move;
				y2 = 15 - atop;
			}
			else //B->C
			{
				x1 = 44 - move;
				y1 = 14 - btop;
				x2 = 76 - move;
				y2 = 15 - ctop;
			}
			break;
		case 'C':
			if (dst == 'A')//C->A
			{
				x1 = 76 - move;
				y1 = 14 - ctop;
				x2 = 12 - move;
				y2 = 15 - atop;
			}
			else//C->B
			{
				x1 = 76 - move;
				y1 = 14 - ctop;
				x2 = 44 - move;
				y2 = 15 - btop;
			}
			break;
	}
	move_singledish(x1, y1, x2, y2, move);
}
/************************08.移动盘子的函数***************************************/


/***********************09.栈操作函数:初始化+弹出+推入****************************/
void initstack(int n, char src,int method)
{
	switch (src)
	{
		case 'A':
			atop = n;
			for (int i = 0; i < 10; i++)
				if (n > 0)
				{
					A[i] = n;
					n--;
				}
				else
					break;
		case 'B':
			btop = n;
			for (int i = 0; i < 10; i++)
				if (n > 0)
				{
					B[i] = n;
					n--;
				}
				else
					break;
		case 'C':
			ctop = n;
			for (int i = 0; i < 10; i++)
				if (n > 0)
				{
					C[i] = n;
					n--;
				}
				else
					break;
	}
	cct_setcolor();
	if (method == 1 || method == 2)
		return;
	if (method == 4)
	{
		cct_gotoxy(0, 17);
		cout << resetiosflags(ios::right);
		cout << setiosflags(ios::left) << setw(21) << "初始:";
		print();
		PRINT(4);
	}
	if (method == 8)
	{
		cct_gotoxy(0, 32);
		cout << resetiosflags(ios::right);
		cout << setiosflags(ios::left) << setw(21) << "初始:";
		print();
		PRINT(8);
	
	}
}
//将栈顶元素返回再赋值为0,代表弹出,指针下移
int pop(char x)
{
	switch (x)
	{
		case 'A':
			int a;
			a = A[atop - 1];
			A[--atop] = 0;
			return a;
		case 'B':
			int b;
			b = B[btop - 1];
			B[--btop] = 0;
			return b;
		case 'C':
			int c;
			c = C[ctop - 1];
			C[--ctop] = 0;
			return c;
	}
	return 0;
}
//将dish加入栈顶,指针上移
void push(char x, int dish)
{
	switch (x)
	{
		case 'A':
			A[atop++] = dish;
			break;
		case 'B':
			B[btop++] = dish;
			break;
		case 'C':
			C[ctop++] = dish;
			break;
	}
}
/***********************09.栈操作函数:初始化+弹出+推入****************************/

/***********************10.汉诺塔输出函数***************************************/
void output(int move,int n,char src,char dst,int method)
{
	cct_setcolor();
	switch(method)
	{ 
		case 1: 
			cout << n << "# " << src << "---->" << dst << endl;
			break;
		case 2:
			cout << "第"<<setiosflags(ios::right) << setw(4) << cnt << " 步(" << setw(2) << n << resetiosflags(ios::right)
				<< "#: " << src << "-->" << dst <<")"<< endl;
			break;
		case 3:
			cout << "第" << setiosflags(ios::right) << setw(4) << cnt << " 步(" << setw(2) << move << "#: " << src << "-->" << dst << ")  ";
			print();
			break;
		case 4:
			sleep();
			cct_gotoxy(0, 17);
			cout << "第" << setiosflags(ios::right) << setw(4) << cnt << " 步(" << move << "#: " << src << "-->" << dst << ")  ";
			print();
			PRINT(4);
			break;
		case 7:
			if (n == 1&&cnt==1)
			{
				move_dish(src, dst, move);
			}
			else
				;
			break;
		case 8:
			sleep();
			cct_gotoxy(0, 32);
			cout << "第" << setiosflags(ios::right) << setw(4) << cnt << " 步(" << move << "#: " << src << "-->" << dst << ")  ";
			print();
			PRINT(8);
			move_dish(src, dst, move);
			break;

	}
}
/***********************10.汉诺塔输出函数***************************************/

/***********************11.汉诺塔递归函数**************************************/
void hanoi(int n, char src, char tmp, char dst,int method)
{
	if (n == 1)
	{
		int move = pop(src);
		push(dst, move);
		cnt++;
		output(move,n,src,dst,method);
	}
	else
	{
		hanoi(n - 1, src, dst, tmp,method);//将x-1移到中间柱子
		int move = pop(src);
		push(dst, move);
		cnt++;
		output(move,n,src,dst,method);
		hanoi(n - 1, tmp, src, dst,method);//x-1从中间柱子移到目标柱子
	}
}
/***********************11.汉诺塔递归函数**************************************/


/***********************12.method系列函数:1-8(9特殊处理)**********************/
void method1()
{
	int n;
	char src, tmp, dst;
	getparameter(n, src, tmp, dst,1);
	initstack(n, src, 1);
	hanoi(n, src, tmp, dst, 1);
	original();
}
void method2()
{
	int n;
	char src, tmp, dst;
	getparameter(n, src, tmp, dst,2);
	initstack(n, src, 2);
	hanoi(n, src, tmp, dst, 2);
	original();
}
void method3()
{
	int n;
	char src, tmp, dst;
	getparameter(n, src, tmp, dst, 3);
	initstack(n, src, 3);
	hanoi(n, src, tmp, dst, 3);//出错因为这部需要用到全局数组因此初始化不可缺少
	original();
}
void method4() 
{
	int n;
	char src, tmp, dst;
	getparameter(n, src, tmp, dst,4);
	cct_cls();
	cct_setcursor(CURSOR_INVISIBLE);
	cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层，延时设置为 "<< delay  << endl;
	initstack(n, src,4);//起始柱初始化
	hanoi(n, src, tmp, dst, 4);
	original();
	cct_gotoxy(0, 18);
}

void method5()
{
	cct_cls();
	drawpole();
	cct_gotoxy(0, 36);
	cct_setcolor();
	cct_setcursor(CURSOR_VISIBLE_NORMAL);

}

void method6()
{
	int n;
	char src, tmp, dst;
	getparameter(n, src, tmp, dst,6);
	cct_cls();
	drawpole();
	int shift = src - 'A';
	drawdish(n,shift);
	cct_gotoxy(0, 36);
	cct_setcolor();
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
}


void method7()
{
	int n;
	char src, tmp, dst;
	getparameter(n, src, tmp, dst, 7);
	cct_cls();
	cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层"<<endl;
	initstack(n, src, 7);
	drawpole();
	int shift = src - 'A';
	drawdish(n, shift);
	Sleep(350);
	hanoi(n, src, tmp, dst, 7);
	cct_gotoxy(0, 36);
	cct_setcolor();
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	original();

}
void method8()
{
	int n;
	char src, tmp, dst;
	getparameter(n, src, tmp, dst,8);
	cct_cls();
	cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层，延时设置为 " << delay << endl;
	initstack(n, src, 8);
	drawpole();
	int shift = src - 'A';
	drawdish(n, shift);
	Sleep(350);
	hanoi(n, src, tmp, dst, 8);
	cct_gotoxy(0, 36);
	cct_setcolor();
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	original();
}
/***********************12.method系列函数:1-8(9特殊处理)**********************/

/***********************13.mehtod9(游戏模式)系列函数:判断移动合理+判断游戏结束+移动**************/

/***********************13-1.判断源柱为空的函数*************************************************/
int empty(char ch)
{
	if (ch == 'A')
		if (atop == 0)
			return 0;
		else
			return 1;
	if (ch == 'B')
		if (btop == 0)
			return 0;
		else
			return 1;
	if (ch == 'C')
		if (ctop == 0)
			return 0;
		else
			return 1;
	return 1;
}
/***********************13-1.判断源柱为空的函数*************************************************/


/***********************13-2.判断大盘压小盘*****************************************************/
int bigup(char c1, char c2)
{
	if (c1 == 'A') {
		if (c2 == 'B')
		{
			if (btop != 0&&A[atop - 1] > B[btop - 1])
				return 0;
		}
		else
		{
			if (ctop!=0&&A[atop - 1] > C[ctop - 1])
			{
				return 0;
			}
		}
	}
	else
		return 1;
	if (c1 == 'B') {
		if (c2 == 'A')
		{
			if (atop!=0&&B[btop - 1] > A[atop - 1])
				return 0;
		}
		else
		{
			if (ctop!=0&&B[btop - 1] > C[ctop - 1])
				return 0;
		}

	}
	else
		return 1;
	if (c1 == 'C') {
		if (c2 == 'B')
		{
			if (btop!=0&&C[ctop - 1] > B[btop - 1])
				return 0;
		}
		else
		{
			if (atop!=0&&C[ctop - 1] > A[atop - 1])
				return 0;
		}

	}
	else 
		return 1;
	return 1;
}
/***********************13-2.判断大盘压小盘*****************************************************/


/***********************13-3.判断游戏结束******************************************************/
int checkfinish(char dst,int n)
{
	int p = n;
	switch (dst)
	{
		case 'A':
			for(int i=0;i<n;i++)
			{
				if (A[i] == p)
				{
					p--;
				}
				else
					return 0;
			}
			return 1;
		case 'B':
			for (int i = 0; i < n; i++)
			{
				if (B[i] == p)
				{
					p--;
				}
				else
					return 0;
			}
			return 1;
		case 'C':
			for (int i = 0; i < n; i++)
			{
				if (C[i] == p)
				{
					p--;
				}
				else
					return 0;
			}
			return 1;
	}
	return 0;
}
/***********************13-3.判断游戏结束******************************************************/

void method9() 
{
	int n;
	char src, tmp, dst;
	getparameter(n, src, tmp, dst,9);
	cct_cls();
	cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层" << endl;
	initstack(n, src, 8);
	drawpole();
	int shift = src - 'A';
	drawdish(n, shift);
	Sleep(350);
	cct_gotoxy(0, 33);
	cct_setcolor();
	cout << endl;
	cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";
	
	while (1)
	{
		char s[20] = { 0 };
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
		cin >> s;
		cct_setcursor(CURSOR_INVISIBLE);
		if (strcmp(s, "q") == 0||strcmp(s,"Q")==0)
		{
			cout << "游戏中止!!!!!" << endl;
			break;
		}
		else {
			if (strlen(s) != 2)
			{
				cct_showch(60, 34, ' ', COLOR_BLACK, COLOR_WHITE, 20);
				cct_gotoxy(60, 34);
				continue;
			}
			else {
				for (int i = 0; i < 2; i++)
				{
					if ('a' <= s[i] && s[i] <= 'c')
						s[i] = s[i] - 32;//全部转变大写
				}
				if(!((s[0]=='A'||s[0]=='B'||s[0]=='C')&& (s[1] == 'A' || s[1] == 'B' || s[1] == 'C')))
				{ 
					cct_showch(60, 34, ' ', COLOR_BLACK, COLOR_WHITE, 20);
					cct_gotoxy(60, 34);
					continue;
				}
				else {
					if (empty(s[0]) == 0)
					{
						cout << endl;
						cout << "源柱为空" << endl;
						Sleep(700);
						cct_showch(0, 36, ' ', COLOR_BLACK, COLOR_WHITE, 20);
						cct_showch(60, 34, ' ', COLOR_BLACK, COLOR_WHITE, 20);
						cct_gotoxy(60, 34);
						continue;
					}
						
					else if (bigup(s[0], s[1]) == 0)
					{
						cout << endl;
						cout << "大盘压小盘,非法移动!" << endl;
						Sleep(700);
						cct_showch(0,36, ' ', COLOR_BLACK, COLOR_WHITE, 20);
						cct_showch(60, 34, ' ', COLOR_BLACK, COLOR_WHITE, 20);
						cct_gotoxy(60, 34);
						continue;
					}
					else {
						int move = pop(s[0]);
						push(s[1], move);
						cnt++;
						cct_gotoxy(0,32);
						cout << "第" << setiosflags(ios::right) << setw(4) << cnt << " 步(" << move << "#: " << s[0] << "-->" << s[1] << ")  ";
						print();
						PRINT(8);
						move_dish(s[0], s[1], move);
						if (checkfinish(dst, n))
						{
							cct_setcolor();
							cct_gotoxy(0, 35);
							cout << "游戏结束!!!!!" << endl;
							break;
						}
						else
						{
							
							cct_showch(60, 34, ' ', COLOR_BLACK, COLOR_WHITE, 20);
							cct_gotoxy(60, 34);
							continue;
						}
					}
				}
			}

		}
	}
	original();
}
/***********************13.mehtod9(游戏模式)系列函数:判断移动合理+判断游戏结束+移动**************/
