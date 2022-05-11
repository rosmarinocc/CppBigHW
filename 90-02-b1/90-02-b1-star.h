/*1852877 计科 赵昊堃*/
#pragma once
#define BONUS		10
#define BLOCK_W     6
#define BLOCK_H     3
struct RC {
	int row;
	int column;
};

struct elem {
	int key;
	int state;//state表示状态:1:原始状态,2:找到状态,3:消去状态:标记后等待执行key变为0的操作
};


void choose_menu(char ch);
int input_rc(char ch);
void funA(RC rc);
void funB(RC rc);
void funC(RC rc);
void funD(RC rc);
void funE(RC rc);
void funF(RC rc);
void funG(RC rc);
void init_array(RC rc, elem star[12][12]);
void show_array(RC rc, elem star[12][12], int state);
RC command_pos(RC rc, elem star[12][12]);
bool command_checksur(RC rc1, elem star[][12]);
void search_sur(elem star[][12], int row, int col, int* pcnt, int execute = 0);
void check_end();
int command_clear(RC rc, RC rc1, elem star[][12], int& score);//1表示正常运行,选择了Y/N,0代表选择了Q
bool check_gameover(RC rc, elem star[][12]);//gameover返回0代表未结束,返回1代表结束
int traverse_array(RC rc, elem star[][12], const char* s = "count");//clear:将查找后并Y确认后的元素置0;reset将所有state=3的元素置1;count:计算余下非零元素个数
void draw_border(RC rc);//画边框
void draw_borderplus(RC rc);//画边框(分界线)
void draw_block(RC rc, elem star[][12], RC rc1 = { 0,0 }, int highlight = 0);//画方块,不含分界线
void draw_blockplus(RC rc, elem star[][12], RC rc1 = { 0,0 }, int highlight = 0);//画方块,含分界线
void Mouse_and_Key(RC rc, elem star[][12],RC &rc1, bool boundary=0);
void game_once(RC rc, elem star[][12]);
bool is_allzero(elem star[][12], int row, int col_no);
void GAME(RC rc, elem star[][12],int &score);