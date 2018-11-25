#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CCoin.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScoreManager.h"


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

	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionCallback = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void{

		if (pOther->GetObjectType() == Types::OT_PLAYER)
		{
			CScoreManager::GetInstance()->IncreaseCoinCount();
			CScoreManager::GetInstance()->IncreaseScore(10);
			SetActive(false);
		}

	};
	pCollider->SetOnCollision(onCollisionCallback);
	pCollider->SetSize(m_iObjectWidth * 0.4f, m_iObjectHeight * 0.5f);
	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;

	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;

	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("CoinAnimation"), m_iObjectWidth, m_iObjectHeight, true, TEXT("IdleLeft")))
		return false;
	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("CoinAnimation"), m_iObjectWidth, m_iObjectHeight, true, TEXT("IdleRight")))
		return false;
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

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

