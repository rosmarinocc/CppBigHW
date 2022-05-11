/*1852877 �ƿ� ��껈�*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
#include <cmath>
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//δ���������²Ŵ���������

/* ���������Ҫ��ͷ�ļ� */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS�д˴����в����ߣ���Ӱ������ʹ��

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */

/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */


/********************001:���������**********************************/
static int cross(const int xa, const int ya, const int xb, const int yb, const int xp, const int yp)
{
	return (xb - xa) * (yp - ya) - (yb - ya) * (xp - xa);
}

static int dot(const int xa, const int ya, const int xb, const int yb)
{
	return xa * xb + ya * yb;
}

/********************002:�жϵ��Ƿ����������ڲ�**********************************/


static bool inLeft(const int xa, const int ya, const int xb, const int yb, const int xp, const int yp)
{
	return cross(xa, ya, xb, yb, xp, yp) > 0;
}

static bool inTriangle(const int xa, const int ya, const int xb, const int yb, const int xc, const int yc, const int xp, const int yp)
{
	bool res = inLeft(xa, ya, xb, yb, xp, yp);
	if (res != inLeft(xb, yb, xc, yc, xp, yp))
		return false;
	if (res != inLeft(xc, yc, xa, ya, xp, yp))
		return false;
	if ((xb - xa) * (yc - ya) - (yb - ya) * (xc - xa) == 0)    //abc is in one line
		return false;
	return true;
}
/********************003:�жϵ��Ƿ��ھ����ڲ�**********************************/
static bool inRectangle(const int xa, const int ya, const int xb, const int yb, const int xc, const int yc, const int xd, const int yd, const int xp, const int yp) {
	int c1 = cross(xa, ya, xb, yb, xp, yp);
	int c2 = cross(xc, yc, xd, yd, xp, yp);
	int c3 = cross(xb, yb, xc, yc, xp, yp);
	int c4 = cross(xd, yd, xa, ya, xp, yp);

	bool res1 = c1 >= 0 && c2 >= 0 || c1 <= 0 && c2 <= 0;
	bool res2 = c3 >= 0 && c4 >= 0 || c3 <= 0 && c4 <= 0;

	return res1 && res2;//ֱ����˻����!!

}
/********************004:�жϵ��Ƿ��������ڲ�**********************************/
static bool inSector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int xp, const int yp)
{
	//�����ж�
	int dx = xp - point_x;
	int dy = yp - point_y;
	int distance2 = dx * dx + dy * dy;

	if (distance2 > radius * radius)//��Բ��
		return false;

	//�Ƕ��ж�
	int angle_begin1 = angle_begin % 360;
	int angle_end1 = angle_end % 360;

	while (angle_begin1 < 0)
		angle_begin1 += 360;

	while (angle_end1 <= angle_begin1)
		angle_end1 += 360;

	//��Բ����:
	if (angle_end1 - angle_begin1 >= 360)
		return true;


	//һ���ж�

	double cos_theta = dot(0, -1, dx, dy) / sqrt(distance2);//����жϽǶ�,��ֱ���ϵ�����Ϊ(0,-1)
	double theta = acos(cos_theta) * 180 / PI;//0~PI
	if (xp < point_x)
		theta = 360 - theta;

	while (theta < angle_begin1)
		theta += 360;

	return theta > angle_end1 ? false : true;
}
/********************005:�жϵ��Ƿ���Բ���ڲ�**********************************/
static bool inCircle(const int point_x, const int point_y, const int radius, const int xp, const int yp)
{
	return ((xp - point_x) * (xp - point_x) + (yp - point_y) * (yp - point_y) - radius * radius) <= 0;
}

