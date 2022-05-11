/*1852877 计科 赵昊堃*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
using namespace std;

#include "../include/cmd_hdc_tools.h"


#define BLACK RGB(0,0,0)
#define WHITE RGB(255,255,255)
static const double PI = 3.14159;
//背景800*800
static void hdc_draw_background(const int base_x, const int base_y)
{
	hdc_rectangle(0, 0, 2 * base_x, 2 * base_y, 0, 1, 3, RGB(243, 180, 145));
	hdc_square(base_x, base_y / 2, base_x, 45, true, 2, RGB(104, 64, 56));
	hdc_square(base_x, base_y / 8, base_x / 2, 45, true, 2, RGB(144, 219, 53));
	hdc_square(base_x / 2, base_y / 4, base_x / 2, 45, true, 2, RGB(247, 239, 75));
	hdc_square(base_x * 3 / 2, base_y / 4, base_x / 2, 45, true, 2, RGB(231, 131, 41));
}

//多来A梦的大头
static void hdc_draw_head(const int base_x, const int base_y)
{
	//脸
	hdc_ellipse(base_x, base_y - 110, 142, 132, 0, true, 2, RGB(0, 173, 239));
	hdc_ellipse(base_x, base_y - 110, 142, 132, 0, false, 3, BLACK);
	//脸白
	hdc_ellipse(base_x, base_y - 110, 129, 71, 0, true, 2, WHITE);

	hdc_sector(base_x, base_y - 130, 135, 120, 240, true, 2, WHITE);
	hdc_arc(base_x, base_y - 130, 135, 120, 240, 3, BLACK);

	hdc_sector(base_x, base_y - 125, 131, 230, 263, true, 2, WHITE);
	hdc_sector(base_x, base_y - 125, 131, 97, 130, true, 2, WHITE);
	hdc_arc(base_x, base_y - 125, 132, 230, 263, 3, BLACK);
	hdc_arc(base_x, base_y - 125, 132, 97, 130, 3, BLACK);

	hdc_arc(base_x, base_y - 20, 168, -48, 48, 3, BLACK);

	hdc_arc(base_x, base_y - 75, 136, 284, 296, 3, BLACK);
	hdc_arc(base_x, base_y - 75, 136, 64, 76, 3, BLACK);


	//胡子1
	hdc_line(base_x - 30, base_y - 119, base_x - 105, base_y - 140, 3, BLACK);
	hdc_line(base_x + 30, base_y - 119, base_x + 105, base_y - 140, 3, BLACK);
	//胡子2
	hdc_line(base_x - 30, base_y - 104, base_x - 115, base_y - 107, 3, BLACK);
	hdc_line(base_x + 30, base_y - 104, base_x + 115, base_y - 107, 3, BLACK);
	//胡子3
	hdc_line(base_x - 30, base_y - 87, base_x - 113, base_y - 77, 3, BLACK);
	hdc_line(base_x + 30, base_y - 87, base_x + 113, base_y - 77, 3, BLACK);
	//舌头
	hdc_sector(base_x + 80, base_y - 85, 30, 190, 90, true, 2, RGB(246, 122, 17));
	hdc_sector(base_x + 80, base_y - 85, 30, 190, 90, false, 2, BLACK);
	int point_x = int(base_x + 80 - 30 * sin(10 * PI / 180));
	int point_y = int(base_y - 85 + 30 * cos(10 * PI / 180));
	hdc_triangle(base_x + 80, base_y - 85, base_x + 110, base_y - 85, point_x, point_y, true, 2, RGB(246, 122, 17));
	hdc_arc(base_x - 50, base_y - 45, 140, 68, 80, 3, BLACK);

	//嘴巴
	hdc_line(base_x, base_y - 127, base_x, base_y - 35, 3, BLACK);
	hdc_arc(base_x, base_y - 175, 140, 180, 235, 3, BLACK);
	hdc_arc(base_x, base_y - 175, 140, 125, 180, 3, BLACK);

	//两只眼睛
	hdc_ellipse(base_x - 36, base_y - 190, 36, 42, 0, true, 2, WHITE);
	hdc_ellipse(base_x + 36, base_y - 190, 36, 42, 0, true, 2, WHITE);
	hdc_ellipse(base_x - 36, base_y - 190, 36, 42, 0, false, 3, BLACK);
	hdc_ellipse(base_x + 36, base_y - 190, 36, 42, 0, false, 3, BLACK);
	//眼球
	hdc_ellipse(base_x - 20, base_y - 190, 10, 15, 0, true, 3, BLACK);
	hdc_ellipse(base_x + 20, base_y - 190, 10, 15, 0, true, 3, BLACK);
	//眼球高光
	hdc_circle(base_x - 15, base_y - 190, 2, true, 2, WHITE);
	hdc_circle(base_x + 15, base_y - 190, 2, true, 2, WHITE);
	//鼻子
	hdc_circle(base_x, base_y - 150, 23, true, 2, RGB(217, 2, 41));
	hdc_circle(base_x, base_y - 150, 23, false, 2, BLACK);
	hdc_circle(base_x - 5, base_y - 155, 6, true, 2, WHITE);
}

static void hdc_draw_necklace(const int base_x, const int base_y)
{
	//项圈
	hdc_rectangle(base_x - 100, base_y, 200, 20, 0, true, 2, RGB(221, 4, 43));
	hdc_line(base_x - 100, base_y, base_x + 100, base_y, 3, BLACK);
	hdc_line(base_x - 100, base_y + 20, base_x + 100, base_y + 20, 3, BLACK);
	hdc_sector(base_x - 95, base_y + 10, 10, 190, 360, true, 3, RGB(221, 4, 43));
	hdc_arc(base_x - 95, base_y + 10, 10, 190, 360, 3, BLACK);
	hdc_sector(base_x + 95, base_y + 10, 10, 0, 190, true, 3, RGB(221, 4, 43));
	hdc_arc(base_x + 95, base_y + 10, 10, 0, 190, 3, BLACK);
	//铃铛
	hdc_circle(base_x, base_y + 35, 30, true, 2, RGB(249, 228, 64));
	hdc_circle(base_x, base_y + 35, 30, false, 3, BLACK);

	hdc_line(base_x - 32, base_y + 24, base_x + 32, base_y + 24, 2, RGB(176, 145, 84));
	hdc_line(base_x - 32, base_y + 28, base_x + 32, base_y + 28, 2, RGB(176, 145, 84));
	hdc_circle(base_x, base_y + 41, 6, true, 2, RGB(104, 99, 45));
	hdc_circle(base_x, base_y + 41, 6, false, 2, RGB(176, 145, 84));

	hdc_line(base_x - 2, base_y + 47, base_x - 2, base_y + 64, 2, RGB(176, 145, 84));
	hdc_line(base_x + 2, base_y + 47, base_x + 2, base_y + 64, 2, RGB(176, 145, 84));

}

static void hdc_draw_body(const int base_x, const int base_y)
{
	//躯干
	hdc_rectangle(base_x - 95, base_y + 5, 190, 160, 0, true, 2, RGB(0, 173, 239));
	hdc_line(base_x - 95, base_y + 165, base_x + 95, base_y + 165, 3, BLACK);
	hdc_triangle(base_x - 95, base_y + 15, base_x - 95, base_y + 165, base_x - 120, base_y + 15, true, 2, RGB(0, 173, 239));
	hdc_triangle(base_x + 95, base_y + 15, base_x + 95, base_y + 165, base_x + 110, base_y + 15, true, 2, RGB(0, 173, 239));
	hdc_arc(base_x + 840, base_y + 30, 950, 262, 270, 3, BLACK);
	hdc_arc(base_x - 843, base_y + 30, 950, 89, 98, 3, BLACK);

	//肚皮
	hdc_ellipse(base_x, base_y + 75, 82, 68, 0, false, 4, BLACK);
	hdc_ellipse(base_x, base_y + 75, 80, 65, 0, true, 2, WHITE);

	//左手
	hdc_arc(base_x + 90, base_y + 75, 62, 270, 360, 4, BLACK);
	hdc_sector(base_x + 90, base_y + 75, 60, 270, 360, true, 2, RGB(0, 173, 239));
	hdc_arc(base_x + 45, base_y + 45, 50, 90, 130, 3, BLACK);

	//口袋
	hdc_arc(base_x, base_y + 65, 60, 100, 260, 3, BLACK);
	hdc_line(base_x - 60, base_y + 75, base_x + 60, base_y + 75, 3, BLACK);

	//右手
	hdc_sector(base_x - 40, base_y - 65, 120, 215, 250, true, 2, RGB(0, 173, 239));
	hdc_arc(base_x - 40, base_y - 65, 120, 215, 250, 3, BLACK);

	hdc_circle(base_x - 150, base_y - 50, 35, true, 2, WHITE);
	hdc_circle(base_x - 150, base_y - 50, 35, false, 3, BLACK);

	//裤缝
	hdc_line(base_x - 10, base_y + 152, base_x + 10, base_y + 152, 3, BLACK);
	hdc_line(base_x, base_y + 152, base_x, base_y + 165, 3, BLACK);

}

static void hdc_draw_feet(const int base_x, const int base_y)
{
	hdc_ellipse(base_x + 52, base_y + 175, 76, 25, 0, true, 2, WHITE);
	hdc_ellipse(base_x + 52, base_y + 175, 76, 25, 0, false, 3, BLACK);
	hdc_ellipse(base_x - 52, base_y + 175, 76, 25, 0, true, 2, WHITE);
	hdc_ellipse(base_x - 52, base_y + 175, 76, 25, 0, false, 3, BLACK);
	hdc_rectangle(base_x - 30, base_y + 160, 60, 30, 0, true, 2, WHITE);
	hdc_line(base_x, base_y + 160, base_x, base_y + 195, 3, BLACK);
}


void hdc_draw_catoon_1852877(const int base_x, const int base_y)
{
	hdc_cls();
	hdc_draw_background(base_x, base_y);
	hdc_draw_feet(base_x, base_y);
	hdc_draw_body(base_x, base_y);
	hdc_draw_head(base_x, base_y);

	hdc_draw_necklace(base_x, base_y);


}