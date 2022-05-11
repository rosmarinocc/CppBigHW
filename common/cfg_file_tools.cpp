/*1852877 计科 赵昊堃*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <io.h>
#include "../include/cfg_file_tools.h"
using namespace std;

/* ---------------------------------------------------------------
	 允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */
   /*函数01:将配置文件内容读入string内存中*/
static void get_info_to_string(fstream& fp, stringstream& ss)
{
	char ch;
	if (!fp.good())
		fp.clear();

	fp.seekg(0, ios::beg);
	while ((ch = fp.get()) != EOF)
		ss << ch;
}
/*函数02:检查文件内是否还没有内容，即为首行*/
static bool is_firstline(fstream& fp)
{
	stringstream ss;
	get_info_to_string(fp, ss);
	char buf[1000] = { 0 };
	ss.getline(buf, 1000, '\n');
	if (!fp.good()) {
		fp.clear();
	}
	if (strlen(buf) == 0)
		return true;
	return false;
}


/*函数02:读取配置文件的一行有效内容(去除#,\t,空格等)*/
static void validation(const char buf[1000], char valid_buf[1000])
{
	int i = 0, j = 0;
	while (buf[i] != 0) {
		if (buf[i] == '\t' || buf[i] == ' ' || buf[i] == '\r' || buf[i] == '\n')//实际上\n不可能
			;
		else if (buf[i] == '#' || buf[i] == ';')//遇到注释符不读
			break;
		else
			valid_buf[j++] = buf[i];
		i++;
	}
}

