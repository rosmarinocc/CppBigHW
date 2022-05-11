/*1852877 计科 赵昊堃*/
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iostream>
#include <windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;
#define FONT_W 16

static void draw_led(const int start_x, const int start_y, const int pt_size, const int cnt, const char* record, FILE* f, const int RGB_value, bool colorchange = false, bool delay = true, bool reverse = false)
{
	char temp[2];
	char matrix[32];
	memcpy(temp, record + 2 * cnt, 2 * sizeof(char));

	int area = (temp[0] & 255) - 0xa0;
	int location = (temp[1] & 255) - 0xa0;
	int offset = ((area - 1) * 94 + (location - 1)) * 32;

	fseek(f, offset, SEEK_SET);
	fread(matrix, 1, 32, f);

	int xt, yt;
	for (int i = 0; i < 32; i++) {
		for (int j = 7; j >= 0; j--) {
			if (!reverse) {
				if (matrix[i] & int(pow(2, j))) {
					xt = (i % 2 == 0) ? start_x + (7 - j) * pt_size : start_x + (7 + 8 - j) * pt_size;
					yt = start_y + (i / 2) * pt_size;
					if (!colorchange)
						hdc_point(xt, yt, pt_size, RGB_value);
					else
						hdc_point(xt, yt, pt_size, RGB_value + 10 * i * j);
					if (delay)
						Sleep(10);
				}
			}
			else {
				if (!(matrix[i] & int(pow(2, j)))) {
					xt = (i % 2 == 0) ? start_x + (7 - j) * pt_size : start_x + (7 + 8 - j) * pt_size;
					yt = start_y + (i / 2) * pt_size;
					if (!colorchange)
						hdc_point(xt, yt, pt_size, RGB_value);
					else
						hdc_point(xt, yt, pt_size, RGB_value + 10 * i * j);
					if (delay)
						Sleep(10);
				}
			}
		}

	}
}

static int check_halfchar(const char* record)
{
	int len_s = strlen(record);

	for (int i = 0; i < len_s; i += 2)
		if (len_s % 2 != 0 || record[i] > 0) {
			return 0;//存在半角
		}
	return 1;//不存在
}

void no_special(const int row, const int col, const int pt_size, const int len_s, const char* record, FILE* f, const int RGB_value)
{
	hdc_cls();
	int cnt = 0;
	for (int r = 0; r < row; r++)
		for (int c = 0; c < col; c++) {
			draw_led(pt_size / 2 + pt_size * FONT_W * c, pt_size / 2 + pt_size * FONT_W * r, pt_size, cnt, record, f, RGB_value);
			cnt++;
			if (cnt == len_s / 2) {
				Sleep(1200);
				return;

			}
		}

}

/******************特效01:彩色霓虹**********************/
void special_colorful(const int row, const int col, const int pt_size, const int len_s, const char* record, FILE* f)
{
	hdc_cls();
	int cnt = 0;
	for (int r = 0; r < row; r++)
		for (int c = 0; c < col; c++) {
			int RGB_value = RGB(((r + c) * 40 + 100) % 256, (r * c * 30 + 50) % 256, ((r + c) * 80 + 10) % 256);
			draw_led(pt_size / 2 + pt_size * FONT_W * c, pt_size / 2 + pt_size * FONT_W * r, pt_size, cnt, record, f, RGB_value);
			cnt++;
			if (cnt == len_s / 2) {
				Sleep(1200);
				return;
			}
		}
}

/******************特效02:百叶窗**********************/
void draw_window(const int r, const int col, const int start_x, const int start_y, const int pt_size, const int len_s, int& cnt, const char* record, FILE* f, const int RGB_value)
{
	char temp[14][2];//最大列数为14,如此可以存放一行的中文字符
	char matrix[14][32] = { 0 };//最大列数为14,如此可以存放一行的数字矩阵
	for (int i = 0; i < col; i++)
		memcpy(temp[i], record + 2 * (r * col + i), 2 * sizeof(char));
	for (int i = 0; i < col; i++) {
		int area = (temp[i][0] & 255) - 0xa0;
		int location = (temp[i][1] & 255) - 0xa0;
		int offset = ((area - 1) * 94 + (location - 1)) * 32;
		fseek(f, offset, SEEK_SET);
		fread(matrix[i], 1, 32, f);
		cnt++;
		if (cnt == len_s / 2)
			break;
	}

	int xt, yt;
	for (int i = 0; i < 32; i++) {
		for (int j = 7; j >= 0; j--) {
			for (int c = 0; c < col; c++) {
				if (matrix[c][i] != 0 && matrix[c][i] & int(pow(2, j))) {
					int start_xc = start_x + c * FONT_W * pt_size;
					xt = (i % 2 == 0) ? start_xc + (7 - j) * pt_size : start_xc + (7 + 8 - j) * pt_size;
					yt = start_y + (i / 2) * pt_size;
					hdc_point(xt, yt, pt_size, RGB_value);
				}

			}

		}
		if (i % 2 == 1)
			Sleep(50);
	}
}

void special_window(const int row, const int col, const int pt_size, const int len_s, const char* record, FILE* f, const int RGB_value)
{
	hdc_cls();
	int cnt = 0;
	for (int r = 0; r < row; r++) {//一次显示一行文字
		draw_window(r, col, pt_size / 2, pt_size / 2 + pt_size * FONT_W * r, pt_size, len_s, cnt, record, f, RGB_value);
		if (cnt == len_s / 2) {
			Sleep(1200);
			return;
		}
	}
}
/******************特效03:闪烁字符**********************/
void special_shine(const int row, const int col, const int pt_size, const int len_s, const char* record, FILE* f, const int RGB_value)
{
	cout << "特效03:闪烁字符" << endl;
	int cnt = 0;
	for (int r = 0; r < row; r++)
		for (int c = 0; c < col; c++) {
			draw_led(pt_size / 2 + pt_size * FONT_W * c, pt_size / 2 + pt_size * FONT_W * r, pt_size, cnt, record, f, RGB_value, false, false, true);
			for (int i = 0; i < 8; i++)
				draw_led(pt_size / 2 + pt_size * FONT_W * c, pt_size / 2 + pt_size * FONT_W * r, pt_size, cnt, record, f, RGB(255 * (i & 1), 255 * (i & 2), 255 * (i & 4)), false, false, false);
			cnt++;
			if (cnt == len_s / 2) {
				Sleep(1200);
				return;
			}
		}

}

void display_led(const int row, const int col, const int pt_size, const int sim, const char* record, const int RGB_value, const int special_type)
{

	hdc_cls();
	int len_s = strlen(record);
	if (check_halfchar(record) == 0) {
		cout << "存在非法半角字符" << endl;
		exit(-1);
	}
	/*打开文件字符库*/
	char path[20] = { 0 };
	if (sim == 1)
		strcpy(path, "./HZK16");
	else
		strcpy(path, "./HZK16F");

	FILE* f = fopen(path, "rb");
	if (f == NULL) {
		cout << "文件打开失败";
		exit(-1);
	}
	if (special_type == 1)
		special_colorful(row, col, pt_size, len_s, record, f);
	else if (special_type == 2)
		special_window(row, col, pt_size, len_s, record, f, RGB_value);
	else if (special_type == 3)
		special_shine(row, col, pt_size, len_s, record, f, RGB_value);
	else
		no_special(row, col, pt_size, len_s, record, f, RGB_value);

	fclose(f);
}