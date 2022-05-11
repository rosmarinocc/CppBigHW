/*1852877 计科 赵昊堃*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */


   /* --------------------------------------------------
		   此处可以给出需要的内部辅助工具函数
		   1、函数名不限，建议为 gmw_inner_*
		   2、个数不限
		   3、必须是static函数，确保只在本源文件中使用
	  -------------------------------------------------- */
	  /*****************************01.将xy坐标转换为对应行列***********************************/
static int convert_xy_to_rc(const CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int& MRow, int& MCol);
/*****************************02.处理可能的中文截断***********************************/
static void half_chinesechar(char* s);
/*****************************03.将框架符号保证为宽度2***********************************/
static const char* modify(const char* src, char restore[CFI_LEN]);
/*****************************04.在起始xy坐标处画方块***********************************/
static void draw_block_xy(const CONSOLE_GRAPHICS_INFO* const pCGI, const int block_start_x, const int block_start_y, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi, bool need_delay = true);
/*****************************05.计算不能直接设置的参数***********************************/
static void gmw_inner_cal_parameter(CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/*计算不能直接通过函数设置得到的参数*/

	if (pCGI->CFI.separator) {
		//框架总宽度
		pCGI->CFI.bwidth = (pCGI->CFI.block_width + pCGI->CFI.separator * 2) * pCGI->col_num + 2;
		//框架总高度
		pCGI->CFI.bhigh = (pCGI->CFI.block_high + pCGI->CFI.separator * 1) * pCGI->row_num + 1;
	}
	else {
		pCGI->CFI.bwidth = pCGI->CFI.block_width * pCGI->col_num + 2 * 2;
		pCGI->CFI.bhigh = pCGI->CFI.block_high * pCGI->row_num + 1 * 2;
	}
	//框架起始x坐标
	pCGI->start_x = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no;
	//框架起始y坐标
	pCGI->start_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
	//屏幕总高度
	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no +
		pCGI->CFI.bhigh + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	//屏幕总宽度
	pCGI->cols = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no +
		pCGI->CFI.bwidth + pCGI->extern_right_cols + 1;
	//上状态栏起始x坐标
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	//上状态栏起始y坐标
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	//下状态栏起始x坐标
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	//下状态栏起始y坐标
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh;
}
/* -----------------------------------------------
		实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */
   /***************************************************************************
	 函数名称：
	 功    能：设置游戏主框架的行列数
	 输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			   const int row						：行数(错误则为0，不设上限，人为保证正确性)
			   const int col						：列数(错误则为0，不设上限，人为保证正确性)
	 返 回 值：
	 说    明：1、指消除类游戏的矩形区域的行列值
			   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
   ***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (row < 0)
		pCGI->row_num = 0;
	if (col < 0)
		pCGI->col_num = 0;
	if (row >= 0 && col >= 0) {
		pCGI->row_num = row;
		pCGI->col_num = col;
	}
	gmw_inner_cal_parameter(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
					前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	if (cascade) {//级联
		pCGI->CFI.bgcolor = bgcolor;//修改主框架信息
		pCGI->CFI.fgcolor = fgcolor;
		pCGI->SLI.top_normal_bgcolor = bgcolor;//修改上状态栏信息
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_bgcolor = bgcolor;//修改下状态栏信息
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
			2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO* const pCGI, const char* fontname, const int fs_high, const int fs_width)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (strcmp(fontname, "新宋体") != 0 && strcmp(fontname, "Terminal") != 0)
		return -1;
	strcpy(pCGI->CFT.font_type, fontname);
	pCGI->CFT.font_size_high = (fs_high < 0) ? 16 : fs_high;
	pCGI->CFT.font_size_width = (fs_width < 0) ? 8 : fs_width;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int delay_ms)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type) {
		case DELAY_OF_DRAW_FRAME:
			pCGI->delay_of_draw_frame = (delay_ms < 0) ? 0 : delay_ms;
			break;
		case DELAY_OF_DRAW_BLOCK:
			pCGI->delay_of_draw_block = (delay_ms < 0) ? 0 : delay_ms;
			break;
		case DELAY_OF_BLOCK_MOVED:
			pCGI->delay_of_block_moved = (delay_ms < BLOCK_MOVED_DELAY_MS) ? BLOCK_MOVED_DELAY_MS : delay_ms;
			break;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->extern_up_lines = (up_lines < 0) ? 0 : up_lines;
	pCGI->extern_down_lines = (down_lines < 0) ? 0 : down_lines;
	pCGI->extern_left_cols = (left_cols < 0) ? 0 : left_cols;
	pCGI->extern_right_cols = (right_cols < 0) ? 0 : right_cols;
	gmw_inner_cal_parameter(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type) {
		case 1://全线
			strcpy(pCGI->CFI.top_left, "╔");
			strcpy(pCGI->CFI.lower_left, "╚");
			strcpy(pCGI->CFI.top_right, "╗");
			strcpy(pCGI->CFI.lower_right, "╝");
			strcpy(pCGI->CFI.h_normal, "═");
			strcpy(pCGI->CFI.v_normal, "║");
			strcpy(pCGI->CFI.h_top_separator, "╦");
			strcpy(pCGI->CFI.h_lower_separator, "╩");
			strcpy(pCGI->CFI.v_left_separator, "╠");
			strcpy(pCGI->CFI.v_right_separator, "╣");
			strcpy(pCGI->CFI.mid_separator, "╬");
			break;
		case 2://全单线
			strcpy(pCGI->CFI.top_left, "┌");
			strcpy(pCGI->CFI.lower_left, "└");
			strcpy(pCGI->CFI.top_right, "┐");
			strcpy(pCGI->CFI.lower_right, "┘");
			strcpy(pCGI->CFI.h_normal, "─");
			strcpy(pCGI->CFI.v_normal, "│");
			strcpy(pCGI->CFI.h_top_separator, "┬");
			strcpy(pCGI->CFI.h_lower_separator, "┴");
			strcpy(pCGI->CFI.v_left_separator, "├");
			strcpy(pCGI->CFI.v_right_separator, "┤");
			strcpy(pCGI->CFI.mid_separator, "┼");
			break;
		case 3://横双竖单
			strcpy(pCGI->CFI.top_left, "╒");
			strcpy(pCGI->CFI.lower_left, "╘");
			strcpy(pCGI->CFI.top_right, "╕");
			strcpy(pCGI->CFI.lower_right, "╛");
			strcpy(pCGI->CFI.h_normal, "═");
			strcpy(pCGI->CFI.v_normal, "│");
			strcpy(pCGI->CFI.h_top_separator, "╤");
			strcpy(pCGI->CFI.h_lower_separator, "╧");
			strcpy(pCGI->CFI.v_left_separator, "╞");
			strcpy(pCGI->CFI.v_right_separator, "╡");
			strcpy(pCGI->CFI.mid_separator, "╪");
			break;
		case 4://横单竖双
			strcpy(pCGI->CFI.top_left, "╓");
			strcpy(pCGI->CFI.lower_left, "╙");
			strcpy(pCGI->CFI.top_right, "╖");
			strcpy(pCGI->CFI.lower_right, "╜");
			strcpy(pCGI->CFI.h_normal, "─");
			strcpy(pCGI->CFI.v_normal, "║");
			strcpy(pCGI->CFI.h_top_separator, "╥");
			strcpy(pCGI->CFI.h_lower_separator, "╨");
			strcpy(pCGI->CFI.v_left_separator, "╟");
			strcpy(pCGI->CFI.v_right_separator, "╢");
			strcpy(pCGI->CFI.mid_separator, "╫");
			break;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right,
	const char* lower_right, const char* h_normal, const char* v_normal, const char* h_top_separator,
	const char* h_lower_separator, const char* v_left_separator, const char* v_right_separator, const char* mid_separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	strcpy(pCGI->CFI.top_left, top_left);
	strcpy(pCGI->CFI.lower_left, lower_left);
	strcpy(pCGI->CFI.top_right, top_right);
	strcpy(pCGI->CFI.lower_right, lower_right);
	strcpy(pCGI->CFI.h_normal, h_normal);
	strcpy(pCGI->CFI.v_normal, v_normal);
	strcpy(pCGI->CFI.h_top_separator, h_top_separator);
	strcpy(pCGI->CFI.h_lower_separator, h_lower_separator);
	strcpy(pCGI->CFI.v_left_separator, v_left_separator);
	strcpy(pCGI->CFI.v_right_separator, v_right_separator);
	strcpy(pCGI->CFI.mid_separator, mid_separator);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high						：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO* const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CFI.block_width = (block_width < 0) ? 2 : (block_width % 2 == 1) ? block_width + 1 : block_width;
	pCGI->CFI.block_high = (block_high < 0) ? 1 : block_high;
	pCGI->CFI.separator = separator;
	gmw_inner_cal_parameter(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CFI.bgcolor = (bgcolor == -1) ? pCGI->area_bgcolor : bgcolor;
	pCGI->CFI.fgcolor = (fgcolor == -1) ? pCGI->area_fgcolor : fgcolor;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type) {
		case 1:
			strcpy(pCGI->CBI.top_left, "╔");
			strcpy(pCGI->CBI.lower_left, "╚");
			strcpy(pCGI->CBI.top_right, "╗");
			strcpy(pCGI->CBI.lower_right, "╝");
			strcpy(pCGI->CBI.h_normal, "═");
			strcpy(pCGI->CBI.v_normal, "║");
			break;
		case 2:
			strcpy(pCGI->CBI.top_left, "┌");
			strcpy(pCGI->CBI.lower_left, "└");
			strcpy(pCGI->CBI.top_right, "┐");
			strcpy(pCGI->CBI.lower_right, "┘");
			strcpy(pCGI->CBI.h_normal, "─");
			strcpy(pCGI->CBI.v_normal, "│");
			break;
		case 3:
			strcpy(pCGI->CBI.top_left, "╒");
			strcpy(pCGI->CBI.lower_left, "╘");
			strcpy(pCGI->CBI.top_right, "╕");
			strcpy(pCGI->CBI.lower_right, "╛");
			strcpy(pCGI->CBI.h_normal, "═");
			strcpy(pCGI->CBI.v_normal, "│");
			break;
		case 4:
			strcpy(pCGI->CBI.top_left, "╓");
			strcpy(pCGI->CBI.lower_left, "╙");
			strcpy(pCGI->CBI.top_right, "╖");
			strcpy(pCGI->CBI.lower_right, "╜");
			strcpy(pCGI->CBI.h_normal, "─");
			strcpy(pCGI->CBI.v_normal, "║");
			break;

	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
static const char* modify(const char* src, char restore[CFI_LEN])
{
	/*填充字符时对NULL/单字节字符进行修饰*/
	if (src == NULL)
		return "  ";
	else if (strlen(src) == 2)
		return src;
	else if (strlen(src) == 1) {
		restore[0] = src[0];
		restore[1] = ' ';
		restore[2] = '\0';
		return (const char*)restore;
	}
	else {
		strncpy(restore, src, 2);
		restore[2] = '\0';
		return (const char*)restore;
	}
}
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right, const char* lower_right, const char* h_normal, const char* v_normal)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	char restore[CFI_LEN] = { 0 };
	strcpy(pCGI->CBI.top_left, modify(top_left, restore));
	strcpy(pCGI->CBI.lower_left, modify(lower_left, restore));
	strcpy(pCGI->CBI.top_right, modify(top_right, restore));
	strcpy(pCGI->CBI.lower_right, modify(lower_right, restore));
	strcpy(pCGI->CBI.h_normal, modify(h_normal, restore));
	strcpy(pCGI->CBI.v_normal, modify(v_normal, restore));
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CBI.block_border = on_off;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (type == TOP_STATUS_LINE) {
		pCGI->SLI.is_top_status_line = on_off;
		pCGI->top_status_line = on_off;
	}

	if (type == LOWER_STATUS_LINE) {
		pCGI->SLI.is_lower_status_line = on_off;
		pCGI->lower_status_line = on_off;

	}	gmw_inner_cal_parameter(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (type == TOP_STATUS_LINE) {
		pCGI->SLI.top_normal_bgcolor = (normal_bgcolor == -1) ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.top_normal_fgcolor = (normal_fgcolor == -1) ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.top_catchy_bgcolor = (catchy_bgcolor == -1) ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.top_catchy_fgcolor = (catchy_fgcolor == -1) ? COLOR_HYELLOW : catchy_fgcolor;
	}
	if (type == LOWER_STATUS_LINE) {
		pCGI->SLI.lower_normal_bgcolor = (normal_bgcolor == -1) ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.lower_normal_fgcolor = (normal_fgcolor == -1) ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = (catchy_bgcolor == -1) ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.lower_catchy_fgcolor = (catchy_fgcolor == -1) ? COLOR_HYELLOW : catchy_fgcolor;

	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
			2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_row_no = on_off;
	gmw_inner_cal_parameter(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
			2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_col_no = on_off;
	gmw_inner_cal_parameter(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
			2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* 首先置标记 */
	pCGI->inited = CGI_INITED;
	/*整体参数设置*/
	gmw_set_rowcol(pCGI, row, col);
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_font(pCGI);
	gmw_set_ext_rowcol(pCGI);
	/*主框架参数*/
	gmw_set_frame_default_linetype(pCGI, 1);
	gmw_set_frame_style(pCGI);
	gmw_set_frame_color(pCGI);
	gmw_set_block_default_linetype(pCGI, 1);
	gmw_set_block_border_switch(pCGI);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, 0);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, 0);
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, BLOCK_MOVED_DELAY_MS);
	/*状态栏参数*/
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE);
	/*行号列号*/
	gmw_set_rowno_switch(pCGI);
	gmw_set_colno_switch(pCGI);
	/*填充计算参数*/
	gmw_inner_cal_parameter(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	cct_cls();

	cct_setconsoleborder(pCGI->cols, pCGI->lines);
	/*游戏主框架:行列显示/色块外框*/
	/*上额外+上状态+列号+主体+下状态+下额外+4*/
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	/*列号*/
	if (pCGI->draw_frame_with_col_no) {
		cct_gotoxy(pCGI->start_x + 1 + pCGI->CFI.block_width / 2, pCGI->start_y - 1);
		for (int i = 0; i < pCGI->col_num; i++) {
			if (i < 100)
				cout << setiosflags(ios::left) << setw(2 * pCGI->CFI.separator + pCGI->CFI.block_width) << i << resetiosflags(ios::left);
			else
				cout << setiosflags(ios::left) << setw(2 * pCGI->CFI.separator + pCGI->CFI.block_width) << "**" << resetiosflags(ios::left);
			Sleep(pCGI->delay_of_draw_frame);
		}
	}
	/*行号*/
	if (pCGI->draw_frame_with_row_no)
		for (int i = 0; i < pCGI->row_num; i++) {
			cct_gotoxy(pCGI->extern_left_cols, pCGI->start_y + pCGI->CFI.separator + pCGI->CFI.block_high / 2 +
				i * (1 * pCGI->CFI.separator + pCGI->CFI.block_high));
			if (i < 26)
				cout << char(i + 'A') << ' ';
			else if (i < 52)
				cout << char(i - 26 + 'a') << ' ';
			else
				cout << '*' << ' ';
			Sleep(pCGI->delay_of_draw_frame);
		}

	/*主框架*/
	cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	/*分有无分割线两种情况绘制*/
	if (pCGI->CFI.separator) {//有分割线
		for (int i = 0; i < pCGI->CFI.bhigh; i++) {
			cct_gotoxy(pCGI->start_x, pCGI->start_y + i);
			for (int j = 0; j < pCGI->CFI.bwidth / 2; j++) {
				/*首行*/
				if (i == 0) {
					if (j == 0)//左上角
						cout << pCGI->CFI.top_left;
					else if (j == pCGI->CFI.bwidth / 2 - 1)//右上角
						cout << pCGI->CFI.top_right;
					else if (j % (pCGI->CFI.block_width / 2 + 1) == 0)//上分界
						cout << pCGI->CFI.h_top_separator;
					else//水平线
						cout << pCGI->CFI.h_normal;
				}
				/*末行*/
				else if (i == pCGI->CFI.bhigh - 1) {
					if (j == 0)//左下角
						cout << pCGI->CFI.lower_left;
					else if (j == pCGI->CFI.bwidth / 2 - 1)//右下角
						cout << pCGI->CFI.lower_right;
					else if (j % (pCGI->CFI.block_width / 2 + 1) == 0)//下分界
						cout << pCGI->CFI.h_lower_separator;
					else//水平线
						cout << pCGI->CFI.h_normal;
				}
				/*中间横行*/
				else if (i % (pCGI->CFI.block_high + 1) == 0) {
					if (j == 0)//左分界
						cout << pCGI->CFI.v_left_separator;
					else if (j == pCGI->CFI.bwidth / 2 - 1)//右分界
						cout << pCGI->CFI.v_right_separator;
					else if (j % (pCGI->CFI.block_width / 2 + 1) == 0)//中分界
						cout << pCGI->CFI.mid_separator;
					else//水平线
						cout << pCGI->CFI.h_normal;
				}
				/*中间格子行*/
				else {
					if (j % (pCGI->CFI.block_width / 2 + 1) == 0)
						cout << pCGI->CFI.v_normal;
					else
						cout << "  ";
				}
				Sleep(pCGI->delay_of_draw_frame);
			}
		}

	}
	else {//无分界线
		for (int i = 0; i < pCGI->CFI.bhigh; i++) {
			cct_gotoxy(pCGI->start_x, pCGI->start_y + i);
			for (int j = 0; j < pCGI->CFI.bwidth / 2; j++) {
				if (i == 0 && j == 0)//左上角
					cout << pCGI->CFI.top_left;
				else if (i == pCGI->CFI.bhigh - 1 && j == 0)//左下角
					cout << pCGI->CFI.lower_left;
				else if (i == 0 && j == pCGI->CFI.bwidth / 2 - 1)//右上角
					cout << pCGI->CFI.top_right;
				else if (i == pCGI->CFI.bhigh - 1 && j == pCGI->CFI.bwidth / 2 - 1)//右下角
					cout << pCGI->CFI.lower_right;
				else if (i == 0 || i == pCGI->CFI.bhigh - 1)//水平线
					cout << pCGI->CFI.h_normal;
				else if (j == 0 || j == pCGI->CFI.bwidth / 2 - 1)//竖直线
					cout << pCGI->CFI.v_normal;
				else
					cout << "  ";
				Sleep(pCGI->delay_of_draw_frame);
			}
		}
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
			2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
static void half_chinesechar(char* s)
{
	/*判断最后一个字符是否为中文的第一个字节,即是否有截断(截断为true),调用前保证s非空串*/
	/*更新:改为直接处理字符串的可能截断,不返回bool*/
	char* last = s;
	while (*last != 0)
		last++;
	last = last - 1;//第一个尾零前的字符,即最末字符
	if (*last > 0)//ascii字符,无截断无需处理
		return;
	else {
		char* ps = s;
		while (ps < last) {
			if (*ps > 0)
				ps = ps + 1;
			else if (*ps < 0)
				ps = ps + 2;
		}
		if (ps > last)
			return;//无截断不处理
		else
			*last = 0;//末字符赋值为0
	}
}
int gmw_status_line(const CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const char* msg, const char* catchy_msg)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == TOP_STATUS_LINE && pCGI->SLI.is_top_status_line == false)
		return 0;
	if (type == LOWER_STATUS_LINE && pCGI->SLI.is_lower_status_line == false)
		return 0;

	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	int restrict = 2 * pCGI->draw_frame_with_row_no + pCGI->CFI.bwidth;//宽度限制
	int len = msg == NULL ? 0 : strlen(msg);
	int catchy_len = catchy_msg == NULL ? 0 : strlen(catchy_msg);
	/*动态内存申请*/
	char* ss = new char[restrict + 1]();
	if (ss == NULL)
		return -1;
	char* catchy_ss = new char[restrict + 1]();
	if (catchy_ss == NULL)
		return -1;

	if (len != 0)
	{
		/*情况2:普通非空,高光为空*,因为参数顺序不存在高光非空,普通为空的情况*/
		if (catchy_len == 0) {
			strncpy(ss, msg, restrict);
			half_chinesechar(ss);//截断处理
		}
		/*情况3:普通非空,高光非空*/
		else {
			if (catchy_len >= restrict) {//高光信息超出限制,只输出高光
				strncpy(catchy_ss, catchy_msg, restrict);
				half_chinesechar(catchy_ss);
			}
			else {
				int restrict2 = restrict - catchy_len;//显示完高光显示普通
				strcpy(catchy_ss, catchy_msg);
				strncpy(ss, msg, restrict2);
				half_chinesechar(ss);
			}
		}
	}

	if (type == TOP_STATUS_LINE) {
		cct_gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);
		cct_setcolor(pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
		cout << catchy_ss;
		cct_setcolor(pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
		cout << ss;
		if (int(strlen(ss) + strlen(catchy_ss)) < restrict)
			cout << setw(restrict - strlen(ss) - strlen(catchy_ss)) << " ";
	}
	if (type == LOWER_STATUS_LINE) {
		cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);
		cct_setcolor(pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
		cout << catchy_ss;
		cct_setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
		cout << ss;
		if (int(strlen(ss) + strlen(catchy_ss)) < restrict)
			cout << setw(restrict - strlen(ss) - strlen(catchy_ss)) << " ";
	}
	delete[]ss, catchy_ss;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
			2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
static void draw_block_xy(const CONSOLE_GRAPHICS_INFO* const pCGI, const int block_start_x, const int block_start_y, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi, bool need_delay)
{
	/*给定一个色块起始坐标画出色块*/
	/*设置色块信息*/
	const BLOCK_DISPLAY_INFO* pbdi = bdi;
	while (pbdi->value != bdi_value && pbdi->value != BDI_VALUE_END)
		pbdi++;

	/*填充颜色*/
	int block_bg_color = (pbdi->bgcolor == -1) ? pCGI->CFI.bgcolor : pbdi->bgcolor;
	int block_fg_color = (pbdi->fgcolor == -1) ? pCGI->CFI.fgcolor : pbdi->fgcolor;
	cct_setcolor(block_bg_color, block_fg_color);

	/*填充色块*/
	// 有边框
	if (pCGI->CBI.block_border && bdi_value != BDI_VALUE_BLANK) {

		for (int i = 0; i < pCGI->CFI.block_high; i++) {
			cct_gotoxy(block_start_x, block_start_y + i);
			for (int j = 0; j < pCGI->CFI.block_width / 2; j++) {
				//首行
				if (i == 0) {
					if (j == 0)
						cout << pCGI->CBI.top_left;//左上角
					else if (j == pCGI->CFI.block_width / 2 - 1)
						cout << pCGI->CBI.top_right;//右上角
					else
						cout << pCGI->CBI.h_normal;
				}
				//末行
				else if (i == pCGI->CFI.block_high - 1) {
					if (j == 0)
						cout << pCGI->CBI.lower_left;//左下角
					else if (j == pCGI->CFI.block_width / 2 - 1)
						cout << pCGI->CBI.lower_right;//右下角
					else
						cout << pCGI->CBI.h_normal;
				}
				//普通行
				else {
					if (j == 0 || j == pCGI->CFI.block_width / 2 - 1)
						cout << pCGI->CBI.v_normal;
					else
						cout << "  ";
				}
				if (need_delay)
					Sleep(pCGI->delay_of_draw_block);
			}
		}

	}
	//无边框
	else {
		for (int i = 0; i < pCGI->CFI.block_high; i++) {
			cct_gotoxy(block_start_x, block_start_y + i);
			for (int j = 0; j < pCGI->CFI.block_width / 2; j++) {
				cout << "  ";
				if (need_delay)
					Sleep(pCGI->delay_of_draw_block);
			}
		}
	}

	/*填充字符*/
	int char_start_x, char_start_y;

	if (pbdi->content == NULL) {
		if (bdi_value != BDI_VALUE_BLANK) {//=0不显示
			char_start_x = block_start_x + 2 * pCGI->CBI.block_border;
			char_start_y = block_start_y + (pCGI->CFI.block_high - 1) / 2;
			char* value = new char[pCGI->CFI.block_width + 1]();
			sprintf(value, "%d", bdi_value);
			int value_len = strlen(value);
			int blank_len = (pCGI->CFI.block_width - 2 * 2 * pCGI->CBI.block_border - value_len) / 2;
			cct_gotoxy(char_start_x, char_start_y);
			if (blank_len > 0)
				cout << setw(blank_len) << ' ';
			cout << value;
			delete[]value;
		}

	}

	else {
		char_start_x = block_start_x + (pCGI->CFI.block_width - 1) / 2;
		char_start_y = block_start_y + (pCGI->CFI.block_high - 1) / 2;
		cct_gotoxy(char_start_x, char_start_y);
		cout << pbdi->content;
	}
	return;
}

int gmw_draw_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*找位置*/
	int block_start_x0 = pCGI->start_x + 2;
	int block_start_y0 = pCGI->start_y + 1;//有无分界线都是一样
	int block_start_x = block_start_x0 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);//给定行列(row_no,col_no)位置的色块起始坐标
	int block_start_y = block_start_y0 + row_no * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);
	draw_block_xy(pCGI, block_start_x, block_start_y, bdi_value, bdi);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO* const bdi, const int direction, const int distance)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*找位置*/
	int block_start_x0 = pCGI->start_x + 2;
	int block_start_y0 = pCGI->start_y + 1;//有无分界线都是一样
	int block_start_x = block_start_x0 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);//给定行列(row_no,col_no)位置的色块起始坐标
	int block_start_y = block_start_y0 + row_no * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);
	int block_end_x, block_end_y;
	//有分割线
	if (pCGI->CFI.separator) {
		switch (direction) {

			case DOWN_TO_UP:
				/*  ↑  */
				block_end_x = block_start_x;
				block_end_y = block_start_y0 + (row_no - distance) * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);

				for (int y = block_start_y - 1; y >= block_end_y; y--) {
					draw_block_xy(pCGI, block_start_x, y, bdi_value, bdi, false);

					cct_gotoxy(block_start_x, y + pCGI->CFI.block_high);//留下的空行的位置
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int i = 0; i < pCGI->CFI.block_width / 2; i++)
						if ((block_start_y - y) % (pCGI->CFI.block_high + 1 * pCGI->CFI.separator) == 0) //画横条分界线
							cout << pCGI->CFI.h_normal;
						else //画空白
							cout << "  ";
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case UP_TO_DOWN:
				/*  ↓  */
				block_end_x = block_start_x;
				block_end_y = block_start_y0 + (row_no + distance) * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);

				for (int y = block_start_y + 1; y <= block_end_y; y++) {
					draw_block_xy(pCGI, block_start_x, y, bdi_value, bdi, false);
					cct_gotoxy(block_start_x, y - 1);//留下的空行的位置
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int i = 0; i < pCGI->CFI.block_width / 2; i++)
						if ((y - block_start_y) % (pCGI->CFI.block_high + 1 * pCGI->CFI.separator) == 0) //画横条分界线
							cout << pCGI->CFI.h_normal;
						else //画空白
							cout << "  ";
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case RIGHT_TO_LEFT:
				/*  ←  */
				block_end_x = block_start_x0 + (col_no - distance) * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);//distance表行列,因此横向时要除2
				block_end_y = block_start_y;

				for (int x = block_start_x - 2; x >= block_end_x; x -= 2) {//横坐标变化每次移动两个单位
					draw_block_xy(pCGI, x, block_start_y, bdi_value, bdi, false);
					for (int i = 0; i < pCGI->CFI.block_high; i++) {
						cct_gotoxy(x + pCGI->CFI.block_width, block_start_y + i);//留下的空行的位置
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						if (((block_start_x - x) / 2) % (pCGI->CFI.block_width / 2 + 1 * pCGI->CFI.separator) == 0)//画竖条分界线
							cout << pCGI->CFI.v_normal;
						else//画空白
							cout << "  ";
					}
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case LEFT_TO_RIGHT:
				/*  →  */
				block_end_x = block_start_x0 + (col_no + distance) * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
				block_end_y = block_start_y;

				for (int x = block_start_x + 2; x <= block_end_x; x += 2) {//横坐标变化每次移动两个单位
					draw_block_xy(pCGI, x, block_start_y, bdi_value, bdi, false);
					for (int i = 0; i < pCGI->CFI.block_high; i++) {
						cct_gotoxy(x - 2, block_start_y + i);//留下的空行的位置
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						if (((x - block_start_x) / 2) % (pCGI->CFI.block_width / 2 + 1 * pCGI->CFI.separator) == 0)//画竖条分界线
							cout << pCGI->CFI.v_normal;
						else//画空白
							cout << "  ";
					}
					Sleep(pCGI->delay_of_block_moved);
				}
				break;
		}
	}

	//无分割线
	else {
		switch (direction) {

			case DOWN_TO_UP:
				/*  ↑  */
				block_end_x = block_start_x;
				block_end_y = block_start_y0 + (row_no - distance) * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);

				for (int y = block_start_y - 1; y >= block_end_y; y--) {
					draw_block_xy(pCGI, block_start_x, y, bdi_value, bdi, false);

					cct_gotoxy(block_start_x, y + pCGI->CFI.block_high);//留下的空行的位置
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int i = 0; i < pCGI->CFI.block_width / 2; i++)
						cout << "  ";
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case UP_TO_DOWN:
				/*  ↓  */
				block_end_x = block_start_x;
				block_end_y = block_start_y0 + (row_no + distance) * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);

				for (int y = block_start_y + 1; y <= block_end_y; y++) {
					draw_block_xy(pCGI, block_start_x, y, bdi_value, bdi, false);
					cct_gotoxy(block_start_x, y - 1);//留下的空行的位置
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int i = 0; i < pCGI->CFI.block_width / 2; i++)
						cout << "  ";
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case RIGHT_TO_LEFT:
				/*  ←  */
				block_end_x = block_start_x0 + (col_no - distance) * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);//distance表行列,因此横向时要除2
				block_end_y = block_start_y;

				for (int x = block_start_x - 2; x >= block_end_x; x -= 2) {//横坐标变化每次移动两个单位
					draw_block_xy(pCGI, x, block_start_y, bdi_value, bdi, false);
					for (int i = 0; i < pCGI->CFI.block_high; i++) {
						cct_gotoxy(x + pCGI->CFI.block_width, block_start_y + i);//留下的空行的位置
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						cout << "  ";
					}
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case LEFT_TO_RIGHT:
				/*  →  */
				block_end_x = block_start_x0 + (col_no + distance) * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
				block_end_y = block_start_y;

				for (int x = block_start_x + 2; x <= block_end_x; x += 2) {//横坐标变化每次移动两个单位
					draw_block_xy(pCGI, x, block_start_y, bdi_value, bdi, false);
					for (int i = 0; i < pCGI->CFI.block_high; i++) {
						cct_gotoxy(x - 2, block_start_y + i);//留下的空行的位置
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						cout << "  ";
					}
					Sleep(pCGI->delay_of_block_moved);
				}
				break;
		}

	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 三者之一
													   如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列/位置非法"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
		   1、如果是鼠标移动，得到的MRow/MCol与传入的相同(鼠标指针微小的移动)，则不返回，继续读
							  得到行列非法位置，则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
							  得到的MRow/MCol与传入的不同(行列至少一个变化)，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"，再返回MOUSE_ONLY_MOVED（有些游戏返回后要处理色块的不同颜色显示）
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
								  且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow、MCol不可信
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/