/********************006:����任��ʽ**********************************/
static void convert_coord(int& x, int& y, const int rotation_angles)//����ת����ϵ�ĵ�ת����ƽ������ϵ��
{
	if (rotation_angles % 360 == 0)
		return;
	int tx = x, ty = y;
	//��Ϊ��˳ʱ����ת,����x'��ʱ��任���Եõ�x����
	x = int(tx * cos(rotation_angles * PI / 180) + ty * sin(rotation_angles * PI / 180));
	y = int(ty * cos(rotation_angles * PI / 180) - tx * sin(rotation_angles * PI / 180));

}
//����ת����ϵ�ĵ�ת����ƽ������ϵ��,��ת�Ƕ�ȡ�෴��ʵ����ת
static int convert_x(const int x, const int y, const int rotation_angles)
{

	if (rotation_angles % 360 == 0)
		return x;
	return  int(x * cos(rotation_angles * PI / 180) + y * sin(rotation_angles * PI / 180));

}
static int convert_y(const int x, const int y, const int rotation_angles)
{

	if (rotation_angles % 360 == 0)
		return y;
	return  int(y * cos(rotation_angles * PI / 180) - x * sin(rotation_angles * PI / 180));

}
/********************007:�ж��Ƿ�����Բ��**********************************/
static bool	inEllipse(const int radius_a, const int radius_b, const int xp, const int yp)
{
	//��ѵ!ת���ڳ�����=���ͽ��ת�߾������͵�double
	double res = double(xp * xp) / double(radius_a * radius_a) + double(yp * yp) / double(radius_b * radius_b);
	return res <= 1;
}



