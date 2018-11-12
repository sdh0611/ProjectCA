#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CGround.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\CTile.h"
#include "..\..\..\Include\Core\CResourceManager.h"


bool CGround::PostInit(const Types::ActorData &data, CGameScene *pScene)
{
	if (!CProb::PostInit(data, pScene))
		return false;

	return LoadTileImage();
}

bool CGround::Init()
{
	return true;
}

void CGround::Update(double dDeltaTime)
{
	CActor::Update(dDeltaTime);
}

void CGround::Render(const HDC & hDC)
{
	POSITION outputPosition = GetComponent<TransformComponent>()->GetScreenPivot();
	float originPositionX = outputPosition.x;

	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)GetCurrentObject(hDC, OBJ_BITMAP);

	for (const auto& row : m_TileInfoList)
	{
		for (const auto& tile : row)
		{
			SelectObject(memDC, tile.lock()->GetBitmap());
			TransparentBlt(hDC, outputPosition.x, outputPosition.y,
				TILE_WIDTH, TILE_HEIGHT, memDC, 0, 0, 16, 16, RGB(248, 7, 220));

			outputPosition.x += TILE_WIDTH;
		}
		outputPosition.y += TILE_HEIGHT;
		outputPosition.x = originPositionX;
	}

	SelectObject(memDC, hOldBit);
	DeleteDC(memDC);

}

void CGround::ActorBehavior(double dDeltaTime)
{
}

bool CGround::LoadTileImage()
{
	CResourceManager* pResourceMgr = CResourceManager::GetInstance();

	for (int row = 0; row < m_iActorHeight / TILE_HEIGHT; ++row)
	{
		m_TileInfoList.emplace_back();
		for (int tileIndex = 0; tileIndex < m_iActorWidth / TILE_WIDTH; ++tileIndex)
		{
			m_TileInfoList[row].push_back(pResourceMgr->GetWeakSprtiePtr(TEXT("GroundInner")));
		}

	}

	return true;
}
