
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
	RECT          rcOldSpritePos;//精灵原始位置
	SPRITEACTION  saSpriteAction;
	vector<Sprite*>::iterator siSprite;
	BOOL IsDeleteFirst = FALSE;
	BOOL Key;
	int x, y;//行列号
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
		if (g_bIsReSetPosition)//若回到原位置  出错？
			(*siSprite)->OffsetPosition(-30, 0);//地图更新一列，位置后退一格

		if ((*siSprite)->IsJump())//若起跳则模拟重力情况
		//if (!(*siSprite)->IsDown())//排除特殊情况  不需下落的怪物
			(*siSprite)->SetVelocity((*siSprite)->GetVelocity().x, (*siSprite)->GetVelocity().y + 1);

		// Handle the SA_KILL sprite action
		if (saSpriteAction & SA_KILL)
		{
			delete (*siSprite);
			siSprite = m_vSprites.erase(siSprite);
			if (siSprite != m_vSprites.begin())
				siSprite--;
			else if (m_vSprites.size() == 0)//没有精灵直接break
				break;
			else
				IsDeleteFirst = TRUE;
			continue;
		}

		// See if the sprite collided with any others
		if (CheckSpriteCollision(*siSprite))
			// Restore the old sprite position
			(*siSprite)->SetPosition(rcOldSpritePos);

		//移动判定
		if ((*siSprite)->GetQXSpirit() < 10 && (*siSprite)->GetQXSpirit() != 5 && (*siSprite)->GetQXSpirit() != 7)//类型大于9且等于5，7（冲击怪，云彩怪）的不参与判定
		{
			//脚下障碍判定
			if (!(*siSprite)->IsJump() && (*siSprite)->GetVelocity().y == 0 && (*siSprite)->GetQXSpirit() != 4)//4为火箭高跳怪
			{
				Key = FALSE;//是否踩空下落
				x = (*siSprite)->GetPosition().bottom / 30;
				y = (*siSprite)->GetPosition().right / 30 - 1;

				if (g_iMapData[x][y + g_iX] == 0 || g_iMapData[x][y + g_iX] > 7)//左下格子无障碍
					if ((*siSprite)->GetPosition().right % 30 != 0)//有右下格子（列未对齐）
					{
						if (g_iMapData[x][y + g_iX + 1] == 0 || g_iMapData[x][y + g_iX + 1] > 7)//右下格子无障碍
							Key = TRUE;
					}
					else//无右下格子
						Key = TRUE;
				if (Key)
				{
					(*siSprite)->SetVelocity((*siSprite)->GetVelocity().x, 10);
					(*siSprite)->SetDown(TRUE);
				}
			}

			//横向障碍判定
			if ((*siSprite)->GetVelocity().x > 0)
			{
				Key = TRUE;//是否碰撞物体
				x = (*siSprite)->GetPosition().bottom / 30 - 1;
				y = (*siSprite)->GetPosition().right / 30;

				if (g_iMapData[x][y + g_iX] == 0 || g_iMapData[x][y + g_iX] > 8)//右上格子无障碍
					if ((*siSprite)->GetPosition().bottom % 30 != 0)//有右下边格子（列未对齐）
					{
						if (g_iMapData[x + 1][y + g_iX] == 0 || g_iMapData[x + 1][y + g_iX] > 8)//右下格子无障碍
							Key = FALSE;
					}
					else//无右下格子
						Key = FALSE;
				if (Key)//碰撞处理
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
				Key = TRUE;//是否碰撞物体
				x = (*siSprite)->GetPosition().top / 30;
				y = (*siSprite)->GetPosition().left / 30;

				if (g_iMapData[x][y + g_iX] == 0 || g_iMapData[x][y + g_iX] > 8)//左上格子无障碍
					if ((*siSprite)->GetPosition().top % 30 != 0)//有左下边格子（列未对齐）
					{
						if (g_iMapData[x + 1][y + g_iX] == 0 || g_iMapData[x + 1][y + g_iX] > 8)//左下格子无障碍
							Key = FALSE;
					}
					else//无左下格子
						Key = FALSE;
				if (Key)//碰撞处理
				{
					(*siSprite)->OffsetPosition(30 - (*siSprite)->GetPosition().left % 30, 0);
					if ((*siSprite)->GetQXSpirit() == 1)
						(*siSprite)->SetVelocity(0, (*siSprite)->GetVelocity().y);
					else
						(*siSprite)->SetVelocity(-(*siSprite)->GetVelocity().x, (*siSprite)->GetVelocity().y);
				}
			}

			//纵向障碍判定
			if ((*siSprite)->GetVelocity().y > 0)
			{
				Key = TRUE;//是否落在物体上  
				x = (*siSprite)->GetPosition().bottom / 30;
				y = (*siSprite)->GetPosition().right / 30 - 1;

				if (g_iMapData[x][y + g_iX] == 0 || g_iMapData[x][y + g_iX] > 7 && g_iMapData[x][y + g_iX] < 20)//左下格子无障碍
					if ((*siSprite)->GetPosition().right % 30 != 0)//有右下格子（列未对齐）
					{
						if (g_iMapData[x][y + g_iX + 1] == 0 || g_iMapData[x][y + g_iX + 1] > 7 && g_iMapData[x][y + g_iX + 1] < 20)//右下格子无障碍
							Key = FALSE;
					}
					else//无右下格子
						Key = FALSE;
				if (Key)
				{
					if ((*siSprite)->GetQXSpirit() == 9)//无敌星
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
				//还需要判定碰撞的物体编号,添加破碎砖块的精灵
				Key = TRUE;//是否碰撞物体
				BOOL IsLeft = FALSE;//左上格子是否有障碍
				BOOL IsJump = FALSE;//是否可以继续跳
				BOOL IsLeftCollision = FALSE;//是否碰撞左上格子
				if ((*siSprite)->GetPosition().top % 30 == 0)
					x = (*siSprite)->GetPosition().top / 30 - 1;
				else
					x = (*siSprite)->GetPosition().top / 30;
				y = (*siSprite)->GetPosition().left / 30;

				if (g_iMapData[x][y + g_iX] == 0 || g_iMapData[x][y + g_iX] > 7 && g_iMapData[x][y + g_iX] < 19)//左上格子无障碍
					if ((*siSprite)->GetPosition().left % 30 != 0)//有右上边格子（列未对齐）
					{
						if (g_iMapData[x][y + g_iX + 1] == 0 || g_iMapData[x][y + g_iX + 1] > 7 && g_iMapData[x][y + g_iX + 1] < 19)//右上格子无障碍
							Key = FALSE;
					}
					else//无右上格子
						Key = FALSE;
				else
					IsLeft = TRUE;
				if (Key)//碰撞处理
				{
					if ((*siSprite)->GetPosition().left % 30 != 0)//未与列对齐，判断是否只是擦边
					{
						if ((*siSprite)->GetPosition().left % 30 < 11)
							if (IsLeft)//有左边,直接碰撞
								IsLeftCollision = TRUE;
							else //无左边，可以跳
							{
								if ((*siSprite)->GetVelocity().x <= 0 && g_iMapData[x][y + g_iX + 1] != 19)//不能是向右移动的状态,不能是隐藏图块
									IsJump = TRUE;
							}
						else if ((*siSprite)->GetPosition().left % 30 > 19)
						{
							if (IsLeft)//有左边
								if (g_iMapData[x][y + g_iX + 1] == 0 || g_iMapData[x][y + g_iX + 1] > 7 && g_iMapData[x][y + g_iX + 1] < 19)//无右边，可以跳
								{
									if ((*siSprite)->GetVelocity().x >= 0 && g_iMapData[x][y + g_iX] != 19)//不能是向左移动的状态,不能是隐藏图块
										IsJump = TRUE;
								}
						}
						else
							if (IsLeft)//有左边,碰撞左边
								IsLeftCollision = TRUE;
					}
					else//与列对齐  只有对应列碰撞
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
							case 1://产碎块
								CreatBrokenBrick((col - g_iX) * 30, x * 30);
								g_iMapData[x][col] = 0;
								break;
							case 2://产金币
								CreatGold((col - g_iX) * 30, x * 30);
								g_iMapData[x][col] = 3;
								break;
							case 20://产蘑菇
								CreatMushroom((col - g_iX) * 30, x * 30);
								g_iMapData[x][col] = 3;
								break;
							case 21://产小怪
								CreatSpirit((col - g_iX) * 30, x * 30);
								g_iMapData[x][col] = 3;
								break;
							case 22://产无敌星
								CreatStar((col - g_iX) * 30, x * 30);
								g_iMapData[x][col] = 3;
								break;
							case 19://产砖块
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
		//siSprite--;           删除首元素后 - 运算越界
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
		if (pTestSprite->TestCollision(*siSprite))//添加排除代码
			// Collision detected
			return SpriteCollision((*siSprite), pTestSprite);
	}

	// No collision
	return FALSE;
}