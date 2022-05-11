/*1852877 �ƿ� ��껈�*/
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <cstring>
#include <stdio.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_hdc_tools.h"
#include "../include/common_menu.h"

using namespace std;


typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef DWORD   COLORREF;
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))


//����ͼ������߳�Ϊ16*16
const int FONT_W = 16;

//�����ʼ����������/��ɫ/��С��Ϣ
void init_para();

//led��ʾ������
void mode_led();
//�ַ���ʾ������
void mode_char();
//��ȡ��������
void get_para(const int mode, int& row, int& col, int& size, int& sim, char* record);
//mode1--char--�ַ���С;mode2--led--pointֱ��;sim:1-����,0-����

//��ȡ��ɫ����
void get_color_char(int& bgcolor, int& fgcolor);
void get_color_led(int& bg_color, int& fg_red, int& fg_green, int& fg_blue);

//����:�ַ�ģʽ��ͨ���������չʾ
void display_char(const int row, const int col, const int sim, const char* record);
//����:ledģʽ��ͨ���������չʾ
void display_led(const int row, const int col, const int pt_size, const int sim, const char* record, const int RGB_value);
//����:���Ը���x,y����Ϊ��ʼλ�ô�,����һ��record[cnt*2]��������
//void draw_font(const int start_x, const int start_y, const int cnt, const char* record, FILE* f);
//void draw_led(const int start_x, const int start_y, const int pt_size, const int cnt, const char* record, FILE* f, const int RGB_value);