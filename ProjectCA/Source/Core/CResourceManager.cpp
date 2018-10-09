#include "..\..\stdafx.h"
#include "..\..\Include\Core\CResourceManager.h"
#include "..\..\Include\Core\Graphic\CSprite.h"


CResourceManager::CResourceManager() {

}

CResourceManager::~CResourceManager() {

	m_strongSpriteTable.clear();

}

bool CResourceManager::Init() {

	//if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_idle_left.bmp"), TEXT("PlayerIdleLeft")))
	//	return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Background/background_mountain1.bmp"), TEXT("BackgroundMountain")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Background/background_mountain2.bmp"), TEXT("BackgroundMountain2")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_idle_right.bmp"), TEXT("PlayerIdleRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_idle_left.bmp"), TEXT("PlayerIdleLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_walk_right.bmp"), TEXT("PlayerWalkRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_walk_left.bmp"), TEXT("PlayerWalkLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_lookup_right.bmp"), TEXT("PlayerLookupRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_lookup_left.bmp"), TEXT("PlayerLookupLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_sitdown_right.bmp"), TEXT("PlayerSitdownRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_sitdown_left.bmp"), TEXT("PlayerSitdownLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_run_right.bmp"), TEXT("PlayerRunRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_run_left.bmp"), TEXT("PlayerRunLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_jump_right.bmp"), TEXT("PlayerJumpRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_jump_left.bmp"), TEXT("PlayerJumpLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_falldown_right.bmp"), TEXT("PlayerFalldownRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_falldown_left.bmp"), TEXT("PlayerFalldownLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_run_jump_right.bmp"), TEXT("PlayerRunJumpRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_run_jump_left.bmp"), TEXT("PlayerRunJumpLeft")))
		return false;

	return true;
}


WeakSpritePtr CResourceManager::GetWeakSprtiePtr(const Types::tstring & strName)
{
	//없는 경우 nullptr 반환
	if(m_strongSpriteTable.find(strName) == m_strongSpriteTable.end())
		return WeakSpritePtr();

	//있는 경우 해당 Sprite의 포인터 반환
	return m_strongSpriteTable[strName];
}

bool CResourceManager::AddSpritePtr(const Types::tstring & strName, CSprite * pSprite)
{
	if (pSprite == nullptr)
		return false;

	//해당 키값에 대응하는 sprite가 이미 존재하는 경우
	if(m_strongSpriteTable.find(strName) != m_strongSpriteTable.end())
		return false;

	StrongSpritePtr pStrongSprite = std::shared_ptr<CSprite>(pSprite);
	m_strongSpriteTable.insert(std::make_pair(strName, pStrongSprite));
	
	return true;
}

bool CResourceManager::DeleteSpritePtr(const Types::tstring & strName)
{
	//이미 테이블에 존재하지 않는 경우
	if (m_strongSpriteTable.find(strName) == m_strongSpriteTable.end())
		return false;

	m_strongSpriteTable.erase(strName);

	return true;
}

bool CResourceManager::AddSpriteUsePath(const Types::tstring & strPath, const Types::tstring & strName)
{
	//이미 존재하는 경우
	if (m_strongSpriteTable.find(strName) != m_strongSpriteTable.end())
		return false;

	StrongSpritePtr pSprite = StrongSpritePtr(new CSprite);

	if(!pSprite->Init(strPath))
		return false;
	
	m_strongSpriteTable.insert(std::make_pair(strName, pSprite));

	return true;

}
