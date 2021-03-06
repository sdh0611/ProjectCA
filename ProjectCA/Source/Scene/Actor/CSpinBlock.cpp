#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CSpinBlock.h"
#include "..\..\..\Include\Scene\CObject.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Scene\Actor\CMushroom.h"
#include "..\..\..\Include\Scene\Actor\CFlower.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Graphic\CBlockParticle.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScoreManager.h"
#include "..\..\..\Include\Scene\CLayer.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Sound\CSoundManager.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.hpp"


CSpinBlock::CSpinBlock()
{
}

CSpinBlock::~CSpinBlock()
{
}

bool CSpinBlock::PostInit(const OBJECT_DATA & data, CScene * pScene)
{
	if (!CProp::PostInit(data, pScene))
		return false;
	
	//Collider에 OnCollision 콜백 메소드 설정
	auto onCollision = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {

		switch (pOther->GetObjectType())
		{
		case Types::OT_PLAYER:
		{
			auto pPlayer = static_cast<CPlayer*>(pOther);
			if (type == Collider::COLLISION_BOT)
			{
				HandlingEvent(Types::EVENT_DAMAGED);
			}
			else if (type == Collider::COLLISION_TOP)
			{
				if (pPlayer->GetActorAct() == Types::ACT_DESTROY)
				{
					if (pPlayer->GetPlayerState() != CPlayer::PS_SMALL)
					{
						pPlayer->GetComponent<PhysicsComponent>().lock()->SetCurJumpForce(300.f);
						HandlingEvent(Types::EVENT_DESTROY);
					}
				}
			}
		}
		break;
		case Types::OT_PICKABLE:
			if (static_cast<CActor*>(pOther)->GetActorAct() == Types::ACT_ATTACK)
			{
				if (type != Collider::COLLISION_TOP)
				{
					HandlingEvent(Types::EVENT_DAMAGED);
				}
			}
			break;
		}

	};
	GetComponent<ColliderBox>().lock()->SetOnCollision(onCollision);

	//RenderComponent
	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->AddAnimation(0.f, TEXT("Default"), TEXT("SpinBlockIdle"), false, TEXT("Idle")))
		return false;
	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("SpinBlockSpining"), true, TEXT("Spin")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	//Particle 설정
	auto pParticle = CObjectManager::GetInstance()->CreateEntity<CBlockParticle>(SPRITE_WIDTH, SPRITE_HEIGHT, GetEntityPosition().x, GetEntityPosition().y, TEXT("BlockParticle"), m_pOwnerScene);
	if (pParticle == nullptr)
	{
		return false;
	}
	pParticle->SetActive(false);
	m_pOwnerScene->AddEntityToScene(pParticle);
	m_pOwnerScene->FindLayer(TEXT("Block"))->AddActor(pParticle);
	AddParticle(pParticle);

	m_dTimeElapsed = 0.f;

	return true;

}
	
void CSpinBlock::Init()
{
	m_dTimeElapsed = 0.f;
	GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Idle"));
	SetActive(true);
	SetObjectState(Types::OS_IDLE);
}

void CSpinBlock::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		BlockBehavior(dDeltaTime);
		CProp::Update(dDeltaTime);
	}
}

void CSpinBlock::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		GetComponent<AnimationRender>().lock()->Draw(hDC);
	}
}

void CSpinBlock::BlockBehavior(double dDeltaTime)
{
	if (GetObjectState() == Types::OS_DAMAGED)
	{
		m_dTimeElapsed += dDeltaTime;
		if (m_dTimeElapsed > 3.0f)
		{
			m_dTimeElapsed = 0.f;
			SetObjectState(Types::OS_IDLE);
			GetComponent<ColliderBox>().lock()->SetActive(true);
			GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Idle"));
		}
	}
	else if (GetObjectState() != Types::OS_IDLE)
	{
		SetActive(false);
	}
}

void CSpinBlock::HandlingEvent(EVENT_TYPE type)
{
	switch (type)
	{
	case Types::EVENT_DAMAGED:
		SetObjectState(Types::OS_DAMAGED);
		GetComponent<ColliderBox>().lock()->SetActive(false);
		GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Spin"));
		break;
	case Types::EVENT_DESTROY:
		SetObjectState(Types::OS_DESTROYED);
		SetActive(false);
		m_pParticle.lock()->SetActive(true);
		CSoundManager::GetInstance()->SoundPlay(TEXT("SFXBreakBlock"));
		break;
	}
}
