#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CGround.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\CTile.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"

bool CGround::PostInit(const Types::ActorData &data, CGameScene *pScene)
{
	if (!CProb::PostInit(data, pScene))
		return false;

	//auto onCollision = [](CObject* pOther, Collider::CollisionType type)->void {



	//	switch (type)
	//	{

	//	}

	//};

	//GetComponent<ColliderBox>().lock()->SetOnCollision(onCollision);
	GetComponent<ImageRender>().lock()->SetDrawSize(TILE_WIDTH, TILE_HEIGHT);

	return LoadTileImage();
}

void CGround::Init()
{

}

void CGround::Update(double dDeltaTime)
{
	CActor::Update(dDeltaTime);
}

void CGround::Render(const HDC & hDC)
{
	std::shared_ptr<ImageRender> pRender = GetComponent<ImageRender>().lock();
	POSITION outputPosition = GetComponent<TransformComponent>().lock()->GetScreenPivot();
	UINT iCameraWidth = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraWidth();
	float fOriginPositionX = outputPosition.x;

	for (const auto& row : m_TileInfoList)
	{
		for (const auto& tile : row)
		{
			//Tile culling Àû¿ë
			if (outputPosition.x + TILE_WIDTH >= 0.f 
				&& outputPosition.x - TILE_WIDTH <= iCameraWidth)
			{
				pRender->Draw(hDC, outputPosition, m_TileSpriteTable[tile]);
			}
			outputPosition.x += TILE_WIDTH;
		}
		outputPosition.y += TILE_HEIGHT;
		outputPosition.x = fOriginPositionX;
	}

}

void CGround::ActorBehavior(double dDeltaTime)
{
}

bool CGround::LoadTileImage()
{
	CResourceManager* pResourceMgr = CResourceManager::GetInstance();

	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundInner")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundEdgeRightTop")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundEdgeRightBot")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundEdgeRightInner")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundEdgeLeftTop")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundEdgeLeftBot")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundEdgeLeftInner")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundLineTop")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundLineLeft")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundLineBot")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundLineRight")));


	for (int row = 0; row < m_iObjectHeight / TILE_HEIGHT; ++row)
	{
		m_TileInfoList.emplace_back();
		for (int tileIndex = 0; tileIndex < m_iObjectWidth / TILE_WIDTH; ++tileIndex)
		{
			if (row == 0)
			{
				if (tileIndex == 0)
				{
					m_TileInfoList[row].push_back(GI_EDGE_LEFT_TOP);
				}
				else if (tileIndex == m_iObjectWidth / TILE_WIDTH - 1)
				{
					m_TileInfoList[row].push_back(GI_EDGE_RIGHT_TOP);
				}
				else
				{
					m_TileInfoList[row].push_back(GI_LINE_TOP);
				}
			}
			else if (row == m_iObjectHeight / TILE_HEIGHT - 1)
			{
				if (tileIndex == 0)
				{
					m_TileInfoList[row].push_back(GI_EDGE_LEFT_BOT);
				}
				else if (tileIndex == m_iObjectWidth / TILE_WIDTH - 1)
				{
					m_TileInfoList[row].push_back(GI_EDGE_RIGHT_BOT);
				}
				else
				{
					m_TileInfoList[row].push_back(GI_LINE_BOT);
				}
			}
			else
			{
				if (tileIndex == 0)
				{
					m_TileInfoList[row].push_back(GI_LINE_LEFT);
				}
				else if (tileIndex == m_iObjectWidth / TILE_WIDTH - 1)
				{
					m_TileInfoList[row].push_back(GI_LINE_RIGHT);
				}
				else
				{
					m_TileInfoList[row].push_back(GI_INNER);
				}

			}

		}

	}

	return true;
}
