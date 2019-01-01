#define WIN32_LEAN_AND_MEAN

#include<time.h>
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <mmSystem.h>

#include "Bitmap.h"
#include "sprite.h"
#include "SpriteManager.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "msimg32.lib")

// MACROS /////////////////////////////////////////////////
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1) 

// CONSTS /////////////////////////////////////////////////
const int WINDOW_WIDTH = 640; //���ڴ�С
const int WINDOW_HEIGHT = 480;

// GLOBALS /////////////////////////////////////////////////
HWND hWnd;//����ȫ�ִ��ھ��
HINSTANCE g_hInstance;//����ȫ��ʵ�����

HDC         g_hOffscreenDC;
HBITMAP     g_hOffscreenBitmap;
Bitmap*     g_hMarioBitmap;
Bitmap*     g_hMapDataBitmap[29];
Bitmap*     g_hLogoBitmap[2];
Sprite*     g_hMarioSprite;

int         g_iMapData[16][160];//��ͼ��������
int         g_iEnemyElement[30];//�������λ��  �±���������ʽ��Ԫ�ش������к�
int         g_iInputDelay;//�ӵ������ӳ�
int         g_iLife = 3;//������
int         g_iOffX, g_iX;//��ɫxƫ��������ͼxƫ����
BOOL        g_bGameOver;//��Ϸ�Ƿ����
BOOL        g_bGameWin;//��Ϸʤ��
BOOL        g_bIsReSetPosition;//�Ƿ����辫��λ��

void GameStartInit();
void DrawGameData(HDC hDC);
void DrawMap();
void UserInterface(HDC hDC);

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)//�����������ײ��������FALSE�򲻴������������ԭ·���ƶ�
{
	BOOL IsCollision = FALSE;//�Ƿ�ֱ����ײ
	BOOL IsPoison = FALSE;//�Ƿ��ж�����
	BOOL IsJump = FALSE;//�Ƿ���ڵз�������
	BOOL IsBigJump = FALSE;//�Ƿ�����������������

	if (pSpriteHittee->GetQXSpirit() == 1|| pSpriteHittee->GetQXSpirit() == 20)//���ǻ��߹��(�����δʵ��)
	{
		if (pSpriteHitter->GetQXSpirit() > 1 && pSpriteHitter->GetQXSpirit() < 7)//ͷ����2�βȺ������δʵ��
		{
			if (pSpriteHittee->GetVelocity().y > 0 && pSpriteHitter->GetQXSpirit() != 5)//�ٶ�����ʱ���ȵ��ж�
			{
				if (pSpriteHittee->GetPosition().right > pSpriteHitter->GetPosition().left&&pSpriteHittee->GetPosition().top < pSpriteHitter->GetPosition().top)//�����ж�
				{
					IsJump = TRUE;
					if (pSpriteHitter->GetQXSpirit() == 6|| pSpriteHitter->GetQXSpirit() == 4)//���� ���� ���������
						IsBigJump = TRUE;
				}
				else
					IsCollision = TRUE;
			}
			else
				IsCollision = TRUE;
		}
		else if (pSpriteHitter->GetQXSpirit() == 7)//�Ʋʹ�
		{
			IsCollision = TRUE;
			Sprite* sprite;
			RECT    rcBounds = { 0, 0, 930,480 };
			RECT rcPosition = pSpriteHitter->GetPosition();
			sprite = new Sprite(g_hMapDataBitmap[20], rcBounds, 13, BA_DIE);
			sprite->SetPosition(rcPosition);
			sprite->SetVelocity(0, 0);
			sprite->SetZOrder(13);
			AddSprite(sprite);
			pSpriteHitter->SetKill(TRUE);
		}
		else if (pSpriteHitter->GetQXSpirit() == 8|| pSpriteHitter->GetQXSpirit() == 9)//Ģ������������ �޵��ǣ�����
		{
			IsCollision = TRUE;
			IsPoison = TRUE;
			pSpriteHitter->SetKill(TRUE);
		}
		else if (pSpriteHitter->GetQXSpirit() == 10)//�м���
		{
			//�����м�״̬
		}
		else if (pSpriteHitter->GetQXSpirit() == 11)//����
			IsCollision = TRUE;
		else if (pSpriteHitter->GetQXSpirit() == 15)//�յ���
		{
			PlaySound(L"re/goal.wav", NULL, SND_FILENAME | SND_ASYNC);
			Sprite* sprite;
			RECT    rcBounds = { 0, 0, 930,480 };
			RECT rcPosition = pSpriteHitter->GetPosition();
			sprite = new Sprite(g_hMapDataBitmap[12], rcBounds, 16, BA_DIE);
			sprite->SetPosition(rcPosition);
			sprite->SetVelocity(0, 0);
			sprite->SetZOrder(16);
			AddSprite(sprite);
			pSpriteHitter->SetKill(TRUE);
			g_bGameWin = TRUE;
			pSpriteHittee->SetPosition((134 - g_iX) * 30, pSpriteHittee->GetPosition().top);
			pSpriteHittee->SetVelocity(5, 0);
		}
		else if (pSpriteHitter->GetQXSpirit() == 16)//�Ӵ�����յ���
		{
			pSpriteHittee->OffsetPosition(-5, 5);
		}
	}

	if (IsCollision)//��Ϸ��������,���������������߽�ʱ��ȫ����
	{
		PlaySound(L"re/death.wav", NULL, SND_FILENAME | SND_ASYNC);
		RECT rcPosition = pSpriteHittee->GetPosition();
		pSpriteHittee->SetKill(TRUE);

		Sprite* sprite;
		RECT    rcBounds = { 0, 0, 930,480 };
		if (IsPoison)
			sprite = new Sprite(g_hMapDataBitmap[17], rcBounds, 30, BA_DIE);
		else
			sprite = new Sprite(g_hMarioBitmap, rcBounds, 30, BA_DIE);
		sprite->SetPosition(rcPosition);
		sprite->SetVelocity(0, -12);
		sprite->SetJump(TRUE);
		sprite->SetZOrder(1);
		AddSprite(sprite);
	}
	else if (IsJump)
	{
		if (IsBigJump)
			pSpriteHittee->SetVelocity(pSpriteHittee->GetVelocity().x, -20);
		else
		{
			pSpriteHittee->SetVelocity(pSpriteHittee->GetVelocity().x, -10);
			pSpriteHitter->SetKill(TRUE);
		}
		pSpriteHittee->SetJump(TRUE);
		PlaySound(L"re/humi.wav", NULL, SND_FILENAME | SND_ASYNC);
	}

	return FALSE;
}

