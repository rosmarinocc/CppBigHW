/*1852877 �ƿ� ��껈�*/
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
	int state;//state��ʾ״̬:1:ԭʼ״̬,2:�ҵ�״̬,3:��ȥ״̬:��Ǻ�ȴ�ִ��key��Ϊ0�Ĳ���
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
int command_clear(RC rc, RC rc1, elem star[][12], int& score);//1��ʾ��������,ѡ����Y/N,0����ѡ����Q
bool check_gameover(RC rc, elem star[][12]);//gameover����0����δ����,����1�������
int traverse_array(RC rc, elem star[][12], const char* s = "count");//clear:�����Һ�Yȷ�Ϻ��Ԫ����0;reset������state=3��Ԫ����1;count:�������·���Ԫ�ظ���
void draw_border(RC rc);//���߿�
void draw_borderplus(RC rc);//���߿�(�ֽ���)
void draw_block(RC rc, elem star[][12], RC rc1 = { 0,0 }, int highlight = 0);//������,�����ֽ���
void draw_blockplus(RC rc, elem star[][12], RC rc1 = { 0,0 }, int highlight = 0);//������,���ֽ���
void Mouse_and_Key(RC rc, elem star[][12],RC &rc1, bool boundary=0);
void game_once(RC rc, elem star[][12]);
bool is_allzero(elem star[][12], int row, int col_no);
void GAME(RC rc, elem star[][12],int &score);