static int convert_xy_to_rc(const CONSOLE_GRAPHICS_INFO* const pCGI, int MX, int MY, int& MRow, int& MCol)
{
	/*将xy坐标转换成游戏区域的行列号,若为非法区域则置为INVALID_POS且返回-1,否则返回0*/

	//找位置
	int block_start_x0 = pCGI->start_x + 2;
	int block_start_y0 = pCGI->start_y + 1;//有无分界线都是一样

	int block_start_x, block_start_y, i, j;
	//如果循环中找到,则修改值,否则以非法值返回
	MRow = MCol = INVALID_POS;

	for (i = 0; i < pCGI->row_num; i++)
		for (j = 0; j < pCGI->col_num; j++) {
			//给定行列(row_no,col_no)位置的色块起始坐标
			block_start_y = block_start_y0 + i * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);
			block_start_x = block_start_x0 + j * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);

			if (block_start_x <= MX && MX < block_start_x + pCGI->CFI.block_width &&
				block_start_y <= MY && MY < block_start_y + pCGI->CFI.block_high) {
				MRow = i;
				MCol = j;
				return 0;//合法
			}
		}
	return -1;//非法
}

int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO* const pCGI, int& MAction, int& MRow, int& MCol, int& KeyCode1, int& KeyCode2, const bool update_lower_status_line)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int MX, MY, odd_MRow, odd_MCol, event;
	while (1) {
		odd_MRow = MRow;
		odd_MCol = MCol;
		event = cct_read_keyboard_and_mouse(MX, MY, MAction, KeyCode1, KeyCode2);
		//鼠标事件
		if (event == CCT_MOUSE_EVENT) {
			/*鼠标移动*/
			if (MAction == MOUSE_ONLY_MOVED) {
				int legal = convert_xy_to_rc(pCGI, MX, MY, MRow, MCol);
				if (legal == -1) {//非法
					if (update_lower_status_line == true) //要求更新下状态栏
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");
				}
				else {//位置合法
					if (!(odd_MRow == MRow && odd_MCol == MCol)) {//和老坐标有变动!
						if (update_lower_status_line == true) {
							char display[30] = { 0 };
							char chRow = MRow + 'A';
							sprintf(display, "[当前光标] %c行%d列", chRow, MCol);
							gmw_status_line(pCGI, LOWER_STATUS_LINE, display);
						}
						return  CCT_MOUSE_EVENT;//返回
					}
					//其余情况继续循环
				}
			}
			/*鼠标左击/右击*/
			else if (MAction == MOUSE_LEFT_BUTTON_CLICK|| MAction == MOUSE_RIGHT_BUTTON_CLICK) {
				int legal = convert_xy_to_rc(pCGI, MX, MY, MRow, MCol);
				if (legal == -1) {//非法
					if (update_lower_status_line == true) //要求更新下状态栏
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");
				}
				else {//位置合法
					return  CCT_MOUSE_EVENT;//返回
				}
			}
		}

		//键盘事件
		else {
			return CCT_KEYBOARD_EVENT;//返回
		}
	}

}
