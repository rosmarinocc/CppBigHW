/*1852877 �ƿ� ��껈�*/
#include "90-02-b4.h"


void mode_led()
{
	/*��Ҫ�õ��Ĳ���*/
	int row, col;//��ʾ����/ÿ�к����� -- �ɴ˼�����Ļ��С
	//����ɫ/ǰ��ɫ
	int bg_color;
	int fg_red, fg_green, fg_blue;
	int size;//��ѡ:charģʽ���ַ���С;ledģʽ�µ��ֱ��
	int sim;//����or����
	char record[1024] = { 0 };//����û��Զ����ַ���

	get_para(2, row, col, size, sim, record);
	get_color_led(bg_color, fg_red, fg_green, fg_blue);


	int pt_size = 6 + 2 * (size - 1);//���ֱ��:1-6px,2-8px,3-10px
	int win_width = col * FONT_W * pt_size + 10;//Ԥ��һ��ռ�
	int win_height = row * FONT_W * pt_size + 10;

	hdc_cls();
	hdc_init(bg_color, COLOR_WHITE, win_width, win_height);
	display_led(row, col, pt_size, sim, record, RGB(fg_red, fg_green, fg_blue));

}