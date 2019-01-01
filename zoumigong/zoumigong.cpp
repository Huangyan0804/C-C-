#include <Windows.h>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;
const int Width = 50;
const int Length = 50;

void start(); //初始化
void show(); //显示画面
void manuallcontrol(); //人工更新 输入方向
void automaticcontrol(); //自动更新
void gotoxy(int x, int y);
void HideCursor(); //清除光标
void CtrMoveDirection(); //蛇移动

//----------------全局变量----------------
int live = true;
int canvas[Length + 1][Width + 1]; // 初始化画布
int man_x, man_y; // 人物的位置
int target_x, target_y; // 终点位置
//--------------------------------------
int main()
{
    while (live) {
        show();
        manuallcontrol();
        automaticcontrol();
    }
    system("pause");
    return 0;
}
void start()
{
    man_y = Width / 2;
    man_x = Length / 2;
    srand(time(0));
    target_x = rand() % Length;
    target_y = rand() % Width;
    for (int i = 0; i < Length; i++) { // 竖着的边框
        canvas[i][0] = -1;
        canvas[i][Width - 1] = -1;
    }
    for (int i = 0; i < Width; i++) { // 横着的边框
        canvas[0][i] = -1;
        canvas[Length - 1][i] = -1;
    }
    HideCursor();
}

void show()
{
    int i, j; // i -> x, j -> y
    gotoxy(0, 0);
    /*  -1表示边框  -2 表示墙  0表示空格  1表示人的位置   2 表示终点  */
    for (i = 0; i < Length; i++) {
        for (j = 0; j < Width; j++) {
            if (canvas[i][j] == -1)
                printf("/");
            else if (canvas[i][j] == -2)
                printf("/");
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
}

void manuallcontrol()
{
    char input;

    if (_kbhit()) {
        input = _getch();
        if (input == 'w' && moveDirection != 2)
            moveDirection = 1;
        if (input == 's' && moveDirection != 1)
            moveDirection = 2;
        if (input == 'a' && moveDirection != 4)
            moveDirection = 3;
        if (input == 'd' && moveDirection != 3)
            moveDirection = 4;
    }
}

void automaticcontrol()
{
}
void CtrMoveDirection()
{
}
void gotoxy(int x, int y) // 清屏函数
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}
void HideCursor() // 隐藏光标
{
    CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info); //隐藏输入符号
}