#include<iostream>
#include<stdio.h>
#include <string>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
const int MAX = 100;
void write();
int serch();
void ccout();
void del();
void change();
void failed();
void list();
int i, j, k;
string filename;
struct student
{
	char num[10];
	char name[10];
	string sex;
	int math;
	int chinese;
	int english;
	int average;
	int number;
	string getnum(int a)
	{
		cin >> stu[a].num;
		return  stu[a].num;
	}
	string getname(int a)
	{
		cin >> stu[a].name;
		return stu[a].name;
	}
	string getsex(int a)
	{
		cin >> stu[a].sex;
		return stu[a].sex;
	}
	int getmath(int a)
	{
		cin >> stu[a].math;
		return stu[a].math;
	}
	int getchinese(int a)
	{
		cin >> stu[a].chinese;
		return stu[a].chinese;
	}
	int getenglish(int a)
	{
		cin >> stu[a].english;
		return stu[a].english;
	}

}stu[MAX];
int count()
{
	char m[1000];
	int t = 0, r = 0;
	ifstream read;
	read.open(filename, ios::in);
	if (!read)
	{
		cout << "can't open the file." << endl;
	}
	read >> m;
	for (; strcmp(m, "#") == 0; )
	{
		read >> stu[i].num >> stu[i].name >> stu[i].sex >> stu[i].math >> stu[i].chinese >> stu[i].english;
		r = r + 1;
		read >> m;
	}
	return r;
	read.close();
}
string getnum(int a)
{
	cin >> stu[a].num;
	return  stu[a].num;
}
string getname(int a)
{
	cin >> stu[a].name;
	return stu[a].name;
}
string getsex(int a)
{
	cin >> stu[a].sex;
	return stu[a].sex;
}
int getmath(int a)
{
	cin >> stu[a].math;
	return stu[a].math;
}
int getchinese(int a)
{
	cin >> stu[a].chinese;
	return stu[a].chinese;
}
int getenglish(int a)
{
	cin >> stu[a].english;
	return stu[a].english;
}

int getm(int m)
{
	return m;
}
string s(int a)
{
	string res;
	stringstream ss;
	ss << a;
	ss >> res;
	return res;
}
int z(string res)
{
	int a;
	stringstream ss;
	ss << res;
	ss >> a;
	return a;
}
int serch_num()
{
	int o = 0, u;
	char m[10], y[10];
	ifstream read;
	read.open(filename, ios::in);
	if (!read)
	{
		cout << "can't open the file." << endl;
	}
	cout << "请输入查找学号：" << endl;
	cin >> m;
	for (i = 0; i < count(); i++)
	{
		read >> y >> stu[i].num >> stu[i].name >> stu[i].sex >> stu[i].math >> stu[i].chinese >> stu[i].english;
		if (strcmp(m, stu[i].num) == 0)
		{
			cout << "学号" << "\t" << "姓名" << "\t" << "性别" << "\t" << "数学" << "\t" << "语文" << "\t" << "英语" << endl;
			cout << stu[i].num << "\t" << stu[i].name << "\t" << stu[i].sex << "\t" << stu[i].math << "\t" << stu[i].chinese << "\t" << stu[i].english << endl;
			o = 1;
			u = i;
		}
	}
	if (o == 0)
	{
		cout << "未找到该学生" << endl;
		return 0;
	}
	else
		return u;
}
int serch_name()
{
	int o = 0, u;
	char m[10], y[10];
	ifstream read;
	read.open(filename, ios::in);
	if (!read)
	{
		cout << "can't open the file." << endl;
	}
	cout << "请输入查找姓名：" << endl;
	cin >> m;
	for (i = 0; i < count(); i++)
	{
		read >> y >> stu[i].num >> stu[i].name >> stu[i].sex >> stu[i].math >> stu[i].chinese >> stu[i].english;
		if (strcmp(m, stu[i].name) == 0)
		{
			cout << "学号" << "\t" << "姓名" << "\t" << "性别" << "\t" << "数学" << "\t" << "语文" << "\t" << "英语" << endl;
			cout << stu[i].num << "\t" << stu[i].name << "\t" << stu[i].sex << "\t" << stu[i].math << "\t" << stu[i].chinese << "\t" << stu[i].english << endl;
			o = 1;
			u = i;
		}
	}
	if (o == 0)
	{
		cout << "未找到该学生" << endl;
		return 0;
	}
	else
		return u;
}
int main()
{
	char ch;
	cout << "Please input the filename(student):";
	cin >> filename;
	cout << "Y or N 进入菜单栏" << endl;
	cin >> ch;
	ch = toupper(ch);
	while (ch == 'Y' || 1 || 2 || 3 || 4 || 5 || 6)
	{
		cout << endl << "         菜单：" << endl
			<< "         1.录入学生成绩" << endl
			<< "         2.查询学生成绩" << endl
			<< "         3.显示所有学生成绩" << endl
			<< "         4.删除学生成绩" << endl
			<< "         5.修改学生成绩" << endl
			<< "         6.显示至少有一门不及格的学生" << endl
			<< "         7.显示平均分并排序" << endl;
		cin >> k;
		cout << endl;
		switch (k)
		{
		case 1:write(); break;
		case 2:serch(); break;
		case 3:ccout(); break;
		case 4:del(); break;
		case 5:change(); break;
		case 6:failed(); break;
		case 7:list(); break;
		}
		cout << "Y or N 进入菜单栏" << endl;
		cin >> ch;
		ch = toupper(ch);
	}
	cout << "Tank for using" << endl;
	return 0;
}

