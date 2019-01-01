
#include "SpriteManager.h"


vector<Sprite*>     m_vSprites;

extern int g_iMapData[16][160];
extern int  g_iOffX, g_iX;
extern BOOL g_bIsReSetPosition;
extern void CreatBrokenBrick(int x, int y);
extern void CreatGold(int x, int y);
extern void CreatMushroom(int x, int y);
extern void CreatSpirit(int x, int y);
extern void CreatStar(int x, int y);

void AddSprite(Sprite* pSprite)
{
	// Add a sprite to the sprite vector
	if (pSprite != NULL)
	{
		// See if there are sprites already in the sprite vector
		if (m_vSprites.size() > 0)
		{
			// Find a spot in the sprite vector to insert the sprite by its z-order
			vector<Sprite*>::iterator siSprite;
			for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
				if (pSprite->GetZOrder() < (*siSprite)->GetZOrder())
				{
					// Insert the sprite into the sprite vector
					m_vSprites.insert(siSprite, pSprite);
					return;
				}
		}

		// The sprite's z-order is highest, so add it to the end of the vector
		m_vSprites.push_back(pSprite);
	}
}

void DrawSprites(HDC hDC)
{
	// Draw the sprites in the sprite vector
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
		(*siSprite)->Draw(hDC);
}
void UpdateSprites()
{
	// Check to see if the sprite vector needs to grow
	if (m_vSprites.size() >= (m_vSprites.capacity() / 2))
		m_vSprites.reserve(m_vSprites.capacity() * 2);

	// Update the sprites in the sprite vector
	RECT          rcOldSpritePos;//����ԭʼλ��
	SPRITEACTION  saSpriteAction;
	vector<Sprite*>::iterator siSprite;
	BOOL IsDeleteFirst = FALSE;
	BOOL Key;
	int x, y;//���к�
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		if (IsDeleteFirst)
		{
			siSprite--;
			IsDeleteFirst = FALSE;
		}
		// Save the old sprite position in case we need to restore it
		rcOldSpritePos = (*siSprite)->GetPosition();

		// Update the sprite
		saSpriteAction = (*siSprite)->Update();
		if (g_bIsReSetPosition)//���ص�ԭλ��  ����
			(*siSprite)->OffsetPosition(-30, 0);//��ͼ����һ�У�λ�ú���һ��

		if ((*siSprite)->IsJump())//��������ģ���������
		//if (!(*siSprite)->IsDown())//�ų��������  ��������Ĺ���
			(*siSprite)->SetVelocity((*siSprite)->GetVelocity().x, (*siSprite)->GetVelocity().y + 1);

		// Handle the SA_KILL sprite action
		if (saSpriteAction & SA_KILL)
		{
			delete (*siSprite);
			siSprite = m_vSprites.erase(siSprite);
			if (siSprite != m_vSprites.begin())
				siSprite--;
			else if (m_vSprites.size() == 0)//û�о���ֱ��break
				break;
			else
				IsDeleteFirst = TRUE;
			continue;
		}

		// See if the sprite collided with any others
		if (CheckSpriteCollision(*siSprite))
			// Restore the old sprite position
			(*siSprite)->SetPosition(rcOldSpritePos);

		//�ƶ��ж�
		if ((*siSprite)->GetQXSpirit() < 10 && (*siSprite)->GetQXSpirit() != 5 && (*siSprite)->GetQXSpirit() != 7)//���ʹ���9�ҵ���5��7������֣��Ʋʹ֣��Ĳ������ж�
		{
			//�����ϰ��ж�
			if (!(*siSprite)->IsJump() && (*siSprite)->GetVelocity().y == 0 && (*siSprite)->GetQXSpirit() != 4)//4Ϊ���������
			{
				Key = FALSE;//�Ƿ�ȿ�����
				x = (*siSprite)->GetPosition().bottom / 30;
				y = (*siSprite)->GetPosition().right / 30 - 1;

				if (g_iMapData[x][y + g_iX] == 0 || g_iMapData[x][y + g_iX] > 7)//���¸������ϰ�
					if ((*siSprite)->GetPosition().right % 30 != 0)//�����¸��ӣ���δ���룩
					{
						if (g_iMapData[x][y + g_iX + 1] == 0 || g_iMapData[x][y + g_iX + 1] > 7)//���¸������ϰ�
							Key = TRUE;
					}
					else//�����¸���
						Key = TRUE;
				if (Key)
				{
					(*siSprite)->SetVelocity((*siSprite)->GetVelocity().x, 10);
					(*siSprite)->SetDown(TRUE);
				}
			}

			//�����ϰ��ж�
			if ((*siSprite)->GetVelocity().x > 0)
			{
				Key = TRUE;//�Ƿ���ײ����
				x = (*siSprite)->GetPosition().bottom / 30 - 1;
				y = (*siSprite)->GetPosition().right / 30;

				if (g_iMapData[x][y + g_iX] == 0 || g_iMapData[x][y + g_iX] > 8)//���ϸ������ϰ�
					if ((*siSprite)->GetPosition().bottom % 30 != 0)//�����±߸��ӣ���δ���룩
					{
						if (g_iMapData[x + 1][y + g_iX] == 0 || g_iMapData[x + 1][y + g_iX] > 8)//���¸������ϰ�
							Key = FALSE;
					}
					else//�����¸���
						Key = FALSE;
				if (Key)//��ײ����
				{
					(*siSprite)->OffsetPosition(-(*siSprite)->GetPosition().right % 30, 0);
					if ((*siSprite)->GetQXSpirit() == 1)
						(*siSprite)->SetVelocity(0, (*siSprite)->GetVelocity().y);
					else
						(*siSprite)->SetVelocity(-(*siSprite)->GetVelocity().x, (*siSprite)->GetVelocity().y);
				}
			}
			else if ((*siSprite)->GetVelocity().x < 0)
			{
				Key = TRUE;//�Ƿ���ײ����
				x = (*siSprite)->GetPosition().top / 30;
				y = (*siSprite)->GetPosition().left / 30;

				if (g_iMapData[x][y + g_iX] == 0 || g_iMapData[x][y + g_iX] > 8)//���ϸ������ϰ�
					if ((*siSprite)->GetPosition().top % 30 != 0)//�����±߸��ӣ���δ���룩
					{
						if (g_iMapData[x + 1][y + g_iX] == 0 || g_iMapData[x + 1][y + g_iX] > 8)//���¸������ϰ�
							Key = FALSE;
					}
					else//�����¸���
						Key = FALSE;
				if (Key)//��ײ����
				{
					(*siSprite)->OffsetPosition(30 - (*siSprite)->GetPosition().left % 30, 0);
					if ((*siSprite)->GetQXSpirit() == 1)
						(*siSprite)->SetVelocity(0, (*siSprite)->GetVelocity().y);
					else
						(*siSprite)->SetVelocity(-(*siSprite)->GetVelocity().x, (*siSprite)->GetVelocity().y);
				}
			}

			//�����ϰ��ж�
			if ((*siSprite)->GetVelocity().y > 0)
			{
				Key = TRUE;//�Ƿ�����������  
				x = (*siSprite)->GetPosition().bottom / 30;
				y = (*siSprite)->GetPosition().right / 30 - 1;

				if (g_iMapData[x][y + g_iX] == 0 || g_iMapData[x][y + g_iX] > 7 && g_iMapData[x][y + g_iX] < 20)//���¸������ϰ�
					if ((*siSprite)->GetPosition().right % 30 != 0)//�����¸��ӣ���δ���룩
					{
						if (g_iMapData[x][y + g_iX + 1] == 0 || g_iMapData[x][y + g_iX + 1] > 7 && g_iMapData[x][y + g_iX + 1] < 20)//���¸������ϰ�
							Key = FALSE;
					}
					else//�����¸���
						Key = FALSE;
				if (Key)
				{
					if ((*siSprite)->GetQXSpirit() == 9)//�޵���
						(*siSprite)->SetVelocity((*siSprite)->GetVelocity().x, -15);
					else
					{
						(*siSprite)->SetVelocity((*siSprite)->GetVelocity().x, 0);
						(*siSprite)->SetJump(FALSE);
					}
					(*siSprite)->SetPosition((*siSprite)->GetPosition().left, x * 30 - (*siSprite)->GetHeight());
					(*siSprite)->SetDown(FALSE);
					
				}
			}
			else if ((*siSprite)->GetVelocity().y < 0)
			{
				//����Ҫ�ж���ײ��������,�������ש��ľ���
				Key = TRUE;//�Ƿ���ײ����
				BOOL IsLeft = FALSE;//���ϸ����Ƿ����ϰ�
				BOOL IsJump = FALSE;//�Ƿ���Լ�����
				BOOL IsLeftCollision = FALSE;//�Ƿ���ײ���ϸ���
				if ((*siSprite)->GetPosition().top % 30 == 0)
					x = (*siSprite)->GetPosition().top / 30 - 1;
				else
					x = (*siSprite)->GetPosition().top / 30;
				y = (*siSprite)->GetPosition().left / 30;

				if (g_iMapData[x][y + g_iX] == 0 || g_iMapData[x][y + g_iX] > 7 && g_iMapData[x][y + g_iX] < 19)//���ϸ������ϰ�
					if ((*siSprite)->GetPosition().left % 30 != 0)//�����ϱ߸��ӣ���δ���룩
					{
						if (g_iMapData[x][y + g_iX + 1] == 0 || g_iMapData[x][y + g_iX + 1] > 7 && g_iMapData[x][y + g_iX + 1] < 19)//���ϸ������ϰ�
							Key = FALSE;
					}
					else//�����ϸ���
						Key = FALSE;
				else
					IsLeft = TRUE;
				if (Key)//��ײ����
				{
					if ((*siSprite)->GetPosition().left % 30 != 0)//δ���ж��룬�ж��Ƿ�ֻ�ǲ���
					{
						if ((*siSprite)->GetPosition().left % 30 < 11)
							if (IsLeft)//�����,ֱ����ײ
								IsLeftCollision = TRUE;
							else //����ߣ�������
							{
								if ((*siSprite)->GetVelocity().x <= 0 && g_iMapData[x][y + g_iX + 1] != 19)//�����������ƶ���״̬,����������ͼ��
									IsJump = TRUE;
							}
						else if ((*siSprite)->GetPosition().left % 30 > 19)
						{
							if (IsLeft)//�����
								if (g_iMapData[x][y + g_iX + 1] == 0 || g_iMapData[x][y + g_iX + 1] > 7 && g_iMapData[x][y + g_iX + 1] < 19)//���ұߣ�������
								{
									if ((*siSprite)->GetVelocity().x >= 0 && g_iMapData[x][y + g_iX] != 19)//�����������ƶ���״̬,����������ͼ��
										IsJump = TRUE;
								}
						}
						else
							if (IsLeft)//�����,��ײ���
								IsLeftCollision = TRUE;
					}
					else//���ж���  ֻ�ж�Ӧ����ײ
						IsLeftCollision = TRUE;

					if (IsJump)
					{
						if (IsLeft)
							(*siSprite)->OffsetPosition(30 - (*siSprite)->GetPosition().left % 30, 0);
						else
							(*siSprite)->OffsetPosition(-((*siSprite)->GetPosition().right % 30), 0);
					}
					else 
					{
						int col;
						if (IsLeftCollision)
							col = y + g_iX;
						else
							col = y + g_iX + 1;
						if ((*siSprite)->GetQXSpirit() != 9)
							switch (g_iMapData[x][col])
							{
							case 1://�����
								CreatBrokenBrick((col - g_iX) * 30, x * 30);
								g_iMapData[x][col] = 0;
								break;
							case 2://�����
								CreatGold((col - g_iX) * 30, x * 30);
								g_iMapData[x][col] = 3;
								break;
							case 20://��Ģ��
								CreatMushroom((col - g_iX) * 30, x * 30);
								g_iMapData[x][col] = 3;
								break;
							case 21://��С��
								CreatSpirit((col - g_iX) * 30, x * 30);
								g_iMapData[x][col] = 3;
								break;
							case 22://���޵���
								CreatStar((col - g_iX) * 30, x * 30);
								g_iMapData[x][col] = 3;
								break;
							case 19://��ש��
								g_iMapData[x][col] = 3;
								break;
							}
						(*siSprite)->SetPosition((*siSprite)->GetPosition().left, (x + 1) * 30);
						(*siSprite)->SetVelocity((*siSprite)->GetVelocity().x, -(*siSprite)->GetVelocity().y);
					}
				}
			}
		}
	}
	if (g_bIsReSetPosition)
		g_bIsReSetPosition = FALSE;
}

void CleanupSprites()
{
	// Delete and remove the sprites in the sprite vector
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		delete (*siSprite);
		//siSprite = m_vSprites.erase(siSprite);
		//siSprite--;           ɾ����Ԫ�غ� - ����Խ��
	}
	m_vSprites.clear();
}

Sprite* IsPointInSprite(int x, int y)
{
	// See if the point is in a sprite in the sprite vector
	vector<Sprite*>::reverse_iterator siSprite;
	for (siSprite = m_vSprites.rbegin(); siSprite != m_vSprites.rend(); siSprite++)
		if (!(*siSprite)->IsHidden() && (*siSprite)->IsPointInside(x, y))
			return (*siSprite);

	// The point is not in a sprite
	return NULL;
}


BOOL CheckSpriteCollision(Sprite* pTestSprite)
{
	// See if the sprite has collided with any other sprites
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		// Make sure not to check for collision with itself
		if (pTestSprite == (*siSprite))
			continue;

		// Test the collision
		if (pTestSprite->TestCollision(*siSprite))//����ų�����
			// Collision detected
			return SpriteCollision((*siSprite), pTestSprite);
	}

	// No collision
	return FALSE;
}