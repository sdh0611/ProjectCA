#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CItemInfo.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"



CItemInfo::CItemInfo()
{
}

CItemInfo::~CItemInfo()
{
}

bool CItemInfo::PostInit(const ENTITY_DATA & data, CScene * pScene)
{
	if (!CInterface::PostInit(data, pScene))
		return false;
	

	auto pResourceMgr = CResourceManager::GetInstance();

	m_InfoImageList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UIItemInfoEmpty")));
	m_InfoImageList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UIItemInfoMushroom")));
	m_InfoImageList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UIItemInfoFlower")));
	
	GetComponent<ImageRender>().lock()->SetSprite(TEXT("UIItemInfoEmpty"));
	
	//Player 포인터 지정
	m_pPlayer = static_cast<CGameScene*>(pScene)->GetPlayerPtr();
	if (m_pPlayer.expired())
		return false;
	
	return true;
}

void CItemInfo::Update(double dDeltaTime)
{
	CheckStoredPickup(); 
	CInterface::Update(dDeltaTime);
}

void CItemInfo::CheckStoredPickup()
{
	auto pRender = GetComponent<ImageRender>().lock();
	if (m_pPlayer.lock()->GetStoredPickup().expired())
	{
		pRender->SetSprite(m_InfoImageList[PI_EMPTY].lock());
	}
	else
	{
		auto strName = m_pPlayer.lock()->GetStoredPickup().lock()->GetObjectName();
		if (strName == TEXT("Mushroom"))
		{
			pRender->SetSprite(m_InfoImageList[PI_MUSHROOM].lock());

		}
		else if (strName == TEXT("Flower"))
		{
			pRender->SetSprite(m_InfoImageList[PI_FLOWER].lock());

		}
	}

}
