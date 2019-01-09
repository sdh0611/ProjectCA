#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Graphic\CCoinParticle.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Graphic\CBlockParticle.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.hpp"
#include "..\..\..\Include\Scene\CScene.h"
#include "..\..\..\Include\Core\CResourceManager.h"



CBlockParticle::CBlockParticle()
{
}

CBlockParticle::~CBlockParticle()
{
}

bool CBlockParticle::PostInit(const ENTITY_DATA & data, CScene * pScene)
{
	if (!CParticle::PostInit(data, pScene))
	{
		return false;
	}

	if (!GenerateBlockPeace())
	{
		return false;
	}
	m_dPlayTime = 3.f;

	return true;
}

void CBlockParticle::Init()
{
	CParticle::Init();
	std::shared_ptr<PhysicsComponent> pPhysics;
	for (const auto& peace : m_BlockPeaceList)
	{
		peace->Init();
		pPhysics = peace->GetComponent<PhysicsComponent>().lock();
		pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
	}

}

void CBlockParticle::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		m_dTimeElapsed += dDeltaTime;
		ParticleUpdate(dDeltaTime);
		if (IsParticleEnd())
		{
			SetActive(false);
			for (const auto& peace : m_BlockPeaceList)
			{
				peace->SetActive(false);
			}
		}
	}
}

void CBlockParticle::Render(const HDC & hDC)
{
	for (const auto& peace : m_BlockPeaceList)
	{
		peace->GetComponent<ImageRender>().lock()->Draw(hDC);
	}
}

void CBlockParticle::LateUpdate()
{
	for (const auto& peace : m_BlockPeaceList)
	{
		peace->LateUpdate();
	}

}

