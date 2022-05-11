/*1852877 计科 赵昊堃*/
#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include "../include/cfg_file_tools.h"
#include "../include/cmd_console_tools.h"
#include "../include/cmd_hdc_tools.h"
#include "../include/common_menu.h"
#include "../include/common_ledtools.h"
using namespace std;
#define FONT_W 16

struct item_info {
	int item_existed = 0;//是否存在这条
	int item_color;//颜色
	string item_content;//内容
};

/************************函数01:初始化控制台界面********************************/
static void init_console()
{
	cct_setcolor();
	cct_cls();
	cct_setconsoleborder(120, 50);
	cct_setfontsize("点阵字体", 16, 8);
}
/********************************************************/
/************************函数02:FFFFFF颜色串相关函数********************************/
static bool check_legal_color(string s)
{
	//检查颜色是不是合理00-FF
	if (s.length() < 6)
		return false;

	for (int i = 0; i < (int)s.length(); i++) {
		if ((s[i] < '0' || s[i] > '9') && (s[i] < 'a' || s[i] > 'f') && (s[i] < 'A' || s[i] > 'F'))
			return false;
	}
	return true;

}

static int convert_to_rgbint(const string color_s)
{
	//把形如FFFFFF的字符串转换成颜色int
	int color[3] = {};
	string temp;
	for (int i = 0; i < 3; i++)
	{
		temp = color_s.substr(color_s.size() - 2 * (3 - i), 2);//每次从低位截2位
		if (temp[0] >= '0' && temp[0] <= '9')
			color[i] += (temp[0] - '0') * 16;
		else if (temp[0] >= 'a' && temp[0] <= 'f')
			color[i] += (10 + temp[0] - 'a') * 16;
		else if (temp[0] >= 'A' && temp[0] <= 'F')
			color[i] += (10 + temp[0] - 'A') * 16;
		if (temp[1] >= '0' && temp[1] <= '9')
			color[i] += temp[0] - '0';
		else if (temp[1] >= 'a' && temp[1] <= 'f')
			color[i] += 10 + temp[0] - 'a';
		else if (temp[1] >= 'A' && temp[1] <= 'F')
			color[i] += 10 + temp[0] - 'A';
	}
	return RGB(color[0], color[1], color[2]);
}
/********************************************************/
static int check_GBtype(int ch[4])
{
	//GB2312全角字符
	if (((ch[0] - 0xA0 >= 1 && ch[0] - 0xA0 <= 9) ||
		(ch[0] - 0xA0 >= 16 && ch[0] - 0xA0 <= 87)) && ch[1] - 0xA0 >= 1 && ch[1] - 0xA0 <= 94)
		return 1;
	//GB2312半角字符
	if (0x21 <= ch[0] && ch[0] <= 0x7E)
		return 2;
	//GB18030二字节
	if ((ch[0] >= 0xB0 && ch[0] <= 0xF7 && ch[1] >= 0xA1 && ch[1] <= 0xFE) ||
		(ch[0] >= 0x81 && ch[0] <= 0xA0 && ch[1] >= 0x40 && ch[1] <= 0xFE) ||
		(ch[0] >= 0xAA && ch[0] <= 0xFE && ch[1] >= 0x40 && ch[1] <= 0xA0))
		return 3;
	//GB18030四字节
	if ((ch[0] >= 0x81 && ch[0] <= 0x82 && ch[1] >= 0x30 && ch[1] <= 0x39
		&& ch[2] >= 0x81 && ch[2] <= 0xFE && ch[3] >= 0x30 && ch[3] <= 0x39) ||
		(ch[0] >= 0x95 && ch[0] <= 0x98 && ch[1] >= 0x30 && ch[1] <= 0x39
			&& ch[2] >= 0x81 && ch[2] <= 0xFE && ch[3] >= 0x30 && ch[3] <= 0x39))
		return 4;
	return 5;
}
static void content_validation(char valid_s[300], string content_s)
{
	//英文字母符号转为全角,非字库汉字忽略,cfg实现的时候已经对注释内容进行无视了
	int len = content_s.length();

	int v_i = 0;
	for (int i = 0; i < len; ) {
		int ch[4] = { 0 };
		for (int t = 0; t < 4; t++) {
			if (content_s[i + t] == 0)
				break;
			ch[t] = (unsigned char)content_s[i + t];
		}
		int res = check_GBtype(ch);
		if (res == 1) {
			valid_s[v_i] = content_s[i];
			valid_s[v_i + 1] = content_s[i + 1];
			v_i += 2;
			i += 2;

		}
		else if (res == 2) {
			valid_s[v_i] = unsigned char(161 + 0x02);
			valid_s[v_i + 1] = content_s[i] + 161 - 0x21;
			v_i += 2;
			i++;
		}
		else if (res == 3) {
			i += 2;
		}
		else if (res == 4) {
			i += 4;
		}
	}

}
/************************函数03:读取cfg中的参数********************************/
static int get_para(int& row, int& col, int& bgcolor, bool special_type[20], int& delay_screen, int& delay_strip, bool& sim, item_info myinfo[20])
{
	string color;
	CFT led_cfg("led.cfg");
	if (!led_cfg.is_open())
		return 0;
	/*******全局设置******/
	//行列数
	if (led_cfg.item_get_value("全局设置", "行数", row) == 0 || row < 1 || row > 5)
		row = 4;
	if (led_cfg.item_get_value("全局设置", "列数", col) == 0 || col < 6 || col > 12)
		col = 8;

	//背景色
	string _bg_color;
	if (led_cfg.item_get_value("全局设置", "背景色", _bg_color) == 0 || !check_legal_color(_bg_color))
		bgcolor = convert_to_rgbint("000000");
	else
		bgcolor = convert_to_rgbint(_bg_color);

	//特效20条
	char tch_special;
	for (int i = 0; i < 20; i++) {
		string ts_special = "特效" + to_string(i + 1);
		if (led_cfg.item_get_value("全局设置", ts_special.c_str(), tch_special) == 1 && (tch_special == 'Y'))
			special_type[i] = true;
		else
			special_type[i] = false;
	}

	//屏延时 条延时
	if (led_cfg.item_get_value("全局设置", "屏延时", delay_screen) == 0 || delay_screen < 0 || delay_screen > 10)
		delay_screen = 2;
	if (led_cfg.item_get_value("全局设置", "条延时", delay_strip) == 0 || delay_strip < 0 || delay_strip > 10)
		delay_strip = 3;

	//字库
	string ts_sim;
	if (led_cfg.item_get_value("全局设置", "字库", ts_sim) == 1 && ts_sim == "HZK16F")
		sim = false;
	else
		sim = true;

	/*******内容设置******/
	for (int i = 0; i < 20; i++) {
		string ts_item = "item" + to_string(i + 1);
		string ts_color = ts_item + "_color";
		if (led_cfg.item_get_value("内容设置", ts_item.c_str(), myinfo[i].item_content) == 0)
			myinfo[i].item_existed = 0;
		else {
			myinfo[i].item_existed = 1;
			//内容过长:截断,128个汉字相当于256长度
			if (myinfo[i].item_content.length() > 256)
				myinfo[i].item_content = myinfo[i].item_content.substr(0, 256);//0开始长度256
			//将半角字符转换
			char valid_s[300] = { 0 };
			content_validation(valid_s, myinfo[i].item_content);
			myinfo[i].item_content = valid_s;

			//颜色
			string ts_itemcolor;
			if (led_cfg.item_get_value("内容设置", ts_color.c_str(), ts_itemcolor) == 0 || !check_legal_color(ts_itemcolor))
				myinfo[i].item_color = convert_to_rgbint("FFFFFF");
			else
				myinfo[i].item_color = convert_to_rgbint(ts_itemcolor);
		}

	}
	return 1;
}

