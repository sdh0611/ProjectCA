#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CGround.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\CTile.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"



bool CGround::PostInit(const OBJECT_DATA &data, CScene *pScene)
{
	UINT& iWidth = const_cast<UINT&>(data.m_iEntityWidth);
	UINT& iHeight = const_cast<UINT&>(data.m_iEntityHeight);

	iWidth *= TILE_WIDTH;
	iHeight *= TILE_HEIGHT;

	if (!CProb::PostInit(data, pScene))
		return false;

	GetTransform().lock()->SetPivotRatio(0.f, 0.f);

	//ImageRender 추가
	auto pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;
	pRender->SetDrawSize(TILE_WIDTH, TILE_HEIGHT);

	return LoadTileImage();
}

void CGround::Init()
{

}

void CGround::Update(double dDeltaTime)
{
	CObject::Update(dDeltaTime);
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
			//Tile culling 적용
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

	//GetComponent<ColliderBox>().lock()->DrawCollider(hDC);

}

void CGround::LateUpdate()
{
	CObject::LateUpdate();

	UINT cameraWidth = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraWidth();
	UINT cameraHeight = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraHeight();
	POSITION cameraPosition = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraPosition();
	POSITION position = GetObjectPosition();

	if (IsActive())
	{
		if (position.x + m_iEntityWidth <  cameraPosition.x - cameraWidth
			|| position.x > cameraPosition.x + 2 * cameraWidth)
		{
			puts("InActive");
			SetActive(false);
			return;
		}
		else if (position.y < cameraPosition.y - cameraHeight
			|| position.y + m_iEntityHeight > cameraPosition.y + 2 * cameraHeight)
		{
			puts("InActive");
			SetActive(false);
			//SetObjectState(Types::OS_DEAD);
			return;
		}
	}
	else
	{
		auto pPlayer = static_cast<CGameScene*>(m_pOwnerScene)->GetPlayerPtr().lock();
		POSITION playerPosition = pPlayer->GetObjectPosition();

		if (position.x + m_iEntityWidth > cameraPosition.x - cameraWidth
			&& position.x < cameraPosition.x + 2 * cameraWidth)
		{
			puts("Active");
			//Init();
			SetActive(true);
		}
	}

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
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundEdgeRightTop2")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundEdgeLeftTop2")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundLineRight2")));
	m_TileSpriteTable.push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundLineLeft2")));



	for (int row = 0; row < m_iEntityHeight / TILE_HEIGHT; ++row)
	{
		m_TileInfoList.emplace_back();
		for (int tileIndex = 0; tileIndex < m_iEntityWidth / TILE_WIDTH; ++tileIndex)
		{
			if (row == 0)
			{
				if (m_ObjectType == Types::OT_PROB)
				{
					if (tileIndex == 0)
					{
						m_TileInfoList[row].push_back(GI_EDGE_LEFT_TOP);
					}
					else if (tileIndex == m_iEntityWidth / TILE_WIDTH - 1)
					{
						m_TileInfoList[row].push_back(GI_EDGE_RIGHT_TOP);
					}
					else
					{
						m_TileInfoList[row].push_back(GI_LINE_TOP);
					}
				}
				else if (m_ObjectType == Types::OT_GROUND)
				{
					if (tileIndex == 0)
					{
						m_TileInfoList[row].push_back(GI_EDGE_LEFT_TOP2);
					}
					else if (tileIndex == m_iEntityWidth / TILE_WIDTH - 1)
					{
						m_TileInfoList[row].push_back(GI_EDGE_RIGHT_TOP2);
					}
					else
					{
						m_TileInfoList[row].push_back(GI_LINE_TOP);
					}
				}
			}
			else if (row == m_iEntityHeight / TILE_HEIGHT - 1)
			{
				if (m_ObjectType == Types::OT_PROB)
				{
					if (tileIndex == 0)
					{
						m_TileInfoList[row].push_back(GI_EDGE_LEFT_BOT);
					}
					else if (tileIndex == m_iEntityWidth / TILE_WIDTH - 1)
					{
						m_TileInfoList[row].push_back(GI_EDGE_RIGHT_BOT);
					}
					else
					{
						m_TileInfoList[row].push_back(GI_LINE_BOT);
					}
				}
				else if (m_ObjectType == Types::OT_GROUND)
				{
					if (tileIndex == 0)
					{
						m_TileInfoList[row].push_back(GI_LINE_LEFT2);
					}
					else if (tileIndex == m_iEntityWidth / TILE_WIDTH - 1)
					{
						m_TileInfoList[row].push_back(GI_LINE_RIGHT2);
					}
					else
					{
						m_TileInfoList[row].push_back(GI_INNER);
					}
				}
			}
			else
			{
				if (m_ObjectType == Types::OT_PROB)
				{
					if (tileIndex == 0)
					{
						m_TileInfoList[row].push_back(GI_LINE_LEFT);
					}
					else if (tileIndex == m_iEntityWidth / TILE_WIDTH - 1)
					{
						m_TileInfoList[row].push_back(GI_LINE_RIGHT);
					}
					else
					{
						m_TileInfoList[row].push_back(GI_INNER);
					}
				}
				else if (m_ObjectType == Types::OT_GROUND)
				{
					if (tileIndex == 0)
					{
						m_TileInfoList[row].push_back(GI_LINE_LEFT2);
					}
					else if (tileIndex == m_iEntityWidth / TILE_WIDTH - 1)
					{
						m_TileInfoList[row].push_back(GI_LINE_RIGHT2);
					}
					else
					{
						m_TileInfoList[row].push_back(GI_INNER);
					}
				}				
			}
		}
	}

	return true;
}
