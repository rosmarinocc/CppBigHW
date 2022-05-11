/*1852877 �ƿ� ��껈�*/
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
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */


   /* --------------------------------------------------
		   �˴����Ը�����Ҫ���ڲ��������ߺ���
		   1�����������ޣ�����Ϊ gmw_inner_*
		   2����������
		   3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
	  -------------------------------------------------- */
	  /*****************************01.��xy����ת��Ϊ��Ӧ����***********************************/
static int convert_xy_to_rc(const CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int& MRow, int& MCol);
/*****************************02.������ܵ����Ľض�***********************************/
static void half_chinesechar(char* s);
/*****************************03.����ܷ��ű�֤Ϊ���2***********************************/
static const char* modify(const char* src, char restore[CFI_LEN]);
/*****************************04.����ʼxy���괦������***********************************/
static void draw_block_xy(const CONSOLE_GRAPHICS_INFO* const pCGI, const int block_start_x, const int block_start_y, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi, bool need_delay = true);
/*****************************05.���㲻��ֱ�����õĲ���***********************************/
static void gmw_inner_cal_parameter(CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/*���㲻��ֱ��ͨ���������õõ��Ĳ���*/

	if (pCGI->CFI.separator) {
		//����ܿ��
		pCGI->CFI.bwidth = (pCGI->CFI.block_width + pCGI->CFI.separator * 2) * pCGI->col_num + 2;
		//����ܸ߶�
		pCGI->CFI.bhigh = (pCGI->CFI.block_high + pCGI->CFI.separator * 1) * pCGI->row_num + 1;
	}
	else {
		pCGI->CFI.bwidth = pCGI->CFI.block_width * pCGI->col_num + 2 * 2;
		pCGI->CFI.bhigh = pCGI->CFI.block_high * pCGI->row_num + 1 * 2;
	}
	//�����ʼx����
	pCGI->start_x = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no;
	//�����ʼy����
	pCGI->start_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
	//��Ļ�ܸ߶�
	pCGI->lines = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no +
		pCGI->CFI.bhigh + pCGI->lower_status_line + pCGI->extern_down_lines + 4;
	//��Ļ�ܿ��
	pCGI->cols = pCGI->extern_left_cols + 2 * pCGI->draw_frame_with_row_no +
		pCGI->CFI.bwidth + pCGI->extern_right_cols + 1;
	//��״̬����ʼx����
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	//��״̬����ʼy����
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	//��״̬����ʼx����
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	//��״̬����ʼy����
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh;
}
/* -----------------------------------------------
		ʵ����������ĺ���������������׼����
   ----------------------------------------------- */
   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ�������Ϸ����ܵ�������
	 ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			   const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
			   const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
	 �� �� ֵ��
	 ˵    ����1��ָ��������Ϸ�ľ������������ֵ
			   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
   ***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
					ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	if (cascade) {//����
		pCGI->CFI.bgcolor = bgcolor;//�޸��������Ϣ
		pCGI->CFI.fgcolor = fgcolor;
		pCGI->SLI.top_normal_bgcolor = bgcolor;//�޸���״̬����Ϣ
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_bgcolor = bgcolor;//�޸���״̬����Ϣ
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
			2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO* const pCGI, const char* fontname, const int fs_high, const int fs_width)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (strcmp(fontname, "������") != 0 && strcmp(fontname, "Terminal") != 0)
		return -1;
	strcpy(pCGI->CFT.font_type, fontname);
	pCGI->CFT.font_size_high = (fs_high < 0) ? 16 : fs_high;
	pCGI->CFT.font_size_width = (fs_width < 0) ? 8 : fs_width;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int delay_ms)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->extern_up_lines = (up_lines < 0) ? 0 : up_lines;
	pCGI->extern_down_lines = (down_lines < 0) ? 0 : down_lines;
	pCGI->extern_left_cols = (left_cols < 0) ? 0 : left_cols;
	pCGI->extern_right_cols = (right_cols < 0) ? 0 : right_cols;
	gmw_inner_cal_parameter(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type) {
		case 1://ȫ��
			strcpy(pCGI->CFI.top_left, "�X");
			strcpy(pCGI->CFI.lower_left, "�^");
			strcpy(pCGI->CFI.top_right, "�[");
			strcpy(pCGI->CFI.lower_right, "�a");
			strcpy(pCGI->CFI.h_normal, "�T");
			strcpy(pCGI->CFI.v_normal, "�U");
			strcpy(pCGI->CFI.h_top_separator, "�j");
			strcpy(pCGI->CFI.h_lower_separator, "�m");
			strcpy(pCGI->CFI.v_left_separator, "�d");
			strcpy(pCGI->CFI.v_right_separator, "�g");
			strcpy(pCGI->CFI.mid_separator, "�p");
			break;
		case 2://ȫ����
			strcpy(pCGI->CFI.top_left, "��");
			strcpy(pCGI->CFI.lower_left, "��");
			strcpy(pCGI->CFI.top_right, "��");
			strcpy(pCGI->CFI.lower_right, "��");
			strcpy(pCGI->CFI.h_normal, "��");
			strcpy(pCGI->CFI.v_normal, "��");
			strcpy(pCGI->CFI.h_top_separator, "��");
			strcpy(pCGI->CFI.h_lower_separator, "��");
			strcpy(pCGI->CFI.v_left_separator, "��");
			strcpy(pCGI->CFI.v_right_separator, "��");
			strcpy(pCGI->CFI.mid_separator, "��");
			break;
		case 3://��˫����
			strcpy(pCGI->CFI.top_left, "�V");
			strcpy(pCGI->CFI.lower_left, "�\");
			strcpy(pCGI->CFI.top_right, "�Y");
			strcpy(pCGI->CFI.lower_right, "�_");
			strcpy(pCGI->CFI.h_normal, "�T");
			strcpy(pCGI->CFI.v_normal, "��");
			strcpy(pCGI->CFI.h_top_separator, "�h");
			strcpy(pCGI->CFI.h_lower_separator, "�k");
			strcpy(pCGI->CFI.v_left_separator, "�b");
			strcpy(pCGI->CFI.v_right_separator, "�e");
			strcpy(pCGI->CFI.mid_separator, "�n");
			break;
		case 4://�ᵥ��˫
			strcpy(pCGI->CFI.top_left, "�W");
			strcpy(pCGI->CFI.lower_left, "�]");
			strcpy(pCGI->CFI.top_right, "�Z");
			strcpy(pCGI->CFI.lower_right, "�`");
			strcpy(pCGI->CFI.h_normal, "��");
			strcpy(pCGI->CFI.v_normal, "�U");
			strcpy(pCGI->CFI.h_top_separator, "�i");
			strcpy(pCGI->CFI.h_lower_separator, "�l");
			strcpy(pCGI->CFI.v_left_separator, "�c");
			strcpy(pCGI->CFI.v_right_separator, "�f");
			strcpy(pCGI->CFI.mid_separator, "�o");
			break;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right,
	const char* lower_right, const char* h_normal, const char* v_normal, const char* h_top_separator,
	const char* h_lower_separator, const char* v_left_separator, const char* v_right_separator, const char* mid_separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high						���߶ȣ�����ȱʡ1��
			const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO* const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CFI.block_width = (block_width < 0) ? 2 : (block_width % 2 == 1) ? block_width + 1 : block_width;
	pCGI->CFI.block_high = (block_high < 0) ? 1 : block_high;
	pCGI->CFI.separator = separator;
	gmw_inner_cal_parameter(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CFI.bgcolor = (bgcolor == -1) ? pCGI->area_bgcolor : bgcolor;
	pCGI->CFI.fgcolor = (fgcolor == -1) ? pCGI->area_fgcolor : fgcolor;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type) {
		case 1:
			strcpy(pCGI->CBI.top_left, "�X");
			strcpy(pCGI->CBI.lower_left, "�^");
			strcpy(pCGI->CBI.top_right, "�[");
			strcpy(pCGI->CBI.lower_right, "�a");
			strcpy(pCGI->CBI.h_normal, "�T");
			strcpy(pCGI->CBI.v_normal, "�U");
			break;
		case 2:
			strcpy(pCGI->CBI.top_left, "��");
			strcpy(pCGI->CBI.lower_left, "��");
			strcpy(pCGI->CBI.top_right, "��");
			strcpy(pCGI->CBI.lower_right, "��");
			strcpy(pCGI->CBI.h_normal, "��");
			strcpy(pCGI->CBI.v_normal, "��");
			break;
		case 3:
			strcpy(pCGI->CBI.top_left, "�V");
			strcpy(pCGI->CBI.lower_left, "�\");
			strcpy(pCGI->CBI.top_right, "�Y");
			strcpy(pCGI->CBI.lower_right, "�_");
			strcpy(pCGI->CBI.h_normal, "�T");
			strcpy(pCGI->CBI.v_normal, "��");
			break;
		case 4:
			strcpy(pCGI->CBI.top_left, "�W");
			strcpy(pCGI->CBI.lower_left, "�]");
			strcpy(pCGI->CBI.top_right, "�Z");
			strcpy(pCGI->CBI.lower_right, "�`");
			strcpy(pCGI->CBI.h_normal, "��");
			strcpy(pCGI->CBI.v_normal, "�U");
			break;

	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
static const char* modify(const char* src, char restore[CFI_LEN])
{
	/*����ַ�ʱ��NULL/���ֽ��ַ���������*/
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
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	char restore[CFI_LEN] = { 0 };
	strcpy(pCGI->CBI.top_left, modify(top_left, restore));
	strcpy(pCGI->CBI.lower_left, modify(lower_left, restore));
	strcpy(pCGI->CBI.top_right, modify(top_right, restore));
	strcpy(pCGI->CBI.lower_right, modify(lower_right, restore));
	strcpy(pCGI->CBI.h_normal, modify(h_normal, restore));
	strcpy(pCGI->CBI.v_normal, modify(v_normal, restore));
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CBI.block_border = on_off;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			   1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			   1.2����״̬��ֻ��һ�У������������·����ߵ�����
			   1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
			2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_row_no = on_off;
	gmw_inner_cal_parameter(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
			2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_col_no = on_off;
	gmw_inner_cal_parameter(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
			2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* �����ñ�� */
	pCGI->inited = CGI_INITED;
	/*�����������*/
	gmw_set_rowcol(pCGI, row, col);
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_font(pCGI);
	gmw_set_ext_rowcol(pCGI);
	/*����ܲ���*/
	gmw_set_frame_default_linetype(pCGI, 1);
	gmw_set_frame_style(pCGI);
	gmw_set_frame_color(pCGI);
	gmw_set_block_default_linetype(pCGI, 1);
	gmw_set_block_border_switch(pCGI);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, 0);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, 0);
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, BLOCK_MOVED_DELAY_MS);
	/*״̬������*/
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE);
	/*�к��к�*/
	gmw_set_rowno_switch(pCGI);
	gmw_set_colno_switch(pCGI);
	/*���������*/
	gmw_inner_cal_parameter(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	cct_cls();

	cct_setconsoleborder(pCGI->cols, pCGI->lines);
	/*��Ϸ�����:������ʾ/ɫ�����*/
	/*�϶���+��״̬+�к�+����+��״̬+�¶���+4*/
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	/*�к�*/
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
	/*�к�*/
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

	/*�����*/
	cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	/*�����޷ָ��������������*/
	if (pCGI->CFI.separator) {//�зָ���
		for (int i = 0; i < pCGI->CFI.bhigh; i++) {
			cct_gotoxy(pCGI->start_x, pCGI->start_y + i);
			for (int j = 0; j < pCGI->CFI.bwidth / 2; j++) {
				/*����*/
				if (i == 0) {
					if (j == 0)//���Ͻ�
						cout << pCGI->CFI.top_left;
					else if (j == pCGI->CFI.bwidth / 2 - 1)//���Ͻ�
						cout << pCGI->CFI.top_right;
					else if (j % (pCGI->CFI.block_width / 2 + 1) == 0)//�Ϸֽ�
						cout << pCGI->CFI.h_top_separator;
					else//ˮƽ��
						cout << pCGI->CFI.h_normal;
				}
				/*ĩ��*/
				else if (i == pCGI->CFI.bhigh - 1) {
					if (j == 0)//���½�
						cout << pCGI->CFI.lower_left;
					else if (j == pCGI->CFI.bwidth / 2 - 1)//���½�
						cout << pCGI->CFI.lower_right;
					else if (j % (pCGI->CFI.block_width / 2 + 1) == 0)//�·ֽ�
						cout << pCGI->CFI.h_lower_separator;
					else//ˮƽ��
						cout << pCGI->CFI.h_normal;
				}
				/*�м����*/
				else if (i % (pCGI->CFI.block_high + 1) == 0) {
					if (j == 0)//��ֽ�
						cout << pCGI->CFI.v_left_separator;
					else if (j == pCGI->CFI.bwidth / 2 - 1)//�ҷֽ�
						cout << pCGI->CFI.v_right_separator;
					else if (j % (pCGI->CFI.block_width / 2 + 1) == 0)//�зֽ�
						cout << pCGI->CFI.mid_separator;
					else//ˮƽ��
						cout << pCGI->CFI.h_normal;
				}
				/*�м������*/
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
	else {//�޷ֽ���
		for (int i = 0; i < pCGI->CFI.bhigh; i++) {
			cct_gotoxy(pCGI->start_x, pCGI->start_y + i);
			for (int j = 0; j < pCGI->CFI.bwidth / 2; j++) {
				if (i == 0 && j == 0)//���Ͻ�
					cout << pCGI->CFI.top_left;
				else if (i == pCGI->CFI.bhigh - 1 && j == 0)//���½�
					cout << pCGI->CFI.lower_left;
				else if (i == 0 && j == pCGI->CFI.bwidth / 2 - 1)//���Ͻ�
					cout << pCGI->CFI.top_right;
				else if (i == pCGI->CFI.bhigh - 1 && j == pCGI->CFI.bwidth / 2 - 1)//���½�
					cout << pCGI->CFI.lower_right;
				else if (i == 0 || i == pCGI->CFI.bhigh - 1)//ˮƽ��
					cout << pCGI->CFI.h_normal;
				else if (j == 0 || j == pCGI->CFI.bwidth / 2 - 1)//��ֱ��
					cout << pCGI->CFI.v_normal;
				else
					cout << "  ";
				Sleep(pCGI->delay_of_draw_frame);
			}
		}
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
			2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
static void half_chinesechar(char* s)
{
	/*�ж����һ���ַ��Ƿ�Ϊ���ĵĵ�һ���ֽ�,���Ƿ��нض�(�ض�Ϊtrue),����ǰ��֤s�ǿմ�*/
	/*����:��Ϊֱ�Ӵ����ַ����Ŀ��ܽض�,������bool*/
	char* last = s;
	while (*last != 0)
		last++;
	last = last - 1;//��һ��β��ǰ���ַ�,����ĩ�ַ�
	if (*last > 0)//ascii�ַ�,�޽ض����账��
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
			return;//�޽ضϲ�����
		else
			*last = 0;//ĩ�ַ���ֵΪ0
	}
}
int gmw_status_line(const CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const char* msg, const char* catchy_msg)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == TOP_STATUS_LINE && pCGI->SLI.is_top_status_line == false)
		return 0;
	if (type == LOWER_STATUS_LINE && pCGI->SLI.is_lower_status_line == false)
		return 0;

	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	int restrict = 2 * pCGI->draw_frame_with_row_no + pCGI->CFI.bwidth;//�������
	int len = msg == NULL ? 0 : strlen(msg);
	int catchy_len = catchy_msg == NULL ? 0 : strlen(catchy_msg);
	/*��̬�ڴ�����*/
	char* ss = new char[restrict + 1]();
	if (ss == NULL)
		return -1;
	char* catchy_ss = new char[restrict + 1]();
	if (catchy_ss == NULL)
		return -1;

	if (len != 0)
	{
		/*���2:��ͨ�ǿ�,�߹�Ϊ��*,��Ϊ����˳�򲻴��ڸ߹�ǿ�,��ͨΪ�յ����*/
		if (catchy_len == 0) {
			strncpy(ss, msg, restrict);
			half_chinesechar(ss);//�ضϴ���
		}
		/*���3:��ͨ�ǿ�,�߹�ǿ�*/
		else {
			if (catchy_len >= restrict) {//�߹���Ϣ��������,ֻ����߹�
				strncpy(catchy_ss, catchy_msg, restrict);
				half_chinesechar(catchy_ss);
			}
			else {
				int restrict2 = restrict - catchy_len;//��ʾ��߹���ʾ��ͨ
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
			2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
static void draw_block_xy(const CONSOLE_GRAPHICS_INFO* const pCGI, const int block_start_x, const int block_start_y, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi, bool need_delay)
{
	/*����һ��ɫ����ʼ���껭��ɫ��*/
	/*����ɫ����Ϣ*/
	const BLOCK_DISPLAY_INFO* pbdi = bdi;
	while (pbdi->value != bdi_value && pbdi->value != BDI_VALUE_END)
		pbdi++;

	/*�����ɫ*/
	int block_bg_color = (pbdi->bgcolor == -1) ? pCGI->CFI.bgcolor : pbdi->bgcolor;
	int block_fg_color = (pbdi->fgcolor == -1) ? pCGI->CFI.fgcolor : pbdi->fgcolor;
	cct_setcolor(block_bg_color, block_fg_color);

	/*���ɫ��*/
	// �б߿�
	if (pCGI->CBI.block_border && bdi_value != BDI_VALUE_BLANK) {

		for (int i = 0; i < pCGI->CFI.block_high; i++) {
			cct_gotoxy(block_start_x, block_start_y + i);
			for (int j = 0; j < pCGI->CFI.block_width / 2; j++) {
				//����
				if (i == 0) {
					if (j == 0)
						cout << pCGI->CBI.top_left;//���Ͻ�
					else if (j == pCGI->CFI.block_width / 2 - 1)
						cout << pCGI->CBI.top_right;//���Ͻ�
					else
						cout << pCGI->CBI.h_normal;
				}
				//ĩ��
				else if (i == pCGI->CFI.block_high - 1) {
					if (j == 0)
						cout << pCGI->CBI.lower_left;//���½�
					else if (j == pCGI->CFI.block_width / 2 - 1)
						cout << pCGI->CBI.lower_right;//���½�
					else
						cout << pCGI->CBI.h_normal;
				}
				//��ͨ��
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
	//�ޱ߿�
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

	/*����ַ�*/
	int char_start_x, char_start_y;

	if (pbdi->content == NULL) {
		if (bdi_value != BDI_VALUE_BLANK) {//=0����ʾ
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
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*��λ��*/
	int block_start_x0 = pCGI->start_x + 2;
	int block_start_y0 = pCGI->start_y + 1;//���޷ֽ��߶���һ��
	int block_start_x = block_start_x0 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);//��������(row_no,col_no)λ�õ�ɫ����ʼ����
	int block_start_y = block_start_y0 + row_no * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);
	draw_block_xy(pCGI, block_start_x, block_start_y, bdi_value, bdi);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO* const bdi, const int direction, const int distance)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*��λ��*/
	int block_start_x0 = pCGI->start_x + 2;
	int block_start_y0 = pCGI->start_y + 1;//���޷ֽ��߶���һ��
	int block_start_x = block_start_x0 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);//��������(row_no,col_no)λ�õ�ɫ����ʼ����
	int block_start_y = block_start_y0 + row_no * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);
	int block_end_x, block_end_y;
	//�зָ���
	if (pCGI->CFI.separator) {
		switch (direction) {

			case DOWN_TO_UP:
				/*  ��  */
				block_end_x = block_start_x;
				block_end_y = block_start_y0 + (row_no - distance) * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);

				for (int y = block_start_y - 1; y >= block_end_y; y--) {
					draw_block_xy(pCGI, block_start_x, y, bdi_value, bdi, false);

					cct_gotoxy(block_start_x, y + pCGI->CFI.block_high);//���µĿ��е�λ��
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int i = 0; i < pCGI->CFI.block_width / 2; i++)
						if ((block_start_y - y) % (pCGI->CFI.block_high + 1 * pCGI->CFI.separator) == 0) //�������ֽ���
							cout << pCGI->CFI.h_normal;
						else //���հ�
							cout << "  ";
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case UP_TO_DOWN:
				/*  ��  */
				block_end_x = block_start_x;
				block_end_y = block_start_y0 + (row_no + distance) * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);

				for (int y = block_start_y + 1; y <= block_end_y; y++) {
					draw_block_xy(pCGI, block_start_x, y, bdi_value, bdi, false);
					cct_gotoxy(block_start_x, y - 1);//���µĿ��е�λ��
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int i = 0; i < pCGI->CFI.block_width / 2; i++)
						if ((y - block_start_y) % (pCGI->CFI.block_high + 1 * pCGI->CFI.separator) == 0) //�������ֽ���
							cout << pCGI->CFI.h_normal;
						else //���հ�
							cout << "  ";
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case RIGHT_TO_LEFT:
				/*  ��  */
				block_end_x = block_start_x0 + (col_no - distance) * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);//distance������,��˺���ʱҪ��2
				block_end_y = block_start_y;

				for (int x = block_start_x - 2; x >= block_end_x; x -= 2) {//������仯ÿ���ƶ�������λ
					draw_block_xy(pCGI, x, block_start_y, bdi_value, bdi, false);
					for (int i = 0; i < pCGI->CFI.block_high; i++) {
						cct_gotoxy(x + pCGI->CFI.block_width, block_start_y + i);//���µĿ��е�λ��
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						if (((block_start_x - x) / 2) % (pCGI->CFI.block_width / 2 + 1 * pCGI->CFI.separator) == 0)//�������ֽ���
							cout << pCGI->CFI.v_normal;
						else//���հ�
							cout << "  ";
					}
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case LEFT_TO_RIGHT:
				/*  ��  */
				block_end_x = block_start_x0 + (col_no + distance) * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
				block_end_y = block_start_y;

				for (int x = block_start_x + 2; x <= block_end_x; x += 2) {//������仯ÿ���ƶ�������λ
					draw_block_xy(pCGI, x, block_start_y, bdi_value, bdi, false);
					for (int i = 0; i < pCGI->CFI.block_high; i++) {
						cct_gotoxy(x - 2, block_start_y + i);//���µĿ��е�λ��
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						if (((x - block_start_x) / 2) % (pCGI->CFI.block_width / 2 + 1 * pCGI->CFI.separator) == 0)//�������ֽ���
							cout << pCGI->CFI.v_normal;
						else//���հ�
							cout << "  ";
					}
					Sleep(pCGI->delay_of_block_moved);
				}
				break;
		}
	}

	//�޷ָ���
	else {
		switch (direction) {

			case DOWN_TO_UP:
				/*  ��  */
				block_end_x = block_start_x;
				block_end_y = block_start_y0 + (row_no - distance) * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);

				for (int y = block_start_y - 1; y >= block_end_y; y--) {
					draw_block_xy(pCGI, block_start_x, y, bdi_value, bdi, false);

					cct_gotoxy(block_start_x, y + pCGI->CFI.block_high);//���µĿ��е�λ��
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int i = 0; i < pCGI->CFI.block_width / 2; i++)
						cout << "  ";
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case UP_TO_DOWN:
				/*  ��  */
				block_end_x = block_start_x;
				block_end_y = block_start_y0 + (row_no + distance) * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);

				for (int y = block_start_y + 1; y <= block_end_y; y++) {
					draw_block_xy(pCGI, block_start_x, y, bdi_value, bdi, false);
					cct_gotoxy(block_start_x, y - 1);//���µĿ��е�λ��
					cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					for (int i = 0; i < pCGI->CFI.block_width / 2; i++)
						cout << "  ";
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case RIGHT_TO_LEFT:
				/*  ��  */
				block_end_x = block_start_x0 + (col_no - distance) * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);//distance������,��˺���ʱҪ��2
				block_end_y = block_start_y;

				for (int x = block_start_x - 2; x >= block_end_x; x -= 2) {//������仯ÿ���ƶ�������λ
					draw_block_xy(pCGI, x, block_start_y, bdi_value, bdi, false);
					for (int i = 0; i < pCGI->CFI.block_high; i++) {
						cct_gotoxy(x + pCGI->CFI.block_width, block_start_y + i);//���µĿ��е�λ��
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						cout << "  ";
					}
					Sleep(pCGI->delay_of_block_moved);
				}
				break;

			case LEFT_TO_RIGHT:
				/*  ��  */
				block_end_x = block_start_x0 + (col_no + distance) * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
				block_end_y = block_start_y;

				for (int x = block_start_x + 2; x <= block_end_x; x += 2) {//������仯ÿ���ƶ�������λ
					draw_block_xy(pCGI, x, block_start_y, bdi_value, bdi, false);
					for (int i = 0; i < pCGI->CFI.block_high; i++) {
						cct_gotoxy(x - 2, block_start_y + i);//���µĿ��е�λ��
						cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						cout << "  ";
					}
					Sleep(pCGI->delay_of_block_moved);
				}
				break;
		}

	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
													   ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��/λ�÷Ƿ�"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
		   1�����������ƶ����õ���MRow/MCol�봫�����ͬ(���ָ��΢С���ƶ�)���򲻷��أ�������
							  �õ����зǷ�λ�ã��򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
							  �õ���MRow/MCol�봫��Ĳ�ͬ(��������һ���仯)������ update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"���ٷ���MOUSE_ONLY_MOVED����Щ��Ϸ���غ�Ҫ����ɫ��Ĳ�ͬ��ɫ��ʾ��
		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
								  �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow��MCol������
		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/

static int convert_xy_to_rc(const CONSOLE_GRAPHICS_INFO* const pCGI, int MX, int MY, int& MRow, int& MCol)
{
	/*��xy����ת������Ϸ��������к�,��Ϊ�Ƿ���������ΪINVALID_POS�ҷ���-1,���򷵻�0*/

	//��λ��
	int block_start_x0 = pCGI->start_x + 2;
	int block_start_y0 = pCGI->start_y + 1;//���޷ֽ��߶���һ��

	int block_start_x, block_start_y, i, j;
	//���ѭ�����ҵ�,���޸�ֵ,�����ԷǷ�ֵ����
	MRow = MCol = INVALID_POS;

	for (i = 0; i < pCGI->row_num; i++)
		for (j = 0; j < pCGI->col_num; j++) {
			//��������(row_no,col_no)λ�õ�ɫ����ʼ����
			block_start_y = block_start_y0 + i * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);
			block_start_x = block_start_x0 + j * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);

			if (block_start_x <= MX && MX < block_start_x + pCGI->CFI.block_width &&
				block_start_y <= MY && MY < block_start_y + pCGI->CFI.block_high) {
				MRow = i;
				MCol = j;
				return 0;//�Ϸ�
			}
		}
	return -1;//�Ƿ�
}

int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO* const pCGI, int& MAction, int& MRow, int& MCol, int& KeyCode1, int& KeyCode2, const bool update_lower_status_line)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int MX, MY, odd_MRow, odd_MCol, event;
	while (1) {
		odd_MRow = MRow;
		odd_MCol = MCol;
		event = cct_read_keyboard_and_mouse(MX, MY, MAction, KeyCode1, KeyCode2);
		//����¼�
		if (event == CCT_MOUSE_EVENT) {
			/*����ƶ�*/
			if (MAction == MOUSE_ONLY_MOVED) {
				int legal = convert_xy_to_rc(pCGI, MX, MY, MRow, MCol);
				if (legal == -1) {//�Ƿ�
					if (update_lower_status_line == true) //Ҫ�������״̬��
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");
				}
				else {//λ�úϷ�
					if (!(odd_MRow == MRow && odd_MCol == MCol)) {//���������б䶯!
						if (update_lower_status_line == true) {
							char display[30] = { 0 };
							char chRow = MRow + 'A';
							sprintf(display, "[��ǰ���] %c��%d��", chRow, MCol);
							gmw_status_line(pCGI, LOWER_STATUS_LINE, display);
						}
						return  CCT_MOUSE_EVENT;//����
					}
					//�����������ѭ��
				}
			}
			/*������/�һ�*/
			else if (MAction == MOUSE_LEFT_BUTTON_CLICK|| MAction == MOUSE_RIGHT_BUTTON_CLICK) {
				int legal = convert_xy_to_rc(pCGI, MX, MY, MRow, MCol);
				if (legal == -1) {//�Ƿ�
					if (update_lower_status_line == true) //Ҫ�������״̬��
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");
				}
				else {//λ�úϷ�
					return  CCT_MOUSE_EVENT;//����
				}
			}
		}

		//�����¼�
		else {
			return CCT_KEYBOARD_EVENT;//����
		}
	}

}