void LoadMapData()
{
	g_iMapData[16][160] = { 0 };
	for (int i = 0; i < 160; i++)
		switch (i)
		{
		case 52:
		case 53:
		case 54:
		case 66:
		case 67:
		case 68:
		case 97:
		case 98:
		case 99:
		case 106:
			g_iMapData[14][i] = g_iMapData[15][i] = 0;
			break;
		default:
			g_iMapData[14][i] = 5;
			g_iMapData[15][i] = 6;
			break;
		}
	g_iMapData[11][13] = g_iMapData[11][45] = 11;
	g_iMapData[10][20] = g_iMapData[10][25] = g_iMapData[10][59] = g_iMapData[10][83] = g_iMapData[10][86] = g_iMapData[10][89] = g_iMapData[6][86] = g_iMapData[10][113] = 2;
	g_iMapData[2][19] = g_iMapData[2][45] = g_iMapData[1][76] = 12;
	g_iMapData[10][24] = g_iMapData[10][26] = g_iMapData[10][28] = g_iMapData[10][58] = g_iMapData[10][60] = g_iMapData[6][61] = g_iMapData[6][62] = g_iMapData[6][63] = g_iMapData[6][64] = g_iMapData[6][65] = g_iMapData[6][69] = g_iMapData[6][70] = g_iMapData[6][71] = g_iMapData[10][71] = g_iMapData[10][78] = g_iMapData[10][111] = g_iMapData[10][112] = g_iMapData[10][114] = 1;
	g_iMapData[10][27] = 20;
	g_iMapData[6][26] = 21;
	g_iMapData[10][79] = 22;
	g_iMapData[13][32] = g_iMapData[13][33] = g_iMapData[12][32] = g_iMapData[12][33] = g_iMapData[13][41] = g_iMapData[13][42] = g_iMapData[12][41] = g_iMapData[12][42] = g_iMapData[11][41] = g_iMapData[11][42] = g_iMapData[10][41] = g_iMapData[10][42] = g_iMapData[13][107] = g_iMapData[13][108] = g_iMapData[12][107] = g_iMapData[12][108] = g_iMapData[13][118] = g_iMapData[13][119] = 8;
	g_iMapData[11][32] = g_iMapData[11][33] = g_iMapData[9][41] = g_iMapData[9][42] = g_iMapData[11][107] = g_iMapData[11][108] = g_iMapData[12][118] = g_iMapData[12][119] = 7;
	g_iMapData[13][38] = g_iMapData[13][90] = g_iMapData[13][136] = 10;
	g_iMapData[10][52] = g_iMapData[7][101] = g_iMapData[11][102] = g_iMapData[11][103] = g_iMapData[11][104] = g_iMapData[11][105] = g_iMapData[11][106] = 19;
	g_iMapData[13][94] = g_iMapData[13][95] = g_iMapData[13][96] = g_iMapData[13][100] = g_iMapData[13][101] = g_iMapData[12][95] = g_iMapData[12][96] = g_iMapData[12][100] = g_iMapData[12][101] = g_iMapData[11][96] = g_iMapData[11][100] = 4;
	g_iMapData[13][120] = g_iMapData[13][121] = g_iMapData[13][122] = g_iMapData[13][123] = g_iMapData[13][124] = g_iMapData[13][125] = g_iMapData[13][126] = g_iMapData[13][127] = g_iMapData[13][128] = 4;
	g_iMapData[12][121] = g_iMapData[12][122] = g_iMapData[12][123] = g_iMapData[12][124] = g_iMapData[12][125] = g_iMapData[12][126] = g_iMapData[12][127] = g_iMapData[12][128] = 4;
	g_iMapData[11][122] = g_iMapData[11][123] = g_iMapData[11][124] = g_iMapData[11][125] = g_iMapData[11][126] = g_iMapData[11][127] = g_iMapData[11][128] = 4;
	g_iMapData[10][123] = g_iMapData[10][124] = g_iMapData[10][125] = g_iMapData[10][126] = g_iMapData[10][127] = g_iMapData[10][128] = 4;
	g_iMapData[9][124] = g_iMapData[9][125] = g_iMapData[9][126] = g_iMapData[9][127] = g_iMapData[9][128] = 4;
	g_iMapData[8][125] = g_iMapData[8][126] = g_iMapData[8][127] = g_iMapData[8][128] = 4;
	g_iMapData[7][126] = g_iMapData[7][127] = g_iMapData[7][128] = 4;
	g_iMapData[6][127] = g_iMapData[6][128] = 4;
	g_iMapData[13][134] = 4;
	g_iMapData[11][140] = 9;
}

