#include<stdio.h>
#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<time.h>
#include<stdlib.h>
using namespace std;

void start();//��ʼ��
void show();//��ʾ����
void manuallcontrol(); //�˹�
void automaticcontrol();//�Զ�
void gotoxy(int x, int y);//��������������������ƶ���ԭ��λ�ý����ػ�
void HideCursor();//������ĺ���

//ȫ�ֱ���
int width, length;//W->X      L->Y
int bird_x, bird_y;//С��λ��
int speed_y;//С����
int bar_top, bar_down, bar_y;//�ܵ�λ��
int bar_thick;//�ܵ����
int score;
int difficult;
int speedtime;
bool live = true;//��Ϸ������ʼ

int main()
{
	int input;
first:start();
	printf("/********************************************************\n");
	printf("               ���ո������С��ɸ�\n");
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
	printf("----------------��Ϸ����-----------------\n\n");
	printf("             ������Ϸ�÷���   %d       \n\n", score);
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
				printf("*");// ���ܵ�
			}
			else if ((i == bird_y) && (j == bird_x))
				printf("@");
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
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//�����������
}