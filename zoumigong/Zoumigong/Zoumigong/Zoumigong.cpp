#include <Windows.h>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include"createmaze.h"
using namespace std;
const int Width = 70;
const int Length = 30;

void start(); //��ʼ��
void show(); //��ʾ����
void manuallcontrol(); //�˹����� ���뷽��
void automaticcontrol(); //�Զ�����
void gotoxy(int x, int y);
void HideCursor(); //������
bool okmove(int x, int y);
void create_maze();
void traval_maze(int x, int y, int d);

//----------------ȫ�ֱ���----------------
bool live = true;
bool win = false;
int map[Length + 1][Width + 1]; // ��ʼ������
int man_x, man_y; // �����λ��
int target_x, target_y; // �յ�λ��
int dir[4][2] = { -1,1,0,0,0,0,-1,1 };//��������
int old_d;
int end_x = Width - 1, end_y = Length - 1;
//--------------------------------------
int main()
{
	start();
	while (live && !win) {
		show();
		manuallcontrol();
		automaticcontrol();
	}
	if (win) {
		system("cls");
		printf("         ��Ӯ��\n");
	}
	system("pause");
	return 0;
}
void start()
{
	memset(map, -1, sizeof(map));
	win = false;
	man_y = Width / 2;
	man_x = Length / 2;
	map[man_x][man_y] = 1;
	srand(time(0));
	target_x = rand() % Length;
	target_y = rand() % Width;
	map[target_x][target_y] = 2;
	for (int i = 0; i < Length; i++) { // ���ŵı߿�
		map[i][0] = -1;
		map[i][Width - 1] = -1;
	}
	for (int i = 0; i < Width; i++) { // ���ŵı߿�
		map[0][i] = -1;
		map[Length - 1][i] = -1;
	}
	HideCursor();
}

void show()
{
	int i, j; // i -> x, j -> y
	gotoxy(0, 0);
	/*  -1��ʾ�߿�  -2 ��ʾǽ  0��ʾ�ո�  1��ʾ�˵�λ��   2 ��ʾ�յ�  */
	for (i = 0; i < Length; i++) {
		for (j = 0; j < Width; j++) {
			if (map[i][j] == -1)
				printf("/");
			else if (map[i][j] == -2)
				printf("/");
			else if (map[i][j] == 1)
				printf("*");
			else if (map[i][j] == 2)
				printf("@");
			else if (map[i][j] == 0)
				printf(" ");
			HideCursor();
		}
		printf("\n");
	}
}
bool okmove(int x, int y) {
	if (x < 1 || x >= Length - 1 || y < 1 || y >= Width - 1) // �߽���
		return false;
	if (map[x][y] == -2)
		return false;
	return true;
}

void manuallcontrol()
{
	char input;

	if (_kbhit()) {
		input = _getch();
		if (input == 'w' && okmove(man_x - 1, man_y)) {
			map[man_x][man_y] = 0;
			man_x -= 1;
			map[man_x][man_y] = 1;
		}
		if (input == 's' && okmove(man_x + 1, man_y)) {
			map[man_x][man_y] = 0;
			man_x += 1;
			map[man_x][man_y] = 1;
		}
		if (input == 'a' && okmove(man_x, man_y - 1)) {
			map[man_x][man_y] = 0;
			man_y -= 1;
			map[man_x][man_y] = 1;
		}
		if (input == 'd' && okmove(man_x, man_y + 1)) {
			map[man_x][man_y] = 0;
			man_y += 1;
			map[man_x][man_y] = 1;
		}
	}
}

void create_maze() {

	
}

void traval_maze(int x, int y, int d) {
	
}
void automaticcontrol()
{
	if (man_x == target_x && man_y == target_y)
		win = true;
}

void gotoxy(int x, int y) // ��������
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}
void HideCursor() // ���ع��
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info); //�����������
}