void LoadEnemyData()
{
	g_iEnemyElement[30] = { 0 };

	g_iEnemyElement[0] = 28;
	g_iEnemyElement[1] = 32;
	g_iEnemyElement[2] = 49;
	g_iEnemyElement[3] = 66;
	g_iEnemyElement[4] = 75;
	g_iEnemyElement[5] = 103;
	g_iEnemyElement[6] = 124;
	g_iEnemyElement[7] = 129;
	g_iEnemyElement[8] = 136;
	g_iEnemyElement[9] = 142;
}

void GameInit(HDC hDC)
{
	// Seed the random number generator
	srand((unsigned)time(NULL));

	// Create the offscreen device context and bitmap
	g_hOffscreenDC = CreateCompatibleDC(hDC);
	g_hOffscreenBitmap = CreateCompatibleBitmap(hDC, 930, WINDOW_HEIGHT);
	SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

	// Create and load the bitmaps    
	g_hMarioBitmap = new Bitmap(hDC, L"res/0.bmp");
	g_hLogoBitmap[0] = new Bitmap(hDC, L"res/clogo2_32.bmp");
	g_hLogoBitmap[1] = new Bitmap(hDC, L"res/clogo32.bmp");
	wchar_t str[20];
	for (int i = 0; i < 29; i++)
	{
		memset(str, 0, sizeof(str));
		swprintf_s(str, L"res/%d.bmp", i + 1);
		g_hMapDataBitmap[i] = new Bitmap(hDC, str);
	}

	g_iLife = 3;
	UserInterface(hDC);
	GameStartInit();
}

