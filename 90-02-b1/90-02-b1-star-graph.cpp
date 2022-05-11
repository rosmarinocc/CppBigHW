/*1852877 计科 赵昊堃*/
#include <iostream>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include "../include/common_arraytools.h"
#include "90-02-b1-star.h"
using namespace std;

/********************************************函数01:功能:绘制边框,plus版本代表有分界线************************************************/
void draw_border(RC rc)
{
	cct_cls();
	cct_setconsoleborder(7 + BLOCK_W * rc.column, 12 + BLOCK_H * rc.row, 7 + BLOCK_W * rc.column, 9000);
	cout << "屏幕当前设置为:" << 12 + BLOCK_H * rc.row << "行" << 7 + BLOCK_W * rc.column << "列" << endl;
	cout << ' ';
	for (int i = 0; i < rc.column; i++)
		cout << "     " << i;
	cout << endl;
	cout << "  ";
	int start_x, start_y;//记录边框的起始位置
	cct_getxy(start_x, start_y);
	cct_gotoxy(0, start_y - 1);
	for (int i = 0; i < rc.row; i++) {
		cout << '\n' << endl;
		cout << '\n' << char('A' + i);
	}
	cct_gotoxy(start_x, start_y);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	for (int line = 0; line < 2 + BLOCK_H * rc.row; line++) {
		cct_gotoxy(start_x, start_y + line);
		if (line == 0) {
			cout << "╔";
			for (int i = 1; i <= BLOCK_W / 2 * rc.column; i++) {
				if (i % 3 == 0)
					Sleep(1);//暂停函数
				cout << "═";
			}
			cout << "╗";
		}
		else if (line == 1 + BLOCK_H * rc.row) {
			cout << "╚";
			for (int i = 1; i <= BLOCK_W / 2 * rc.column; i++) {
				if (i % 3 == 0)
					Sleep(1);//暂停函数
				cout << "═";
			}
			cout << "╝";
		}
		else {
			cout << "║";
			for (int i = 1; i <= BLOCK_W / 2 * rc.column; i++) {
				if (i % 3 == 0)
					Sleep(1);//暂停函数
				cout << "  ";
			}
			cout << "║";
		}

	}
	cct_setcolor();
}

void draw_borderplus(RC rc)//改写为公共函数?输入行列,block的长宽,起始坐标
{
	cct_cls();
	cct_setconsoleborder(7 + (2 + BLOCK_W) * rc.column, 12 + (1 + BLOCK_H) * rc.row, 7 + (2 + BLOCK_W) * rc.column, 9000);
	cout << "屏幕当前设置为:" << 12 + (1 + BLOCK_H) * rc.row << "行" << 7 + (2 + BLOCK_W) * rc.column << "列" << endl;
	for (int i = 0; i < rc.column; i++)
		cout << "       " << i;
	cout << endl;
	cout << "   ";
	int start_x, start_y;//记录边框的起始位置
	cct_getxy(start_x, start_y);
	cct_gotoxy(0, start_y - 2);
	for (int i = 0; i < rc.row; i++) {
		cout << '\n' << endl;
		cout << '\n' << endl;
		cout << char('A' + i);
	}
	draw_borderlines(rc.row, rc.column, BLOCK_W, BLOCK_H, start_x, start_y);
}

