/*1852877 �ƿ� ��껈�*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <io.h>
#include "../include/cfg_file_tools.h"
using namespace std;

/* ---------------------------------------------------------------
	 �������������Ҫstatic�������ڲ������ã�
   ---------------------------------------------------------------- */
   /*����01:�������ļ����ݶ���string�ڴ���*/
static void get_info_to_string(fstream& fp, stringstream& ss)
{
	char ch;
	if (!fp.good())
		fp.clear();

	fp.seekg(0, ios::beg);
	while ((ch = fp.get()) != EOF)
		ss << ch;
}
/*����02:����ļ����Ƿ�û�����ݣ���Ϊ����*/
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


/*����02:��ȡ�����ļ���һ����Ч����(ȥ��#,\t,�ո��)*/
static void validation(const char buf[1000], char valid_buf[1000])
{
	int i = 0, j = 0;
	while (buf[i] != 0) {
		if (buf[i] == '\t' || buf[i] == ' ' || buf[i] == '\r' || buf[i] == '\n')//ʵ����\n������
			;
		else if (buf[i] == '#' || buf[i] == ';')//����ע�ͷ�����
			break;
		else
			valid_buf[j++] = buf[i];
		i++;
	}
}

/*����03:��ĳ��Ч����������ȡ����,������������false*/
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
/*����04:��������Ƿ��Ѵ���*/
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
		if (strlen(valid_buf) != 0) {//�ǿմ�
			char gn[50] = { 0 };//���ȡ��������
			if (get_groupname(valid_buf, gn))
				if (strcmp(gn, groupname) == 0) {
					flag_existed = true;
					break;//���������鵽��һ���ͷ���
				}

		}

	}
	return flag_existed;
}
/*����05:ɾ��ĳ��,�Ѽ�����*/
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
		ss.getline(buf, 1000, '\n');//����һ��(������Ч����Ч��Ϣ),��Ϊʹ��ʮ���ƶ�0D0A�϶�Ϊһֻ��0D
		//������


		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);//ת��Ϊ��Ч��Ϣ
		char gn[50] = { 0 };
		if (get_groupname(valid_buf, gn)) {//����Ϊ����
			if (strcmp(gn, group_name) == 0) {//����Ϊ��ɾ������
				cnt++;
				flag_ingroup = true;
			}
			else {//�����Ǵ�ɾ������
				flag_ingroup = false;
				newss << buf << '\n';//����getline������'\n',ͬʱʮ����д���0D0A
			}
		}
		else {//��������
			if (!flag_ingroup)
				newss << buf << '\n'; //����getline������'\n', ͬʱʮ����д���0D0A
		}

	}
	return cnt;
	/*ע��:app��ʽ��ָ��������,���洦��,��дֻ��ĩβ*/
}
/*����:��ȡĳ��Ч���е���Ŀ����Ϣ*/
static bool get_itemname(const char* valid_buf, char* in)
{
	if (strchr(valid_buf, '=') == NULL || strchr(valid_buf, '=') == valid_buf) //û��=�͵Ⱥ�λ�����׾�����
		return false;


	for (int i = 0; i < int(strlen(valid_buf)); i++) {
		if (valid_buf[i] == '=')
			break;
		else
			in[i] = valid_buf[i];
	}
	//�Ⱥ����ȥ0
	for (int i = strlen(in) - 1; i >= 0; i--) {
		if (in[i] == ' ')
			in[i] = '\0';
		else if (in[i] != ' ')
			break;
	}


	return true;
}
/*����06:���ĳ�����������Ƿ����item_name*/
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

		if (strlen(valid_buf) != 0) {//�ǿմ�
			char gn[50] = { 0 };//���ȡ��������
			if (get_groupname(valid_buf, gn)) {
				if (flag_ingroup)//�Ѿ�����������������
					break;
				if (strcmp(gn, group_name) == 0)
					flag_ingroup = true;
			}
			if (flag_ingroup) {//���� 
				char in[50] = { 0 };
				if (get_itemname(valid_buf, in) && strcmp(in, item_name) == 0) {//����Ŀ���м京��'=',������Ѱ��Ŀͬ��
					flag_existed = true;
					break;//��Ȼ����һ��������
				}

			}
		}

	}
	if (!fp.good())
		fp.clear();
	return flag_existed;
}
/*����06����:���������ļ����Ƿ���ڸ�item_name*/
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

		if (strlen(valid_buf) != 0) {//�ǿմ�

			char in[50] = { 0 };
			if (get_itemname(valid_buf, in) && strcmp(in, item_name) == 0) {//����Ŀ���м京��'=',������Ѱ��Ŀͬ��
				flag_existed = true;
				break;//��Ȼ����һ��������
			}

		}
	}
	if (!fp.good())
		fp.clear();
	return flag_existed;
}


