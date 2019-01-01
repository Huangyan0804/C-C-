#include<cstdio>
#include<cstdlib>
#include<Windows.h>
#include<ctime>
#include<conio.h>
#include<cstring>
#include<graphics.h>
using namespace std;
//-------���庯��----------
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

//------ȫ�ֱ���-----------
const int WIN_WIDTH = 1300;                   // ����߶�
const int WIN_LENGTH = 780;                   // ������
const int MAZE_WIDTH = 15 * 34;                 // �Թ����
const int MAZE_HEIGHT = 15 * 17;                // �Թ��߶�
const int B_WIDTH = 30;                  // ���ӿ��
const int X = 19;                       // �Թ����� 
const int Y = 11;                          // �Թ����� 
const int N = X * 2 + 1;                          // �Թ��������� 
const int M = Y * 2 + 1;                          // �Թ��������� 
const int L = (N - 1)*(M - 1);                    // �Թ�·������
const int SX = 80;                         // �������X
const int SY = 50;                         // �������Y
#define C_BLOCK RGB(153,102,85)             // ǽ����ɫ
#define C_GRAY RGB(123,72,55)               // ��Ӱ��ɫ
#define C_GROUND RGB(50,50,50)              // ������ɫ
#define C_ROOT RGB(170,170,210)             // ·����ɫ


const int manimg = 20;                        // С��ͼƬ���/2
IMAGE bk_img, man_img;                          // ����ͼ�� С��ͼ
bool win;                                      // �Ƿ�Ӯ��
int man_x, man_y;                               // С������
int start_x, start_y, end_x, end_y;                // ������� �յ�����
int map[M][N];                                    // ��ͼ  0 -> �ո� 1 -> ǽ 2 -> ·��  3 -> ���  4 -> �յ�  5 -> ��
int path_x[1000][L], path_y[1000][L];
bool showpath = false;
int length[L];                                  // ·����������
int lengtha, count;                              // ��������
int mouseX;                                     // ���λ������X
int mouseY;                                     // ���λ������Y
bool isMouseDown;                               // ��갴��
int old_d;                                     // ��һ���ߵķ���
int Move_flag;                                  // �ƶ����
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
	SetWindowText(GetHWnd(), _T("���Թ�"));
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
	fillrectangle(0, 0, WIN_WIDTH, WIN_LENGTH); // ����
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
	//��ͨ·
	map[y][x] = 0;
	//�������
	int d = rand() % 4;
	int direct = old_d;
	int t[4] = { 0,0,0,0 };// ���߷�����
	Move_flag = 0;// �����ƶ����
	// ��֤����Ե�ͬʱ������ͼ
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
		// ����
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
			// ����
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
			// ����
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
			// ����
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
	// ��·��
	map[y][x] = 2;
	path_x[count][lengtha] = x;
	path_y[count][lengtha] = y;
	lengtha++;
	// ������Ŀ�ĵ�
	if (x == end_x && y == end_y)
	{
		length[count] = lengtha;
		count++;
		if (count > 0)
		{
			// ������һ�ε�·������һ�δ�����·����
			for (int i = 0; i < length[count - 1]; i++)
			{
				path_x[count][i] = path_x[count - 1][i];
				path_y[count][i] = path_y[count - 1][i];
			}
		}
	}
	else
	{
		// ����������
		if (map[y][x + 1] == 0) { FindPath(x + 1, y); map[y][x + 1] = 0; lengtha--; }
		if (map[y + 1][x] == 0) { FindPath(x, y + 1); map[y + 1][x] = 0; lengtha--; }
		if (map[y][x - 1] == 0) { FindPath(x - 1, y); map[y][x - 1] = 0; lengtha--; }
		if (map[y - 1][x] == 0) { FindPath(x, y - 1); map[y - 1][x] = 0; lengtha--; }
	}
}
// �����Թ�
void CreateMaze()
{
	// ��ʼ���Թ�����
	Initmap();
	showpath = false;
	// ���յ㿪ʼ���������Թ�
	start_x = 1;
	start_y = 1;
	end_x = N - 2;
	end_y = M - 2;
	DFS_CreateMaze(end_x, end_y);
	// ����·��
	lengtha = 0;
	count = 0;
	FindPath(start_x, start_y);
	// ���·�����Ƶ���ͼ
	for (int i = 0; i < length[0]; i++)
	{
		map[path_y[0][i]][path_x[0][i]] = 2;
	}
}