#include<cstdio>
#include<cstdlib>
#include<Windows.h>
#include<ctime>
#include<conio.h>
#include<cstring>
#include<graphics.h>
using namespace std;
//-------定义函数----------
void start();
void show();
void updateWithoutInput();
void updateWithInput();
void Initmap();
int Move(int d, int x, int y);
bool okmove(int x, int y);
void OutPutPath();

int Move(int d, int x, int y);
void CreateMaze();
void FindPath(int i, int j);
void DFS_CreateMaze(int x, int y);
//------------------------

//------全局变量-----------
const int WIN_WIDTH = 1300;                   // 窗体高度
const int WIN_LENGTH = 780;                   // 窗体宽度
const int MAZE_WIDTH = 15 * 34;                 // 迷宫宽度
const int MAZE_HEIGHT = 15 * 17;                // 迷宫高度
const int B_WIDTH = 30;                  // 格子宽度
const int X = 19;                       // 迷宫列数 
const int Y = 11;                          // 迷宫行数 
const int N = X * 2 + 1;                          // 迷宫数组列数 
const int M = Y * 2 + 1;                          // 迷宫数组行数 
const int L = (N - 1)*(M - 1);                    // 迷宫路径长度
const int SX = 80;                         // 起点坐标X
const int SY = 50;                         // 起点坐标Y
#define C_BLOCK RGB(153,102,85)             // 墙壁颜色
#define C_GRAY RGB(123,72,55)               // 阴影颜色
#define C_GROUND RGB(50,50,50)              // 地面颜色
#define C_ROOT RGB(170,170,210)             // 路径颜色


const int manimg = 20;                        // 小人图片宽度/2
IMAGE bk_img, man_img;                          // 背景图， 小人图
bool win;                                      // 是否赢了
int man_x, man_y;                               // 小人坐标
int start_x, start_y, end_x, end_y;                // 起点坐标 终点坐标
int map[M][N];                                    // 地图  0 -> 空格 1 -> 墙 2 -> 路径  3 -> 起点  4 -> 终点  5 -> 人
int path_x[1000][L], path_y[1000][L];
bool showpath = false;
int length[L];                                  // 路径搜索长度
int lengtha, count;                              // 搜索计数
int mouseX;                                     // 鼠标位置坐标X
int mouseY;                                     // 鼠标位置坐标Y
bool isMouseDown;                               // 鼠标按下
int old_d;                                     // 上一步走的方向
int Move_flag;                                  // 移动标记
//------------------------

int main() {
	start();
	while (!win) {
		show();
		updateWithoutInput();
		updateWithInput();
		Sleep(15);
	}
	closegraph();
	return 0;
}

void start() {
	SetWindowText(GetHWnd(), _T("走迷宫"));
	srand(time(NULL));
	man_x = man_y = 1;
	start_x = start_y = 0;
	initgraph(WIN_WIDTH, WIN_LENGTH);
	loadimage(&bk_img, "background.jpg");
	loadimage(&man_img, "man.jpg");

	CreateMaze();
	map[man_x][man_y] = 5;
	BeginBatchDraw();
}

void show() {
	setcolor(WHITE);
	setfillcolor(WHITE);
	fillrectangle(0, 0, WIN_WIDTH, WIN_LENGTH); // 清屏
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			int xx = SX + j * B_WIDTH;
			int yy = SY + i * B_WIDTH;
			switch (map[i][j]) {
			case 0:
				setcolor(C_GROUND);
				setfillcolor(C_GROUND);
				fillrectangle(xx, yy, xx + B_WIDTH, yy + B_WIDTH);
				break;
			case 1:
				setcolor(C_BLOCK);
				setfillcolor(C_BLOCK);
				fillrectangle(xx, yy, xx + B_WIDTH, yy + B_WIDTH);
				if (map[i][j] == 1 && (i == M - 1 || map[i + 1][j] != 1))
				{
					setcolor(C_GRAY);
					setfillcolor(C_GRAY);
					fillrectangle(xx, yy + B_WIDTH / 2, xx + B_WIDTH, yy + B_WIDTH);
				}
				break;
			case 2:
				setcolor(C_GROUND);
				setfillcolor(C_GROUND);
				fillrectangle(xx, yy, xx + B_WIDTH, yy + B_WIDTH);
				break;
			case 3:
				setcolor(C_GROUND);
				setfillcolor(C_GROUND);
				fillrectangle(xx, yy, xx + B_WIDTH, yy + B_WIDTH);
				break;
			case 4:
				setcolor(C_GROUND);
				setfillcolor(C_GROUND);
				fillrectangle(xx, yy, xx + B_WIDTH, yy + B_WIDTH);
				break;
			case 5:
				putimage(xx, yy, &man_img);
			}

		}
	}
	OutPutPath();
	FlushBatchDraw();
}

void updateWithoutInput() {
	if (man_x == end_x  && man_y == end_y)
		win = true;
}


void updateWithInput() {
	char input;
	if (_kbhit()) {
		input = _getch();
		if (input == 'w' && okmove(man_x - 1, man_y)) {
			map[man_x][man_y] = 0;
			man_x -= 1;
			map[man_x][man_y] = 5;
		}
		if (input == 's' && okmove(man_x + 1, man_y)) {
			map[man_x][man_y] = 0;
			man_x += 1;
			map[man_x][man_y] = 5;
		}
		if (input == 'a' && okmove(man_x, man_y - 1)) {
			map[man_x][man_y] = 0;
			man_y -= 1;
			map[man_x][man_y] = 5;
		}
		if (input == 'd' && okmove(man_x, man_y + 1)) {
			map[man_x][man_y] = 0;
			man_y += 1;
			map[man_x][man_y] = 5;
		}
		if (input == 13)
			showpath = !showpath;
	}
}

