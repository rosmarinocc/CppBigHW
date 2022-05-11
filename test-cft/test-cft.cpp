/*1852877 �ƿ� ��껈�*/
/*1853769 �ƿ� ��� 1953793 �ƿ� ������ 1950455 ���淫*/
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <iostream>
#include <cassert>
#include <ctime>
#include "..\include\cfg_file_tools.h"
using namespace std;

#define TESTFILE_G "u1852877_g.cfg"
#define TESTFILE_S "u1852877_s.cfg"

static const char content[] =
"[a];aaaaaaaaaaaaaaaaaaaaaaa]\n"
"#[b]\n"
"a\t\t   \t=10\t  \t   \n"
"  \t \t;b=  \t  10.0\n"
"[\t  c \t]  \t \t\t  \n"
"  [d]\t#faefsgrgdh]\n"
"\n"
"  a  \t =  dwafegserdhd\n"
"\n"
"   \t [a]\n"
"\t\t;\t\n";

static void to_be_continued(const char* prompt)
{
	if (prompt)
		cout << prompt << ",���س�������..." << endl;
	else
		cout << "���س�������..." << endl;
	while (_getch() != '\r')
		;
	putchar('\n');
	putchar('\n');
	return;
}

int main()
{
	remove(TESTFILE_G);
	remove(TESTFILE_S);
	CFT cft;
	int i;
	char s[256];
	if (1) {
		cout << "g���Կ�ʼ" << endl;
		cout << "---------------------" << endl;
		to_be_continued(NULL);
		if (1)
		{
			cout << "group_add������Ӳ���:" << endl;
			cout << "---------------------" << endl;
			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());
			s[1] = '\0';
			for (i = 0; i < 5; i++)
			{
				s[0] = 'a' + i;
				if (cft.group_add(s))
					cout << "test" << i + 1 << ":�ɹ������" << s << endl;
				else
					cout << "test" << i + 1 << ":���" << s << "ʧ��, ��ȥ����" << endl;
			}
			putchar('\n');
			to_be_continued(NULL);


			cout << "group_add�ظ���Ӳ���:" << endl;
			cout << "---------------------" << endl;
			for (i = 4; i >= 0; i--)
			{
				s[0] = 'a' + i;
				if (cft.group_add(s))
					cout << "test" << 5 - i << ":�ɹ�������ظ���" << s << ",��ȥ����" << endl;
				else
					cout << "test" << 5 - i << ":�޷�����ظ�����" << s << endl;
			}
			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "group_del����ɾ������:" << endl;
			cout << "---------------------" << endl;
			cout << "���潫�Զ�����ظ���..." << endl;
			fstream f(TESTFILE_G, ios::out | ios::app);

			srand((unsigned int)time(NULL));
			int m[5];
			memset(m, 0, sizeof(m));
			for (i = 0; i < 100; i++)
			{
				char c = rand() % 5 + 'f';
				f << '[' << c << ']' << endl;

				m[c - 'f']++;
			}
			f.close();
			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());
			for (i = 0; i < 5; i++)
				cout << "�����" << m[i] << "����[" << char(i + 'f') << "]" << endl;
			for (i = 0; i < 5; i++)
			{
				s[0] = i + 'f';
				int cnt = cft.group_del(s);
				if (cnt == m[i])
					cout << "�ɹ�ɾ�����е���[" << s << ']' << endl;
				else
					cout << "ֻɾ����" << cnt << "����[" << s << "],������Ӧ��Ϊ" << m[i] << "��,��ȥ����" << endl;
			}
			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "item_add������Ӳ���:" << endl;
			cout << "---------------------" << endl;

			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());
			for (i = 0; i < 5; i++)
			{
				s[0] = i + 'a';
				if (cft.item_add(s, "�ַ���", "helloworld"))
					cout << "��[" << s << "]������ַ�����Ŀ" << endl;
				else
					cout << "���ʧ��!��ȥ����!" << endl;

				if (cft.item_add(s, "�ַ���string", string("���󷨺�<string>")))
					cout << "��[" << s << "]������ַ���<string>��Ŀ" << endl;
				else
					cout << "���ʧ��!��ȥ����!" << endl;

				if (cft.item_add(s, "����", 123123))
					cout << "��[" << s << "]�����������Ŀ" << endl;
				else
					cout << "���ʧ��!��ȥ����!" << endl;

				if (cft.item_add(s, "������", 123.465))
					cout << "��[" << s << "]����Ӹ�������Ŀ" << endl;
				else
					cout << "���ʧ��!��ȥ����!" << endl;

				if (cft.item_add(s, "�ַ�", 'X'))
					cout << "��[" << s << "]������ַ���Ŀ" << endl;
				else
					cout << "���ʧ��!��ȥ����!" << endl;

				if (cft.item_add(s, "NULL"))
					cout << "��[" << s << "]�����<NULL>��Ŀ" << endl;
				else
					cout << "���ʧ��!��ȥ����!" << endl;

			}
			putchar('\n');
			to_be_continued(NULL);
			cout << "item_add�ظ���Ӳ���:" << endl;
			cout << "---------------------" << endl;

			for (i = 4; i >= 0; i--)
			{
				s[0] = i + 'a';
				if (cft.item_add(s, "�ַ���", "helloworld"))
					cout << "�ɹ�������ظ����ַ���,��ȥ����!" << endl;
				else
					cout << "����ظ����ַ���ʧ��!" << endl;

				if (cft.item_add(s, "����", 123456))
					cout << "�ɹ�������ظ�������,��ȥ����!" << endl;
				else
					cout << "����ظ�������ʧ��!" << endl;

				if (cft.item_add(s, "������", 123.465))
					cout << "�ɹ�������ظ������,��ȥ����!" << endl;
				else
					cout << "����ظ������ʧ��!" << endl;

				if (cft.item_add(s, "�ַ�", 'A'))
					cout << "�ɹ�������ظ����ַ�,��ȥ����!" << endl;
				else
					cout << "����ظ����ַ�ʧ��!" << endl;

				if (cft.item_add(s, "NULL"))
					cout << "�ɹ�������ظ���<NULL>,��ȥ����!" << endl;
				else
					cout << "����ظ���<NULL>ʧ��!" << endl;
			}
			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "item_delɾ������:" << endl;
			cout << "---------------------" << endl;
			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());

			if (cft.item_del(NULL, "�ַ���") == 5)
				cout << "���е��ַ�����ɾ���ɹ�" << endl;
			else
				cout << "δ��ɾ�������ַ�������ȥ����" << endl;

			if (cft.item_del("a", "������") == 1)
				cout << "��[a]�ĸ�������ɾ���ɹ�" << endl;
			else
				cout << "δ��ɾ����[a]�ĸ��������ȥ����" << endl;

			if (cft.item_del("c", "�ַ�") == 1)
				cout << "��[c]���ַ���ɾ���ɹ�" << endl;
			else
				cout << "δ��ɾ����[c]���ַ����ȥ����" << endl;

			putchar('\n');
			to_be_continued(NULL);
			cout << "item_delɾ�������������:" << endl;
			cout << "---------------------" << endl;

			if (cft.item_del(NULL, "�����ڵ��ַ�����") != 0)
				cout << "ɾ���˲����ڵ��ַ�����,��ȥ����" << endl;
			else
				cout << "δɾ���������ַ�����,����ͨ��" << endl;

			if (cft.item_del("b", "�����ڵ�������") != 0)
				cout << "ɾ������[b]�����ڵ�������,��ȥ����" << endl;
			else
				cout << "δɾ��������������,����ͨ��" << endl;

			if (cft.item_del("e", "�����ڵ�<NULL>��") != 0)
				cout << "ɾ������[e]�����ڵ�<NULL>��,��ȥ����" << endl;
			else
				cout << "δɾ��������<NULL>��,����ͨ��" << endl;

			if (cft.item_del("�����ڵ���", "NULL") != 0)
				printf("ɾ������[�����ڵ���]�Ĳ����ڵ�<NULL>��,��ȥ����\n");
			else
				cout << "δɾ����[�����ڵ���]�Ĳ�����<NULL>��,����ͨ��" << endl;
			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "item_update���²���:" << endl;
			cout << "---------------------" << endl;
			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());

			if (cft.item_update("a", "����", 666666))
				cout << "�ɹ�������[a]���������ֵ,�����Ƿ�Ϊ*666666*" << endl;
			else
				cout << "�޷�������[a]���������ֵ����ȥ����!" << endl;

			if (cft.item_update("e", "�ַ���", "�����Ҹ��µ��ַ���"))
				cout << "�ɹ�������[e]���ַ������ֵ,�����Ƿ�Ϊ*\"�����Ҹ��µ��ַ���\"*" << endl;
			else
				cout << "�޷�������[e]���ַ������ֵ����ȥ����!" << endl;

			if (cft.item_update("c", "�ַ���string", "�����Ҹ��µ��ַ���"))
				cout << "�ɹ�������[c]���ַ���string���ֵ,�����Ƿ�Ϊ*\"�����Ҹ��µ��ַ���\"*" << endl;
			else
				cout << "�޷�������[c]���ַ���string���ֵ����ȥ����!" << endl;

			if (cft.item_update("a", "������", 654.321))
				cout << "�ɹ�������[a]�ĸ��������ֵ,�����Ƿ�Ϊ*654.321*" << endl;
			else
				cout << "�޷�������[a]�ĸ��������ֵ����ȥ����!" << endl;
			putchar('\n');
			to_be_continued(NULL);
			cout << "item_update����������²���:" << endl;
			cout << "---------------------" << endl;

			if (cft.item_update("�����ڵ���", "������", 888.999))
				cout << "��Ȼ���³ɹ���,����ȥ����" << endl;
			else
				cout << "�޷�������[�����ڵ���]�ĸ��������ֵ,����ͨ��" << endl;
			cft.close();
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "item_get_value����ȡֵ����:" << endl;
			cout << "---------------------" << endl;
			to_be_continued(NULL);
			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());


			if (cft.item_get_value("e", "�ַ���", s))
				cout << "�ɹ���ȡ��[e]���ַ�����,ȡ��ֵ:" << s << " ʵ��ֵ:�����Ҹ��µ��ַ���" << endl;
			else
				cout << "��ȡ��[e]���ַ�����ʧ��,�����" << endl;

			string ss;
			if (cft.item_get_value("c", "�ַ���string", ss))
				cout << "�ɹ���ȡ��[c]���ַ���string��,ȡ��ֵ:" << ss << " ʵ��ֵ:�����Ҹ��µ��ַ���" << endl;
			else
				cout << "��ȡ��[c]���ַ���string��ʧ��,�����" << endl;

			int myint;
			if (cft.item_get_value("a", "����", myint))
				cout << "�ɹ���ȡ��[a]��������,ȡ��ֵ:" << myint << " ʵ��ֵ:666666" << endl;
			else
				cout << "��ȡ��[a]��������ʧ��,�����" << endl;
			char mychar;
			if (cft.item_get_value("d", "�ַ�", mychar))
				cout << "�ɹ���ȡ��[d]���ַ���,ȡ��ֵ:" << mychar << " ʵ��ֵ:X" << endl;
			else
				cout << "��ȡ��[d]���ַ���ʧ��,�����" << endl;

			cout << "item_get_value�������:" << endl;
			cout << "---------------------" << endl;
			to_be_continued(NULL);
			if (cft.item_get_value("b", "NULL"))
				cout << "ʹ������item_get_value������1,pdf��Ϊ0/1����" << endl;
			else
				cout << "ʹ������item_get_value������0,pdf��Ϊ0/1����" << endl;

			if (cft.item_get_value("�����ڵ���", "�ַ���", s))
				cout << "�ɹ���ȡ��[�����ڵ���]���ַ����������������?" << endl;
			else
				cout << "�޷���ȡ��[�����ڵ���]���ַ����ͨ������" << endl;
			double mydouble;
			if (cft.item_get_value(NULL, "������", mydouble))
				cout << "����ΪNULLӦ����0���������!" << endl;
			else
				cout << "�޷���ȡNULL���е�һ�������, ����0, ��ȷ" << endl;
			cft.close();
			to_be_continued(NULL);
		}

	}

	if (1) {
		cout << "g���Խ���,���濪ʼs����" << endl;
		cout << "---------------------" << endl;
		to_be_continued(NULL);

		if (1)
		{
			cout << "item_add������Ӳ���:" << endl;
			cout << "---------------------" << endl;

			cft.open(TESTFILE_S, OPEN_OPT_RDWR);
			assert(cft.is_open());

			if (cft.item_add(NULL, "�ַ���", "��¬����ү"))
				cout << "��ӳɹ�,����� �ַ���=\"��¬����ү\"��?" << endl;
			else
				cout << "���ʧ��!��ȥ����!" << endl;

			if (cft.item_add(NULL, "�ַ���string", string("�����ү")))
				cout << "��ӳɹ�,����� �ַ���string=\"�����ү\"��?" << endl;
			else
				cout << "���ʧ��!��ȥ����!" << endl;

			if (cft.item_add(NULL, "����", 123123))
				cout << "��ӳɹ�,����� ����=123123��?" << endl;
			else
				cout << "���ʧ��!��ȥ����!" << endl;

			if (cft.item_add(NULL, "������", 123.465))
				cout << "��ӳɹ�,����� ������=123.456��?" << endl;
			else
				cout << "���ʧ��!��ȥ����!" << endl;

			if (cft.item_add(NULL, "�ַ�", 'S'))
				cout << "��ӳɹ�,����� �ַ�='S'��?" << endl;
			else
				cout << "���ʧ��!��ȥ����!" << endl;

			if (cft.item_add(NULL, "NULL"))
				cout << "��ӳɹ�,����� NULL=    (�հ�)��?" << endl;
			else
				cout << "���ʧ��!��ȥ����!" << endl;

			putchar('\n');
			to_be_continued(NULL);
			cout << "item_add�ظ���Ӳ���:" << endl;
			cout << "---------------------" << endl;

			if (cft.item_add(NULL, "�ַ���", "��¬����ү"))
				cout << "�ɹ�������ظ����ַ���,��ȥ����!" << endl;
			else
				cout << "����ظ����ַ���ʧ��!" << endl;

			if (cft.item_add(NULL, "����", 123123))
				cout << "�ɹ�������ظ�������,��ȥ����!" << endl;
			else
				cout << "����ظ�������ʧ��!" << endl;

			if (cft.item_add(NULL, "������", 123.465))
				cout << "�ɹ�������ظ������,��ȥ����!" << endl;
			else
				cout << "����ظ������ʧ��!" << endl;

			if (cft.item_add(NULL, "�ַ�", 'S'))
				cout << "�ɹ�������ظ����ַ�,��ȥ����!" << endl;
			else
				cout << "����ظ����ַ�ʧ��!" << endl;

			if (cft.item_add(NULL, "NULL"))
				cout << "�ɹ�������ظ���<NULL>,��ȥ����!" << endl;
			else
				cout << "����ظ���<NULL>ʧ��!" << endl;

			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "item_delɾ������:" << endl;
			cout << "---------------------" << endl;
			to_be_continued(NULL);
			cft.open(TESTFILE_S, OPEN_OPT_RDWR);
			assert(cft.is_open());
			cout << "���ֶ����⸴���ַ�����Ŀ3��,��������Ŀ1��" << endl;
			to_be_continued(NULL);

			if (cft.item_del(NULL, "�ַ���") == 4)
				cout << "���е��ַ�����ɾ���ɹ�" << endl;
			else
				cout << "δ��ɾ�������ַ�������ȥ����" << endl;


			if (cft.item_del(NULL, "������") == 2)
				cout << "���и�������ɾ���ɹ�" << endl;
			else
				cout << "δ��ɾ�����и��������ȥ����" << endl;

			if (cft.item_del(NULL, "�ַ�") == 1)
				cout << "�ַ���ɾ���ɹ�" << endl;
			else
				cout << "δ��ɾ���ַ����ȥ����" << endl;

			putchar('\n');
			to_be_continued(NULL);
			cout << "item_delɾ�������������:" << endl;
			cout << "---------------------" << endl;

			if (cft.item_del(NULL, "�����ڵ��ַ�����") != 0)
				cout << "ɾ���˲����ڵ��ַ�����,��ȥ����" << endl;
			else
				cout << "δɾ���������ַ�����,����ͨ��" << endl;

			if (cft.item_del(NULL, "�����ڵ�������") != 0)
				cout << "ɾ���˲����ڵ�������,��ȥ����" << endl;
			else
				cout << "δɾ��������������,����ͨ��" << endl;

			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1) {
			cout << "item_update���²���:" << endl;
			cout << "---------------------" << endl;
			cft.open(TESTFILE_S, OPEN_OPT_RDWR);
			assert(cft.is_open());

			if (cft.item_update(NULL, "����", 666666))
				cout << "�ɹ������������ֵ,�����Ƿ�Ϊ*666666*" << endl;
			else
				cout << "�޷������������ֵ����ȥ����!" << endl;

			if (cft.item_update(NULL, "�ַ���string", "�����Ҹ��µ��ַ���"))
				cout << "�ɹ������ַ���string���ֵ,�����Ƿ�Ϊ*\"�����Ҹ��µ��ַ���\"*" << endl;
			else
				cout << "�޷������ַ���string���ֵ����ȥ����!" << endl;

			if (cft.item_update(NULL, "������", 654.321))
				cout << "���ڲ�������Ŀ�ĸ���,�����ĩβ,����ĩβ�Ƿ�Ϊ*������=654.321*" << endl;
			else
				cout << "�޷����¸��������ֵ����ȥ����!" << endl;
			putchar('\n');
			to_be_continued(NULL);
			cout << "item_update���ļ�ʹ���������²���:" << endl;
			cout << "---------------------" << endl;

			if (cft.item_update("�����ڵ���", "������", 888.999))
				cout << "��Ȼ���³ɹ���,����ȥ����" << endl;
			else
				cout << "���ļ��޷����º������ĸ��������ֵ,����ͨ��" << endl;
			cft.close();
			to_be_continued(NULL);

		}

		if (1) {
			cout << "item_get_value����ȡֵ����:" << endl;
			cout << "---------------------" << endl;
			to_be_continued(NULL);
			cft.open(TESTFILE_S, OPEN_OPT_RDWR);
			assert(cft.is_open());


			string ss;
			if (cft.item_get_value(NULL, "�ַ���string", ss))
				cout << "�ɹ���ȡ�ַ���string��,ȡ��ֵ:" << ss << " ʵ��ֵ:�����Ҹ��µ��ַ���" << endl;
			else
				cout << "��ȡ�ַ���string��ʧ��,�����" << endl;

			int myint;
			if (cft.item_get_value(NULL, "����", myint))
				cout << "�ɹ���ȡ������,ȡ��ֵ:" << myint << " ʵ��ֵ:666666" << endl;
			else
				cout << "��ȡ������ʧ��,�����" << endl;
			putchar('\n');

			cout << "item_get_value�������:" << endl;
			cout << "---------------------" << endl;
			to_be_continued(NULL);
			if (cft.item_get_value("b", "NULL"))
				cout << "ʹ������item_get_value������1,pdf��Ϊ0/1����" << endl;
			else
				cout << "ʹ������item_get_value������0,pdf��Ϊ0/1����" << endl;

			if (cft.item_get_value(NULL, "�ַ���", ss))
				cout << "�ɹ���ȡ�����ڵ��ַ����������������?" << endl;
			else
				cout << "�޷���ȡ�����ڵ��ַ����ͨ������" << endl;

			if (cft.item_get_value("�����ڵ���", "����", s))
				cout << "���ļ��ɹ���ȡ�������������������������?" << endl;
			else
				cout << "���ļ��޷���ȡ�������������ͨ������" << endl;
			cft.close();
			to_be_continued(NULL);

		}

		cft.close();
		cout << "������Խ���!��л���!" << endl;
		cout << "---------------------" << endl;


	}
}