bool CBlockParticle::GenerateBlockPeace()
{
	auto pObjMgr = CObjectManager::GetInstance();
	POSITION ownerPosition = GetEntityPosition();
	UINT iOwnerWidth = GetEntityWidth();
	UINT iOwnerHeight = GetEntityHeight();

	//Block Peace 4°³ »ý¼º
	// ---------
	//	| 1 | 2 |
	// ---------
	//	| 3 | 4 |
	// ---------
	{
		auto pResourceMgr = CResourceManager::GetInstance();

		//1
		auto pPeace = pObjMgr->CreateEntity<CEntity>(iOwnerWidth * 0.5f, iOwnerHeight * 0.5f, ownerPosition.x - iOwnerWidth * 0.25f, ownerPosition.y - iOwnerHeight * 0.5f, TEXT("BlockPeace1"), m_pOwnerScene);
		{
			auto pPhysics = std::make_shared<PhysicsComponent>();
			if (!pPhysics->PostInit(pPeace.get(), -200.f, 200.f, 1300.f, 400.f))
			{
				return false;
			}
			pPhysics->SetCurJumpForce(400.f);
			if (!pPeace->AddComponent(pPhysics, pPhysics->GetComponentTag()))
			{
				return false;
			}
			
			auto pRender = std::make_shared<ImageRender>();
			if (!pRender->PostInit(pPeace.get()))
			{
				return false;
			}

			if (!pRender->SetSprite(pResourceMgr->GetWeakSprtiePtr(TEXT("EffectBrokenBlockPeace")).lock()))
			{
				return false;
			}
			pRender->SetExpansionRatio(2.5f);
			pRender->SetPivotRatio(0.5f, 1.f);
			if (!pPeace->AddComponent(pRender, pRender->GetComponentTag()))
			{
				return false;
			}
			
		}
		m_BlockPeaceList.push_back(pPeace);

		//2
		pPeace = pObjMgr->CreateEntity<CEntity>(iOwnerWidth * 0.5f, iOwnerHeight * 0.5f, ownerPosition.x + iOwnerWidth * 0.25f, ownerPosition.y - iOwnerHeight * 0.5f, TEXT("BlockPeace2"), m_pOwnerScene);
		{
			auto pPhysics = std::make_shared<PhysicsComponent>();
			if (!pPhysics->PostInit(pPeace.get(), 200.f, 200.f, 1300.f, 400.f))
			{
				return false;
			}
			pPhysics->SetCurJumpForce(400.f);
			if (!pPeace->AddComponent(pPhysics, pPhysics->GetComponentTag()))
			{
				return false;
			}

			auto pRender = std::make_shared<ImageRender>();
			if (!pRender->PostInit(pPeace.get()))
			{
				return false;
			}

			if (!pRender->SetSprite(pResourceMgr->GetWeakSprtiePtr(TEXT("EffectBrokenBlockPeace")).lock()))
			{
				return false;
			}
			pRender->SetExpansionRatio(2.5f);
			pRender->SetPivotRatio(0.5f, 1.f);
			if (!pPeace->AddComponent(pRender, pRender->GetComponentTag()))
			{
				return false;
			}

		}
		m_BlockPeaceList.push_back(pPeace);

		//3
		pPeace = pObjMgr->CreateEntity<CEntity>(iOwnerWidth * 0.5f, iOwnerHeight * 0.5f, ownerPosition.x - iOwnerWidth * 0.25f, ownerPosition.y, TEXT("BlockPeace1"), m_pOwnerScene);
		{
			auto pPhysics = std::make_shared<PhysicsComponent>();
			if (!pPhysics->PostInit(pPeace.get(), -100.f, 100.f, 1100.f, 200.f))
			{
				return false;
			}
			pPhysics->SetCurJumpForce(200.f);
			if (!pPeace->AddComponent(pPhysics, pPhysics->GetComponentTag()))
			{
				return false;
			}

			auto pRender = std::make_shared<ImageRender>();
			if (!pRender->PostInit(pPeace.get()))
			{
				return false;
			}

			if (!pRender->SetSprite(pResourceMgr->GetWeakSprtiePtr(TEXT("EffectBrokenBlockPeace")).lock()))
			{
				return false;
			}
			pRender->SetExpansionRatio(2.5f);
			pRender->SetPivotRatio(0.5f, 1.f);
			if (!pPeace->AddComponent(pRender, pRender->GetComponentTag()))
			{
				return false;
			}

		}
		m_BlockPeaceList.push_back(pPeace);

		//4
		pPeace = pObjMgr->CreateEntity<CEntity>(iOwnerWidth * 0.5f, iOwnerHeight * 0.5f, ownerPosition.x + iOwnerWidth * 0.25f, ownerPosition.y, TEXT("BlockPeace2"), m_pOwnerScene);
		{
			auto pPhysics = std::make_shared<PhysicsComponent>();
			if (!pPhysics->PostInit(pPeace.get(), 100.f, 100.f, 1100.f, 200.f))
			{
				return false;
			}
			pPhysics->SetCurJumpForce(200.f);
			if (!pPeace->AddComponent(pPhysics, pPhysics->GetComponentTag()))
			{
				return false;
			}

			auto pRender = std::make_shared<ImageRender>();
			if (!pRender->PostInit(pPeace.get()))
			{
				return false;
			}

			if (!pRender->SetSprite(pResourceMgr->GetWeakSprtiePtr(TEXT("EffectBrokenBlockPeace")).lock()))
			{
				return false;
			}
			pRender->SetExpansionRatio(2.5f);
			pRender->SetPivotRatio(0.5f, 1.f);
			if (!pPeace->AddComponent(pRender, pRender->GetComponentTag()))
			{
				return false;
			}

		}
		m_BlockPeaceList.push_back(pPeace);

	}

	return true;
}

void CBlockParticle::ParticleUpdate(double dDeltaTime)
{
	std::shared_ptr<PhysicsComponent> pPhysics;
	for (const auto& peace : m_BlockPeaceList)
	{
		peace->Update(dDeltaTime);
		pPhysics = peace->GetComponent<PhysicsComponent>().lock();
		peace->GetComponent<TransformComponent>().lock()->Move(dDeltaTime * pPhysics->GetSpeed(), dDeltaTime * pPhysics->GetCurJumpForce());
	}

	

}
