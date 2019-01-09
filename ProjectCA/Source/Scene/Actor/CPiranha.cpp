#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPiranha.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScoreManager.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Scene\Actor\CRandomBlock.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Sound\CSoundManager.h"
#include "..\..\..\Include\Scene\Actor\CPiranha.h"


CPiranha::CPiranha()
{
}

CPiranha::~CPiranha()
{
}

bool CPiranha::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	//기본 Actor의 속성 초기화
	if (!CEnemy::PostInit(data, pScene))
		return false;

	//Collider 초기화
	std::shared_ptr<ColliderBox> pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionDelegater = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {
		switch (pOther->GetObjectType()) {
		case Types::OT_BULLET:
			HandlingEvent(Types::EVENT_DEAD);
			break;
		case Types::OT_PICKABLE:
			if (static_cast<CActor*>(pOther)->GetActorAct() == Types::ACT_ATTACK)
			{
				HandlingEvent(Types::EVENT_DEAD);
			}
			break;
		}

	};

	pCollider->SetDelegate(onCollisionDelegater);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//RenderComponent 초기화
	std::shared_ptr<AnimationRender> pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->AddAnimation(0.25f, TEXT("Default"), TEXT("PiranhaDefault"), true, TEXT("Default")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("Default"), TEXT("EffectDestroyEnemy"), false, TEXT("Destroy"), false))
		return false;

	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	m_fPeak = GetEntityPosition().y - m_iEntityHeight;

	return true;
}

void CPiranha::Init()
{
	CEnemy::Init();
	GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("Default"), TEXT("Default"));
}


void CPiranha::DeadProcess(double dDeltaTime)
{
}

void CPiranha::ActorBehavior(double dDeltaTime)
{
	switch (m_ObjectState)
	{
	case Types::OS_DEAD:
		if (GetComponent<AnimationRender>().lock()->IsCurAnimationEnd())
		{
			SetActive(false);
		}
		break;
	default:
		if (m_dTimeElapsed == 0.f)
		{
			auto playerPosition = static_cast<CGameScene*>(m_pOwnerScene)->GetPlayerPtr().lock()->GetEntityPosition();
			if (playerPosition.x > GetEntityPosition().x - SPRITE_WIDTH * 2.f && playerPosition.x < GetEntityPosition().x + SPRITE_WIDTH * 2.f)
			{
				return;
			}
		}

		m_dTimeElapsed += dDeltaTime;
		float fPositionY = GetEntityPosition().y;

		if (m_dTimeElapsed < 2.f)
		{
			if (fPositionY > m_fPeak)
			{
				GetTransform().lock()->Move(0, 200.f * dDeltaTime);
			}
		}
		else if (m_dTimeElapsed > 3.f && m_dTimeElapsed < 5.f)
		{
			if (fPositionY < GetTransform().lock()->GetSpawnPosition().y)
			{
				GetTransform().lock()->Move(0, -200.f * dDeltaTime);
			}
		}
		else if (m_dTimeElapsed > 5.f)
		{
			m_dTimeElapsed = 0.f;
		}
		break;
	}
}

void CPiranha::HandlingEvent(EVENT_TYPE type)
{
	switch (type)
	{
	case Types::EVENT_DEAD:
		SetObjectState(Types::OS_DEAD);
		GetComponent<ColliderBox>().lock()->SetActive(false);
		GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("Default"), TEXT("Destroy"));
		CScoreManager::GetInstance()->IncreaseScore(200);
		CSoundManager::GetInstance()->SoundPlay(TEXT("SFXStomp"));
		break;
	}

}