/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
			const int fgcolor��ǰ��ɫ
			const int width  ����Ļ��ȣ�����
			const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
	hdc_release();

	/* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("��������", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //������Ŀ�ȼ��߶�ת��Ϊ�ض��������������!!�ĺ��壺�������8/16�ı��������ж�+1
	cct_cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
	/* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* ���ֻ���������demoʱ�������hdc_cls()���һ������ȱ�ߵ����������ʱ����
	   ������ֻ������л������⣬���ߴ���ʱֵ */
	Sleep(30);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
			const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x1,y1)-(x2,y2)֮�仭��һ�����ص������
  ���������const int x1�����x���꣬���Ͻ�Ϊ(0,0)
			const int y1�����y���꣬���Ͻ�Ϊ(0,0)
			const int x2���յ�y���꣬���Ͻ�Ϊ(0,0)
			const int y2���յ�y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
			const int y			��y���꣬���Ͻ�Ϊ(0,0)
			const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
			const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	�������к�����ʵ�֣���������������׼�Ķ�
	1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
	2�����к�����ʵ�ֶ�����hdc_base_point/hdc_base_line���������
	3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
	4�������ʵ������ӵĲ���
	5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */

   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ����߶�
	 ��������� const int x1		������x
			   const int y1		������y
			   const int x2		���յ��x
			   const int y2		���յ��y
			   const int thickness	���߶εĴ�ϸ����ȱʡֵ��
			   const int RGB_value	���߶ε���ɫ����ȱʡֵ��
	 �� �� ֵ��
	 ˵    ���������������û���hdc_point����ϣ��ٶ�����
					 Ҳ������hdc_base_point/hdc_base_line����ϣ��ٶȿ죩
   ***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	//����hdc_base_point/hdc_base_line�����
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	hdc_base_point(x1, y1);
	hdc_base_point(x2, y2);

	int dx = x2 - x1;
	int dy = y2 - y1;

	double distance = sqrt(dx * dx + dy * dy);

	int up_x = x1 + int(tn * dy / distance / 2);
	int down_x = x1 - int(tn * dy / distance / 2);
	int left_y = y1 - int(tn * dx / distance / 2);
	int right_y = y1 + int(tn * dx / distance / 2);

	int cnt = 0;
	for (int i = min(up_x, down_x); i <= max(up_x, down_x) || cnt < tn; i++) {
		for (int j = min(left_y, right_y); j <= max(left_y, right_y); j++) {
			hdc_base_line(i, j, i + dx, j + dy);
			cnt++;
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
			const int y1		����1�����y
			const int x2		����2�����x
			const int y2		����2�����y
			const int x3		����3�����x
			const int y3		����3�����y
			bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness	���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x2, y2, x3, y3, thickness, RGB_value);
	hdc_line(x3, y3, x1, y1, thickness, RGB_value);
	if (filled) {
		int x_min = min(min(x1, x2), x3);
		int x_max = max(max(x1, x2), x3);
		int y_min = min(min(y1, y2), y3);
		int y_max = max(max(y1, y2), y3);

		int tx_left, tx_right;
		for (int ty = y_min; ty <= y_max; ty++) {
			bool find_l = false;
			for (int tx = x_min; tx <= x_max; tx++) {
				if (inTriangle(x1, y1, x2, y2, x3, y3, tx, ty) && !find_l) { //�����
					tx_left = tx;
					find_l = true;
				}
				if (!inTriangle(x1, y1, x2, y2, x3, y3, tx, ty) && find_l) {//���Ҳ�
					tx_right = tx - 1;
					hdc_base_line(tx_left, ty, tx_right, ty);
					break;
				}
				if (tx == x_max && find_l) //һֱ�ڽ��ڳ��������
					hdc_base_line(tx_left, ty, tx, ty);
			}
		}



	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int width				�����
			const int high				���߶�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
			bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{

	int left_down_x, left_down_y, right_up_x, right_up_y, right_down_x, right_down_y;

	left_down_x = int(left_up_x - high * sin(rotation_angles * PI / 180));
	left_down_y = int(left_up_y + high * cos(rotation_angles * PI / 180));

	right_up_x = int(left_up_x + width * cos(rotation_angles * PI / 180));
	right_up_y = int(left_up_y + width * sin(rotation_angles * PI / 180));

	right_down_x = int(right_up_x - high * sin(rotation_angles * PI / 180));
	right_down_y = int(right_up_y + high * cos(rotation_angles * PI / 180));

	hdc_line(left_up_x, left_up_y, left_down_x, left_down_y, thickness, RGB_value);
	hdc_line(left_up_x, left_up_y, right_up_x, right_up_y, thickness, RGB_value);
	hdc_line(right_up_x, right_up_y, right_down_x, right_down_y, thickness, RGB_value);
	hdc_line(left_down_x, left_down_y, right_down_x, right_down_y, thickness, RGB_value);

	if (filled) {
		int x_min = min(min(min(left_up_x, right_up_x), right_down_x), left_down_x);
		int x_max = max(max(max(left_up_x, right_up_x), right_down_x), left_down_x);
		int y_min = min(min(min(left_up_y, right_up_y), right_down_y), left_down_y);
		int y_max = max(max(max(left_up_y, right_up_y), right_down_y), left_down_y);


		int tx_left, tx_right;
		for (int ty = y_min; ty <= y_max; ty++) {
			bool find_l = false;
			for (int tx = x_min; tx <= x_max; tx++) {
				if (inRectangle(left_up_x, left_up_y, left_down_x, left_down_y, right_down_x, right_down_y, right_up_x, right_up_y, tx, ty) && !find_l) {
					tx_left = tx;
					find_l = true;
				}
				if (!inRectangle(left_up_x, left_up_y, left_down_x, left_down_y, right_down_x, right_down_y, right_up_x, right_up_y, tx, ty) && find_l) {
					tx_right = tx - 1;
					hdc_base_line(tx_left, ty, tx_right, ty);
					break;
				}
				if (tx == x_max && find_l)
					hdc_base_line(tx_left, ty, tx, ty);
			}
		}
	}

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int length			���߳�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{

	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{

	int angle_begin1 = (angle_begin) % 360;
	int angle_end1 = (angle_end) % 360;

	while (angle_begin1 < 0)
		angle_begin1 += 360;

	while (angle_end1 <= angle_begin1)
		angle_end1 += 360;

	if (angle_end1 - angle_begin1 >= 360) {//��Բ����,�����ȱ��
		hdc_circle(point_x, point_y, radius, false, thickness, RGB_value);
	}
	else {
		int tx, ty;
		int odd_tx = int(point_x + radius * sin(angle_begin1 * PI / 180));
		int	odd_ty = int(point_y - radius * cos(angle_begin1 * PI / 180));

		for (int angle = angle_begin1 + 1; angle <= angle_end1; angle++) {
			tx = int(point_x + radius * sin(angle * PI / 180));
			ty = int(point_y - radius * cos(angle * PI / 180));
			hdc_line(tx, ty, odd_tx, odd_ty, thickness, RGB_value);
			odd_tx = tx;
			odd_ty = ty;
		}

	}

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	int begin_x = int(point_x + radius * sin(angle_begin * PI / 180));
	int begin_y = int(point_y - radius * cos(angle_begin * PI / 180));
	int end_x = int(point_x + radius * sin(angle_end * PI / 180));
	int end_y = int(point_y - radius * cos(angle_end * PI / 180));

	if ((angle_end - angle_begin) % 360 != 0) {
		hdc_line(point_x, point_y, begin_x, begin_y, thickness, RGB_value);
		hdc_line(point_x, point_y, end_x, end_y, thickness, RGB_value);
	}
	hdc_arc(point_x, point_y, radius, angle_begin, angle_end, thickness, RGB_value);

	if (filled) {

		int x_min = point_x - radius;
		int x_max = point_x + radius;
		int y_min = point_y - radius;
		int y_max = point_y + radius;
		for (int ty = y_min; ty <= y_max; ty++) {
			int left_x, right_x;
			bool find_l = false;
			for (int tx = x_min; tx <= x_max; tx++) {
				if (!find_l && inSector(point_x, point_y, radius, angle_begin, angle_end, tx, ty)) {
					left_x = tx;
					find_l = true;
				}
				if (find_l && !inSector(point_x, point_y, radius, angle_begin, angle_end, tx, ty)) {
					right_x = tx;
					hdc_base_line(left_x, ty, right_x, ty);
					find_l = false;
				}
				if (tx == x_max && find_l)
					hdc_base_line(left_x, ty, tx, ty);
			}

		}

	}

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	int tx, ty;
	int odd_tx = point_x;
	int	odd_ty = point_y - radius;

	for (int angle = 1; angle <= 361; angle++) {//����Ϊ361�����1ȱ��
		tx = int(point_x + radius * sin(angle * PI / 180));
		ty = int(point_y - radius * cos(angle * PI / 180));
		hdc_line(tx, ty, odd_tx, odd_ty, thickness, RGB_value);
		odd_tx = tx;
		odd_ty = ty;
	}
	if (filled) {

		int x_min = point_x - radius;
		int x_max = point_x + radius;
		int y_min = point_y - radius;
		int y_max = point_y + radius;

		int tx_left, tx_right;
		for (int ty = y_min; ty <= y_max; ty++) {
			bool find_l = false;
			for (int tx = x_min; tx <= x_max; tx++) {
				if (inCircle(point_x, point_y, radius, tx, ty) && !find_l) {
					tx_left = tx;
					find_l = true;
				}
				if (!inCircle(point_x, point_y, radius, tx, ty) && find_l) {
					tx_right = tx - 1;
					hdc_base_line(tx_left, ty, tx_right, ty);
					break;
				}
				if (tx == x_max && find_l)
					hdc_base_line(tx_left, ty, tx, ty);
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
			const int point_y			��Բ�ĵ�y
			const int radius_a			��ƽ����X��İ뾶
			const int radius_b			��ƽ����Y��İ뾶
			const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			����ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{

	if (radius_a == radius_b)
		hdc_circle(point_x, point_y, radius_a, false, thickness, RGB_value);
	else {
		int tx, ty;
		int odd_tx = radius_a;
		int odd_ty = 0;
		convert_coord(odd_tx, odd_ty, -rotation_angles);
		for (int angle = 1; angle <= 361; angle++) {
			tx = int(radius_a * cos(angle * PI / 180));
			ty = int(-radius_b * sin(angle * PI / 180));
			convert_coord(tx, ty, -rotation_angles);
			hdc_line(tx + point_x, ty + point_y, odd_tx + point_x, odd_ty + point_y, thickness, RGB_value);
			odd_tx = tx;
			odd_ty = ty;
		}
	}
	if (filled) {
		//��ת����ϵ

		if (radius_a == radius_b)
			hdc_circle(point_x, point_y, radius_a, true, thickness, RGB_value);
		else {
			int x_max = radius_a;
			int x_min = -radius_a;
			int y_max = radius_b;
			int y_min = -radius_b;
			for (int ty = y_min; ty <= y_max; ty++) {
				int tx_left, tx_right;
				bool find_l = false;
				for (int tx = x_min; tx <= x_max; tx++) {
					if (inEllipse(radius_a, radius_b, tx, ty) && !find_l) {
						tx_left = tx;
						find_l = true;
					}
					if (find_l) {
						tx_right = -tx_left;
						int x_l = convert_x(tx_left, ty, -rotation_angles) + point_x;
						int y_l = convert_y(tx_left, ty, -rotation_angles) + point_y;
						int x_r = convert_x(tx_right, ty, -rotation_angles) + point_x;
						int y_r = convert_y(tx_right, ty, -rotation_angles) + point_y;
						hdc_base_line(x_l, y_l, x_r, y_r);
						break;
					}
				}
			}

		}

	}

}

#endif !HDC_SERIES_BY_TEACHER