/*函数03:从某有效行内容中提取组名,若非组名返回false*/
static bool get_groupname(const char* buf, char* gn)
{
	bool is_groupname = false;
	int len_s = strlen(buf);
	if (buf[0] == '[' && buf[len_s - 1] == ']')
		is_groupname = true;
	int i = 0, j = 0;
	if (is_groupname) {
		for (i = 1; i < len_s; i++)
			if (buf[i] != ' ')
				break;

		for (j = strlen(buf) - 2; j > 0; j--)
			if (buf[j] != ' ')
				break;
		int t = 0;
		for (int k = i; k <= j; k++)
			gn[t++] = buf[k];
	}
	return is_groupname;
}
/*函数04:检查组名是否已存在*/
static bool gn_existed(fstream& fp, const char* groupname) {

	stringstream ss;
	get_info_to_string(fp, ss);
	bool flag_existed = false;
	while (1) {
		char buf[1000] = { 0 };
		if (!ss.good()) {
			ss.clear();
			break;
		}
		ss.getline(buf, 1000, '\n');


		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);
		if (strlen(valid_buf) != 0) {//非空串
			char gn[50] = { 0 };//存放取出的组名
			if (get_groupname(valid_buf, gn))
				if (strcmp(gn, groupname) == 0) {
					flag_existed = true;
					break;//多个重名检查到第一个就返回
				}

		}

	}
	return flag_existed;
}
/*函数05:删除某组,已检测存在*/
static int delete_group(fstream& fp, const char* group_name, stringstream& newss)
{
	stringstream ss;
	get_info_to_string(fp, ss);

	int cnt = 0;
	bool flag_ingroup = false;

	while (1) {
		char buf[1000] = { 0 };
		if (!ss.good()) {
			ss.clear();
			break;
		}
		ss.getline(buf, 1000, '\n');//读了一行(包括有效或无效信息),因为使用十进制读0D0A合二为一只读0D
		//读完了


		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);//转换为有效信息
		char gn[50] = { 0 };
		if (get_groupname(valid_buf, gn)) {//该行为组名
			if (strcmp(gn, group_name) == 0) {//组名为待删除组名
				cnt++;
				flag_ingroup = true;
			}
			else {//组名非待删除组名
				flag_ingroup = false;
				newss << buf << '\n';//补上getline读掉的'\n',同时十进制写输出0D0A
			}
		}
		else {//非组名行
			if (!flag_ingroup)
				newss << buf << '\n'; //补上getline读掉的'\n', 同时十进制写输出0D0A
		}

	}
	return cnt;
	/*注意:app方式下指针正常跑,能随处读,但写只在末尾*/
}
/*函数:提取某有效行中的项目名信息*/
static bool get_itemname(const char* valid_buf, char* in)
{
	if (strchr(valid_buf, '=') == NULL || strchr(valid_buf, '=') == valid_buf) //没有=和等号位于行首均不行
		return false;


	for (int i = 0; i < int(strlen(valid_buf)); i++) {
		if (valid_buf[i] == '=')
			break;
		else
			in[i] = valid_buf[i];
	}
	//等号左侧去0
	for (int i = strlen(in) - 1; i >= 0; i--) {
		if (in[i] == ' ')
			in[i] = '\0';
		else if (in[i] != ' ')
			break;
	}


	return true;
}
/*函数06:检查某存在组名下是否存在item_name*/
static bool item_existed(fstream& fp, const char* group_name, const char* item_name)
{
	stringstream ss;
	get_info_to_string(fp, ss);
	bool flag_ingroup = false, flag_existed = false;

	while (1) {
		char buf[1000] = { 0 };
		if (!ss.good()) {
			ss.clear();
			break;
		}
		ss.getline(buf, 1000, '\n');


		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);

		if (strlen(valid_buf) != 0) {//非空串
			char gn[50] = { 0 };//存放取出的组名
			if (get_groupname(valid_buf, gn)) {
				if (flag_ingroup)//已经进过组了再遇组名
					break;
				if (strcmp(gn, group_name) == 0)
					flag_ingroup = true;
			}
			if (flag_ingroup) {//进组 
				char in[50] = { 0 };
				if (get_itemname(valid_buf, in) && strcmp(in, item_name) == 0) {//是项目名中间含有'=',且与所寻项目同名
					flag_existed = true;
					break;//依然是有一个就走人
				}

			}
		}

	}
	if (!fp.good())
		fp.clear();
	return flag_existed;
}
/*函数06重载:检测简单配置文件下是否存在该item_name*/
static bool item_existed(fstream& fp, const char* item_name)
{
	stringstream ss;
	get_info_to_string(fp, ss);
	bool  flag_existed = false;

	while (1) {
		char buf[1000] = { 0 };
		if (!ss.good()) {
			ss.clear();
			break;
		}

		ss.getline(buf, 1000, '\n');

		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);

		if (strlen(valid_buf) != 0) {//非空串

			char in[50] = { 0 };
			if (get_itemname(valid_buf, in) && strcmp(in, item_name) == 0) {//是项目名中间含有'=',且与所寻项目同名
				flag_existed = true;
				break;//依然是有一个就走人
			}

		}
	}
	if (!fp.good())
		fp.clear();
	return flag_existed;
}