void Initmap() {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			map[i][j] = 1;
		}
	}
	map[start_x][start_y] = 3;
	map[M - 2][N - 1] = 4;
}

void OutPutPath()
{
	if (showpath)
	{
		for (int i = 0; i < length[0] - 1; i++)
		{
			setcolor(RED);
			int x1 = path_x[0][i] * B_WIDTH + B_WIDTH / 2 + SX;
			int x2 = path_x[0][i + 1] * B_WIDTH + B_WIDTH / 2 + SX;
			int y1 = path_y[0][i] * B_WIDTH + B_WIDTH / 2 + SY;
			int y2 = path_y[0][i + 1] * B_WIDTH + B_WIDTH / 2 + SY;
			line(x1, y1, x2, y2);
		}
	}
}

bool okmove(int x, int y) {
	if (x < 1 || x >= M - 1 || y < 1 || y >= N - 1)
		return false;
	if (map[x][y] == 1)
		return false;
	return true;
}

void DFS_CreateMaze(int x, int y)
{
	//置通路
	map[y][x] = 0;
	//随机方向
	int d = rand() % 4;
	int direct = old_d;
	int t[4] = { 0,0,0,0 };// 已走方向标记
	Move_flag = 0;// 重置移动标记
	// 保证随机性的同时走满地图
	while (t[0] != 1 || t[1] != 1 || t[2] != 1 || t[3] != 1)
	{
		int i = rand() % 4;
		if (t[i] != 1)
		{
			t[i] = 1;
			Move(i, x, y);
		}
	}
}
int Move(int d, int x, int y)
{
	switch (d)
	{
		// 向上
	case 0: {
		if (map[y - 3][x] > 0 && map[y - 2][x - 1] > 0 && map[y - 2][x + 1] > 0 && y - 2 > 0)
		{
			map[y - 2][x] = 0;
			map[y - 1][x] = 0;
			Move_flag = 1;
			old_d = 0;
			DFS_CreateMaze(x, y - 2);
		}
		break;
	}
			// 向下
	case 1: {
		if (map[y + 3][x] > 0 && map[y + 2][x - 1] > 0 && map[y + 2][x + 1] > 0 && y + 2 < M)
		{
			map[y + 2][x] = 0;
			map[y + 1][x] = 0;
			Move_flag = 1;
			old_d = 1;
			DFS_CreateMaze(x, y + 2);
		}
		break;
	}
			// 向左
	case 2: {
		if (map[y][x - 3] > 0 && map[y - 1][x - 2] > 0 && map[y + 1][x - 2] > 0 && x - 2 > 0)
		{
			map[y][x - 1] = 0;
			map[y][x - 2] = 0;
			Move_flag = 1;
			old_d = 2;
			DFS_CreateMaze(x - 2, y);
		}
		break;
	}
			// 向右
	case 3: {
		if (map[y][x + 3] > 0 && map[y - 1][x + 2] > 0 && map[y + 1][x + 2] > 0 && x + 2 < N)
		{
			map[y][x + 1] = 0;
			map[y][x + 2] = 0;
			Move_flag = 1;
			old_d = 3;
			DFS_CreateMaze(x + 2, y);
		}
		break;
	}
	}
	return Move_flag;
}

void FindPath(int x, int y)
{
	// 置路径
	map[y][x] = 2;
	path_x[count][lengtha] = x;
	path_y[count][lengtha] = y;
	lengtha++;
	// 搜索到目的地
	if (x == end_x && y == end_y)
	{
		length[count] = lengtha;
		count++;
		if (count > 0)
		{
			// 复制上一次的路径到下一次待搜索路径中
			for (int i = 0; i < length[count - 1]; i++)
			{
				path_x[count][i] = path_x[count - 1][i];
				path_y[count][i] = path_y[count - 1][i];
			}
		}
	}
	else
	{
		// 搜索并回溯
		if (map[y][x + 1] == 0) { FindPath(x + 1, y); map[y][x + 1] = 0; lengtha--; }
		if (map[y + 1][x] == 0) { FindPath(x, y + 1); map[y + 1][x] = 0; lengtha--; }
		if (map[y][x - 1] == 0) { FindPath(x - 1, y); map[y][x - 1] = 0; lengtha--; }
		if (map[y - 1][x] == 0) { FindPath(x, y - 1); map[y - 1][x] = 0; lengtha--; }
	}
}
// 创建迷宫
void CreateMaze()
{
	// 初始化迷宫数组
	Initmap();
	showpath = false;
	// 从终点开始逆向生成迷宫
	start_x = 1;
	start_y = 1;
	end_x = N - 2;
	end_y = M - 2;
	DFS_CreateMaze(end_x, end_y);
	// 搜索路径
	lengtha = 0;
	count = 0;
	FindPath(start_x, start_y);
	// 结果路径复制到地图
	for (int i = 0; i < length[0]; i++)
	{
		map[path_y[0][i]][path_x[0][i]] = 2;
	}
}