/*1852877 �ƿ� ��껈�*/
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include <cstring>
#include "../include/cmd_console_tools.h"
using namespace std;

int cnt = 0;//�Ʋ���
int A[10] = { 0 }, B[10] = { 0 }, C[10] = { 0 };//����һά�����¼Բ�̱��
int atop = 0, btop = 0, ctop = 0;//��¼Բ����Բ������,��ջ��ָ��λ��
int delay;//��ʱ
/* -----------------------------------------

     ���ļ����ܣ�
	1���ű� hanoi_main.cpp/hanoi_menu.cpp �еĸ��������õĲ˵������Ӧ��ִ�к���

     ���ļ�Ҫ��
	1�����������ⲿȫ�ֱ�����const��#define�������Ʒ�Χ�ڣ�
	2�������徲̬ȫ�ֱ�����������Ҫ��������Ҫ�����ĵ���ʾ��ȫ�ֱ�����ʹ��׼���ǣ����á����á��ܲ��þ������ã�
	3����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	4���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------- */


/********************01.ջ��ԭ����:ʹ����ջ֮����и�ԭջ�ͼƲ���********************************************/
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
/********************01.ջ��ԭ����:ʹ����ջ֮����и�ԭջ�ͼƲ���********************************************/


/***************02.�������뺯��:Ϊmethod��������������ʼ.Ŀ��,�м�,�����Ĳ���****************/
void getparameter(int & n, char & src, char & tmp,char & dst,int method)
{
	while (1)
	{
		cout << "�����뺺ŵ���Ĳ���(1-10)" << endl;
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
		cout << "��������ʼ��(A-C)" << endl;
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
		cout << "������Ŀ����(A-C)" << endl;
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
			cout << "�������ƶ��ٶ�(0 - 5: 0 - ���س�������ʾ 1 - ��ʱ� 5 - ��ʱ���)" << endl;
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
/***************02.�������뺯��:Ϊmethod��������������ʼ.Ŀ��,�м�,�����Ĳ���****************/


/***************03.��ͣ����:����delayֵ��ͬʱ������ͣ����******************************************/
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
/***************03.��ͣ����:����delayֵ��ͬʱ������ͣ����******************************************/


/***************04.�����ӡ��ŵ������***********************************************************************/
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
/***************04.�����ӡ��ŵ������***********************************************************************/


/**********************05.�����ӡ��ŵ������*****************************************************/
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
/**********************05.�����ӡ��ŵ������*****************************************************/


/************************06.�滭���Ӻ���***************************************/
void drawpole()//������
{
	const int Y = 15;
	const char ch = ' ';
	int LENGTH = 23;
	const int bg_color = COLOR_HYELLOW;	//����Ϊ����ɫ
	const int fg_color = COLOR_HYELLOW;
	cct_setcursor(CURSOR_INVISIBLE);
	for (int x = 1; x < 66; x = x + 32) {
		cct_showch(x, Y, ch, bg_color, fg_color, LENGTH);//��������
		/* ��ʱ0.1�� */
		Sleep(30);
		for (int y = 14; 2 < y; y--)//��������
		{
			cct_showch(x + 11, y, ch, bg_color, fg_color, 1);
			Sleep(20);
		}
	}
}
/************************06.�滭���Ӻ���***************************************/

/************************07.�滭���Ӻ���****************************************/
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
/************************07.�滭���Ӻ���****************************************/


/************************08.�ƶ����ӵĺ���***************************************/
void move_singledish(int x1, int y1, int x2, int y2, int move)//ʵ��һ������Ϊlen(����Ϊ2*move+1)��ɫ��(��ɫΪmove)�ƶ�1->2
{
	//ȷ���ƶ�ɫ�����ɫ=move
	//���Ƶ�y=1
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
	//����/���Ƶ�x=x2
	if (x2 - x1 > 0)//����
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
	else//���Ƶ�x2
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
	//���Ƶ�y2
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

void move_dish(char src, char dst, int move)//����һ��ʼĩ������ʾ��src�ƶ�n���̵�dst,move���Ա�ʾ�ƶ����ӵ��̺�(��Ӧ��С);��ʼλ������ʼλ��ջ��ָ��ȷ��
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
/************************08.�ƶ����ӵĺ���***************************************/


/***********************09.ջ��������:��ʼ��+����+����****************************/
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
		cout << setiosflags(ios::left) << setw(21) << "��ʼ:";
		print();
		PRINT(4);
	}
	if (method == 8)
	{
		cct_gotoxy(0, 32);
		cout << resetiosflags(ios::right);
		cout << setiosflags(ios::left) << setw(21) << "��ʼ:";
		print();
		PRINT(8);
	
	}
}
//��ջ��Ԫ�ط����ٸ�ֵΪ0,������,ָ������
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
//��dish����ջ��,ָ������
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
/***********************09.ջ��������:��ʼ��+����+����****************************/

/***********************10.��ŵ���������***************************************/
void output(int move,int n,char src,char dst,int method)
{
	cct_setcolor();
	switch(method)
	{ 
		case 1: 
			cout << n << "# " << src << "---->" << dst << endl;
			break;
		case 2:
			cout << "��"<<setiosflags(ios::right) << setw(4) << cnt << " ��(" << setw(2) << n << resetiosflags(ios::right)
				<< "#: " << src << "-->" << dst <<")"<< endl;
			break;
		case 3:
			cout << "��" << setiosflags(ios::right) << setw(4) << cnt << " ��(" << setw(2) << move << "#: " << src << "-->" << dst << ")  ";
			print();
			break;
		case 4:
			sleep();
			cct_gotoxy(0, 17);
			cout << "��" << setiosflags(ios::right) << setw(4) << cnt << " ��(" << move << "#: " << src << "-->" << dst << ")  ";
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
			cout << "��" << setiosflags(ios::right) << setw(4) << cnt << " ��(" << move << "#: " << src << "-->" << dst << ")  ";
			print();
			PRINT(8);
			move_dish(src, dst, move);
			break;

	}
}
/***********************10.��ŵ���������***************************************/

/***********************11.��ŵ���ݹ麯��**************************************/
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
		hanoi(n - 1, src, dst, tmp,method);//��x-1�Ƶ��м�����
		int move = pop(src);
		push(dst, move);
		cnt++;
		output(move,n,src,dst,method);
		hanoi(n - 1, tmp, src, dst,method);//x-1���м������Ƶ�Ŀ������
	}
}
/***********************11.��ŵ���ݹ麯��**************************************/


