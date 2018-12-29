#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\CObject.h"
#include "..\..\..\Include\Scene\Actor\CRandomBlock.h"
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



CRandomBlock::CRandomBlock()
{
}

CRandomBlock::~CRandomBlock()
{
}

bool CRandomBlock::PostInit(const OBJECT_DATA & objectData, CScene * pScene)
{
	if (!CProb::PostInit(objectData, pScene))
		return false;
	GetTransform().lock()->SetPivotRatio(0.5f, 1.f);

	m_bHiding	= false;
	m_Type		= BT_DEFAULT;


	//auto colliderCallback = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {
	//	
	//	if (m_ObjectState != Types::OS_DEAD)
	//	{
	//		switch (pOther->GetObjectType())
	//		{
	//		case Types::OT_PLAYER:
	//		{
	//			auto pPlayer = static_cast<CPlayer*>(pOther);
	//			if (type == Collider::COLLISION_BOT)
	//			{
	//				if (m_bHiding)//Hiding Block일 때
	//				{
	//					if (pPlayer->GetComponent<PhysicsComponent>().lock()->GetCurJumpForce() >= 0.f)
	//					{
	//						HandlingEvent(Types::EVENT_DEAD);
	//						m_bHiding = false;
	//					}
	//				}
	//				else
	//				{
	//					HandlingEvent(Types::EVENT_DEAD);
	//				}
	//			}
	//		}
	//			break;
	//		case Types::OT_PICKABLE:
	//			if (!m_bHiding)//Hiding상태가 아닐 경우에만 반응
	//			{
	//				if (static_cast<CActor*>(pOther)->GetActorAct() == Types::ACT_ATTACK)
	//				{
	//					if (type != Collider::COLLISION_TOP)
	//					{
	//						HandlingEvent(Types::EVENT_DEAD);
	//					}
	//				}
	//			}
	//			break;

	//		}

	//	}
	//};

	//GetComponent<ColliderBox>().lock()->SetOnCollision(colliderCallback);

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

	//m_DrawPivot				= pRender->GetDrawPivot();

	//m_fYSpeed					= 0.f;
	//m_fBumpForce				= 200.f;
	//m_dBumpTimeElapsed	= 0.f;
	//m_dBumpTimeLimit		= 0.1f;

	return true;
}

void CRandomBlock::Init()
{
	CObject::Init();
	if (m_Type == BT_HIDE)
	{
		m_bHiding = true;
	}
	GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Idle"));
	m_bActive					= true;
	//m_dBumpTimeElapsed	= 0.f;
	m_ObjectState				= Types::OS_IDLE;
	
}

void CRandomBlock::Render(const HDC & hDC)
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

void CRandomBlock::LateUpdate()
{
	if (m_bActive)
	{
		CObject::LateUpdate();
	}

}

//void CRandomBlock::SetStoredPickup(std::shared_ptr<CPickup> pPickup)
//{
//	m_pPickup = pPickup;
//	m_pPickup.lock()->SetActive(false);
//}

void CRandomBlock::SetHide()
{
	m_bHiding	= true;
	m_Type		= BT_HIDE;

}

bool CRandomBlock::IsHiding() const
{
	return m_bHiding;
}

void CRandomBlock::Bump(double dDeltaTime)
{

}

//void CRandomBlock::SetDead()
//{
//	CScoreManager::GetInstance()->IncreaseScore(10);
//	GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Hit"));
//	if (m_pPickup.lock()->GetPickupType() != CPickup::PickupType::PT_COIN)
//	{
//		m_pPickup.lock()->SetActive(true);
//	}
//	else
//	{
//
//	}
//	m_ObjectState = Types::OS_DEAD;
//
//
//}

//void CRandomBlock::HandlingEvent(EVENT_TYPE type)
//{
//	switch (type)
//	{
//	case Types::EVENT_DEAD:
//		CScoreManager::GetInstance()->IncreaseScore(10);
//		GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Hit"));
//		if (m_pPickup.lock()->GetPickupType() != CPickup::PickupType::PT_COIN)
//		{
//			m_pPickup.lock()->SetActive(true);
//		}
//		else
//		{
//
//		}
//		m_ObjectState = Types::OS_DEAD;
//		break;
//
//	}
//}
