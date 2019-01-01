/*���������*/

#include "Sprite.h"
#include <vector>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern vector<Sprite*>     m_vSprites;

//������ײ��Ӧ�����ӿڣ����ض�����Ϸʵ��
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee);

BOOL CheckSpriteCollision(Sprite* pTestSprite);

void AddSprite(Sprite* pSprite);

void DrawSprites(HDC hDC);

void UpdateSprites();

void CleanupSprites();

Sprite* IsPointInSprite(int x, int y);