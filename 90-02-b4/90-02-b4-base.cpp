/*1852877 计科 赵昊堃*/
#pragma once
#include "90-02-b4.h"


/********************公共部分**********************/
//初始化
void init_para()
{
	cct_setcolor();
	cct_setfontsize("新宋体", 12);
	cct_setconsoleborder(100, 40);

}

//通用参数设置
void get_para(const int mode, int& row, int& col, int& size, int& sim, char* record)
{
	cct_cls();

	if (mode == 1)
		get_dig_para("请选择字符模式下字体的大小档(1-3),范围不合理修正为2 :", 1, 3, size, true, 2);//6,8,10

	else
		get_dig_para("请选择LED模式下点的直径档(1-3),范围不合理修正为2 :", 1, 3, size, true, 2);// 20 30 40 px


	if (size == 1) {
		get_dig_para("请输入显示的行数(3-9),范围不合理修正为3 :", 3, 9, row, true, 3);
		get_dig_para("请输入每行显示的字数(6-14),范围不合理修正为6 :", 6, 14, col, true, 6);
	}
	else if (size == 2) {
		get_dig_para("请输入显示的行数(3-6),范围不合理修正为3 :", 3, 6, row, true, 3);
		get_dig_para("请输入每行显示的字数(6-10),范围不合理修正为6 :", 6, 10, col, true, 6);
	}
	else if (size == 3) {
		get_dig_para("请输入显示的行数(3-5),范围不合理修正为3 :", 3, 5, row, true, 3);
		get_dig_para("请输入每行显示的字数(6-8),范围不合理修正为6 :", 6, 8, col, true, 6);
	}

	get_dig_para("请选择1-简体,0-繁体:", 0, 1, sim, true, 1);

	cout << "请输入需要显示的文本内容(全角字符) :" << endl;
	cin >> record;

}

//char模式颜色设置
void get_color_char(int& bgcolor, int& fgcolor)
{
	cct_cls();
	cout << "颜色对应表为 :" << endl;
	cout << "0-黑,1-蓝,2-绿,3-青,4-红,5-粉,6-黄,7-白" << endl;
	cout << "8-亮黑,9-亮蓝,10-亮绿,11-亮青,12-亮红,13-亮粉,14-亮黄,15-亮白" << endl;
	putchar('\n');
	get_dig_para("请选择背景颜色(0-15),范围不合理修正为0-黑 :", 0, 15, bgcolor, true, 0);
	get_dig_para("请选择前景颜色(0-15),范围不合理修正为7-白 :", 0, 15, fgcolor, true, 7);
}

//led模式颜色设置:不知道是不是前景背景都要rgb,先不写了
void get_color_led(int& bg_color, int& fg_red, int& fg_green, int& fg_blue)
{
	cct_cls();
	cout << "颜色对应表为 :" << endl;
	cout << "0-黑,1-蓝,2-绿,3-青,4-红,5-粉,6-黄,7-白" << endl;
	cout << "8-亮黑,9-亮蓝,10-亮绿,11-亮青,12-亮红,13-亮粉,14-亮黄,15-亮白" << endl;
	putchar('\n');
	get_dig_para("请选择背景颜色(0-15),范围不合理修正为0-黑 :", 0, 15, bg_color, true, 0);

	cout << "常用颜色RGB值举例 :" << endl;
	cout << "红-(255,0,0),绿-(0,255,0),蓝-(0,0,255),黑-(0,0,0),白(255,255,255)" << endl;
	putchar('\n');
	cout << "请输入前景颜色的RGB值:" << endl;
	get_dig_para("请选择R分量(0-255):", 0, 255, fg_red);
	get_dig_para("请选择G分量(0-255):", 0, 255, fg_green);
	get_dig_para("请选择B分量(0-255):", 0, 255, fg_blue);
}


/********************cha-mode部分**********************/
void draw_font(const int start_x, const int start_y, const int cnt, const char* record, FILE* f)
{
	cct_gotoxy(start_x, start_y);
	char temp[2];
	char matrix[32];
	memcpy(temp, record + 2 * cnt, 2 * sizeof(char));

	int area = (temp[0] & 255) - 0xa0;
	int location = (temp[1] & 255) - 0xa0;
	int offset = ((area - 1) * 94 + (location - 1)) * 32;

	fseek(f, offset, SEEK_SET);
	fread(matrix, 1, 32, f);

	for (int i = 0; i < 32; i++) {
		for (int j = 7; j >= 0; j--) {
			if (matrix[i] & int(pow(2, j)))
				cout << "* ";
			else
				cout << "  ";
		}
		if (i % 2 == 1)
			cct_gotoxy(start_x, start_y + i / 2 + 1);
	}

}

void display_char(const int row, const int col, const int sim, const char* record)
{
	int len_s = strlen(record);

	for (int i = 0; i < len_s; i += 2)
		if (len_s % 2 != 0 || record[i] > 0) {
			cout << "检查出错!存在半角字符" << endl;
			wait_for_enter();
			return;
		}

	if (len_s / 2 > row * col) {
		len_s = 2 * row * col;
		cout << "输入内容长于所设最大显示数" << row * col << ",已截短" << endl;
		wait_for_enter();

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
		wait_for_enter();
		return;
	}

	int cnt = 0;
	for (int r = 0; r < row; r++)
		for (int c = 0; c < col; c++) {
			draw_font(2 * FONT_W * c, FONT_W * r, cnt, record, f);
			cnt++;
			if (cnt == len_s / 2) {

				wait_for_enter();
				return;
			}

		}

	fclose(f);
}

/********************led-mode部分**********************/
void draw_led(const int start_x, const int start_y, const int pt_size, const int cnt, const char* record, FILE* f, const int RGB_value, bool colorchange = false, bool delay = true, bool reverse = false)
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


/******************无特效:demo展示**********************/

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
	cout << "特效01:彩色霓虹灯" << endl;
	wait_for_enter();
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
	cout << "特效02:百叶窗" << endl;
	wait_for_enter();
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
	wait_for_enter();
	hdc_cls();
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
/***********************************************************/
void display_led(const int row, const int col, const int pt_size, const int sim, const char* record, const int RGB_value)
{

	int len_s = strlen(record);

	for (int i = 0; i < len_s; i += 2)
		if (len_s % 2 != 0 || record[i] > 0) {
			cout << "检查出错!存在半角字符" << endl;
			wait_for_enter();
			return;
		}

	if (len_s / 2 > row * col) {
		len_s = 2 * row * col;
		cout << "输入内容长于所设最大显示数" << row * col << ",已截短" << endl;
		wait_for_enter();

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
		wait_for_enter();
		return;
	}

	no_special(row, col, pt_size, len_s, record, f, RGB_value);

	cout << "下面进行特效展示,\"霓虹灯\",\"百叶窗\",\"雷霆嘎巴\",特效演示完毕后按回车继续" << endl;
	special_colorful(row, col, pt_size, len_s, record, f);
	special_window(row, col, pt_size, len_s, record, f, RGB_value);
	special_shine(row, col, pt_size, len_s, record, f, RGB_value);

	cout << "特效演示完毕" << endl;
	wait_for_enter();
	fclose(f);
}