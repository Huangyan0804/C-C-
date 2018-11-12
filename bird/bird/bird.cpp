#include<stdio.h>
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
void gotoxy(int x, int y);//类似于清屏函数，光标移动到原点位置进行重画
void HideCursor();//清除光标的函数

//全局变量
int width, length;//W->X      L->Y
int bird_x, bird_y;//小球位置
int speed_y;//小球方向
int bar_top, bar_down, bar_y;//管道位置
int bar_thick;//管道厚度
int score;
int difficult;
int speedtime;
bool live = true;//游戏结束开始

int main()
{
	int input;
first:start();
	printf("/********************************************************\n");
	printf("               按空格键控制小鸟飞高\n");
	printf("                  请按回车键开始\n");
	printf("********************************************************/");
	if (getchar() == '\n')
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
	printf("-----------输入任意键重新开始------------\n");
	printf("----------------按esc退出----------------\n");
	input = _getch();
	if (input == '\033') //按了ESC键
		exit(0);
	else
	{
		system("cls");
		live = true;
		goto first;
	}

	return 0;
}
void start()
{
	score = 0;
	srand(time(0));
	width = 50; length = 28;
	bird_x = width / 3; bird_y = length / 2;
	speed_y = 1;
	difficult = 6;
	bar_top = (rand() % (length - 6)) + 1;
	bar_down = bar_top + difficult;
	bar_thick = 6;
	bar_y = width;
	speedtime = 3;
	HideCursor();
}

void show(){
	int i, j;// i->y,j->x
	gotoxy(0, 0);
	for (i = 1; i <= length; i++){
		for (j = 1; j <= width + 1; j++) {
			if ((j == width + 1) && (i >= 1) && (i <= length))
				printf("|");
			else if ((i <= bar_top && j >= bar_y && j <= bar_y + bar_thick) || (i >= bar_down && j >= bar_y && j <= bar_y + bar_thick)) {
				printf("*");// 画管道
			}
			else if ((i == bird_y) && (j == bird_x))
				printf("@");
			else
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
		if (input == ' ')
			bird_y -= 2;
	}
}

void automaticcontrol()
{
	int t=0;
	static int speed = 0;
	if (speed <= speedtime) {
		speed++;
		if (bird_y <= 0)
			bird_y = 0;
	}
	if (speed == speedtime+1){
		bar_y--;
		if (bar_y == 0) {
			bar_y = width;
			bar_top = (rand() % (length - 6)) + 1;
			bar_down = bar_top + difficult;
		}
		if (speed % 2 == 0)
			bird_y += speed_y;
		if (bird_y == length+1)
			bird_y--;
		speed = 0;
		if ((bird_y <= bar_top && bird_x >= bar_y && bird_x < bar_y + bar_thick) || (bird_y >= bar_down && bird_x >= bar_y && bird_x < bar_y + bar_thick))
			live = false;
		else if (bird_y >= bar_top && bird_y <= bar_down && bird_x == bar_y + bar_thick)
			score++;
	}
	if (score == 5) {
		difficult = 5;
		speedtime = 1;
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