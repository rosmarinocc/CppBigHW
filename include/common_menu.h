/*1852877 计科 赵昊堃*/
#pragma once
//功能:根据输入的菜单项生成菜单
char display_menu(const char* items[], int itemsnum);

//功能:获取数字参数,输入提示语,闭区间上下界,默认值,以及待修改参数的引用,可选参数表示超出范围后:使用true默认值/false继续循环
void get_dig_para(const char* prompt, int Min, int Max, int& para, bool use_default = false, int Default = 0);

//功能:按回车继续
void wait_for_enter();