/********************************************函数02:功能:绘制色块,plus版本代表有分界线************************************************/
void draw_block(RC rc, elem star[][12], RC rc1, int highlight)
{
	//highlight=0,无高亮;=1,高亮rc1对应区域;=2高亮rc1周围连续区域
	for (int i = 1; i <= rc.row; i++)
		for (int j = 1; j <= rc.column; j++) {
			cct_gotoxy(4 + BLOCK_W * (j - 1), 3 + BLOCK_H * (i - 1));
			if (star[i][j].key == 0) {
				cct_setcolor(COLOR_HWHITE, COLOR_HWHITE);
				cout << "      ";
				cct_gotoxy(4 + BLOCK_W * (j - 1), 4 + BLOCK_H * (i - 1));
				cout << "      ";
				cct_gotoxy(4 + BLOCK_W * (j - 1), 5 + BLOCK_H * (i - 1));
				cout << "      ";
			}
			else {
				if (highlight == 0) {
					cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "╔═╗";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 4 + BLOCK_H * (i - 1));
					cout << "║★║";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 5 + BLOCK_H * (i - 1));
					cout << "╚═╝";
				}
				else if (highlight == 1) {
					if (i == rc1.row && j == rc1.column)
						cct_setcolor(star[i][j].key, COLOR_HWHITE);
					else
						cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "╔═╗";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 4 + BLOCK_H * (i - 1));
					cout << "║★║";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 5 + BLOCK_H * (i - 1));
					cout << "╚═╝";
				}
				else if (highlight == 2) {//需要调用前保证行列有连续地址,本函数不做任何检测
					if (star[i][j].state == 2 || star[i][j].state == 3)
						cct_setcolor(star[i][j].key, COLOR_HWHITE);
					else
						cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "╔═╗";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 4 + BLOCK_H * (i - 1));
					cout << "║★║";
					cct_gotoxy(4 + BLOCK_W * (j - 1), 5 + BLOCK_H * (i - 1));
					cout << "╚═╝";
				}
			}
		}
	cct_setcolor();
}

void draw_blockplus(RC rc, elem star[][12], RC rc1, int highlight)
{
	//highlight=0,无高亮;=1,高亮rc1对应区域;=2高亮rc1周围连续区域
	for (int i = 1; i <= rc.row; i++)
		for (int j = 1; j <= rc.column; j++) {
			cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 3 + (1 + BLOCK_H) * (i - 1));
			if (star[i][j].key == 0) {
				cct_setcolor(COLOR_HWHITE, COLOR_HWHITE);
				cout << "      ";
				cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 4 + (1 + BLOCK_H) * (i - 1));
				cout << "      ";
				cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 5 + (1 + BLOCK_H) * (i - 1));
				cout << "      ";
			}
			else {
				if (highlight == 0) {
					cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "╔═╗";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 4 + (1 + BLOCK_H) * (i - 1));
					cout << "║★║";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 5 + (1 + BLOCK_H) * (i - 1));
					cout << "╚═╝";
				}
				else if (highlight == 1) {
					if (i == rc1.row && j == rc1.column)
						cct_setcolor(star[i][j].key, COLOR_HWHITE);
					else
						cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "╔═╗";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 4 + (1 + BLOCK_H) * (i - 1));
					cout << "║★║";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 5 + (1 + BLOCK_H) * (i - 1));
					cout << "╚═╝";
				}
				else if (highlight == 2) {//需要调用前保证行列有连续地址,本函数不做任何检测
					if (star[i][j].state == 2 || star[i][j].state == 3)
						cct_setcolor(star[i][j].key, COLOR_HWHITE);
					else
						cct_setcolor(star[i][j].key, COLOR_BLACK);
					cout << "╔═╗";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 4 + (1 + BLOCK_H) * (i - 1));
					cout << "║★║";
					cct_gotoxy(5 + (2 + BLOCK_W) * (j - 1), 5 + (1 + BLOCK_H) * (i - 1));
					cout << "╚═╝";
				}
			}
		}
	cct_setcolor();
}