/*函数07:检查是否为简单配置文件*/
static bool check_easytype(fstream& fp)
{
	stringstream ss;
	get_info_to_string(fp, ss);
	bool flag_easytype = true;
	while (1) {
		char buf[1000] = { 0 };
		if (!ss.good()) {
			ss.clear();
			break;
		}
		ss.getline(buf, 1000, '\n');


		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);

		if (strlen(valid_buf) != 0) {//非空串
			char gn[50] = { 0 };//存放取出的组名
			if (get_groupname(valid_buf, gn)) {
				flag_easytype = false;
				break;
			}

		}

	}
	if (!fp.good())
		fp.clear();
	return flag_easytype;

}
/*函数08:item_add系列*/
template<class item_type>
static void func_add_item(fstream& fp, const char* group_name, const char* item_name, const item_type item_value, stringstream& newss)
{
	//已做判断,group_name组一定存在且item_name不存在
	stringstream ss;
	get_info_to_string(fp, ss);

	bool flag_ingroup = false;

	while (1) {
		char buf[1000] = { 0 };
		//读完退出
		if (!ss.good()) {
			ss.clear();
			break;
		}
		ss.getline(buf, 1000, '\n');


		newss << buf << '\n';

		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);//有效化

		char  gn[50] = { 0 };
		if (get_groupname(valid_buf, gn))
			if (strcmp(gn, group_name) == 0) //是所寻组名行则多加一行输出

				newss << item_name << " = " << item_value << "\r\n";
	}
	return;
}
/*函数09:del_item*/
static int func_del_item(fstream& fp, const char* group_name, const char* item_name, stringstream& newss)
{
	//删除首组下所有名为item_name的项
	stringstream ss;
	get_info_to_string(fp, ss);
	bool flag_ingroup = false, flag_has_ingroup = false;
	int cnt = 0;

	while (1) {
		char buf[1000] = { 0 };
		bool will_output = true;
		if (!ss.good()) {
			ss.clear();
			break;
		}
		ss.getline(buf, 1000, '\n');


		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);

		if (strlen(valid_buf) != 0) {//非空串
			if (group_name == NULL) {//不分组别删除
				char in[50] = { 0 };
				if (get_itemname(valid_buf, in))
					if (strcmp(in, item_name) == 0) {//是项目名中间含有'=',且与所寻项目同名
						will_output = false;
						cnt++;

					}
			}
			else {
				char gn[50] = { 0 };//存放取出的组名
				if (get_groupname(valid_buf, gn))//遇到组名
					if (strcmp(gn, group_name) == 0) //匹配组名
						flag_ingroup = (!flag_has_ingroup) ? true : false;
					else//不匹配组名
						flag_ingroup = false;

				if (flag_ingroup) {//进组 
					flag_has_ingroup = true;
					char in[50] = { 0 };
					if (get_itemname(valid_buf, in))
						if (strcmp(in, item_name) == 0) {//是项目名中间含有'=',且与所寻项目同名
							will_output = false;
							cnt++;
						}

				}
			}
		}

		if (will_output)
			newss << buf << '\n';
	}

	return cnt;
}
/*函数10:item_update*/
template<class item_type>
static void func_update_item(fstream& fp, const char* group_name, const char* item_name, const item_type item_value, stringstream& newss)
{
	//已做判断,group_name组一定存在且item_name也存在
	stringstream ss;
	get_info_to_string(fp, ss);

	bool flag_ingroup = false, flag_has_update = false;

	while (1) {
		char buf[1000] = { 0 };
		bool update_line = false;
		if (!ss.good()) {
			ss.clear();
			break;
		}
		ss.getline(buf, 1000, '\n');


		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);

		if (strlen(valid_buf) != 0) {//非空串
			if (group_name == NULL) {//不分组别
				char in[50] = { 0 };
				if (get_itemname(valid_buf, in))
					if (!flag_has_update && strcmp(in, item_name) == 0) {
						update_line = true;
						flag_has_update = true;
						newss << item_name << " = " << item_value << "\r\n";
					}
			}
			else {//分组别
				char gn[50] = { 0 };//存放取出的组名
				if (get_groupname(valid_buf, gn))//遇到组名
					if (strcmp(gn, group_name) == 0) //匹配组名
						flag_ingroup = (!flag_has_update) ? true : false;
					else//不匹配组名
						flag_ingroup = false;

				if (flag_ingroup) {//进组 
					char in[50] = { 0 };
					if (get_itemname(valid_buf, in))
						if (!flag_has_update && strcmp(in, item_name) == 0) {
							update_line = true;
							flag_has_update = true;
							newss << item_name << " = " << item_value << "\r\n";
						}

				}
			}
		}
		if (!update_line)//非更新行保持原输出
			newss << buf << '\n';
	}
}
/*函数11:get_itemvalue*/
static void get_itemvalue(const char* valid_buf, stringstream& get_v)
{
	bool flag_is_value = false;
	for (int i = 0; i <int(strlen(valid_buf)); i++) {
		if (flag_is_value && (valid_buf[i] == '\n' || valid_buf[i] == '\r' || valid_buf[i] == '\t' || valid_buf[i] == ' '))
			break;
		if (valid_buf[i] == '=') {
			while (valid_buf[i + 1] == '\n' || valid_buf[i + 1] == '\r' || valid_buf[i + 1] == '\t' || valid_buf[i + 1] == ' ')
				i++;
			i++;
			flag_is_value = true;
		}
		if (flag_is_value == true)
			get_v << valid_buf[i];
	}
}
template <class item_type>
static void func_get_value(fstream& fp, const char* group_name, const char* item_name, item_type& item_value)
{
	stringstream ss;
	get_info_to_string(fp, ss);

	bool flag_ingroup = false;

	while (1) {
		char buf[1000] = { 0 };
		if (!ss.good()) {
			ss.clear();
			break;
		}
		ss.getline(buf, 1000, '\n');


		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);

		if (strlen(valid_buf) != 0) {			//非空串

			if (group_name == NULL) {//不分组别,对应简单配置+项目名存在
				char in[50] = { 0 };
				if (get_itemname(valid_buf, in))
					if (strcmp(in, item_name) == 0) {
						stringstream get_v;
						get_itemvalue(valid_buf, get_v);
						get_v >> item_value;
						return;
					}
			}
			else {//分组别,对应复杂配置+组名+项目名存在
				char gn[50] = { 0 };//存放取出的组名
				if (get_groupname(valid_buf, gn))//遇到组名
					if (strcmp(gn, group_name) == 0) //匹配组名
						flag_ingroup = true;

				if (flag_ingroup) {//进组 
					char in[50] = { 0 };
					if (get_itemname(valid_buf, in))
						if (strcmp(in, item_name) == 0) {
							stringstream get_v;
							get_itemvalue(valid_buf, get_v);
							get_v >> item_value;
							return;
						}

				}
			}
		}

	}

}