void UserInterface(HDC hDC)
{
	g_iX = g_iOffX = 0;
	LoadMapData();
	//g_hLogoBitmap[0]->Draw(hDC, 0, 0);
	//Sleep(1500);
	g_hLogoBitmap[1]->Draw(hDC, 0, 0);
	Sleep(1500);
	DrawMap();
	g_hMapDataBitmap[25]->Draw(g_hOffscreenDC, 140, 80, TRUE, RGB(153, 255, 255));
	g_hMarioBitmap->Draw(g_hOffscreenDC, 60, 384, TRUE, RGB(153, 255, 255));
	SetBkMode(g_hOffscreenDC, TRANSPARENT);
	SetTextColor(g_hOffscreenDC, RGB(0, 150, 0));
	wchar_t str[20] = {};
	swprintf_s(str, L"3���ʼ��Ϸ");
	TextOut(g_hOffscreenDC, 270, 200, str, wcslen(str));
	BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_hOffscreenDC, 0, 0, SRCCOPY);
	Sleep(3000);
	DrawGameData(hDC);
	Sleep(3000);
}

void DrawMap()
{

	HBRUSH hBrush = CreateSolidBrush(RGB(153, 255, 255));
	RECT rcBitmap = { 0, 0, 930, WINDOW_HEIGHT };
	FillRect(g_hOffscreenDC, &rcBitmap, hBrush);
	DeleteObject(hBrush);

	for (int i = 0; i < 16; i++)
		for (int j = g_iX; j < g_iX + 31; j++)
			if (g_iMapData[i][j] > 0 && g_iMapData[i][j] < 15)
			{
				if (g_iMapData[i][j] == 8)
				{
					if (g_iMapData[i][j - 1] != 8)
						g_hMapDataBitmap[g_iMapData[i][j] - 1]->Draw(g_hOffscreenDC, (j - g_iX) * 30 + 5, i * 30);
				}
				else if (g_iMapData[i][j] == 7)
				{
					if (g_iMapData[i][j - 1] != 7)
						g_hMapDataBitmap[g_iMapData[i][j] - 1]->Draw(g_hOffscreenDC, (j - g_iX) * 30, i * 30);
				}
				else
					g_hMapDataBitmap[g_iMapData[i][j] - 1]->Draw(g_hOffscreenDC, (j - g_iX) * 30, i * 30);
			}
			else if (g_iMapData[i][j] > 19 && g_iMapData[i][j] < 23)
				g_hMapDataBitmap[1]->Draw(g_hOffscreenDC, (j - g_iX) * 30, i * 30);
}