/********************************************函数03:功能:检测鼠标位置并转换为数组的行列*************************************************/
bool convert_to_rc(elem star[][12], RC rc, RC& rc1, int MX, int MY, bool boundary = 0)//将鼠标所在的坐标转换为行列形式,并判断是否为合法区域
{
	bool legal = false;
	if (!boundary) {
		for (int i = 1; i <= rc.row; i++)
			for (int j = 1; j <= rc.column; j++)
				if (4 + BLOCK_W * (j - 1) <= MX && MX <= 4 + BLOCK_W * j
					&& 3 + BLOCK_H * (i - 1) <= MY && MY <= 3 + BLOCK_H * i) {
					legal = true;
					rc1.row = i;
					rc1.column = j;
				}
	}
	else {
		for (int i = 1; i <= rc.row; i++)
			for (int j = 1; j <= rc.column; j++)
				if (5 + (2 + BLOCK_W) * (j - 1) <= MX && MX <= 5 + (2 + BLOCK_W) * (j - 1) + BLOCK_W
					&& 3 + (1 + BLOCK_H) * (i - 1) <= MY && MY <= 3 + (1 + BLOCK_H) * (i - 1) + BLOCK_H) {
					legal = true;
					rc1.row = i;
					rc1.column = j;
				}
	}

	if (star[rc1.row][rc1.column].key == 0)
		legal = false;
	return legal;
}
/********************************************函数04:功能:处理鼠标移动单击与方向键,实现选择色块高亮并返回色块行列值*************************************************************/
void check_arrow(elem star[][12], int keycode2, RC rc, RC& rc1) //根据键值2判断上下左右移动,边界返回
{
	//已经保证rc1的值非0
	int i = 1;
	switch (keycode2) {
		case KB_ARROW_UP://向上
			if (star[rc1.row - 1][rc1.column].key == 0) //如果上面的元素为0,返回最底部
				rc1.row = rc.row;
			else
				rc1.row--;
			break;

		case KB_ARROW_DOWN://向下
			if (rc1.row == rc.row) {
				for (int i = 1; i <= rc.row; i++)
					if (star[i][rc1.column].key != 0) {
						rc1.row = i;
						break;
					}
			}//已经在最底部,则找到该列最顶部的非零元
			else
				rc1.row++;
			break;

		case KB_ARROW_LEFT://向左
			while (star[rc1.row][(rc1.column - 1 + rc.column - i) % rc.column + 1].key == 0)
				i++;
			rc1.column = (rc1.column - 1 + rc.column - i) % rc.column + 1;
			break;

		case KB_ARROW_RIGHT://向右
			while (star[rc1.row][(rc1.column - 1 + rc.column + i) % rc.column + 1].key == 0)
				i++;
			rc1.column = (rc1.column - 1 + rc.column + i) % rc.column + 1;
			break;

	}
}
void Mouse_and_Key(RC rc, elem star[][12], RC& rc1, bool boundary)//rc1记录高亮目标块
{
	//boundary=0无分界线情况,=1有分界线情况
	int pos_y;
	void (*drawblk)(RC, elem[][12], RC, int);//指向函数的指针
	if (!boundary) {
		pos_y = 4 + BLOCK_H * rc.row;
		drawblk = draw_block;
	}
	else {
		pos_y = 4 + (1 + BLOCK_H) * rc.row;
		drawblk = draw_blockplus;
	}
	drawblk(rc, star, { rc1.row,rc1.column }, 1);//待修改,因为查找出错后不会退回1,1
	int MX, MY, MAction, keycode1, keycode2;
	RC odd_rc1 = rc1;
	cct_enable_mouse();
	while (1) {
		//键盘事件
		if (cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2) == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 0xe0) {//方向键
				check_arrow(star, keycode2, rc, rc1);
				drawblk(rc, star, { rc1.row,rc1.column }, 1);
				cct_gotoxy(0, pos_y);
				cout << "[当前键盘]" << (char)(rc1.row - 1 + 'A') << "行" << rc1.column - 1 << "列" << "                              ";
			}
			if (keycode1 == 0x0D) //回车
				return;

		}

		//鼠标事件
		else {
			if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
				if (convert_to_rc(star, rc, rc1, MX, MY, boundary) == true)
					return;

			}
			if (MAction == MOUSE_ONLY_MOVED) {
				bool legal = convert_to_rc(star, rc, rc1, MX, MY, boundary);
				if (odd_rc1.row != rc1.row || odd_rc1.column != rc1.column) {//防止微小位移造成的抖动
					if (!legal) {
						cct_gotoxy(0, pos_y);
						cout << "[当前鼠标]位置非法                               ";
					}
					else {
						drawblk(rc, star, { rc1.row,rc1.column }, 1);
						cct_gotoxy(0, pos_y);
						cout << "[当前鼠标]" << (char)(rc1.row - 1 + 'A') << "行" << rc1.column - 1 << "列" << "                              ";
					}
				}
			}


		}
		odd_rc1 = rc1;
	}
}

