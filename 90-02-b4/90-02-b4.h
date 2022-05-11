/*1852877 计科 赵昊堃*/
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


//定义图形字体边长为16*16
const int FONT_W = 16;

//定义初始化界面字体/颜色/大小信息
void init_para();

//led显示主函数
void mode_led();
//字符显示主函数
void mode_char();
//获取公共参数
void get_para(const int mode, int& row, int& col, int& size, int& sim, char* record);
//mode1--char--字符大小;mode2--led--point直径;sim:1-简体,0-繁体

//获取颜色参数
void get_color_char(int& bgcolor, int& fgcolor);
void get_color_led(int& bg_color, int& fg_red, int& fg_green, int& fg_blue);

//功能:字符模式下通过输入参数展示
void display_char(const int row, const int col, const int sim, const char* record);
//功能:led模式下通过输入参数展示
void display_led(const int row, const int col, const int pt_size, const int sim, const char* record, const int RGB_value);
//功能:在以给定x,y坐标为起始位置处,画出一个record[cnt*2]所代表汉字
//void draw_font(const int start_x, const int start_y, const int cnt, const char* record, FILE* f);
//void draw_led(const int start_x, const int start_y, const int pt_size, const int cnt, const char* record, FILE* f, const int RGB_value);