void write()
{
	int n, a;
	ofstream write;
	write.open(filename, ios::app);
	if (!write)
	{
		cout << "can't open the file." << endl;
	}
	cout << "录入学生人数：" << endl;
	cin >> n;
	j = count();
	for (a = 0; a < n; a++)
	{
		write << "#" << " ";
		cout << "请输入学生学号" << endl;
		write << getnum(j) << " ";
		cout << "请输入学生姓名" << endl;
		write << getname(j) << " ";
		cout << "请输入学生性别" << endl;
		write << getsex(j) << " ";
		cout << "请输入数学成绩" << endl;
		write << getmath(j) << " ";
		cout << "请输入语文成绩" << endl;
		write << getchinese(j) << " ";
		cout << "请输入英语成绩" << endl;
		write << getenglish(j);
		j = j + 1;
	}
	cout << "录入成功" << endl;
	write.close();
}
int serch()
{
	int sh, l;
	cout << "请选择以下查找方法：" << endl << "1.按学号查找" << endl << "2.按姓名查找" << endl;
	cin >> sh;
	switch (sh)
	{
	case 1:l = serch_num(); break;
	case 2:l = serch_name(); break;
	}
	return l;
}

void ccout()
{
	int i;
	char m[10];
	ifstream read;
	read.open(filename, ios::in);
	if (!read)
	{
		cout << "can't open the file." << endl;
	}
	for (i = 0; i < count(); i++)
	{
		cout << "学号" << "\t" << "姓名" << "\t" << "性别" << "\t" << "数学" << "\t" << "语文" << "\t" << "英语" << endl;
		read >> m >> stu[i].num >> stu[i].name >> stu[i].sex >> stu[i].math >> stu[i].chinese >> stu[i].english;
		cout << stu[i].num << "\t" << stu[i].name << "\t" << stu[i].sex << "\t" << stu[i].math << "\t" << stu[i].chinese << "\t" << stu[i].english << endl;
	}
	read.close();
}
void D(int p)
{
	int w;
	w = count();
	ofstream write;
	write.open(filename, ios::ate);
	if (!write)
	{
		cout << "can't open the file." << endl;
	}
	for (i = 0; i < p; i++)
	{
		write << "#" << " " << stu[i].num << " " << stu[i].name << " " << stu[i].sex << " " << stu[i].math << " " << stu[i].chinese << " " << stu[i].english;
	}
	for (i = p; i < w - 1; i++)
	{
		write << "#" << " " << stu[i + 1].num << " " << stu[i + 1].name << " " << stu[i + 1].sex << " " << stu[i + 1].math << " " << stu[i + 1].chinese << " " << stu[i + 1].english;
	}
}
void del()
{
	char ch[10];
	int d, s;
	d = serch();
	cout << "是否删除该条成绩" << endl << "Y OR N" << endl;
	cin >> ch;
	if (strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)
	{
		s = 1;
	}
	else s = 2;
	switch (s)
	{
	case 1:  D(d); cout << "该条成绩已删除" << endl; break;
	case 2: break;
	}
}

void change()
{
	int c;
	c = serch();
	D(c);;
	ofstream write;
	write.open(filename, ios::app);
	if (!write)
	{
		cout << "can't open the file." << endl;
	}
	j = count();
	write << "#" << " ";
	cout << "请输入学生学号" << endl;
	write << getnum(j) << " ";
	cout << "请输入学生姓名" << endl;
	write << getname(j) << " ";
	cout << "请输入学生性别" << endl;
	write << getsex(j) << " ";
	cout << "请输入数学成绩" << endl;
	write << getmath(j) << " ";
	cout << "请输入语文成绩" << endl;
	write << getchinese(j) << " ";
	cout << "请输入英语成绩" << endl;
	write << getenglish(j);
	cout << "修改成功" << endl;
	write.close();
}

void failed()
{
	char  y[10];
	ifstream read;
	read.open(filename, ios::in);
	if (!read)
	{
		cout << "can't open the file." << endl;
	}
	for (i = 0; i < count(); i++)
	{
		read >> y >> stu[i].num >> stu[i].name >> stu[i].sex >> stu[i].math >> stu[i].chinese >> stu[i].english;
		if (stu[i].math < 60 || stu[i].chinese < 60 || stu[i].english < 60)
		{
			cout << "学号" << "\t" << "姓名" << "\t" << "性别" << "\t" << "数学" << "\t" << "语文" << "\t" << "英语" << endl;
			cout << stu[i].num << "\t" << stu[i].name << "\t" << stu[i].sex << "\t" << stu[i].math << "\t" << stu[i].chinese << "\t" << stu[i].english << endl;
		}
	}
}
void list()
{
	struct student temp;
	int i, max = 0, j;
	char m[10];
	ifstream read;
	read.open(filename, ios::in);
	if (!read)
	{
		cout << "can't open the file." << endl;
	}
	for (i = 0; i < count(); i++)
	{
		read >> m >> stu[i].num >> stu[i].name >> stu[i].sex >> stu[i].math >> stu[i].chinese >> stu[i].english;
		stu[i].average = int((stu[i].math + stu[i].chinese + stu[i].english) / 3);
	}
	for (i = 0; i < count() - 1; i++)
	{
		max = i;
		for (j = i; j <= count() - 2; j++)
			if (stu[max].average < stu[j + 1].average)
				max = j + 1;
		if (max != i)
		{
			temp = stu[i];
			stu[i] = stu[max];
			stu[max] = temp;
		}
	}
	for (i = 0; i < count(); i++)
	{
		cout << "学号" << "\t" << "姓名" << "\t" << "性别" << "\t" << "数学" << "\t" << "语文" << "\t" << "英语" << "\t" << "平均分" << endl;
		cout << stu[i].num << "\t" << stu[i].name << "\t" << stu[i].sex << "\t" << stu[i].math << "\t" << stu[i].chinese << "\t" << stu[i].english << "\t" << stu[i].average << endl;
	}
	read.close();
}