/******************************函数:功能:检测鼠标左击or回车后消除,移开区域后取消高亮***********************************************/
bool check_to_clear(RC rc, elem star[][12], RC& rc1, int& score)//此时区域已经确定合法
{//返回一个bool值,区分是否消除,从而决定是否调用下落
	RC odd_rc1 = rc1;
	while (1) {
		int MX, MY, MAction, keycode1, keycode2;
		//键盘事件
		if (cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2) == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 13) {//键盘按下回车
				int cnt = 0, * pcnt = &cnt;
				search_sur(star, rc1.row, rc1.column, pcnt, 2);
				score += BONUS * cnt;
				traverse_array(rc, star, "clear");
				draw_block(rc, star);
				cct_gotoxy(0, 0);
				cout << "本次得分:" << BONUS * cnt << "      总得分:" << score << "      ";
				traverse_array(rc, star, "resetstate");
				return true;
			}
			else if (keycode1 == 0xe0) {//键盘按下方向键
				traverse_array(rc, star, "resetstate");
				check_arrow(star, keycode2, rc, rc1);//否则按下方向键并不会移动,只会取消选中,第二次才移动
				return false;
			}
		}
		//鼠标事件
		else {//只有非抖动移动和单击可以退出循环
			if (MAction == MOUSE_LEFT_BUTTON_CLICK) {//鼠标按下左键
				int cnt = 0, * pcnt = &cnt;
				search_sur(star, rc1.row, rc1.column, pcnt, 2);
				score += BONUS * cnt;
				traverse_array(rc, star, "clear");
				draw_block(rc, star);
				cct_gotoxy(0, 0);
				cout << "本次得分:" << BONUS * cnt << "      总得分:" << score << "      ";
				traverse_array(rc, star, "resetstate");
				return true;
			}
			if (MAction == MOUSE_ONLY_MOVED) {
				bool legal = convert_to_rc(star, rc, rc1, MX, MY);
				if (odd_rc1.row != rc1.row || odd_rc1.column != rc1.column) {//鼠标移动非抖动
					traverse_array(rc, star, "resetstate");
					return false;
				}

			}
		}
	}
}

/*******************************函数:功能:实现确认消除后的消除与下落左移等工作************************************************************/
void delete_sur(elem star[][12], RC rc1, int key)//非图形界面强行移植会使代码有些丑陋,这里重写一个,表示在已经确定要消除后,将rc1周围元素指令并标记等待下落
{
	if (star[rc1.row][rc1.column].key != key || star[rc1.row][rc1.column].state == -1)
		return;
	else {
		star[rc1.row][rc1.column].key = 0;
		star[rc1.row][rc1.column].state = -1;//表示已经遍历过
		delete_sur(star, { rc1.row - 1,rc1.column }, key);
		delete_sur(star, { rc1.row + 1,rc1.column }, key);
		delete_sur(star, { rc1.row ,rc1.column - 1 }, key);
		delete_sur(star, { rc1.row ,rc1.column + 1 }, key);
	}
}
void find_pos(elem star[][12], RC rc, int col, int oddrow, int& dstrow) {
	for (int i = rc.row; i > oddrow; i--)
		if (star[i][col].key == 0)
		{
			dstrow = i;//在本列第i行找到了第一个空位
			return;
		}
}
void dynamic_exchange(elem star[][12], RC rc, int col, int oddrow, int dstrow)//实现star[oddrow][col]和star[dstrow][col]的动态交换
{
	for (int i = oddrow; i < dstrow; i++) {
		int temp = star[i][col].key;
		star[i][col].key = star[i + 1][col].key;
		star[i + 1][col].key = temp;
		draw_block(rc, star);
		Sleep(30);
	}
}
void fall_graph(elem star[][12], RC rc)//图形界面下落,如何实现动画效果?需要考虑state=-1和key=0的区别吗?和状态无关
{
	//上下操作
	for (int j = 1; j <= rc.column; j++)
		for (int i = rc.row; i >= 1; i--) {
			if (star[i][j].key != 0) {
				int dstrow = i;
				find_pos(star, rc, j, i, dstrow);//第一步找到下方第一个零位置/没找到dstrow不变仍然通用
				if (i != dstrow)
					dynamic_exchange(star, rc, j, i, dstrow);
			}
		}
	//左右操作
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
						draw_block(rc, star);
						Sleep(10);
					}
				}
		}
	}
}

