/*1852877 计科 赵昊堃*/
#pragma once
int RAND(int N);//功能:产生一个0~N-1的随机数
bool isType(char ch, const char* s);//功能:判断一个字符是大写/小写/数字,对应关键词为capital,small,number
void draw_borderlines(int row, int col, int block_w, int block_h, int start_x, int start_y);//参数:行列数,色块长宽,框线起始坐标