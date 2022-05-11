/*1852877 计科 赵昊堃*/
#include "90-02-b4.h"

void mode_char()
{
	/*需要用到的参数*/
	int row, col;//显示行数/每行汉字数 -- 由此计算屏幕大小
	int bgcolor, fgcolor;//背景色/前景色
	int size;//可选:char模式下字符大小;led模式下点的直径
	int sim;//0-繁体or1-简体
	char record[1024] = { 0 };//存放用户自定义字符串

	get_para(1, row, col, size, sim, record);
	get_color_char(bgcolor, fgcolor);

	cct_cls();
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("新宋体", 6 + 2 * (size - 1));//定义了一个换算标准:1档-6,2档-8.,三档-10
	cct_setconsoleborder(2 * (FONT_W * col + 2), FONT_W * row + 2);//为了美观多留出一点


	display_char(row, col, sim, record);

}