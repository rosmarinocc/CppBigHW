/*1852877 �ƿ� ��껈�*/
#include "90-02-b4.h"

void mode_char()
{
	/*��Ҫ�õ��Ĳ���*/
	int row, col;//��ʾ����/ÿ�к����� -- �ɴ˼�����Ļ��С
	int bgcolor, fgcolor;//����ɫ/ǰ��ɫ
	int size;//��ѡ:charģʽ���ַ���С;ledģʽ�µ��ֱ��
	int sim;//0-����or1-����
	char record[1024] = { 0 };//����û��Զ����ַ���

	get_para(1, row, col, size, sim, record);
	get_color_char(bgcolor, fgcolor);

	cct_cls();
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("������", 6 + 2 * (size - 1));//������һ�������׼:1��-6,2��-8.,����-10
	cct_setconsoleborder(2 * (FONT_W * col + 2), FONT_W * row + 2);//Ϊ�����۶�����һ��


	display_char(row, col, sim, record);

}