void DrawEnemy()
{
	Sprite* sprite;
	RECT    rcBounds = { 0, 0, 930,480 };
	int col = (g_hMarioSprite->GetPosition().right - 1) / 30 + g_iX;//���ﵱǰ����
	for (int i = 0; i < 30; i++)
	{
		if (g_iEnemyElement[i] != 0)
		{
			switch (i)
			{
			case 0:
				if (col == g_iEnemyElement[i])//������һ��������
				{
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 6, BA_DIE);
					sprite->SetPosition((39 - g_iX) * 30, 210);
					sprite->SetVelocity(-1, 0);
					sprite->SetZOrder(6);
					AddSprite(sprite);
					g_iEnemyElement[i] = 0;
				}
				break;
			case 1:
				if (col == g_iEnemyElement[i])//�������������
				{
					PlaySound(L"re/kirra.wav", NULL, SND_FILENAME | SND_ASYNC);
					sprite = new Sprite(g_hMapDataBitmap[14], rcBounds, 5, BA_DIE);
					sprite->SetPosition((32.5 - g_iX) * 30, 330);
					sprite->SetVelocity(0, -10);
					sprite->SetZOrder(5);
					AddSprite(sprite);
					g_iEnemyElement[i] = 0;
				}
				break;
			case 2:
				if (col == g_iEnemyElement[i])//��������СԲ��
				{
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
					sprite->SetPosition((60 - g_iX) * 30, 150);
					sprite->SetVelocity(-1, 0);
					sprite->SetZOrder(2);
					AddSprite(sprite);
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
					sprite->SetPosition((63 - g_iX) * 30, 150);
					sprite->SetVelocity(-1, 0);
					sprite->SetZOrder(2);
					AddSprite(sprite);
					g_iEnemyElement[i] = 0;
				}
				break;
			case 3:
				if (col == g_iEnemyElement[i])//�����½������1
				{
					PlaySound(L"re/kirra.wav", NULL, SND_FILENAME | SND_ASYNC);
					sprite = new Sprite(g_hMapDataBitmap[28], rcBounds, 5, BA_DIE);
					sprite->SetPosition((68 - g_iX) * 30, 0);
					sprite->SetVelocity(0, 20);
					sprite->SetZOrder(5);
					AddSprite(sprite);
					g_iEnemyElement[i] = 0;
				}
				break;
			case 4:
				if (col == g_iEnemyElement[i])//������СԲ�ֺ�һ����ͷ��
				{
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
					sprite->SetPosition((86 - g_iX) * 30, 330);
					sprite->SetVelocity(1, 0);
					sprite->SetZOrder(2);
					AddSprite(sprite);
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
					sprite->SetPosition((89 - g_iX) * 30, 330);
					sprite->SetVelocity(1, 0);
					sprite->SetZOrder(2);
					AddSprite(sprite);
					sprite = new Sprite(g_hMapDataBitmap[15], rcBounds, 3, BA_DIE);
					sprite->SetPosition((92 - g_iX) * 30, 330);
					sprite->SetVelocity(1, 0);
					sprite->SetZOrder(3);
					AddSprite(sprite);
					g_iEnemyElement[i] = 0;
				}
				break;
			case 5:
				if (col == g_iEnemyElement[i])//��������СԲ�ֺ�һ���Ʋʹ�
				{
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
					sprite->SetPosition((113 - g_iX) * 30, 330);
					sprite->SetVelocity(-1, 0);
					sprite->SetZOrder(2);
					AddSprite(sprite);
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
					sprite->SetPosition((116 - g_iX) * 30, 330);
					sprite->SetVelocity(-1, 0);
					sprite->SetZOrder(2);
					AddSprite(sprite);
					sprite = new Sprite(g_hMapDataBitmap[11], rcBounds, 7, BA_DIE);
					sprite->SetPosition((115 - g_iX) * 30, 180);
					sprite->SetVelocity(0, 0);
					sprite->SetZOrder(7);
					AddSprite(sprite);
					g_iEnemyElement[i] = 0;
				}
				break;
			case 6:
				if (col == g_iEnemyElement[i])//�����ĸ�СԲ�ֺ����Ӿ���
				{
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
					sprite->SetPosition((124 - g_iX) * 30, 0);
					sprite->SetVelocity(-1, 0);
					sprite->SetZOrder(2);
					AddSprite(sprite);
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
					sprite->SetPosition((125 - g_iX) * 30, 0);
					sprite->SetVelocity(-1, 0);
					sprite->SetZOrder(2);
					AddSprite(sprite);
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
					sprite->SetPosition((126 - g_iX) * 30, 0);
					sprite->SetVelocity(-1, 0);
					sprite->SetZOrder(2);
					AddSprite(sprite);
					sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
					sprite->SetPosition((127 - g_iX) * 30, 0);
					sprite->SetVelocity(-1, 0);
					sprite->SetZOrder(2);
					AddSprite(sprite);
					sprite = new Sprite(g_hMapDataBitmap[12], rcBounds, 15, BA_DIE);
					sprite->SetPosition((134 - g_iX) * 30, 90);
					sprite->SetVelocity(0, 0);
					sprite->SetZOrder(15);
					AddSprite(sprite);
					g_iEnemyElement[i] = 0;
				}
				break;
			case 7:
				if (col == g_iEnemyElement[i])//����һ�����������
				{
					sprite = new Sprite(g_hMapDataBitmap[23], rcBounds, 4, BA_DIE);
					sprite->SetPosition((144 - g_iX) * 30, 250);
					sprite->SetVelocity(-2, 0);
					sprite->SetZOrder(4);
					AddSprite(sprite);
					g_iEnemyElement[i] = 0;
				}
				break;
			case 8:
				if (col == g_iEnemyElement[i])//�����½������2
				{
					PlaySound(L"re/kirra.wav", NULL, SND_FILENAME | SND_ASYNC);
					sprite = new Sprite(g_hMapDataBitmap[28], rcBounds, 5, BA_DIE);
					sprite->SetPosition((138 - g_iX) * 30, 0);
					sprite->SetVelocity(0, 20);
					sprite->SetZOrder(5);
					AddSprite(sprite);
					g_iEnemyElement[i] = 0;
				}
				break;
			case 9:
				//win
				if (col == g_iEnemyElement[i])
				{
					if (g_bGameWin) 
					{
						g_iEnemyElement[i] = 0;
						MessageBox(hWnd, L"ʤ���ˣ�", L"��Ϸ����", MB_OK);
						PostQuitMessage(1);
					}
				}
				break;
			}
		}
	}
}