/*����07:����Ƿ�Ϊ�������ļ�*/
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

		if (strlen(valid_buf) != 0) {//�ǿմ�
			char gn[50] = { 0 };//���ȡ��������
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
/*����08:item_addϵ��*/
template<class item_type>
static void func_add_item(fstream& fp, const char* group_name, const char* item_name, const item_type item_value, stringstream& newss)
{
	//�����ж�,group_name��һ��������item_name������
	stringstream ss;
	get_info_to_string(fp, ss);

	bool flag_ingroup = false;

	while (1) {
		char buf[1000] = { 0 };
		//�����˳�
		if (!ss.good()) {
			ss.clear();
			break;
		}
		ss.getline(buf, 1000, '\n');


		newss << buf << '\n';

		char valid_buf[1000] = { 0 };
		validation(buf, valid_buf);//��Ч��

		char  gn[50] = { 0 };
		if (get_groupname(valid_buf, gn))
			if (strcmp(gn, group_name) == 0) //����Ѱ����������һ�����

				newss << item_name << " = " << item_value << "\r\n";
	}
	return;
}
/*����09:del_item*/
static int func_del_item(fstream& fp, const char* group_name, const char* item_name, stringstream& newss)
{
	//ɾ��������������Ϊitem_name����
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

		if (strlen(valid_buf) != 0) {//�ǿմ�
			if (group_name == NULL) {//�������ɾ��
				char in[50] = { 0 };
				if (get_itemname(valid_buf, in))
					if (strcmp(in, item_name) == 0) {//����Ŀ���м京��'=',������Ѱ��Ŀͬ��
						will_output = false;
						cnt++;

					}
			}
			else {
				char gn[50] = { 0 };//���ȡ��������
				if (get_groupname(valid_buf, gn))//��������
					if (strcmp(gn, group_name) == 0) //ƥ������
						flag_ingroup = (!flag_has_ingroup) ? true : false;
					else//��ƥ������
						flag_ingroup = false;

				if (flag_ingroup) {//���� 
					flag_has_ingroup = true;
					char in[50] = { 0 };
					if (get_itemname(valid_buf, in))
						if (strcmp(in, item_name) == 0) {//����Ŀ���м京��'=',������Ѱ��Ŀͬ��
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
/*����10:item_update*/
template<class item_type>
static void func_update_item(fstream& fp, const char* group_name, const char* item_name, const item_type item_value, stringstream& newss)
{
	//�����ж�,group_name��һ��������item_nameҲ����
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

		if (strlen(valid_buf) != 0) {//�ǿմ�
			if (group_name == NULL) {//�������
				char in[50] = { 0 };
				if (get_itemname(valid_buf, in))
					if (!flag_has_update && strcmp(in, item_name) == 0) {
						update_line = true;
						flag_has_update = true;
						newss << item_name << " = " << item_value << "\r\n";
					}
			}
			else {//�����
				char gn[50] = { 0 };//���ȡ��������
				if (get_groupname(valid_buf, gn))//��������
					if (strcmp(gn, group_name) == 0) //ƥ������
						flag_ingroup = (!flag_has_update) ? true : false;
					else//��ƥ������
						flag_ingroup = false;

				if (flag_ingroup) {//���� 
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
		if (!update_line)//�Ǹ����б���ԭ���
			newss << buf << '\n';
	}
}
/*����11:get_itemvalue*/
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

		if (strlen(valid_buf) != 0) {			//�ǿմ�

			if (group_name == NULL) {//�������,��Ӧ������+��Ŀ������
				char in[50] = { 0 };
				if (get_itemname(valid_buf, in))
					if (strcmp(in, item_name) == 0) {
						stringstream get_v;
						get_itemvalue(valid_buf, get_v);
						get_v >> item_value;
						return;
					}
			}
			else {//�����,��Ӧ��������+����+��Ŀ������
				char gn[50] = { 0 };//���ȡ��������
				if (get_groupname(valid_buf, gn))//��������
					if (strcmp(gn, group_name) == 0) //ƥ������
						flag_ingroup = true;

				if (flag_ingroup) {//���� 
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
	static��������
   ---------------------------------------------------------------- */





   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ�
	 ���������
	 �� �� ֵ��
	 ˵    �������б�Ҫ�����������������
   ***************************************************************************/
CFT::CFT()
{
	filename = "";
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������б�Ҫ�����������������
***************************************************************************/
CFT::~CFT()
{
	if (fp.is_open())
		fp.close();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
CFT::CFT(const char* cfgname, int opt)
{
	filename = cfgname;
	if (opt == OPEN_OPT_RDONLY) //ֻ����ʽ
		fp.open(cfgname, ios::in | ios::_Nocreate | ios::binary);

	else if (opt == OPEN_OPT_RDWR) //��д��ʽ
		fp.open(cfgname, ios::in | ios::out | ios::binary | ios::app);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void CFT::open(const char* cfgname, int opt)
{
	filename = cfgname;
	if (opt == OPEN_OPT_RDONLY) //ֻ����ʽ
		fp.open(cfgname, ios::in | ios::_Nocreate | ios::binary);

	else if (opt == OPEN_OPT_RDWR) //��д��ʽ
		fp.open(cfgname, ios::in | ios::out | ios::binary | ios::app);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����򿪳ɹ�����true�����򷵻�false
***************************************************************************/
bool CFT::is_open(void)
{
	return fp.is_open();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void CFT::close()
{
	fp.close();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����C++17����filesystem::resize_fileֱ�ӿ����޸��ļ���С��֮ǰ�İ汾��
				   ��˱�����������C�ľɷ���
		��ʵ�֣���Ҫ��
***************************************************************************/
int CFT::file_resize(int newsize)
{
	/* �����С�Ǹ�����ֱ�ӷ���-1 */
	if (newsize < 0)
		return -1;

	if (!fp.good())
		fp.clear();

	/* ȡfp��ǰָ��λ�� */
	int now_pos = int(fp.tellg());

	/* ���ȣ�C++��ʽ�ر� */
	this->close();

	/* ��Σ�C��ʽ�򿪲��ı��ļ���С */
	FILE* fp_cstyle;
	fp_cstyle = fopen(filename.c_str(), "rb+"); //rw��ʽ��
	if (fp_cstyle) {
		_chsize(_fileno(fp_cstyle), newsize);
		fclose(fp_cstyle);
	}
	else
		cout << "open [" << filename << "] for resize failed" << endl;

	/* �ٴΣ�C++��ʽ���´� */
	fp.open(filename, ios::in | ios::out | ios::binary);
	if (fp.is_open() == 0) {
		cout << "reopen [" << filename << "] after resize failed" << endl;
		return -1;
	}

	/* �ļ�ָ���ƶ���ԭλ�� */
	fp.seekg(now_pos < newsize ? now_pos : newsize, ios::beg);

	return 0;
}


/* ---------------------------------------------------------------
	 ����AAT���Զ����Ա������ʵ�֣�protected��
   ---------------------------------------------------------------- */

   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ�
	 ���������
	 �� �� ֵ��
	 ˵    ����
	***************************************************************************/
int CFT::group_add(const char* group_name)
{
	//is_repeat:�ж��Ƿ��Ѿ����ڸ�����
	if (gn_existed(fp, group_name))
		return 0; //�˾������Ҫ�޸�


	//if�ǵ�һ�飬��ǰ�治Ҫ��0A0D
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::group_del(const char* group_name)
{
	//��Ϊ�ɶ�дʱ�����������,��˲�ȡ�����������ǰ,��Ҫ��Ϊɾ������ļ���С
	if (!gn_existed(fp, group_name))//������
		return 0;

	stringstream newss;
	int res = delete_group(fp, group_name, newss);

	file_resize(0);
	/*resize֮�󲢷�app��,Ϊ��ͳһ���޸�*/
	if (!fp.good())
		fp.clear();
	fp << newss.str();
	return res; //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�item_addϵ��
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_add(const char* group_name, const char* item_name, const int item_value)
{
	if (group_name == NULL)
		if (!check_easytype(fp))
			return 0;
		else {
			/*app����ĩ*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << item_value << "\r\n";
			return 1;
		}

	if (!gn_existed(fp, group_name))//������
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, item_value, newss);
	file_resize(0);
	/*resize֮�󲢷�app��*/
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
			/*app����ĩ*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << item_value << "\r\n";
			return 1;
		}

	if (!gn_existed(fp, group_name))//������
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, item_value, newss);
	file_resize(0);
	/*resize֮�󲢷�app��,Ϊ��ͳһ���޸�*/
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
			/*app����ĩ*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << item_value << "\r\n";
			return 1;
		}


	if (!gn_existed(fp, group_name))//������
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, item_value, newss);
	file_resize(0);
	/*resize֮�󲢷�app��,Ϊ��ͳһ���޸�*/
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
			/*app����ĩ*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << item_value << "\r\n";
			return 1;
		}


	if (!gn_existed(fp, group_name))//������
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, item_value, newss);
	file_resize(0);
	/*resize֮�󲢷�app��,Ϊ��ͳһ���޸�*/

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
			/*app����ĩ*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << item_value << "\r\n";
			return 1;
		}


	if (!gn_existed(fp, group_name))//������
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, item_value, newss);
	file_resize(0);
	/*resize֮�󲢷�app��,Ϊ��ͳһ���޸�*/

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
			/*app����ĩ*/
			if (item_existed(fp, item_name))
				return 0;

			if (!fp.good())
				fp.clear();
			fp << item_name << " = " << "\r\n";
			return 1;
		}


	if (!gn_existed(fp, group_name))//������
		return 0;
	if (item_existed(fp, group_name, item_name))
		return 0;
	stringstream newss;
	func_add_item(fp, group_name, item_name, "", newss);
	file_resize(0);
	/*resize֮�󲢷�app��,Ϊ��ͳһ���޸�*/

	if (!fp.good())
		fp.clear();
	fp << newss.str();

	return 1;
}

/***************************************************************************
  �������ƣ�ɾ��
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_del(const char* group_name, const char* item_name)
{
	int res;
	stringstream newss;
	if (group_name == NULL) {
		//��ʵ�ϴ�ʱ�����úͷǼ򵥲�����һ����

		res = func_del_item(fp, group_name, item_name, newss);
	}
	else {
		if (!check_easytype(fp)) {
			if (!gn_existed(fp, group_name))//������
				return 0;
		}

		res = func_del_item(fp, group_name, item_name, newss);
	}

	file_resize(0);
	/*resize֮�󲢷�app��,Ϊ��ͳһ���޸�*/

	if (!fp.good())
		fp.clear();
	fp << newss.str();
	return res;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/


int CFT::item_update(const char* group_name, const char* item_name, const int item_value)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//������
			if (!item_existed(fp, item_name)) {//�����ڸ�item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << item_value << "\r\n";//����ĩβ
				return 1;
			}
			else {//���ڸ�item,�ı��һ��
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
		if (!gn_existed(fp, group_name)) //�����ڸ�����
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const double item_value)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//������
			if (!item_existed(fp, item_name)) {//�����ڸ�item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << item_value << "\r\n";//����ĩβ
				return 1;
			}
			else {//���ڸ�item,�ı��һ��
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
		if (!gn_existed(fp, group_name)) //�����ڸ�����
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const char* item_value)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//������
			if (!item_existed(fp, item_name)) {//�����ڸ�item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << item_value << "\r\n";//����ĩβ
				return 1;
			}
			else {//���ڸ�item,�ı��һ��
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
		if (!gn_existed(fp, group_name)) //�����ڸ�����
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const string& item_value)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//������
			if (!item_existed(fp, item_name)) {//�����ڸ�item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << item_value << "\r\n";//����ĩβ
				return 1;
			}
			else {//���ڸ�item,�ı��һ��
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
		if (!gn_existed(fp, group_name)) //�����ڸ�����
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name, const char item_value)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//������
			if (!item_existed(fp, item_name)) {//�����ڸ�item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << item_value << "\r\n";//����ĩβ
				return 1;
			}
			else {//���ڸ�item,�ı��һ��
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
		if (!gn_existed(fp, group_name)) //�����ڸ�����
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_update(const char* group_name, const char* item_name)
{
	stringstream newss;
	if (group_name == NULL) {
		if (check_easytype(fp)) {//������
			if (!item_existed(fp, item_name)) {//�����ڸ�item

				if (!fp.good())
					fp.clear();
				fp << item_name << " = " << "\r\n";//����ĩβ
				return 1;
			}
			else {//���ڸ�item,�ı��һ��
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
		if (!gn_existed(fp, group_name)) //�����ڸ�����
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/

int CFT::item_get_value(const char* group_name, const char* item_name, int& item_value)
{
	if (group_name == NULL) {
		if (!check_easytype(fp)) //�Ǽ�����
			return 0;
		if (!item_existed(fp, item_name))//��������Ŀ��������
			return 0;
		/*������+��Ŀ������*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
	else {
		if (check_easytype(fp))//������
			return 0;
		if (!gn_existed(fp, group_name))//����������
			return 0;
		if (!item_existed(fp, group_name, item_name))//�ǰ����Ŀ��������
			return 0;
		/*��������+����+��Ŀ������*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, double& item_value)
{
	if (group_name == NULL) {
		if (!check_easytype(fp)) //�Ǽ�����
			return 0;
		if (!item_existed(fp, item_name))//��������Ŀ��������
			return 0;
		/*������+��Ŀ������*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
	else {
		if (check_easytype(fp))//������
			return 0;
		if (!gn_existed(fp, group_name))//����������
			return 0;
		if (!item_existed(fp, group_name, item_name))//�ǰ����Ŀ��������
			return 0;
		/*��������+����+��Ŀ������*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, char* item_value)
{
	if (group_name == NULL) {
		if (!check_easytype(fp)) //�Ǽ�����
			return 0;
		if (!item_existed(fp, item_name))//��������Ŀ��������
			return 0;
		/*������+��Ŀ������*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
	else {
		if (check_easytype(fp))//������
			return 0;
		if (!gn_existed(fp, group_name))//����������
			return 0;
		if (!item_existed(fp, group_name, item_name))//�ǰ����Ŀ��������
			return 0;
		/*��������+����+��Ŀ������*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, string& item_value)
{
	if (group_name == NULL) {
		if (!check_easytype(fp)) //�Ǽ�����
			return 0;
		if (!item_existed(fp, item_name))//��������Ŀ��������
			return 0;
		/*������+��Ŀ������*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
	else {
		if (check_easytype(fp))//������
			return 0;
		if (!gn_existed(fp, group_name))//����������
			return 0;
		if (!item_existed(fp, group_name, item_name))//�ǰ����Ŀ��������
			return 0;
		/*��������+����+��Ŀ������*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name, char& item_value)
{
	if (group_name == NULL) {
		if (!check_easytype(fp)) //�Ǽ�����
			return 0;
		if (!item_existed(fp, item_name))//��������Ŀ��������
			return 0;
		/*������+��Ŀ������*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
	else {
		if (check_easytype(fp))//������
			return 0;
		if (!gn_existed(fp, group_name))//����������
			return 0;
		if (!item_existed(fp, group_name, item_name))//�ǰ����Ŀ��������
			return 0;
		/*��������+����+��Ŀ������*/
		func_get_value(fp, group_name, item_name, item_value);
		return 1;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_get_value(const char* group_name, const char* item_name)
{
	return 0; //�˾������Ҫ�޸�
}

