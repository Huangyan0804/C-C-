#include<iostream>
#include<cstdio>
#include<algorithm>
#include<string>
using namespace std;

string week[] = { "日","一","二","三","四","五","六" };
int monthdays[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
int year, month;
int firstday, cnt;

int leapyear(int year) {
	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
		return 1;
	else
		return 0;
}

int zeller(int y, int m) {
	int d = 1;
	if (m == 1 || m == 2)
	{
		y--;
		m += 12;
	}
	int c = y / 100;
	y %= 100;
	int w = y + y / 4 + c / 4 - 2 * c + 26 * (m + 1) / 10 + d - 1;
	if (w < 0)
	{
		w = (w % 7) + 7;
	}
	return w % 7;
}

void show() {
	for (int i = 0; i < 7; i++)
		cout << week[i] << '\t';
	cout << endl;
	for (int i = 0; i < 7; i++)
		cout << "--" << '\t';
	cout << endl;
	firstday = zeller(year, month);
	for (int i = 0; i < firstday; i++) {
		cout << '\t';
	}
	cnt = firstday;
	for (int i = 1; i <= monthdays[month]; i++) {
		cout << i << '\t';
		cnt++;
		if (cnt == 7) {
			cout << endl;
			cnt = 0;
		}
	}
}
int main() {
	cout << "请输入年、月：";
	cin >> year >> month;
	if (leapyear(year))
		monthdays[2] = 29;
	show();
	return 0;
}