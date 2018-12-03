#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\CObject.h"
#include "..\..\..\Include\Scene\Actor\CBlock.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Scene\Actor\CMushroom.h"
#include "..\..\..\Include\Scene\Actor\CFlower.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScoreManager.h"
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

	m_bHiding	= false;
	m_Type		= BT_DEFAULT;

	//Collider
	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto colliderCallback = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {
		
		if (m_ObjectState != Types::OS_DEAD)
		{
			switch (pOther->GetObjectType())
			{
			case Types::OT_PLAYER:
			{
				auto pPlayer = static_cast<CPlayer*>(pOther);
				if (type == Collider::COLLISION_BOT)
				{
					if (m_bHiding)//Hiding Block일 때
					{
						if (pPlayer->GetComponent<PhysicsComponent>().lock()->GetCurJumpForce() >= 0.f)
						{
							SetDead();
							m_bHiding = false;
						}
					}
					else
					{
						SetDead();
					}
				}
			}
				break;
			case Types::OT_ENEMY:
				if (!m_bHiding)//Hiding상태가 아닐 경우에만 반응
				{
					if (pOther->GetObjectState() == Types::OS_DAMAGED && !pOther->IsSubordinate())
					{
						if (type != Collider::COLLISION_TOP)
						{
							SetDead();
						}
					}
				}
				break;

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
	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("RandomBlockAnimation"),   true, TEXT("Idle")))
		return false;
	if (!pRender->AddAnimation(0.f, TEXT("Default"), TEXT("RandomBlockDead"),   false, TEXT("Dead")))
		return false;
	if (!pRender->AddAnimation(0.1f, TEXT("Default"), TEXT("RandomBlockHit"),   false, TEXT("Hit")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;


	return true;
}

void CBlock::Init()
{
	CObject::Init();
	if (!m_pPickup.expired())
	{
		m_pPickup.lock()->SetActive(false);
	}
	m_ObjectState = Types::OS_IDLE;
	if (m_Type == BT_HIDE)
	{
		m_bHiding = true;
	}
	GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Idle"));
	m_bActive = true;

}

void CBlock::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CObject::Update(dDeltaTime);
	}

}

void CBlock::Render(const HDC & hDC)
{
	if (!m_bHiding)
	{
		if (m_bActive)
		{
			auto pRender = GetComponent<AnimationRender>().lock();
			if (pRender->IsActive())
			{
				pRender->Draw(hDC);
			}

		}
	}
}

void CBlock::LateUpdate()
{
	if (m_bActive)
	{
		CObject::LateUpdate();
	}

}

void CBlock::SetStoredPickup(std::shared_ptr<CPickup> pPickup)
{
	m_pPickup = pPickup;
	m_pPickup.lock()->SetActive(false);
}

void CBlock::SetHide()
{
	m_bHiding = true;
	m_Type = BT_HIDE;

}

bool CBlock::IsHiding() const
{
	return m_bHiding;
}

void CBlock::SetDead()
{
	CScoreManager::GetInstance()->IncreaseScore(10);
	GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Hit"));
	if (m_pPickup.lock()->GetPickupType() != CPickup::PickupType::PT_COIN)
	{
		m_pPickup.lock()->SetActive(true);
	}
	else
	{

	}
	m_ObjectState = Types::OS_DEAD;


}