/*******************************函数:功能:左击or回车后下落**************************************************************************************/
void check_to_fall(RC rc, elem star[][12], RC& rc1) //这个不需要考虑抖动
{
	int MX, MY, MAction, keycode1, keycode2;
	int cnt = 0, * pcnt = &cnt;
	while (1) {
		cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);
		//键盘事件
		if (keycode1 == 13) {
			int key = star[rc1.row][rc1.column].key;
			delete_sur(star, rc1, key);
			traverse_array(rc, star, "resetstate");
			fall_graph(star, rc);
			return;
		}

		//鼠标事件
		if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
			int key = star[rc1.row][rc1.column].key;
			delete_sur(star, rc1, key);
			traverse_array(rc, star, "resetstate");
			fall_graph(star, rc);
			return;
		}
	}
}
/*********************************函数:功能:特殊应对一下F的处理流程*****************************************************/
void game_once(RC rc, elem star[][12])
{

	RC rc1 = { rc.row,1 };
	int pos_y = 4 + BLOCK_H * rc.row;
	int score = 0;
	int cnt = 0, * pcnt = &cnt;
	cct_gotoxy(0, pos_y);
	cout << "箭头键/鼠标移动,回车键/单击左键选择                ";
	while (1) {

		Mouse_and_Key(rc, star, rc1);//获得左键/回车的rc1
		search_sur(star, rc1.row, rc1.column, pcnt);//搜索rc1,判断是否连续
		if (*pcnt == 1) {
			star[rc1.row][rc1.column].state = 1;
			cct_gotoxy(0, pos_y);
			cout << "周围无相同值,箭头键/鼠标移动,回车键/单击左键合成                         ";
			continue;
		}
		else {//选中区域高亮
			cct_gotoxy(0, pos_y);
			cout << "箭头键/鼠标移动取消当前选择,回车键/单击左键合成                          ";
			draw_block(rc, star, rc1, 2);
			if (check_to_clear(rc, star, rc1, score) == true)//检测回车和单击后消去,以及移开区域后取消高亮
			{
				cct_gotoxy(0, pos_y);
				cout << "合成完成,回车键/单击左键下落                             ";
				check_to_fall(rc, star, rc1);
				return;
			}
			else
				continue;
		}
	}
}

void GAME(RC rc, elem star[][12], int& score)
{

	RC rc1 = { rc.row,1 };
	int pos_y = 4 + BLOCK_H * rc.row;
	int cnt = 0, * pcnt = &cnt;
	cct_gotoxy(0, pos_y);
	cout << "箭头键/鼠标移动,回车键/单击左键选择                ";
	while (1) {

		Mouse_and_Key(rc, star, rc1);//获得左键/回车的rc1
		search_sur(star, rc1.row, rc1.column, pcnt);//搜索rc1,判断是否连续
		if (*pcnt == 1) {
			star[rc1.row][rc1.column].state = 1;
			cct_gotoxy(0, pos_y);
			cout << "周围无相同值,箭头键/鼠标移动,回车键/单击左键合成                         ";
			continue;
		}
		else {//选中区域高亮
			cct_gotoxy(0, pos_y);
			cout << "箭头键/鼠标移动取消当前选择,回车键/单击左键合成                          ";
			draw_block(rc, star, rc1, 2);
			if (check_to_clear(rc, star, rc1, score) == true)//检测回车和单击后消去,以及移开区域后取消高亮
			{
				int key = star[rc1.row][rc1.column].key;
				delete_sur(star, rc1, key);
				traverse_array(rc, star, "resetstate");
				fall_graph(star, rc);
				return;
			}
			else
				continue;
		}
	}
}
