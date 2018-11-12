﻿#include<stdio.h>
#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<time.h>
#include<stdlib.h>
using namespace std;

void start();//初始化
void show();//显示画面
void manuallcontrol(); //人工
void automaticcontrol();//自动
void gotoxy(int x, int y);
void HideCursor();//清除光标
void CtrMoveDirection();//蛇移动
//全局变量
#define Width 70
#define Length 28
int score;
int live = true;
int moveDirection;
int canvas[Length][Width] = { 0 };
int food_j, food_i;
int movespeed = 1;

int main()
{
	char input;
first:start();
	printf("/********************************************************\n");
	printf("               按 W A S D控制蛇的方向                     \n");
	printf("                  请按回车键开始                         \n");
	printf("********************************************************\n");
	if (getchar()=='\n')
	{
		system("cls");
		while (live)
		{
			show();
			manuallcontrol();
			automaticcontrol();
		}
	}
	system("cls");
	printf("----------------游戏结束-----------------\n\n");
	printf("             您的游戏得分是   %d       \n\n", score);
	printf("--------------没有重新开始---------------\n");
	system("pause");
	system("pause");
	return 0;
}
void start()
{
	score = 0;
	srand(time(0));
	canvas[Length][Width] = { 0 };
	// 边框
	for (int i = 0; i < Length; i++) {//竖着的边框
		canvas[i][0] = -1;
		canvas[i][Width - 1] = -1;
	}
	for (int i = 0; i < Width; i++) {//横着的边框
		canvas[0][i] = -2;
		canvas[Length - 1][i] = -2;
	}
	// 蛇身
	canvas[Length / 2][Width / 2] = 1;
	for (int i = 1; i <= 2; i++)
		canvas[Length / 2][Width / 2 - i] = i + 1;
	moveDirection = 4;
	//食物
	food_j = (rand() % (Width - 3)) + 2;
	food_i = (rand() % (Length - 3)) + 2;
	canvas[food_i][food_j] = -5; 
	HideCursor();
}

void show() {
	int i, j;// i->y,j->x
	gotoxy(0, 0);
	/*  -1表示边框   0表示空格  >=1表示蛇   -5表示食物  */
	for (i = 0; i < Length; i++) {
		for (j = 0; j < Width; j++) {
			if (canvas[i][j] == -1)
				printf("║");
			else if (canvas[i][j] == -2)
				printf("═");
			else if (canvas[i][j] >= 1)
				printf("*");
			else if (canvas[i][j] == -5)
				printf("@");
			else if (canvas[i][j] == 0)
				printf(" ");
			HideCursor();
		}
		printf("\n");
	}
	printf("当前得分:%d", score);
}

void manuallcontrol()
{
	char input;

	if (_kbhit())
	{
		input = _getch();
		if (input == 'w' && moveDirection != 2)
			moveDirection = 1;
		if (input == 's'&&moveDirection != 1)
			moveDirection = 2;
		if (input == 'a'&&moveDirection != 4)
			moveDirection = 3;
		if (input == 'd'&&moveDirection != 3)
			moveDirection = 4;
	}
}

void automaticcontrol()
{
	int t = 0;
	static int speed = 0;
	if (speed <= movespeed) {
		speed++;
	}
	if (speed == movespeed) {
		CtrMoveDirection();
		speed = 0;
	}
}
void CtrMoveDirection() {
	int i, j;
	int max=0;
	int oldtail_i, oldtail_j;
	int oldhead_i, oldhead_j;
	for (i = 1; i < Length - 1; i++) {
		for (j = 1; j < Width - 1; j++) {
			if (canvas[i][j] > 0)
				canvas[i][j]++;
			if (max < canvas[i][j]) {
				max = canvas[i][j];
				oldtail_i = i;
				oldtail_j = j;
			}
			//记录旧蛇头
			if (canvas[i][j] == 2) {
				oldhead_i = i;
				oldhead_j = j;
			}
		}
	}
	canvas[oldtail_i][oldtail_j] = 0;//蛇尾变0
	int newhead_i, newhead_j;
	if (moveDirection == 1) {// up
		newhead_i = oldhead_i - 1;
		newhead_j = oldhead_j;
	}
	if (moveDirection == 2) {// down
		newhead_i = oldhead_i + 1;
		newhead_j = oldhead_j;
	}
	if (moveDirection == 3) {// left
		newhead_i = oldhead_i;
		newhead_j = oldhead_j - 1;
	}
	if (moveDirection == 4) {// right
		newhead_i = oldhead_i;
		newhead_j = oldhead_j + 1;
	}
	if (newhead_i == 0 || newhead_i == Length - 1 || newhead_j == 0 || newhead_j == Width - 1)// 撞到墙或者自己失败 否则继续
		live = false;
	if (canvas[newhead_i][newhead_j] > 0)
		live = false;
	else
		canvas[newhead_i][newhead_j] = 1;

	if (newhead_i == food_i && newhead_j == food_j) {//如果吃到食物
		score++;
		for (int i = 1; i < Length - 1; i++) {
			for (int j = 1; j < Width - 1; j++) {
				if (canvas[i][j] > 0)
					canvas[i][j]++;
			}
			if (moveDirection == 1)
				canvas[food_i - 1][food_j] = 1;
			else if (moveDirection == 2)
				canvas[food_i + 1][food_j] = 1;
			else if (moveDirection == 3)
				canvas[food_i][food_j - 1] = 1;
			else if (moveDirection == 4)
				canvas[food_i][food_j + 1] = 1;
		}

		food_j = (rand() % (Width - 3)) + 2;
		food_i = (rand() % (Length - 3)) + 2;
		while (canvas[food_i][food_j] >= 1) {
			food_j = (rand() % (Width - 3)) + 2;
			food_i = (rand() % (Length - 3)) + 2;
		}
		canvas[food_i][food_j] = -5;
	}
}
void gotoxy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//隐藏输入符号
}