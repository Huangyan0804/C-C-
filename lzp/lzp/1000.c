#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include<conio.h>
#include<stdbool.h>


void start();//��ʼ��
void show();//��ʾ����
void manuallcontrol(); //�˹�
void automaticcontrol();//�Զ�
void gotoxy(int x, int y);//��������������������ƶ���ԭ��λ�ý����ػ�
void HideCursor();//������ĺ���

//ȫ�ֱ���
int width, length;//W->X      L->Y
int ball_x, ball_y;//С��λ��
int speed_x, speed_y;//С����
int wood_y, wood_x;//ľ���λ��
int wood_l;//ľ�峤��
int block_x, block_y;//ש��λ��
int block_t, block_l;//ש���ȣ�����
bool kill;//�Ƿ�����ש��
int score;
bool live = true;//��Ϸ������ʼ

int main()
{
	int input;
first:start();
	printf("/********************************************************\n");
	printf("             �����<--  -->   ������Ϸ\n");
	printf("                  �밴�س�����ʼ\n");
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
	printf("----------------��Ϸ����-----------------\n");
	printf("-----------������������¿�ʼ------------\n");
	printf("----------------��esc�˳�----------------\n");
	input = _getch();
	if (input == '\033') //����ESC��
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
	block_x = rand() % width; block_y = rand() % (length / 4);//�������ש��λ��
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
			else if ((i == wood_y) && (j >= wood_x) && (j <= wood_x + wood_l))//���ľ��
				printf("=");
			else
				printf(" ");
			HideCursor();

		}

		printf("\n");

	}
	printf("��ǰ�÷�:%d", score);
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
	if (speed < 2)
		speed++;
	if (speed == 2)
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
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//�����������
}