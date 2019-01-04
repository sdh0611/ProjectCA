#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CCoin.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Graphic\CCoinParticle.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CLayer.h"
#include "..\..\..\Include\Scene\CScoreManager.h"
#include "..\..\..\Include\Core\Sound\CSoundManager.h"


CCoin::CCoin()
{
}

CCoin::~CCoin()
{
}

bool CCoin::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	if (!CPickup::PostInit(data, pScene))
		return false;

	m_iScore		= 10;
	m_Type		= PT_COIN;

	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionCallback = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void{

		if (pOther->GetObjectType() == Types::OT_PLAYER)
		{
			CScoreManager::GetInstance()->IncreaseCoinCount();
			CScoreManager::GetInstance()->IncreaseScore(m_iScore);
			SetActive(false);
			m_pParticle.lock()->SetActive(true);
			//이 부분은 CoinParticle 객채 내에서 실행되는 코드로 바꿀 필요가 있음.
			CSoundManager::GetInstance()->SoundPlay(TEXT("SFXCoin"));
		}

	};
	pCollider->SetOnCollision(onCollisionCallback);
	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;

	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;

	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("CoinAnimation"),   true, TEXT("Idle")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	//Particle 설정
	auto pParticle = CObjectManager::GetInstance()->CreateEntity<CCoinParticle>(SPRITE_WIDTH, SPRITE_HEIGHT, GetEntityPosition().x, GetEntityPosition().y, TEXT("CoinParticle"), m_pOwnerScene);
	if (pParticle == nullptr)
	{
		return false;
	}
	pParticle->SetActive(false);
	m_pOwnerScene->AddEntityToScene(pParticle);
	m_pOwnerScene->FindLayer(TEXT("Pickup"))->AddActor(pParticle);
	AddParticle(pParticle);

	return true;
}

void CCoin::Init()
{
	CPickup::Init();
	SetActive(true);
}

void CCoin::Update(double dDeltaTime)
{
	CPickup::Update(dDeltaTime);
}

void CCoin::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		GetComponent<AnimationRender>().lock()->Draw(hDC);
	}

}

