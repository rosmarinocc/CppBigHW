/*1852877 �ƿ� ��껈�*/
#pragma once
//����:��������Ĳ˵������ɲ˵�
char display_menu(const char* items[], int itemsnum);

//����:��ȡ���ֲ���,������ʾ��,���������½�,Ĭ��ֵ,�Լ����޸Ĳ���������,��ѡ������ʾ������Χ��:ʹ��trueĬ��ֵ/false����ѭ��
void get_dig_para(const char* prompt, int Min, int Max, int& para, bool use_default = false, int Default = 0);

//����:���س�����
void wait_for_enter();