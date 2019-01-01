#pragma once
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
#define MAZE_MAX 50
int map[30][30+2];
const int x = 30, y = 30;
int z1, z2;

void printMaze();
void makeMaze(int map[][30]);
int searchPath(int, int);

void printMaze()
{
	for (z2 = 1; z2 <= y * 2 + 1; z2++)
	{
		for (z1 = 1; z1 <= x * 2 + 1; z1++)
			fputs(map[z2][z1] == 0 ? "¡¡" : "¨€", stdout);
		putchar(10);
	}
	cout << endl;
}

void makeMaze(int map[30][30])
{
	for (z1 = 0, z2 = 2 * y + 2; z1 <= 2 * x + 2; ++z1)
	{
		map[z1][0] = 0;
		map[z1][z2] = 0;
	}
	for (z1 = 0, z2 = 2 * x + 2; z1 <= 2 * y + 2; ++z1)
	{
		map[0][z1] = 0;
		map[z2][z1] = 0;
	}
	map[2][1] = 0;
	map[2 * x][2 * y + 1] = 0;

	srand((unsigned)time(NULL));
	searchPath(rand() % x + 1, rand() % y + 1);
}

int searchPath(int x, int y)
{
	static int dir[4][2] = { 0, 1, 1, 0, 0, -1, -1, 0 };
	int zx = x * 2;
	int zy = y * 2;
	int next, turn, i;
	map[zx][zy] = 0;
	turn = rand() % 2 ? 1 : 3;
	for (i = 0, next = rand() % 4; i < 4; ++i, next = (next + turn) % 4)
		if (map[zx + 2 * dir[next][0]][zy + 2 * dir[next][1]] == 1)
		{
			map[zx + dir[next][0]][zy + dir[next][1]] = 0;
			searchPath(x + dir[next][0], y + dir[next][1]);
		}
	return 0;
}