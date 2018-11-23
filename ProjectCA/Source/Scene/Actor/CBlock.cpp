#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\CObject.h"
#include "..\..\..\Include\Scene\Actor\CBlock.h"
#include "..\..\..\Include\Scene\Actor\CMushroom.h"
#include "..\..\..\Include\Scene\Actor\CFlower.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CLayer.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.h"



CBlock::CBlock()
{
}

CBlock::~CBlock()
{
}

bool CBlock::PostInit(const OBJECT_DATA & objectData, CScene * pScene)
{
	if (!CObject::PostInit(objectData, pScene))
		return false;
	GetTransform().lock()->SetPivotRatio(0.5f, 1.f);


	//Collider
	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto colliderCallback = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {
		
		if (m_ObjectState != Types::OS_DEAD)
		{
			if (pOther->GetObjectType() == Types::OT_PLAYER)
			{
				if (type == Collider::COLLISION_BOT)
				{
					GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Hit"));
					m_pPickup->SetActive(true);
					m_ObjectState = Types::OS_DEAD;
				}
			}
		}
	};

	pCollider->SetOnCollision(colliderCallback);
	pCollider->SetSize(m_iObjectWidth * 0.5f, m_iObjectHeight * 0.5f);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//RenderComponent
	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;

	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("RandomBlockAnimation"), m_iObjectWidth, m_iObjectHeight, true, TEXT("Idle")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("Default"), TEXT("RandomBlockDead"), m_iObjectWidth, m_iObjectHeight, false, TEXT("Dead")))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("Default"), TEXT("RandomBlockHit"), m_iObjectWidth, m_iObjectHeight, false, TEXT("Hit")))
		return false;

	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	//m_pPickup = CObjectManager::GetInstance()->CreateActor<CMushroom>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5,
	//	GetObjectPosition().x, GetObjectPosition().y - m_iObjectHeight / 2.f, Types::OT_PICKUP, Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN,
	//	Types::DIR_RIGHT, TEXT("Flower"), static_cast<CGameScene*>(m_pOwnerScene));
	//if (m_pPickup == nullptr)
	//	return false;
	//m_pPickup->SetActive(false);
	//m_pOwnerScene->FindLayer(TEXT("Pickup"))->AddActor(m_pPickup);
	//m_pOwnerScene->AddObjectToScene(m_pPickup);

	return true;
}

void CBlock::Init()
{
	CObject::Init();
	//m_pPickup->Init();
	if (m_pPickup != nullptr)
	{
		m_pPickup->SetActive(false);
	}
	m_ObjectState = Types::OS_IDLE;
	GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Idle"));
	m_bActive = true;

}

void CBlock::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CObject::Update(dDeltaTime);
		//if (m_pPickup->IsActive())
		//{
		//	m_pPickup->Update(dDeltaTime);
		//}

	}

}

void CBlock::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		auto pRender = GetComponent<AnimationRender>().lock();
		if (pRender->IsActive())
		{
			pRender->Draw(hDC);
		}

		//if (m_pPickup->IsActive())
		//{
		//	m_pPickup->Render(hDC);
		//}

	}

}

void CBlock::LateUpdate()
{
	if (m_bActive)
	{
		CObject::LateUpdate();
		//if (m_pPickup->IsActive())
		//{
		//	m_pPickup->LateUpdate();
		//}
	}

}

void CBlock::SetStoredPickup(std::shared_ptr<CPickup> pPickup)
{
	m_pPickup = pPickup;
	m_pPickup->SetActive(false);
}
