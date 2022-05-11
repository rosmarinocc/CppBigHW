/*1852877 �ƿ� ��껈�*/
#pragma once
#include "90-02-b4.h"


/********************��������**********************/
//��ʼ��
void init_para()
{
	cct_setcolor();
	cct_setfontsize("������", 12);
	cct_setconsoleborder(100, 40);

}

//ͨ�ò�������
void get_para(const int mode, int& row, int& col, int& size, int& sim, char* record)
{
	cct_cls();

	if (mode == 1)
		get_dig_para("��ѡ���ַ�ģʽ������Ĵ�С��(1-3),��Χ����������Ϊ2 :", 1, 3, size, true, 2);//6,8,10

	else
		get_dig_para("��ѡ��LEDģʽ�µ��ֱ����(1-3),��Χ����������Ϊ2 :", 1, 3, size, true, 2);// 20 30 40 px


	if (size == 1) {
		get_dig_para("��������ʾ������(3-9),��Χ����������Ϊ3 :", 3, 9, row, true, 3);
		get_dig_para("������ÿ����ʾ������(6-14),��Χ����������Ϊ6 :", 6, 14, col, true, 6);
	}
	else if (size == 2) {
		get_dig_para("��������ʾ������(3-6),��Χ����������Ϊ3 :", 3, 6, row, true, 3);
		get_dig_para("������ÿ����ʾ������(6-10),��Χ����������Ϊ6 :", 6, 10, col, true, 6);
	}
	else if (size == 3) {
		get_dig_para("��������ʾ������(3-5),��Χ����������Ϊ3 :", 3, 5, row, true, 3);
		get_dig_para("������ÿ����ʾ������(6-8),��Χ����������Ϊ6 :", 6, 8, col, true, 6);
	}

	get_dig_para("��ѡ��1-����,0-����:", 0, 1, sim, true, 1);

	cout << "��������Ҫ��ʾ���ı�����(ȫ���ַ�) :" << endl;
	cin >> record;

}

//charģʽ��ɫ����
void get_color_char(int& bgcolor, int& fgcolor)
{
	cct_cls();
	cout << "��ɫ��Ӧ��Ϊ :" << endl;
	cout << "0-��,1-��,2-��,3-��,4-��,5-��,6-��,7-��" << endl;
	cout << "8-����,9-����,10-����,11-����,12-����,13-����,14-����,15-����" << endl;
	putchar('\n');
	get_dig_para("��ѡ�񱳾���ɫ(0-15),��Χ����������Ϊ0-�� :", 0, 15, bgcolor, true, 0);
	get_dig_para("��ѡ��ǰ����ɫ(0-15),��Χ����������Ϊ7-�� :", 0, 15, fgcolor, true, 7);
}

//ledģʽ��ɫ����:��֪���ǲ���ǰ��������Ҫrgb,�Ȳ�д��
void get_color_led(int& bg_color, int& fg_red, int& fg_green, int& fg_blue)
{
	cct_cls();
	cout << "��ɫ��Ӧ��Ϊ :" << endl;
	cout << "0-��,1-��,2-��,3-��,4-��,5-��,6-��,7-��" << endl;
	cout << "8-����,9-����,10-����,11-����,12-����,13-����,14-����,15-����" << endl;
	putchar('\n');
	get_dig_para("��ѡ�񱳾���ɫ(0-15),��Χ����������Ϊ0-�� :", 0, 15, bg_color, true, 0);

	cout << "������ɫRGBֵ���� :" << endl;
	cout << "��-(255,0,0),��-(0,255,0),��-(0,0,255),��-(0,0,0),��(255,255,255)" << endl;
	putchar('\n');
	cout << "������ǰ����ɫ��RGBֵ:" << endl;
	get_dig_para("��ѡ��R����(0-255):", 0, 255, fg_red);
	get_dig_para("��ѡ��G����(0-255):", 0, 255, fg_green);
	get_dig_para("��ѡ��B����(0-255):", 0, 255, fg_blue);
}


/********************cha-mode����**********************/
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
			cout << "������!���ڰ���ַ�" << endl;
			wait_for_enter();
			return;
		}

	if (len_s / 2 > row * col) {
		len_s = 2 * row * col;
		cout << "�������ݳ������������ʾ��" << row * col << ",�ѽض�" << endl;
		wait_for_enter();

	}

	/*���ļ��ַ���*/
	char path[20] = { 0 };
	if (sim == 1)
		strcpy(path, "./HZK16");
	else
		strcpy(path, "./HZK16F");

	FILE* f = fopen(path, "rb");
	if (f == NULL) {
		cout << "�ļ���ʧ��";
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

/********************led-mode����**********************/
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


/******************����Ч:demoչʾ**********************/

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

/******************��Ч01:��ɫ�޺�**********************/
void special_colorful(const int row, const int col, const int pt_size, const int len_s, const char* record, FILE* f)
{
	cout << "��Ч01:��ɫ�޺��" << endl;
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

/******************��Ч02:��Ҷ��**********************/
void draw_window(const int r, const int col, const int start_x, const int start_y, const int pt_size, const int len_s, int& cnt, const char* record, FILE* f, const int RGB_value)
{
	char temp[14][2];//�������Ϊ14,��˿��Դ��һ�е������ַ�
	char matrix[14][32] = { 0 };//�������Ϊ14,��˿��Դ��һ�е����־���
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
	cout << "��Ч02:��Ҷ��" << endl;
	wait_for_enter();
	hdc_cls();
	int cnt = 0;
	for (int r = 0; r < row; r++) {//һ����ʾһ������
		draw_window(r, col, pt_size / 2, pt_size / 2 + pt_size * FONT_W * r, pt_size, len_s, cnt, record, f, RGB_value);
		if (cnt == len_s / 2) {
			Sleep(1200);
			return;
		}
	}
}
/******************��Ч03:��˸�ַ�**********************/
void special_shine(const int row, const int col, const int pt_size, const int len_s, const char* record, FILE* f, const int RGB_value)
{
	cout << "��Ч03:��˸�ַ�" << endl;
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
			cout << "������!���ڰ���ַ�" << endl;
			wait_for_enter();
			return;
		}

	if (len_s / 2 > row * col) {
		len_s = 2 * row * col;
		cout << "�������ݳ������������ʾ��" << row * col << ",�ѽض�" << endl;
		wait_for_enter();

	}

	/*���ļ��ַ���*/
	char path[20] = { 0 };
	if (sim == 1)
		strcpy(path, "./HZK16");
	else
		strcpy(path, "./HZK16F");

	FILE* f = fopen(path, "rb");
	if (f == NULL) {
		cout << "�ļ���ʧ��";
		wait_for_enter();
		return;
	}

	no_special(row, col, pt_size, len_s, record, f, RGB_value);

	cout << "���������Чչʾ,\"�޺��\",\"��Ҷ��\",\"�����°�\",��Ч��ʾ��Ϻ󰴻س�����" << endl;
	special_colorful(row, col, pt_size, len_s, record, f);
	special_window(row, col, pt_size, len_s, record, f, RGB_value);
	special_shine(row, col, pt_size, len_s, record, f, RGB_value);

	cout << "��Ч��ʾ���" << endl;
	wait_for_enter();
	fclose(f);
}