void GameStartInit()
{
	CleanupSprites();

	LoadMapData();
	LoadEnemyData();
	Sprite* sprite;
	RECT    rcBounds = { 0, 0, 930,480 };
	g_hMarioSprite = new Sprite(g_hMarioBitmap, rcBounds, 1, BA_STOP);
	g_hMarioSprite->SetPosition(60, 384);
	g_hMarioSprite->SetVelocity(0, 0);
	g_hMarioSprite->SetZOrder(1);
	AddSprite(g_hMarioSprite);
	sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
	sprite->SetPosition(520, 390);
	sprite->SetVelocity(-1, 0);
	sprite->SetZOrder(2);
	AddSprite(sprite);
	sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
	sprite->SetPosition(350, 390);
	sprite->SetVelocity(-1, 0);
	sprite->SetZOrder(2);
	AddSprite(sprite);

	g_iOffX = 0;
	g_iX = 10;
	g_iInputDelay = 0;
	g_bGameOver = g_bIsReSetPosition = g_bGameWin = FALSE;
}

void DrawGameData(HDC hDC)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	RECT rcBitmap = { 0, 0, 930, WINDOW_HEIGHT };
	FillRect(g_hOffscreenDC, &rcBitmap, hBrush);
	DeleteObject(hBrush);

	g_hMarioBitmap->Draw(g_hOffscreenDC, 270, 200, TRUE, RGB(153, 255, 255));

	SetBkMode(g_hOffscreenDC, TRANSPARENT);
	SetTextColor(g_hOffscreenDC, RGB(255, 255, 255));
	wchar_t str[20] = {};
	swprintf_s(str, L"x  %d", g_iLife);
	TextOut(g_hOffscreenDC, 310, 210, str, wcslen(str));

	BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_hOffscreenDC, 0, 0, SRCCOPY);
}

void GameEnd()
{
	DeleteObject(g_hOffscreenBitmap);
	DeleteDC(g_hOffscreenDC);  

	// Cleanup the bitmaps
	delete g_hMarioBitmap;
	delete g_hLogoBitmap[0];
	delete g_hLogoBitmap[1];
	for (int i = 0; i < 29; i++)
		delete g_hMapDataBitmap[i];

	// Cleanup the sprites
	CleanupSprites();
}

void GamePaint(HDC hDC)
{
	DrawMap();

	DrawEnemy();

	// Draw the sprites
	DrawSprites(g_hOffscreenDC);

	// Blit the offscreen bitmap to the game screen
	BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_hOffscreenDC, g_iOffX, 0, SRCCOPY);
}

void Fire(RECT re)
{
	//����·��ӵ�
}

