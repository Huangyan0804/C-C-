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
	// �� iHitX, iHitY λ�ÿ�ʼ�������ܵ�6��λ�ü�⣬���Ƿ�����ͬ��ɫ�ģ�����2��
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
	// ���������Ӳ���ͬ��ɫ��������
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
		// ����0��ʾû�з���ͬ��ɫ����ײ
		//cout << "iSameCount is " << iSameCount   << endl;
		return 0;
	}
	// ����Χ��ͬ����0,������ǰ�ڵ�
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
		// �鿴6���������Ƿ��з�����������
		iDir = 0;

		// ���������Ӳ���ͬ��ɫ��������

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

		// ����Ϊ�Ѿ�����
		pArr[*itr % 10000][*itr / 10000].setColor(0);
		setRemain.erase(itr);
	}

	//cout << "check same color finish"  << endl;
	// �ӵ�һ�нڵ㿪ʼ��������ȱ������п��Ե���Ľڵ㣬��Ϊ������Ľڵ�
	// ����ʣ�����Ч�ڵ������
	setRemain.clear();
	// ����һ�в�Ϊ�յĽڵ���뵽remain��
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
		// �鿴6���������Ƿ��в�Ϊ0����

		int iOrigPos = *itr;
		// ���������Ӳ����������������
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

	// ȥ�������˵ģ����ǵ�����ˣ�����colorΪ0

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
	// ��Ļy������12������
	const int iRowMax = 12;

	// ��Ļx������10��λ�� 
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
	// ��ʼ����1�У�Ϊ���������
	// ��5����ɫ��1 -  5 ��ʾΪ��λ��
	// ż���еĵ� 0�� 2�� 4��λ����Ԫ�أ������еĵ�1��3��5��7��λ����Ԫ��
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

	// ����һ�������ɫ��bubble������λ��[  1 , 3 ] , Ȼ����


	pArr[2][2].setColor(pArr[2][0].iColor);
	pArr[3][1].setColor(pArr[2][0].iColor);
	pArr[1][1].setColor(pArr[2][0].iColor);
	printBubble(pArr, iRowMax, iColumnMax);
	checkErase(pArr, iRowMax, iColumnMax, 1, 3);

	cout << "============================================" << endl;

	printBubble(pArr, iRowMax, iColumnMax);

	return 0;
}