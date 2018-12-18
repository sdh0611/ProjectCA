#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CCoinBlock.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Scene\Actor\CMushroom.h"
#include "..\..\..\Include\Scene\Actor\CFlower.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\..\Include\Scene\CObject.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScoreManager.h"
#include "..\..\..\Include\Scene\CLayer.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"



CCoinBlock::CCoinBlock()
{
}

CCoinBlock::~CCoinBlock()
{
}

bool CCoinBlock::PostInit(const OBJECT_DATA & objectData, CScene * pScene)
{
	if (!CRandomBlock::PostInit(objectData, pScene))
		return false;

	m_dTimeElapsed	= 0.f;
	m_dLimitTime		= 0.f;
	

	//auto pPhysics = std::make_shared<PhysicsComponent>();
	//if (!pPhysics->PostInit(this, 0.f, 0.f, 1300.f, 200.f))
	//	return false;
	//if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
	//	return false;


	auto onCollision = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {

		auto pPhysics = GetComponent<PhysicsComponent>().lock();
		
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
							HandlingEvent(Types::EVENT_DAMAGED);
							//pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
							m_bHiding = false;
						}
					}
					else
					{
						HandlingEvent(Types::EVENT_DAMAGED);
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
							HandlingEvent(Types::EVENT_DAMAGED);
						}
					}
				}
				break;
			}
		}
	};
	GetComponent<ColliderBox>().lock()->SetOnCollision(onCollision);

	m_DrawPosition = GetComponent<AnimationRender>().lock()->GetDrawPivot();

	return true;
}

void CCoinBlock::Init()
{
	CRandomBlock::Init();
	m_dTimeElapsed = 0.f;
}

void CCoinBlock::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CObject::Update(dDeltaTime);
		Behavior(dDeltaTime);
	}
}

void CCoinBlock::Render(const HDC & hDC)
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


void CCoinBlock::SetLimitTime(double dTime)
{
	if (dTime < 0.f)
		return;
	m_dLimitTime = dTime;
}

void CCoinBlock::Behavior(double dDeltaTime)
{
	switch (m_ObjectState)
	{
	case Types::OS_DAMAGED:
		if (m_dTimeElapsed >= m_dLimitTime)
		{
			GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Hit"));
			HandlingEvent(Types::EVENT_DEAD);
			m_dTimeElapsed = 0.f;
		}
		else
		{
			m_dTimeElapsed += dDeltaTime;
		}
		break;
	//default:

	//	break;		
	}

	//{
	//	auto pRender = GetComponent<AnimationRender>().lock();
	//	auto pPhysics = GetComponent<PhysicsComponent>().lock();
	//	if (pRender->GetDrawPivot().y != m_DrawPosition.y)
	//	{
	//		m_DrawPosition.y -= pPhysics->GetCurJumpForce();
	//	}

	//}
}

void CCoinBlock::HandlingEvent(EVENT_TYPE type)
{
	switch (type)
	{
	case Types::EVENT_DAMAGED:
		CScoreManager::GetInstance()->IncreaseScore(10);
		CScoreManager::GetInstance()->IncreaseCoinCount();
		SetObjectState(Types::OS_DAMAGED);
		break;
	case Types::EVENT_DEAD:
		GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Dead"));
		CScoreManager::GetInstance()->IncreaseScore(10);
		CScoreManager::GetInstance()->IncreaseCoinCount();
		SetObjectState(Types::OS_DEAD);
		//SetActive(false);
		break;
	}
}
