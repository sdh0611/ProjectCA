#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPickupBlock.h"
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
#include "..\..\..\Include\Core\Sound\CSoundManager.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.hpp"




CPickupBlock::CPickupBlock()
{
}

CPickupBlock::~CPickupBlock()
{
}

bool CPickupBlock::PostInit(const OBJECT_DATA & objectData, CScene * pScene)
{
	if (!CRandomBlock::PostInit(objectData, pScene))
		return false;
	
	auto onCollision= [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {

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
							HandlingEvent(Types::EVENT_DEAD);
							m_bHiding = false;
						}
					}
					else
					{
						HandlingEvent(Types::EVENT_DEAD);
					}
				}
			}
			break;
			case Types::OT_PICKABLE:
				if (!m_bHiding)//Hiding상태가 아닐 경우에만 반응
				{
					if (static_cast<CActor*>(pOther)->GetActorAct() == Types::ACT_ATTACK)
					{
						if (type != Collider::COLLISION_TOP)
						{
							HandlingEvent(Types::EVENT_DEAD);
						}
					}
				}
				break;

			}

		}
	};
	GetComponent<ColliderBox>().lock()->SetOnCollision(onCollision);

	return true;
}

void CPickupBlock::Init()
{
	CRandomBlock::Init();
	if (!m_pPickup.expired())
	{
		m_pPickup.lock()->SetActive(false);
	}
	m_ObjectState = Types::OS_IDLE;

}

void CPickupBlock::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CObject::Update(dDeltaTime);
	}
}



void CPickupBlock::SetStoredPickup(std::shared_ptr<CPickup> pPickup)
{
	m_pPickup = pPickup;
	m_pPickup.lock()->SetActive(false);
}

void CPickupBlock::HandlingEvent(EVENT_TYPE type)
{
	switch (type)
	{
	case Types::EVENT_DEAD:
		CScoreManager::GetInstance()->IncreaseScore(10);
		GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Hit"));
		if (m_pPickup.lock()->GetPickupType() != CPickup::PickupType::PT_COIN)
		{
			m_pPickup.lock()->SetActive(true);
			CSoundManager::GetInstance()->SoundPlay(TEXT("SFXPowerUpAppear"));
		}
		else
		{

		}
		m_ObjectState = Types::OS_DEAD;
		break;

	}
}
