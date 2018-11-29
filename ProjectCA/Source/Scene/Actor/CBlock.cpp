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
					if (m_bHiding)//Hiding BlockÀÏ ¶§
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
				if (!m_bHiding)
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

	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("RandomBlockAnimation"), m_iObjectWidth, m_iObjectHeight, true, TEXT("Idle")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("Default"), TEXT("RandomBlockDead"), m_iObjectWidth, m_iObjectHeight, false, TEXT("Dead")))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("Default"), TEXT("RandomBlockHit"), m_iObjectWidth, m_iObjectHeight, false, TEXT("Hit")))
		return false;

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
		if (m_bHiding)
		{
			//auto playerTop = static_cast<CGameScene*>(m_pOwnerScene)->GetPlayerPtr().lock()->GetComponent<ColliderBox>().lock()->GetRect().top;
			//if (playerTop <= GetObjectPosition().y + 10.f)
			//{
			//	GetComponent<ColliderBox>().lock()->SetActive(true);
			//}

		}

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
	//GetComponent<ColliderBox>().lock()->SetActive(false);

}

bool CBlock::IsHiding() const
{
	return m_bHiding;
}

void CBlock::SetDead()
{
	CScoreManager::GetInstance()->IncreaseScore(10);
	GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Hit"));
	m_pPickup.lock()->SetActive(true);
	m_ObjectState = Types::OS_DEAD;


}
