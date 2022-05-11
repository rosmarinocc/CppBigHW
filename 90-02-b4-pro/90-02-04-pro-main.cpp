/*1852877 �ƿ� ��껈�*/
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
	int item_existed = 0;//�Ƿ��������
	int item_color;//��ɫ
	string item_content;//����
};

/************************����01:��ʼ������̨����********************************/
static void init_console()
{
	cct_setcolor();
	cct_cls();
	cct_setconsoleborder(120, 50);
	cct_setfontsize("��������", 16, 8);
}
/********************************************************/
/************************����02:FFFFFF��ɫ����غ���********************************/
static bool check_legal_color(string s)
{
	//�����ɫ�ǲ��Ǻ���00-FF
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
	//������FFFFFF���ַ���ת������ɫint
	int color[3] = {};
	string temp;
	for (int i = 0; i < 3; i++)
	{
		temp = color_s.substr(color_s.size() - 2 * (3 - i), 2);//ÿ�δӵ�λ��2λ
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
	//GB2312ȫ���ַ�
	if (((ch[0] - 0xA0 >= 1 && ch[0] - 0xA0 <= 9) ||
		(ch[0] - 0xA0 >= 16 && ch[0] - 0xA0 <= 87)) && ch[1] - 0xA0 >= 1 && ch[1] - 0xA0 <= 94)
		return 1;
	//GB2312����ַ�
	if (0x21 <= ch[0] && ch[0] <= 0x7E)
		return 2;
	//GB18030���ֽ�
	if ((ch[0] >= 0xB0 && ch[0] <= 0xF7 && ch[1] >= 0xA1 && ch[1] <= 0xFE) ||
		(ch[0] >= 0x81 && ch[0] <= 0xA0 && ch[1] >= 0x40 && ch[1] <= 0xFE) ||
		(ch[0] >= 0xAA && ch[0] <= 0xFE && ch[1] >= 0x40 && ch[1] <= 0xA0))
		return 3;
	//GB18030���ֽ�
	if ((ch[0] >= 0x81 && ch[0] <= 0x82 && ch[1] >= 0x30 && ch[1] <= 0x39
		&& ch[2] >= 0x81 && ch[2] <= 0xFE && ch[3] >= 0x30 && ch[3] <= 0x39) ||
		(ch[0] >= 0x95 && ch[0] <= 0x98 && ch[1] >= 0x30 && ch[1] <= 0x39
			&& ch[2] >= 0x81 && ch[2] <= 0xFE && ch[3] >= 0x30 && ch[3] <= 0x39))
		return 4;
	return 5;
}
static void content_validation(char valid_s[300], string content_s)
{
	//Ӣ����ĸ����תΪȫ��,���ֿ⺺�ֺ���,cfgʵ�ֵ�ʱ���Ѿ���ע�����ݽ���������
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
/************************����03:��ȡcfg�еĲ���********************************/
static int get_para(int& row, int& col, int& bgcolor, bool special_type[20], int& delay_screen, int& delay_strip, bool& sim, item_info myinfo[20])
{
	string color;
	CFT led_cfg("led.cfg");
	if (!led_cfg.is_open())
		return 0;
	/*******ȫ������******/
	//������
	if (led_cfg.item_get_value("ȫ������", "����", row) == 0 || row < 1 || row > 5)
		row = 4;
	if (led_cfg.item_get_value("ȫ������", "����", col) == 0 || col < 6 || col > 12)
		col = 8;

	//����ɫ
	string _bg_color;
	if (led_cfg.item_get_value("ȫ������", "����ɫ", _bg_color) == 0 || !check_legal_color(_bg_color))
		bgcolor = convert_to_rgbint("000000");
	else
		bgcolor = convert_to_rgbint(_bg_color);

	//��Ч20��
	char tch_special;
	for (int i = 0; i < 20; i++) {
		string ts_special = "��Ч" + to_string(i + 1);
		if (led_cfg.item_get_value("ȫ������", ts_special.c_str(), tch_special) == 1 && (tch_special == 'Y'))
			special_type[i] = true;
		else
			special_type[i] = false;
	}

	//����ʱ ����ʱ
	if (led_cfg.item_get_value("ȫ������", "����ʱ", delay_screen) == 0 || delay_screen < 0 || delay_screen > 10)
		delay_screen = 2;
	if (led_cfg.item_get_value("ȫ������", "����ʱ", delay_strip) == 0 || delay_strip < 0 || delay_strip > 10)
		delay_strip = 3;

	//�ֿ�
	string ts_sim;
	if (led_cfg.item_get_value("ȫ������", "�ֿ�", ts_sim) == 1 && ts_sim == "HZK16F")
		sim = false;
	else
		sim = true;

	/*******��������******/
	for (int i = 0; i < 20; i++) {
		string ts_item = "item" + to_string(i + 1);
		string ts_color = ts_item + "_color";
		if (led_cfg.item_get_value("��������", ts_item.c_str(), myinfo[i].item_content) == 0)
			myinfo[i].item_existed = 0;
		else {
			myinfo[i].item_existed = 1;
			//���ݹ���:�ض�,128�������൱��256����
			if (myinfo[i].item_content.length() > 256)
				myinfo[i].item_content = myinfo[i].item_content.substr(0, 256);//0��ʼ����256
			//������ַ�ת��
			char valid_s[300] = { 0 };
			content_validation(valid_s, myinfo[i].item_content);
			myinfo[i].item_content = valid_s;

			//��ɫ
			string ts_itemcolor;
			if (led_cfg.item_get_value("��������", ts_color.c_str(), ts_itemcolor) == 0 || !check_legal_color(ts_itemcolor))
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

	int row, col;//������
	//int font_height = 0;//����߶�
	int bgcolor;//����ɫ
	//int thickness;//���
	int delay_screen = 2, delay_strip = 3;//����ʱ ����ʱ

	bool sim;//����
	bool special_type[20] = { 0 };//���20����Ч,��ΪY�Ǿ���true N����false

	item_info my_info[20];

	if (!get_para(row, col, bgcolor, special_type, delay_screen, delay_strip, sim, my_info))
		return 0;

	/*һ��item��û���Ǿ�ֻ��welcome*/
	int _i;
	for (_i = 0; _i < 20; _i++)
		if (my_info[_i].item_existed == 1)
			break;

	if (_i == 20) {
		my_info[0].item_existed = 1;
		my_info[0].item_color = convert_to_rgbint("FFFFFF");
		my_info[0].item_content = "Welcome";
	}

	/*�����ʼ��*/
	int win_width = col * FONT_W * 6 + 10;//Ԥ��һ��ռ�
	int win_height = row * FONT_W * 6 + 10;
	hdc_cls();
	hdc_init(bgcolor, COLOR_WHITE, win_width, win_height);

	/*��Чѭ��*/
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
				special_no = 0;//ֻ����4����Чemmm

			//��ҳ
			int total_ch_num = my_info[k].item_content.length() / 2;//ȫ���ַ�����
			int onepage_ch_num = row * col;//ÿҳ�����ʾ��ȫ���ַ�����
			int page_num = int(ceil(double(total_ch_num) / double(onepage_ch_num)));
			for (int var_page = 0; var_page < page_num; var_page++) {//��ʼ��ҳ
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
	//�����������tm���
}