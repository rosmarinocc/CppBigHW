/*1852877 计科 赵昊堃*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
#include <cmath>
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//未定义的情况下才打开条件编译

/* 允许添加需要的头文件 */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS中此处会有波浪线，不影响正常使用

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */

/* 此处允许添加自定义函数，但仅限static，即外部不可见 */


/********************001:求叉积、点积**********************************/
static int cross(const int xa, const int ya, const int xb, const int yb, const int xp, const int yp)
{
	return (xb - xa) * (yp - ya) - (yb - ya) * (xp - xa);
}

static int dot(const int xa, const int ya, const int xb, const int yb)
{
	return xa * xb + ya * yb;
}

/********************002:判断点是否在三角形内部**********************************/


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
/********************003:判断点是否在矩形内部**********************************/
static bool inRectangle(const int xa, const int ya, const int xb, const int yb, const int xc, const int yc, const int xd, const int yd, const int xp, const int yp) {
	int c1 = cross(xa, ya, xb, yb, xp, yp);
	int c2 = cross(xc, yc, xd, yd, xp, yp);
	int c3 = cross(xb, yb, xc, yc, xp, yp);
	int c4 = cross(xd, yd, xa, ya, xp, yp);

	bool res1 = c1 >= 0 && c2 >= 0 || c1 <= 0 && c2 <= 0;
	bool res2 = c3 >= 0 && c4 >= 0 || c3 <= 0 && c4 <= 0;

	return res1 && res2;//直接相乘会溢出!!

}
/********************004:判断点是否在扇形内部**********************************/
static bool inSector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int xp, const int yp)
{
	//距离判断
	int dx = xp - point_x;
	int dy = yp - point_y;
	int distance2 = dx * dx + dy * dy;

	if (distance2 > radius * radius)//在圆外
		return false;

	//角度判断
	int angle_begin1 = angle_begin % 360;
	int angle_end1 = angle_end % 360;

	while (angle_begin1 < 0)
		angle_begin1 += 360;

	while (angle_end1 <= angle_begin1)
		angle_end1 += 360;

	//整圆特判:
	if (angle_end1 - angle_begin1 >= 360)
		return true;


	//一般判断

	double cos_theta = dot(0, -1, dx, dy) / sqrt(distance2);//点乘判断角度,竖直向上的向量为(0,-1)
	double theta = acos(cos_theta) * 180 / PI;//0~PI
	if (xp < point_x)
		theta = 360 - theta;

	while (theta < angle_begin1)
		theta += 360;

	return theta > angle_end1 ? false : true;
}
/********************005:判断点是否在圆形内部**********************************/
static bool inCircle(const int point_x, const int point_y, const int radius, const int xp, const int yp)
{
	return ((xp - point_x) * (xp - point_x) + (yp - point_y) * (yp - point_y) - radius * radius) <= 0;
}

/********************006:坐标变换公式**********************************/
static void convert_coord(int& x, int& y, const int rotation_angles)//将旋转坐标系的点转换到平凡坐标系中
{
	if (rotation_angles % 360 == 0)
		return;
	int tx = x, ty = y;
	//因为是顺时针旋转,所以x'逆时针变换可以得到x坐标
	x = int(tx * cos(rotation_angles * PI / 180) + ty * sin(rotation_angles * PI / 180));
	y = int(ty * cos(rotation_angles * PI / 180) - tx * sin(rotation_angles * PI / 180));

}
//将旋转坐标系的点转换到平凡坐标系中,旋转角度取相反数实现逆转
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
/********************007:判断是否在椭圆内**********************************/
static bool	inEllipse(const int radius_a, const int radius_b, const int xp, const int yp)
{
	//教训!转换在除法后=整型结果转高精度整型的double
	double res = double(xp * xp) / double(radius_a * radius_a) + double(yp * yp) / double(radius_b * radius_b);
	return res <= 1;
}



/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
			const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("点阵字体", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cct_cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
	/* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
			const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x1,y1)-(x2,y2)之间画出一个像素点的连线
  输入参数：const int x1：起点x坐标，左上角为(0,0)
			const int y1：起点y坐标，左上角为(0,0)
			const int x2：终点y坐标，左上角为(0,0)
			const int y2：终点y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
			const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_base_point/hdc_base_line函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */

   /***************************************************************************
	 函数名称：
	 功    能：画线段
	 输入参数： const int x1		：起点的x
			   const int y1		：起点的y
			   const int x2		：终点的x
			   const int y2		：终点的y
			   const int thickness	：线段的粗细（有缺省值）
			   const int RGB_value	：线段的颜色（有缺省值）
	 返 回 值：
	 说    明：本函数可以用基于hdc_point的组合（速度慢）
					 也可以用hdc_base_point/hdc_base_line的组合（速度快）
   ***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	//采用hdc_base_point/hdc_base_line的组合
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

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
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
			const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
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
				if (inTriangle(x1, y1, x2, y2, x3, y3, tx, ty) && !find_l) { //最左侧
					tx_left = tx;
					find_l = true;
				}
				if (!inTriangle(x1, y1, x2, y2, x3, y3, tx, ty) && find_l) {//最右侧
					tx_right = tx - 1;
					hdc_base_line(tx_left, ty, tx_right, ty);
					break;
				}
				if (tx == x_max && find_l) //一直在界内持续到最后
					hdc_base_line(tx_left, ty, tx, ty);
			}
		}



	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
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
  函数名称：
  功    能：给出左上角坐标及边长，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{

	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);

}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{

	int angle_begin1 = (angle_begin) % 360;
	int angle_end1 = (angle_end) % 360;

	while (angle_begin1 < 0)
		angle_begin1 += 360;

	while (angle_end1 <= angle_begin1)
		angle_end1 += 360;

	if (angle_end1 - angle_begin1 >= 360) {//整圆特判,否则会缺点
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
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
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
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	int tx, ty;
	int odd_tx = point_x;
	int	odd_ty = point_y - radius;

	for (int angle = 1; angle <= 361; angle++) {//必须为361才能填补1缺口
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
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
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
		//旋转坐标系

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