int LED_pro()
{
	srand((unsigned int)(time(0)));
	init_console();

	int row, col;//行列数
	//int font_height = 0;//字体高度
	int bgcolor;//背景色
	//int thickness;//点粗
	int delay_screen = 2, delay_strip = 3;//屏延时 条延时

	bool sim;//简体
	bool special_type[20] = { 0 };//最多20种特效,如为Y那就是true N就是false

	item_info my_info[20];

	if (!get_para(row, col, bgcolor, special_type, delay_screen, delay_strip, sim, my_info))
		return 0;

	/*一个item都没有那就只能welcome*/
	int _i;
	for (_i = 0; _i < 20; _i++)
		if (my_info[_i].item_existed == 1)
			break;

	if (_i == 20) {
		my_info[0].item_existed = 1;
		my_info[0].item_color = convert_to_rgbint("FFFFFF");
		my_info[0].item_content = "Welcome";
	}

	/*画板初始化*/
	int win_width = col * FONT_W * 6 + 10;//预留一点空间
	int win_height = row * FONT_W * 6 + 10;
	hdc_cls();
	hdc_init(bgcolor, COLOR_WHITE, win_width, win_height);

	/*特效循环*/
	int k = 0;
	while (1) {
		if (k == 20)
			k = 0;

		if (my_info[k].item_existed) {
			int special_no;
			while (1) {
				special_no = rand() % 20;
				if (special_type[special_no] == true)
					break;
			}
			if (special_no > 3)
				special_no = 0;//只做了4种特效emmm

			//分页
			int total_ch_num = my_info[k].item_content.length() / 2;//全角字符总数
			int onepage_ch_num = row * col;//每页最大显示的全角字符数量
			int page_num = int(ceil(double(total_ch_num) / double(onepage_ch_num)));
			for (int var_page = 0; var_page < page_num; var_page++) {//开始翻页
				if (var_page != 0)
					Sleep(delay_screen * 1000);
				string record = my_info[k].item_content.substr(2 * onepage_ch_num * var_page, 2 * onepage_ch_num);
				display_led(row, col, 6, sim, record.c_str(), my_info[k].item_color, special_no);
			}
			Sleep(delay_strip * 1000);
		}
		k++;

	}
	return 1;
}




int main()
{
	int res = LED_pro();
	return res;
	//完结撒花，真tm想哭
}