/* ---------------------------------------------------------------
	static函数见上
   ---------------------------------------------------------------- */





   /***************************************************************************
	 函数名称：
	 功    能：
	 输入参数：
	 返 回 值：
	 说    明：如有必要，按需加入其它内容
   ***************************************************************************/
CFT::CFT()
{
	filename = "";
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：如有必要，按需加入其它内容
***************************************************************************/
CFT::~CFT()
{
	if (fp.is_open())
		fp.close();
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
CFT::CFT(const char* cfgname, int opt)
{
	filename = cfgname;
	if (opt == OPEN_OPT_RDONLY) //只读方式
		fp.open(cfgname, ios::in | ios::_Nocreate | ios::binary);

	else if (opt == OPEN_OPT_RDWR) //读写方式
		fp.open(cfgname, ios::in | ios::out | ios::binary | ios::app);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void CFT::open(const char* cfgname, int opt)
{
	filename = cfgname;
	if (opt == OPEN_OPT_RDONLY) //只读方式
		fp.open(cfgname, ios::in | ios::_Nocreate | ios::binary);

	else if (opt == OPEN_OPT_RDWR) //读写方式
		fp.open(cfgname, ios::in | ios::out | ios::binary | ios::app);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：打开成功返回true，否则返回false
***************************************************************************/
bool CFT::is_open(void)
{
	return fp.is_open();
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void CFT::close()
{
	fp.close();
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：C++17中有filesystem::resize_file直接可以修改文件大小，之前的版本无
				   因此本函数扔沿用C的旧方法
		已实现，不要动
***************************************************************************/
int CFT::file_resize(int newsize)
{
	/* 如果大小是负数则直接返回-1 */
	if (newsize < 0)
		return -1;

	if (!fp.good())
		fp.clear();

	/* 取fp当前指针位置 */
	int now_pos = int(fp.tellg());

	/* 首先：C++方式关闭 */
	this->close();

	/* 其次：C方式打开并改变文件大小 */
	FILE* fp_cstyle;
	fp_cstyle = fopen(filename.c_str(), "rb+"); //rw方式打开
	if (fp_cstyle) {
		_chsize(_fileno(fp_cstyle), newsize);
		fclose(fp_cstyle);
	}
	else
		cout << "open [" << filename << "] for resize failed" << endl;

	/* 再次：C++方式重新打开 */
	fp.open(filename, ios::in | ios::out | ios::binary);
	if (fp.is_open() == 0) {
		cout << "reopen [" << filename << "] after resize failed" << endl;
		return -1;
	}

	/* 文件指针移动到原位置 */
	fp.seekg(now_pos < newsize ? now_pos : newsize, ios::beg);

	return 0;
}


/* ---------------------------------------------------------------
	 允许AAT中自定义成员函数的实现（protected）
   ---------------------------------------------------------------- */

   /***************************************************************************
	 函数名称：
	 功    能：
	 输入参数：
	 返 回 值：
	 说    明：
	***************************************************************************/
int CFT::group_add(const char* group_name)
{
	//is_repeat:判断是否已经存在该组名
	if (gn_existed(fp, group_name))
		return 0; //此句根据需要修改


	//if是第一组，则前面不要加0A0D
	if (!is_firstline(fp)) {
		if (!fp.good())
			fp.clear();
		fp << '\r' << '\n';
	}

	if (!fp.good())
		fp.clear();
	fp << '[' << group_name << ']' << '\r' << '\n';

	return 1;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::group_del(const char* group_name)
{
	//因为可读写时不会清空内容,因此采取重新输出方法前,需要改为删除后的文件大小
	if (!gn_existed(fp, group_name))//不存在
		return 0;

	stringstream newss;
	int res = delete_group(fp, group_name, newss);

	file_resize(0);
	/*resize之后并非app打开,为了统一已修改*/
	if (!fp.good())
		fp.clear();
	fp << newss.str();
	return res; //此句根据需要修改
}


/***************************************************************************
  函数名称：item_add系列
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const int item_value)
{
	if (group_name == NULL)
		if (!check_easytype(fp))
			return 0;
		else {
			/*app到最末*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << item_value << "\r\n";
			return 1;
		}

	if (!gn_existed(fp, group_name))//不存在
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, item_value, newss);
	file_resize(0);
	/*resize之后并非app打开*/
	if (!fp.good())
		fp.clear();
	fp << newss.str();

	return 1;
}

/***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const double item_value)
{
	if (group_name == NULL)
		if (!check_easytype(fp))
			return 0;
		else {
			/*app到最末*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << item_value << "\r\n";
			return 1;
		}

	if (!gn_existed(fp, group_name))//不存在
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, item_value, newss);
	file_resize(0);
	/*resize之后并非app打开,为了统一已修改*/
	if (!fp.good())
		fp.clear();
	fp << newss.str();

	return 1;
}

/***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const char* item_value)
{

	if (group_name == NULL)
		if (!check_easytype(fp))
			return 0;
		else {
			/*app到最末*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << item_value << "\r\n";
			return 1;
		}


	if (!gn_existed(fp, group_name))//不存在
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, item_value, newss);
	file_resize(0);
	/*resize之后并非app打开,为了统一已修改*/
	if (!fp.good())
		fp.clear();
	fp << newss.str();

	return 1;
}

/***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const string& item_value)
{
	if (group_name == NULL)
		if (!check_easytype(fp))
			return 0;
		else {
			/*app到最末*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << item_value << "\r\n";
			return 1;
		}


	if (!gn_existed(fp, group_name))//不存在
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, item_value, newss);
	file_resize(0);
	/*resize之后并非app打开,为了统一已修改*/

	if (!fp.good())
		fp.clear();
	fp << newss.str();

	return 1;
}

/***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const char item_value)
{
	if (group_name == NULL)
		if (!check_easytype(fp))
			return 0;
		else {
			/*app到最末*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << item_value << "\r\n";
			return 1;
		}


	if (!gn_existed(fp, group_name))//不存在
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, item_value, newss);
	file_resize(0);
	/*resize之后并非app打开,为了统一已修改*/

	if (!fp.good())
		fp.clear();
	fp << newss.str();

	return 1;
}

/***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name)
{
	if (group_name == NULL)
		if (!check_easytype(fp))
			return 0;
		else {
			/*app到最末*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << "\r\n";
			return 1;
		}


	if (!gn_existed(fp, group_name))//不存在
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, "", newss);
	file_resize(0);
	/*resize之后并非app打开,为了统一已修改*/

	if (!fp.good())
		fp.clear();
	fp << newss.str();

	return 1;
}

/***************************************************************************
  函数名称：删除
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_del(const char* group_name, const char* item_name)
{
	int res;
	stringstream newss;
	if (group_name == NULL) {
		//事实上此时简单配置和非简单操作是一样的

		res = func_del_item(fp, group_name, item_name, newss);
	}
	else {
		if (!check_easytype(fp)) {
			if (!gn_existed(fp, group_name))//不存在
				return 0;
		}

		res = func_del_item(fp, group_name, item_name, newss);
	}

	file_resize(0);
	/*resize之后并非app打开,为了统一已修改*/

	if (!fp.good())
		fp.clear();
	fp << newss.str();
	return res;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/


int CFT::item_update(const char* group_name, const char* item_name, const int item_value)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//简单类型
			if (!item_existed(fp, item_name)) {//不存在该item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << item_value << "\r\n";//加在末尾
				return 1;
			}
			else {//存在该item,改变第一个
				func_update_item(fp, group_name, item_name, item_value, newss);
				file_resize(0);

				if (!fp.good())
					fp.clear();
				fp << newss.str();
				return 1;
			}

		}
		return 0;
	}
	else {
		if (!gn_existed(fp, group_name)) //不存在该组名
			return 0;
		if (!item_existed(fp, group_name, item_name)) {
			item_add(group_name, item_name, item_value);
			return 1;
		}
		func_update_item(fp, group_name, item_name, item_value, newss);
		file_resize(0);

		if (!fp.good())
			fp.clear();
		fp << newss.str();
		return 1;
	}

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const double item_value)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//简单类型
			if (!item_existed(fp, item_name)) {//不存在该item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << item_value << "\r\n";//加在末尾
				return 1;
			}
			else {//存在该item,改变第一个
				func_update_item(fp, group_name, item_name, item_value, newss);
				file_resize(0);

				if (!fp.good())
					fp.clear();
				fp << newss.str();
				return 1;
			}

		}
		return 0;
	}
	else {
		if (!gn_existed(fp, group_name)) //不存在该组名
			return 0;
		if (!item_existed(fp, group_name, item_name)) {
			item_add(group_name, item_name, item_value);
			return 1;
		}
		func_update_item(fp, group_name, item_name, item_value, newss);
		file_resize(0);

		if (!fp.good())
			fp.clear();
		fp << newss.str();
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const char* item_value)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//简单类型
			if (!item_existed(fp, item_name)) {//不存在该item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << item_value << "\r\n";//加在末尾
				return 1;
			}
			else {//存在该item,改变第一个
				func_update_item(fp, group_name, item_name, item_value, newss);
				file_resize(0);

				if (!fp.good())
					fp.clear();
				fp << newss.str();
				return 1;
			}

		}
		return 0;
	}
	else {
		if (!gn_existed(fp, group_name)) //不存在该组名
			return 0;
		if (!item_existed(fp, group_name, item_name)) {
			item_add(group_name, item_name, item_value);
			return 1;
		}
		func_update_item(fp, group_name, item_name, item_value, newss);
		file_resize(0);

		if (!fp.good())
			fp.clear();
		fp << newss.str();
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const string& item_value)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//简单类型
			if (!item_existed(fp, item_name)) {//不存在该item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << item_value << "\r\n";//加在末尾
				return 1;
			}
			else {//存在该item,改变第一个
				func_update_item(fp, group_name, item_name, item_value, newss);
				file_resize(0);

				if (!fp.good())
					fp.clear();
				fp << newss.str();
				return 1;
			}

		}
		return 0;
	}
	else {
		if (!gn_existed(fp, group_name)) //不存在该组名
			return 0;
		if (!item_existed(fp, group_name, item_name)) {
			item_add(group_name, item_name, item_value);
			return 1;
		}
		func_update_item(fp, group_name, item_name, item_value, newss);
		file_resize(0);

		if (!fp.good())
			fp.clear();
		fp << newss.str();
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const char item_value)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//简单类型
			if (!item_existed(fp, item_name)) {//不存在该item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << item_value << "\r\n";//加在末尾
				return 1;
			}
			else {//存在该item,改变第一个
				func_update_item(fp, group_name, item_name, item_value, newss);
				file_resize(0);

				if (!fp.good())
					fp.clear();
				fp << newss.str();
				return 1;
			}

		}
		return 0;
	}
	else {
		if (!gn_existed(fp, group_name)) //不存在该组名
			return 0;
		if (!item_existed(fp, group_name, item_name)) {
			item_add(group_name, item_name, item_value);
			return 1;
		}
		func_update_item(fp, group_name, item_name, item_value, newss);
		file_resize(0);

		if (!fp.good())
			fp.clear();
		fp << newss.str();
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//简单类型
			if (!item_existed(fp, item_name)) {//不存在该item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << "\r\n";//加在末尾
				return 1;
			}
			else {//存在该item,改变第一个
				func_update_item(fp, group_name, item_name, "", newss);
				file_resize(0);

				if (!fp.good())
					fp.clear();
				fp << newss.str();
				return 1;
			}

		}
		return 0;
	}
	else {
		if (!gn_existed(fp, group_name)) //不存在该组名
			return 0;
		if (!item_existed(fp, group_name, item_name)) {
			item_add(group_name, item_name, "");
			return 1;
		}
		func_update_item(fp, group_name, item_name, "", newss);
		file_resize(0);

		if (!fp.good())
			fp.clear();
		fp << newss.str();
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

int CFT::item_get_value(const char* group_name, const char* item_name, int& item_value)
{
	if (group_name == NULL) {
		if (!check_easytype(fp)) //非简单配置
			return 0;
		if (!item_existed(fp, item_name))//简单配置项目名不存在
			return 0;
		/*简单配置+项目名存在*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
	else {
		if (check_easytype(fp))//简单配置
			return 0;
		if (!gn_existed(fp, group_name))//组名不存在
			return 0;
		if (!item_existed(fp, group_name, item_name))//最靠前组项目名不存在
			return 0;
		/*复杂配置+组名+项目名存在*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, double& item_value)
{
	if (group_name == NULL) {
		if (!check_easytype(fp)) //非简单配置
			return 0;
		if (!item_existed(fp, item_name))//简单配置项目名不存在
			return 0;
		/*简单配置+项目名存在*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
	else {
		if (check_easytype(fp))//简单配置
			return 0;
		if (!gn_existed(fp, group_name))//组名不存在
			return 0;
		if (!item_existed(fp, group_name, item_name))//最靠前组项目名不存在
			return 0;
		/*复杂配置+组名+项目名存在*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, char* item_value)
{
	if (group_name == NULL) {
		if (!check_easytype(fp)) //非简单配置
			return 0;
		if (!item_existed(fp, item_name))//简单配置项目名不存在
			return 0;
		/*简单配置+项目名存在*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
	else {
		if (check_easytype(fp))//简单配置
			return 0;
		if (!gn_existed(fp, group_name))//组名不存在
			return 0;
		if (!item_existed(fp, group_name, item_name))//最靠前组项目名不存在
			return 0;
		/*复杂配置+组名+项目名存在*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, string& item_value)
{
	if (group_name == NULL) {
		if (!check_easytype(fp)) //非简单配置
			return 0;
		if (!item_existed(fp, item_name))//简单配置项目名不存在
			return 0;
		/*简单配置+项目名存在*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
	else {
		if (check_easytype(fp))//简单配置
			return 0;
		if (!gn_existed(fp, group_name))//组名不存在
			return 0;
		if (!item_existed(fp, group_name, item_name))//最靠前组项目名不存在
			return 0;
		/*复杂配置+组名+项目名存在*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, char& item_value)
{
	if (group_name == NULL) {
		if (!check_easytype(fp)) //非简单配置
			return 0;
		if (!item_existed(fp, item_name))//简单配置项目名不存在
			return 0;
		/*简单配置+项目名存在*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
	else {
		if (check_easytype(fp))//简单配置
			return 0;
		if (!gn_existed(fp, group_name))//组名不存在
			return 0;
		if (!item_existed(fp, group_name, item_name))//最靠前组项目名不存在
			return 0;
		/*复杂配置+组名+项目名存在*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name)
{
	return 0; //此句根据需要修改
}

