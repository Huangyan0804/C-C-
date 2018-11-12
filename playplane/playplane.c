#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include<conio.h>
#include<stdbool.h>
#include<time.h>

void start();//初始化
void show();//显示画面
void manuallcontrol(); //人工
void automaticcontrol();//自动
void gotoxy(int x, int y);//类似于清屏函数，光标移动到原点位置进行重画
void HideCursor();//清除光标的函数

//全局变量
int width, length;//W->X      L->Y
int ball_x, ball_y;//小球位置
int speed_x, speed_y;//小球方向
int wood_y, wood_x;//木板的位置
int wood_l;//木板长度
int block_x, block_y;//砖块位置
int block_t, block_l;//砖块厚度，长度
bool kill;//是否消灭砖块
int score;
bool live = true;//游戏结束开始

int main()
{
	int input;
first:start();
	printf("/********************************************************\n");
	printf("             方向键<--  -->   控制游戏\n");
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
	printf("----------------游戏结束-----------------\n");
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
	ball_x = rand() % width; ball_y = ((rand() % length) / 3) * 2; speed_x = 1; speed_y = 1;
	wood_y = length; wood_x = width / 2; wood_l = 10;
	kill = false;
	block_x = rand() % width; block_y = rand() % (length / 4);//随机生成砖块位置
	block_t = 2; block_l = 4;
	HideCursor();
}

void show()
{
	int i, j;//i->y,j->y
	gotoxy(0, 0);
	for (i = 1; i <= length; i++)
	{
		for (j = 1; j <= width + 1; j++)
		{
			if ((i == ball_y) && (j == ball_x))
				printf("O");
			else if ((j == width + 1) && (i >= 1) && (i <= length))
				printf("|");
			else if ((i >= block_y) && (j >= block_x) && (i <= block_y + block_t) && (j <= block_x + block_l))
			{
				if (!kill)
					printf("@");
			}
			else if ((i == wood_y) && (j >= wood_x) && (j <= wood_x + wood_l))//输出木板
				printf("=");
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
		if (input == 75)
			wood_x -= 1;
		if (input == 77)
			wood_x += 1;
	}
}

void automaticcontrol()
{
	int t;
	static int speed = 0;
	if (speed < 5)
		speed++;
	if (speed == 5)
	{
		ball_x += speed_x; ball_y += speed_y;
		if ((ball_x == 1) || (ball_x == width))
			speed_x = -speed_x;
		if ((ball_y == 1) || (ball_y == wood_y - 1) && (ball_x >= wood_x) && (ball_x <= wood_x + wood_l))
			speed_y = -speed_y;
		speed = 0;
	}
	for (t = 0; t <= 4; t++)
	{
		if ((ball_y >= block_y) && (ball_x >= block_x) && (ball_y <= block_y + block_t) && (ball_x <= block_x + block_l))
		{
			speed_x = -speed_x;
			speed_y = -speed_y;
			kill = true;
		}
		if (kill)
		{
			score += 1;
			block_x = rand() % width; block_y = rand() % (length / 4);
			kill = false;
		}
	}
	if (ball_y > length)
		live = false;

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