void CreatBrokenBrick(int x, int y)
{
	PlaySound(L"re/brockbreak.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sprite* sprite;
	RECT    rcBounds = { 0, 0, 640,480 };
	sprite = new Sprite(g_hMapDataBitmap[24], rcBounds, 12, BA_DIE);
	sprite->SetPosition(x, y);
	sprite->SetVelocity(-1, -8);
	sprite->SetZOrder(12);
	sprite->SetJump(TRUE);
	AddSprite(sprite);
	sprite = new Sprite(g_hMapDataBitmap[24], rcBounds, 12, BA_DIE);
	sprite->SetPosition(x + 16, y);
	sprite->SetVelocity(1, -8);
	sprite->SetZOrder(12);
	sprite->SetJump(TRUE);
	AddSprite(sprite);
	sprite = new Sprite(g_hMapDataBitmap[24], rcBounds, 12, BA_DIE);
	sprite->SetPosition(x, y + 16);
	sprite->SetVelocity(-2, -6);
	sprite->SetZOrder(12);
	sprite->SetJump(TRUE);
	AddSprite(sprite);
	sprite = new Sprite(g_hMapDataBitmap[24], rcBounds, 12, BA_DIE);
	sprite->SetPosition(x + 16, y + 16);
	sprite->SetVelocity(2, -6);
	sprite->SetZOrder(12);
	sprite->SetJump(TRUE);
	AddSprite(sprite);
}

void CreatGold(int x, int y)
{
	PlaySound(L"re/brockcoin.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sprite* sprite;
	RECT    rcBounds = { 0, y - 60, 640,480 };
	sprite = new Sprite(g_hMapDataBitmap[19], rcBounds, 13, BA_DIE);
	sprite->SetPosition(x, y);
	sprite->SetVelocity(0, -10);
	sprite->SetZOrder(13);
	AddSprite(sprite);
}

void CreatMushroom(int x, int y)
{
	PlaySound(L"re/brockkinoko.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sprite* sprite;
	RECT    rcBounds = { 0, 0, 910,480 };
	sprite = new Sprite(g_hMapDataBitmap[21], rcBounds, 8, BA_DIE);
	sprite->SetPosition(x, y);
	sprite->SetVelocity(1, -7);
	sprite->SetJump(TRUE);
	sprite->SetZOrder(8);
	AddSprite(sprite);
}

void CreatSpirit(int x, int y)
{
	PlaySound(L"re/brockkinoko.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sprite* sprite;
	RECT    rcBounds = { 0, 0, 910,480 };
	sprite = new Sprite(g_hMapDataBitmap[26], rcBounds, 2, BA_DIE);
	sprite->SetPosition(x, y);
	sprite->SetVelocity(1, -7);
	sprite->SetJump(TRUE);
	sprite->SetZOrder(2);
	AddSprite(sprite);
}

void CreatStar(int x, int y)
{
	PlaySound(L"re/brockkinoko.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sprite* sprite;
	RECT    rcBounds = { 0, 0, 910,480 };
	sprite = new Sprite(g_hMapDataBitmap[22], rcBounds, 9, BA_DIE);
	sprite->SetPosition(x, y);
	sprite->SetVelocity(3, -7);
	sprite->SetJump(TRUE);
	sprite->SetZOrder(9);
	AddSprite(sprite);
}

void HandleKeys()
{
	int num = g_hMarioSprite->GetVelocity().x;
	if (!g_bGameOver)
	{
		if (KEYDOWN(VK_LEFT) && !g_bGameWin)
			g_hMarioSprite->SetVelocity(-5, g_hMarioSprite->GetVelocity().y);
		else if (KEYDOWN(VK_RIGHT) && !g_bGameWin)
		{
			if (g_hMarioSprite->GetPosition().left >= 320)
			{
				g_iOffX += 5;
				if (g_iOffX >= 30)
				{
					g_iOffX = 0;
					if (g_iX < 137)
						g_iX++;
					g_bIsReSetPosition = TRUE;
				}
			}
			g_hMarioSprite->SetVelocity(5, g_hMarioSprite->GetVelocity().y);
		}
		else if ((KEYUP(VK_LEFT) || KEYUP(VK_RIGHT)) && !g_bGameWin)
			g_hMarioSprite->SetVelocity(0, g_hMarioSprite->GetVelocity().y);

		if (KEYDOWN(VK_UP) && !g_bGameWin)
		{
			if (!g_hMarioSprite->IsJump() && !g_hMarioSprite->IsDown())
			{
				PlaySound(L"re/jump.wav", NULL, SND_FILENAME | SND_ASYNC);
				g_hMarioSprite->SetVelocity(g_hMarioSprite->GetVelocity().x, -15);
				g_hMarioSprite->SetJump(TRUE);
			}
		}
		//else if (KEYDOWN(VK_DOWN))
		//{
		//	//��ˮ��
		//}
		//else if (KEYUP(VK_UP))
		//{
		//	//�жϳ��̰�
		//}
	}
}

void GameLogic()
{
	HandleKeys();
	// Update the sprites
	UpdateSprites();
}

int GameRun(HDC hDC)
{
	if (!g_bGameOver)
	{
		//��Ϸ�߼�
		GameLogic();
		//��Ϸ����
		GamePaint(hDC);
		// Cleanup
		ReleaseDC(hWnd, hDC);
	}
	else
	{
		g_iLife--;
		if (g_iLife > 0)
		{
			Sleep(1000);//�ȴ�����ֹͣ?
			GameStartInit();
			//��Ϸ��ʼ����
			DrawGameData(hDC);

			Sleep(3000);
			g_bGameOver = FALSE;
		}
		else
		{
			//�����Ϸ��������
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
			RECT rcBitmap = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
			FillRect(hDC, &rcBitmap, hBrush);
			DeleteObject(hBrush);
			g_hMapDataBitmap[27]->Draw(hDC, 180, 150, TRUE, RGB(255, 0, 255));
			Sleep(3000);
			PostQuitMessage(1);
		}
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�ص���������ɶ���Ϣ�Ĵ����ɲ���ϵͳֱ�ӵ���
LRESULT CALLBACK WinProc(HWND hwnd,UINT uMsg, WPARAM wParam,LPARAM lParam)
{	//���ݲ�ͬ����Ϣ���ͽ�����Ӧ�Ĵ���
	switch(uMsg)
	{	
	case WM_CREATE: //���ڴ���ʱ��������Ϣ
		return(0);		
	
	case WM_DESTROY: //��������ʱ��������Ϣ
		{			
			PostQuitMessage(0);			
			return(0);
		} 		
	default:
		break;		
	} 
	//δ���������봦�����Ϣ���������ﴦ��
	return DefWindowProc(hwnd,uMsg,wParam,lParam); 
}
/*windows���������������е���ڣ����������ִ�У��뿪�ú�����ʾ����Ľ�����*/
int WINAPI WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,    int nCmdShow
				   )
{
	//����һ��������Ķ��󣨲ο�MSDN��
	WNDCLASSEX winclass = { 0 };
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winclass.lpfnWndProc = WinProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hInstance;
	winclass.hIcon = (HICON)::LoadImage(NULL, L"res/icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);//�޷����ͼ������������
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = L"GameWndClass";
	winclass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	//ע�ᴰ����
	if(!RegisterClassEx(&winclass))
		return 0;

	//��ʵ�������һ��ȫ�ֱ������棬����������������ʹ��
	g_hInstance = hInstance; 

	//���ݿͻ�����С��ȡ���ڴ�С
	RECT rect;
	SetRect(&rect, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE, NULL);

	//�������涨��Ĵ����ഴ��һ���ض��Ĵ��ڶ���
	HWND hwnd = CreateWindowEx(
		NULL, 
		L"GameWndClass",
		L"Mario",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		400,
		200,
		rect.right-rect.left, 
		rect.bottom-rect.top,  
		NULL,
		NULL, 
		hInstance, 
		NULL 
		); 

	//�����ھ����һ��ȫ�ֱ������棬����������������ʹ��
	hWnd = hwnd;

	ShowWindow(hwnd, SW_SHOW);//��ʾ����
	UpdateWindow(hwnd);//���»���һ�鴰��

	HDC hdc = GetDC(hwnd);
	GameInit(hdc);
	//������Ϣѭ����ע������ʹ��һ�ַ������Ļ�ȡ��Ϣ�ķ�ʽ
	MSG msg;
	while(1)
	{
		DWORD starttime = ::GetTickCount();

		if(PeekMessage(&msg, NULL, 0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		GameRun(hdc);

		if(GetTickCount()-starttime<33)
			Sleep(33- (GetTickCount()-starttime));
	}

	GameEnd();
	return 1;
}