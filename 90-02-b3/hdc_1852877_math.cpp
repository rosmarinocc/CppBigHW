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

static const double PI = 3.14159;


void hdc_draw_Mathematical_curve_1852877()
{
	//花瓣线
	hdc_cls();
	int base_x = 450, base_y = 450;
	for (int level = 100; level <= 400; level += 10) {
		int odd_x = base_x + level - 1;
		int odd_y = base_y;
		for (int angle = 1; angle <= 360; angle++) {
			double r = level - pow(level, sin(5 * angle * PI / 180));
			int x = int(base_x + r * cos(angle * PI / 180));
			int y = int(base_y + r * sin(angle * PI / 180));
			hdc_line(odd_x, odd_y, x, y, 3, RGB(angle % 256, 127, 214));
			odd_x = x;
			odd_y = y;
		}
	}
}
