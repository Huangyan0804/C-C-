#include <assert.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/time.h> 
#include <iostream>
#include <set>

using namespace std;
struct Bubble
{
	int iColor;
	Bubble() : iColor(0)
	{
	}
	Bubble(int iSetColor) : iColor(iSetColor)
	{
	}
	void setColor(int iSetColor)
	{
		iColor = iSetColor;
	}
};

void printBubble(Bubble  pArr[][10], int iRowMax, int iColumnMax)
{
	for (int j = 0; j < iRowMax; ++j)
	{
		for (int i = 0; i < iColumnMax; ++i)
		{
			cout << pArr[j][i].iColor << " ";
		}
		cout << endl;
	}

}

int  checkErase(Bubble  pArr[][10], int iRowMax, int iColumnMax, int iHitX, int iHitY)
{
	int iEraseHappened = 0;
	// 从 iHitX, iHitY 位置开始，向四周的6个位置检测，看是否有相同颜色的，超过2个
	int iDirectionArr[] = { 0, -2, -1, -1,  -1, 1,   0, 2,  1, 1,  1, -1, };
	int iHitColor = pArr[iHitY][iHitX].iColor;
	int iRelateX = 0;
	int iRelateY = 0;
	int iSameCount = 0;
	set<int> setResult;
	int iDir = 0;
	setResult.insert(iHitX * 10000 + iHitY);
	set<int>::iterator itr;
	int iPos = 0;
	iDir = 0;
	// 向四周蔓延查找同颜色的所有项
	for (; iDir < 6; ++iDir)
	{
		iRelateX = iHitX + iDirectionArr[2 * iDir + 1];
		iRelateY = iHitY + iDirectionArr[2 * iDir];

		if (iRelateX >= 0 && iRelateX < iColumnMax
			&& iRelateY >= 0 && iRelateY < iRowMax)
		{
			if (pArr[iRelateY][iRelateX].iColor == iHitColor)
			{
				++iSameCount;
				setResult.insert(iRelateX * 10000 + iRelateY);
			}
		}
	}

	if (iSameCount < 2)
	{
		// 返回0表示没有发生同颜色的碰撞
		//cout << "iSameCount is " << iSameCount   << endl;
		return 0;
	}
	// 将周围相同的清0,包括当前节点
	itr = setResult.begin();
	set<int> setRemain;
	for (; itr != setResult.end(); ++itr)
	{
		pArr[*itr % 10000][*itr / 10000].setColor(0);
		setRemain.insert(*itr);
	}

	while (!setRemain.empty())
	{
		itr = setRemain.begin();
		// 查看6个方向上是否有符合条件的项
		iDir = 0;

		// 向四周蔓延查找同颜色的所有项

		for (; iDir < 6; ++iDir)
		{
			iRelateX = *itr / 10000 + iDirectionArr[2 * iDir + 1];
			iRelateY = *itr % 10000 + iDirectionArr[2 * iDir];

			int iPos = iRelateX * 10000 + iRelateY;
			if (iRelateX >= 0 && iRelateX < iColumnMax
				&& iRelateY >= 0 && iRelateY < iRowMax
				&& pArr[iRelateY][iRelateX].iColor == iHitColor
				&& setRemain.find(iPos) == setRemain.end())
			{
				setRemain.insert(iPos);
			}
		}

		// 设置为已经消除
		pArr[*itr % 10000][*itr / 10000].setColor(0);
		setRemain.erase(itr);
	}

	//cout << "check same color finish"  << endl;
	// 从第一行节点开始，广度优先遍历所有可以到达的节点，视为不掉落的节点
	// 计算剩余的有效节点的数量
	setRemain.clear();
	// 将第一行不为空的节点加入到remain中
	int i = 0;
	for (; i < iColumnMax; i += 2)
	{
		if (pArr[0][i].iColor > 0)
		{
			setRemain.insert(i * 10000);
		}
	}

	set<int> setNotDrop;
	setNotDrop.insert(setRemain.begin(), setRemain.end());
	while (!setRemain.empty())
	{
		itr = setRemain.begin();
		// 查看6个方向上是否有不为0的项

		int iOrigPos = *itr;
		// 向四周蔓延查找有球球的所有项
		for (iDir = 0; iDir < 6; ++iDir)
		{
			iRelateX = iOrigPos / 10000 + iDirectionArr[2 * iDir + 1];
			iRelateY = iOrigPos % 10000 + iDirectionArr[2 * iDir];

			iPos = iRelateX * 10000 + iRelateY;

			if (iRelateX >= 0 && iRelateX < iColumnMax
				&& iRelateY >= 0 && iRelateY < iRowMax
				&& pArr[iRelateY][iRelateX].iColor > 0
				&& setNotDrop.find(iPos) == setNotDrop.end())
			{
				setRemain.insert(iPos);
				setNotDrop.insert(iPos);
			}
		}
		setRemain.erase(iOrigPos);
	}

	// 去掉掉不了的，就是掉落的了，设置color为0

	for (int j = 0; j < iRowMax; ++j)
	{
		for (i = 0; i < iColumnMax; ++i)
		{
			if (pArr[j][i].iColor > 0)
			{
				iPos = i * 10000 + j;
				if (setNotDrop.find(iPos) == setNotDrop.end())
				{
					pArr[j][i].iColor = 0;
					setResult.insert(iPos);
				}
			}
		}
	}
}



int main(int argc, char** argv)
{
	// 屏幕y轴最多放12行泡泡
	const int iRowMax = 12;

	// 屏幕x轴最多放10个位置 
	const int iColumnMax = 10;// 2 *  iColumn  

	Bubble  pArr[iRowMax][iColumnMax];
	for (int j = 0; j < iRowMax; ++j)
	{
		for (int i = 0; i < iColumnMax; ++i)
		{
			pArr[j][i].setColor(0);
		}
	}

	srand(time(NULL));
	// 初始化第1行，为随机的泡泡
	// 有5种颜色，1 -  5 表示为空位置
	// 偶数行的第 0， 2， 4个位置有元素，奇数行的第1，3，5，7个位置有元素
	int iColorCount = 5;
	int i = 0;
	for (int j = 0; j < 3; ++j)
	{
		if (j % 2 == 0)
		{
			i = 0;
		}
		else
		{
			i = 1;
		}

		for (; i < iColumnMax; i += 2)
		{
			int iRand = rand() % iColorCount;
			pArr[j][i].setColor(iRand + 1);
		}
	}

	// 生成一个随机颜色的bubble，放在位置[  1 , 3 ] , 然后检测


	pArr[2][2].setColor(pArr[2][0].iColor);
	pArr[3][1].setColor(pArr[2][0].iColor);
	pArr[1][1].setColor(pArr[2][0].iColor);
	printBubble(pArr, iRowMax, iColumnMax);
	checkErase(pArr, iRowMax, iColumnMax, 1, 3);

	cout << "============================================" << endl;

	printBubble(pArr, iRowMax, iColumnMax);

	return 0;
}