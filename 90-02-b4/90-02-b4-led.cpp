/*1852877 计科 赵昊堃*/
#include "90-02-b4.h"


void mode_led()
{
	/*需要用到的参数*/
	int row, col;//显示行数/每行汉字数 -- 由此计算屏幕大小
	//背景色/前景色
	int bg_color;
	int fg_red, fg_green, fg_blue;
	int size;//可选:char模式下字符大小;led模式下点的直径
	int sim;//繁体or简体
	char record[1024] = { 0 };//存放用户自定义字符串

	get_para(2, row, col, size, sim, record);
	get_color_led(bg_color, fg_red, fg_green, fg_blue);


	int pt_size = 6 + 2 * (size - 1);//点的直径:1-6px,2-8px,3-10px
	int win_width = col * FONT_W * pt_size + 10;//预留一点空间
	int win_height = row * FONT_W * pt_size + 10;

	hdc_cls();
	hdc_init(bg_color, COLOR_WHITE, win_width, win_height);
	display_led(row, col, pt_size, sim, record, RGB(fg_red, fg_green, fg_blue));

}