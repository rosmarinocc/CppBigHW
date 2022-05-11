/*1852877 计科 赵昊堃*/
/*1853769 计科 许辰昊 1953793 计科 周雨杨 1950455 周珂帆*/
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
		cout << prompt << ",按回车键继续..." << endl;
	else
		cout << "按回车键继续..." << endl;
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
		cout << "g测试开始" << endl;
		cout << "---------------------" << endl;
		to_be_continued(NULL);
		if (1)
		{
			cout << "group_add正常添加测试:" << endl;
			cout << "---------------------" << endl;
			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());
			s[1] = '\0';
			for (i = 0; i < 5; i++)
			{
				s[0] = 'a' + i;
				if (cft.group_add(s))
					cout << "test" << i + 1 << ":成功添加了" << s << endl;
				else
					cout << "test" << i + 1 << ":添加" << s << "失败, 速去纠错" << endl;
			}
			putchar('\n');
			to_be_continued(NULL);


			cout << "group_add重复添加测试:" << endl;
			cout << "---------------------" << endl;
			for (i = 4; i >= 0; i--)
			{
				s[0] = 'a' + i;
				if (cft.group_add(s))
					cout << "test" << 5 - i << ":成功添加了重复组" << s << ",速去纠错" << endl;
				else
					cout << "test" << 5 - i << ":无法添加重复的组" << s << endl;
			}
			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "group_del正常删除测试:" << endl;
			cout << "---------------------" << endl;
			cout << "下面将自动添加重复组..." << endl;
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
				cout << "已添加" << m[i] << "个组[" << char(i + 'f') << "]" << endl;
			for (i = 0; i < 5; i++)
			{
				s[0] = i + 'f';
				int cnt = cft.group_del(s);
				if (cnt == m[i])
					cout << "成功删除所有的组[" << s << ']' << endl;
				else
					cout << "只删除了" << cnt << "个组[" << s << "],理论上应该为" << m[i] << "个,速去纠错" << endl;
			}
			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "item_add正常添加测试:" << endl;
			cout << "---------------------" << endl;

			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());
			for (i = 0; i < 5; i++)
			{
				s[0] = i + 'a';
				if (cft.item_add(s, "字符串", "helloworld"))
					cout << "组[" << s << "]已添加字符串项目" << endl;
				else
					cout << "添加失败!速去纠错!" << endl;

				if (cft.item_add(s, "字符串string", string("沈坚大法好<string>")))
					cout << "组[" << s << "]已添加字符串<string>项目" << endl;
				else
					cout << "添加失败!速去纠错!" << endl;

				if (cft.item_add(s, "整数", 123123))
					cout << "组[" << s << "]已添加整数项目" << endl;
				else
					cout << "添加失败!速去纠错!" << endl;

				if (cft.item_add(s, "浮点数", 123.465))
					cout << "组[" << s << "]已添加浮点数项目" << endl;
				else
					cout << "添加失败!速去纠错!" << endl;

				if (cft.item_add(s, "字符", 'X'))
					cout << "组[" << s << "]已添加字符项目" << endl;
				else
					cout << "添加失败!速去纠错!" << endl;

				if (cft.item_add(s, "NULL"))
					cout << "组[" << s << "]已添加<NULL>项目" << endl;
				else
					cout << "添加失败!速去纠错!" << endl;

			}
			putchar('\n');
			to_be_continued(NULL);
			cout << "item_add重复添加测试:" << endl;
			cout << "---------------------" << endl;

			for (i = 4; i >= 0; i--)
			{
				s[0] = i + 'a';
				if (cft.item_add(s, "字符串", "helloworld"))
					cout << "成功添加了重复项字符串,速去纠错!" << endl;
				else
					cout << "添加重复项字符串失败!" << endl;

				if (cft.item_add(s, "整数", 123456))
					cout << "成功添加了重复项整数,速去纠错!" << endl;
				else
					cout << "添加重复项整数失败!" << endl;

				if (cft.item_add(s, "浮点数", 123.465))
					cout << "成功添加了重复项浮点数,速去纠错!" << endl;
				else
					cout << "添加重复项浮点数失败!" << endl;

				if (cft.item_add(s, "字符", 'A'))
					cout << "成功添加了重复项字符,速去纠错!" << endl;
				else
					cout << "添加重复项字符失败!" << endl;

				if (cft.item_add(s, "NULL"))
					cout << "成功添加了重复项<NULL>,速去纠错!" << endl;
				else
					cout << "添加重复项<NULL>失败!" << endl;
			}
			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "item_del删除测试:" << endl;
			cout << "---------------------" << endl;
			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());

			if (cft.item_del(NULL, "字符串") == 5)
				cout << "所有的字符串项删除成功" << endl;
			else
				cout << "未能删除所有字符串，速去纠错" << endl;

			if (cft.item_del("a", "浮点数") == 1)
				cout << "组[a]的浮点数项删除成功" << endl;
			else
				cout << "未能删除组[a]的浮点数项，速去纠错" << endl;

			if (cft.item_del("c", "字符") == 1)
				cout << "组[c]的字符项删除成功" << endl;
			else
				cout << "未能删除组[c]的字符项，速去纠错" << endl;

			putchar('\n');
			to_be_continued(NULL);
			cout << "item_del删除不存在项测试:" << endl;
			cout << "---------------------" << endl;

			if (cft.item_del(NULL, "不存在的字符串项") != 0)
				cout << "删除了不存在的字符串项,速去纠错" << endl;
			else
				cout << "未删除不存在字符串项,测试通过" << endl;

			if (cft.item_del("b", "不存在的整数项") != 0)
				cout << "删除了组[b]不存在的整数项,速去纠错" << endl;
			else
				cout << "未删除不存在整数项,测试通过" << endl;

			if (cft.item_del("e", "不存在的<NULL>项") != 0)
				cout << "删除了组[e]不存在的<NULL>项,速去纠错" << endl;
			else
				cout << "未删除不存在<NULL>项,测试通过" << endl;

			if (cft.item_del("不存在的组", "NULL") != 0)
				printf("删除了组[不存在的组]的不存在的<NULL>项,速去纠错\n");
			else
				cout << "未删除组[不存在的组]的不存在<NULL>项,测试通过" << endl;
			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "item_update更新测试:" << endl;
			cout << "---------------------" << endl;
			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());

			if (cft.item_update("a", "整数", 666666))
				cout << "成功更新组[a]的整数项的值,请检查是否为*666666*" << endl;
			else
				cout << "无法更新组[a]的整数项的值，速去纠错!" << endl;

			if (cft.item_update("e", "字符串", "这是我更新的字符串"))
				cout << "成功更新组[e]的字符串项的值,请检查是否为*\"这是我更新的字符串\"*" << endl;
			else
				cout << "无法更新组[e]的字符串项的值，速去纠错!" << endl;

			if (cft.item_update("c", "字符串string", "又是我更新的字符串"))
				cout << "成功更新组[c]的字符串string项的值,请检查是否为*\"又是我更新的字符串\"*" << endl;
			else
				cout << "无法更新组[c]的字符串string项的值，速去纠错!" << endl;

			if (cft.item_update("a", "浮点数", 654.321))
				cout << "成功更新组[a]的浮点数项的值,请检查是否为*654.321*" << endl;
			else
				cout << "无法更新组[a]的浮点数项的值，速去纠错!" << endl;
			putchar('\n');
			to_be_continued(NULL);
			cout << "item_update不存在组更新测试:" << endl;
			cout << "---------------------" << endl;

			if (cft.item_update("不存在的组", "浮点数", 888.999))
				cout << "居然更新成功了,还不去纠错" << endl;
			else
				cout << "无法更新组[不存在的组]的浮点数项的值,测试通过" << endl;
			cft.close();
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "item_get_value正常取值测试:" << endl;
			cout << "---------------------" << endl;
			to_be_continued(NULL);
			cft.open(TESTFILE_G, OPEN_OPT_RDWR);
			assert(cft.is_open());


			if (cft.item_get_value("e", "字符串", s))
				cout << "成功获取组[e]的字符串项,取得值:" << s << " 实际值:这是我更新的字符串" << endl;
			else
				cout << "获取组[e]的字符串项失败,请纠错" << endl;

			string ss;
			if (cft.item_get_value("c", "字符串string", ss))
				cout << "成功获取组[c]的字符串string项,取得值:" << ss << " 实际值:又是我更新的字符串" << endl;
			else
				cout << "获取组[c]的字符串string项失败,请纠错" << endl;

			int myint;
			if (cft.item_get_value("a", "整数", myint))
				cout << "成功获取组[a]的整数项,取得值:" << myint << " 实际值:666666" << endl;
			else
				cout << "获取组[a]的整数项失败,请纠错" << endl;
			char mychar;
			if (cft.item_get_value("d", "字符", mychar))
				cout << "成功获取组[d]的字符项,取得值:" << mychar << " 实际值:X" << endl;
			else
				cout << "获取组[d]的字符项失败,请纠错" << endl;

			cout << "item_get_value特殊测试:" << endl;
			cout << "---------------------" << endl;
			to_be_continued(NULL);
			if (cft.item_get_value("b", "NULL"))
				cout << "使用两参item_get_value，返回1,pdf认为0/1均可" << endl;
			else
				cout << "使用两参item_get_value，返回0,pdf认为0/1均可" << endl;

			if (cft.item_get_value("不存在的组", "字符串", s))
				cout << "成功获取组[不存在的组]的字符串项，你觉得这合理吗?" << endl;
			else
				cout << "无法获取组[不存在的组]的字符串项，通过测试" << endl;
			double mydouble;
			if (cft.item_get_value(NULL, "浮点数", mydouble))
				cout << "组名为NULL应返回0，纠错纠错!" << endl;
			else
				cout << "无法获取NULL组中第一个项浮点数, 返回0, 正确" << endl;
			cft.close();
			to_be_continued(NULL);
		}

	}

	if (1) {
		cout << "g测试结束,下面开始s测试" << endl;
		cout << "---------------------" << endl;
		to_be_continued(NULL);

		if (1)
		{
			cout << "item_add正常添加测试:" << endl;
			cout << "---------------------" << endl;

			cft.open(TESTFILE_S, OPEN_OPT_RDWR);
			assert(cft.is_open());

			if (cft.item_add(NULL, "字符串", "迪卢克姥爷"))
				cout << "添加成功,请查验 字符串=\"迪卢克姥爷\"吗?" << endl;
			else
				cout << "添加失败!速去纠错!" << endl;

			if (cft.item_add(NULL, "字符串string", string("沈坚姥爷")))
				cout << "添加成功,请查验 字符串string=\"沈坚姥爷\"吗?" << endl;
			else
				cout << "添加失败!速去纠错!" << endl;

			if (cft.item_add(NULL, "整数", 123123))
				cout << "添加成功,请查验 整数=123123吗?" << endl;
			else
				cout << "添加失败!速去纠错!" << endl;

			if (cft.item_add(NULL, "浮点数", 123.465))
				cout << "添加成功,请查验 浮点数=123.456吗?" << endl;
			else
				cout << "添加失败!速去纠错!" << endl;

			if (cft.item_add(NULL, "字符", 'S'))
				cout << "添加成功,请查验 字符='S'吗?" << endl;
			else
				cout << "添加失败!速去纠错!" << endl;

			if (cft.item_add(NULL, "NULL"))
				cout << "添加成功,请查验 NULL=    (空白)吗?" << endl;
			else
				cout << "添加失败!速去纠错!" << endl;

			putchar('\n');
			to_be_continued(NULL);
			cout << "item_add重复添加测试:" << endl;
			cout << "---------------------" << endl;

			if (cft.item_add(NULL, "字符串", "迪卢克姥爷"))
				cout << "成功添加了重复项字符串,速去纠错!" << endl;
			else
				cout << "添加重复项字符串失败!" << endl;

			if (cft.item_add(NULL, "整数", 123123))
				cout << "成功添加了重复项整数,速去纠错!" << endl;
			else
				cout << "添加重复项整数失败!" << endl;

			if (cft.item_add(NULL, "浮点数", 123.465))
				cout << "成功添加了重复项浮点数,速去纠错!" << endl;
			else
				cout << "添加重复项浮点数失败!" << endl;

			if (cft.item_add(NULL, "字符", 'S'))
				cout << "成功添加了重复项字符,速去纠错!" << endl;
			else
				cout << "添加重复项字符失败!" << endl;

			if (cft.item_add(NULL, "NULL"))
				cout << "成功添加了重复项<NULL>,速去纠错!" << endl;
			else
				cout << "添加重复项<NULL>失败!" << endl;

			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1)
		{
			cout << "item_del删除测试:" << endl;
			cout << "---------------------" << endl;
			to_be_continued(NULL);
			cft.open(TESTFILE_S, OPEN_OPT_RDWR);
			assert(cft.is_open());
			cout << "请手动另外复制字符串项目3行,浮点数项目1行" << endl;
			to_be_continued(NULL);

			if (cft.item_del(NULL, "字符串") == 4)
				cout << "所有的字符串项删除成功" << endl;
			else
				cout << "未能删除所有字符串，速去纠错" << endl;


			if (cft.item_del(NULL, "浮点数") == 2)
				cout << "所有浮点数项删除成功" << endl;
			else
				cout << "未能删除所有浮点数项，速去纠错" << endl;

			if (cft.item_del(NULL, "字符") == 1)
				cout << "字符项删除成功" << endl;
			else
				cout << "未能删除字符项，速去纠错" << endl;

			putchar('\n');
			to_be_continued(NULL);
			cout << "item_del删除不存在项测试:" << endl;
			cout << "---------------------" << endl;

			if (cft.item_del(NULL, "不存在的字符串项") != 0)
				cout << "删除了不存在的字符串项,速去纠错" << endl;
			else
				cout << "未删除不存在字符串项,测试通过" << endl;

			if (cft.item_del(NULL, "不存在的整数项") != 0)
				cout << "删除了不存在的整数项,速去纠错" << endl;
			else
				cout << "未删除不存在整数项,测试通过" << endl;

			cft.close();
			putchar('\n');
			to_be_continued(NULL);
		}

		if (1) {
			cout << "item_update更新测试:" << endl;
			cout << "---------------------" << endl;
			cft.open(TESTFILE_S, OPEN_OPT_RDWR);
			assert(cft.is_open());

			if (cft.item_update(NULL, "整数", 666666))
				cout << "成功更新整数项的值,请检查是否为*666666*" << endl;
			else
				cout << "无法更新整数项的值，速去纠错!" << endl;

			if (cft.item_update(NULL, "字符串string", "又是我更新的字符串"))
				cout << "成功更新字符串string项的值,请检查是否为*\"又是我更新的字符串\"*" << endl;
			else
				cout << "无法更新字符串string项的值，速去纠错!" << endl;

			if (cft.item_update(NULL, "浮点数", 654.321))
				cout << "对于不存在项目的更新,添加至末尾,请检查末尾是否为*浮点数=654.321*" << endl;
			else
				cout << "无法更新浮点数项的值，速去纠错!" << endl;
			putchar('\n');
			to_be_continued(NULL);
			cout << "item_update简单文件使用组名更新测试:" << endl;
			cout << "---------------------" << endl;

			if (cft.item_update("不存在的组", "浮点数", 888.999))
				cout << "居然更新成功了,还不去纠错" << endl;
			else
				cout << "简单文件无法更新含组名的浮点数项的值,测试通过" << endl;
			cft.close();
			to_be_continued(NULL);

		}

		if (1) {
			cout << "item_get_value正常取值测试:" << endl;
			cout << "---------------------" << endl;
			to_be_continued(NULL);
			cft.open(TESTFILE_S, OPEN_OPT_RDWR);
			assert(cft.is_open());


			string ss;
			if (cft.item_get_value(NULL, "字符串string", ss))
				cout << "成功获取字符串string项,取得值:" << ss << " 实际值:又是我更新的字符串" << endl;
			else
				cout << "获取字符串string项失败,请纠错" << endl;

			int myint;
			if (cft.item_get_value(NULL, "整数", myint))
				cout << "成功获取整数项,取得值:" << myint << " 实际值:666666" << endl;
			else
				cout << "获取整数项失败,请纠错" << endl;
			putchar('\n');

			cout << "item_get_value特殊测试:" << endl;
			cout << "---------------------" << endl;
			to_be_continued(NULL);
			if (cft.item_get_value("b", "NULL"))
				cout << "使用两参item_get_value，返回1,pdf认为0/1均可" << endl;
			else
				cout << "使用两参item_get_value，返回0,pdf认为0/1均可" << endl;

			if (cft.item_get_value(NULL, "字符串", ss))
				cout << "成功获取不存在的字符串项，你觉得这合理吗?" << endl;
			else
				cout << "无法获取不存在的字符串项，通过测试" << endl;

			if (cft.item_get_value("不存在的组", "整数", s))
				cout << "简单文件成功获取含组名的整数项，你觉得这合理吗?" << endl;
			else
				cout << "简单文件无法获取含组名的整数项，通过测试" << endl;
			cft.close();
			to_be_continued(NULL);

		}

		cft.close();
		cout << "两项测试结束!感谢配合!" << endl;
		cout << "---------------------" << endl;


	}
}