/***********************12.methodϵ�к���:1-8(9���⴦��)**********************/
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
	hanoi(n, src, tmp, dst, 3);//������Ϊ�ⲿ��Ҫ�õ�ȫ��������˳�ʼ������ȱ��
	original();
}
void method4() 
{
	int n;
	char src, tmp, dst;
	getparameter(n, src, tmp, dst,4);
	cct_cls();
	cct_setcursor(CURSOR_INVISIBLE);
	cout << "�� " << src << " �ƶ��� " << dst << "���� " << n << " �㣬��ʱ����Ϊ "<< delay  << endl;
	initstack(n, src,4);//��ʼ����ʼ��
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
	cout << "�� " << src << " �ƶ��� " << dst << "���� " << n << " ��"<<endl;
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
	cout << "�� " << src << " �ƶ��� " << dst << "���� " << n << " �㣬��ʱ����Ϊ " << delay << endl;
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
/***********************12.methodϵ�к���:1-8(9���⴦��)**********************/

/***********************13.mehtod9(��Ϸģʽ)ϵ�к���:�ж��ƶ�����+�ж���Ϸ����+�ƶ�**************/

/***********************13-1.�ж�Դ��Ϊ�յĺ���*************************************************/
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
/***********************13-1.�ж�Դ��Ϊ�յĺ���*************************************************/


/***********************13-2.�жϴ���ѹС��*****************************************************/
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
/***********************13-2.�жϴ���ѹС��*****************************************************/


/***********************13-3.�ж���Ϸ����******************************************************/
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
/***********************13-3.�ж���Ϸ����******************************************************/

void method9() 
{
	int n;
	char src, tmp, dst;
	getparameter(n, src, tmp, dst,9);
	cct_cls();
	cout << "�� " << src << " �ƶ��� " << dst << "���� " << n << " ��" << endl;
	initstack(n, src, 8);
	drawpole();
	int shift = src - 'A';
	drawdish(n, shift);
	Sleep(350);
	cct_gotoxy(0, 33);
	cct_setcolor();
	cout << endl;
	cout << "�������ƶ�������(������ʽ��AC=A���˵������ƶ���C��Q=�˳�) ��";
	
	while (1)
	{
		char s[20] = { 0 };
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
		cin >> s;
		cct_setcursor(CURSOR_INVISIBLE);
		if (strcmp(s, "q") == 0||strcmp(s,"Q")==0)
		{
			cout << "��Ϸ��ֹ!!!!!" << endl;
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
						s[i] = s[i] - 32;//ȫ��ת���д
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
						cout << "Դ��Ϊ��" << endl;
						Sleep(700);
						cct_showch(0, 36, ' ', COLOR_BLACK, COLOR_WHITE, 20);
						cct_showch(60, 34, ' ', COLOR_BLACK, COLOR_WHITE, 20);
						cct_gotoxy(60, 34);
						continue;
					}
						
					else if (bigup(s[0], s[1]) == 0)
					{
						cout << endl;
						cout << "����ѹС��,�Ƿ��ƶ�!" << endl;
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
						cout << "��" << setiosflags(ios::right) << setw(4) << cnt << " ��(" << move << "#: " << s[0] << "-->" << s[1] << ")  ";
						print();
						PRINT(8);
						move_dish(s[0], s[1], move);
						if (checkfinish(dst, n))
						{
							cct_setcolor();
							cct_gotoxy(0, 35);
							cout << "��Ϸ����!!!!!" << endl;
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
/***********************13.mehtod9(��Ϸģʽ)ϵ�к���:�ж��ƶ�����+�ж���Ϸ����+�ƶ�**************/
