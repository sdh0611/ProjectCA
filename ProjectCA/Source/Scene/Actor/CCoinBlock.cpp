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
#include "..\..\..\Include\Core\Graphic\CCoinParticle.h"
#include "..\..\..\Include\Core\Sound\CSoundManager.h"



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

	//Particle 설정
	auto pParticle = CObjectManager::GetInstance()->CreateEntity<CCoinParticle>(SPRITE_WIDTH, SPRITE_HEIGHT, GetEntityPosition().x, GetEntityPosition().y, TEXT("CoinParticle"), m_pOwnerScene);
	if (pParticle == nullptr)
	{
		return false;
	}
	pParticle->SetActive(false);
	m_pOwnerScene->AddEntityToScene(pParticle);
	m_pOwnerScene->FindLayer(TEXT("Block"))->AddActor(pParticle);
	//m_pOwnerLayer->AddActor(pParticle);
	AddParticle(pParticle);

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

//void CCoinBlock::Render(const HDC & hDC)
//{
//	if (!m_bHiding)
//	{
//		if (m_bActive)
//		{
//			auto pRender = GetComponent<AnimationRender>().lock();
//			if (pRender->IsActive())
//			{
//				pRender->Draw(hDC);
//			}
//
//		}
//	}
//}


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
		auto pRender = GetComponent<AnimationRender>().lock();

		if (m_dTimeElapsed >= m_dLimitTime)
		{
			pRender->ChangeAnimation(TEXT("Hit"));
			HandlingEvent(Types::EVENT_DEAD);
			m_dTimeElapsed = 0.f;
		}
		else
		{
			m_dTimeElapsed += dDeltaTime;
		}
		//if (m_DrawPivot.y > pRender->GetDrawPivot().y)
		//{
		//	m_DrawPivot = pRender->GetDrawPivot();
		//	m_fYSpeed = m_fBumpForce;
		//	HandlingEvent(Types::EVENT_DEAD);
		//}
		//else
		//{
		//	m_DrawPivot.y -= m_fYSpeed * dDeltaTime;
		//	m_fYSpeed -= 20.f;
		//}
	
		break;
	//default:

	//	break;		
	}
}

void CCoinBlock::HandlingEvent(EVENT_TYPE type)
{
	switch (type)
	{
	case Types::EVENT_DAMAGED:
		GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Hit"));
		CScoreManager::GetInstance()->IncreaseScore(10);
		CScoreManager::GetInstance()->IncreaseCoinCount();
		SetObjectState(Types::OS_DAMAGED);
		break;
	case Types::EVENT_DEAD:
		GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Dead"));
		CScoreManager::GetInstance()->IncreaseScore(10);
		CScoreManager::GetInstance()->IncreaseCoinCount();
		SetObjectState(Types::OS_DEAD);
		m_pParticle.lock()->SetActive(true);
		//이 부분은 CoinParticle이 활성화 될 때 CoinParticle 객체 내에서 실행하게끔 바꿀 필요가 있음.
		CSoundManager::GetInstance()->SoundPlay(TEXT("SFXCoin"));
		break;
	}
}
