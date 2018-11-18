#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CMushroom.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"


CMushroom::CMushroom()
{
}

CMushroom::~CMushroom()
{
}

bool CMushroom::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	if (!CActor::PostInit(data, pScene))
		return false;

	m_ActorType = Types::AT_PICKUP;

	//PhysicsComponent 추가
	auto pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 200.f, 200.f, 1300.f, 0.f))
		return false;
	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;


	//BoxCollider 추가
	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;
	auto colliderCallback = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {

		auto pPhysics = GetComponent<PhysicsComponent>().lock();
		//auto pOwnerActor = static_cast<CActor*>(pObject);

		switch (static_cast<CActor*>(pOther)->GetActorType())
		{
		case Types::AT_PROB:
			switch (type)
			{
			case Collider::COLLISION_BOT:
				pPhysics->SetGrounded(true);
				SetObjectPosition(GetObjectPosition().x, pOther->GetObjectPosition().y - pOther->GetObjectHeight());
				//pOwnerActor->SetActorState(Types::AS_IDLE);
				SetActorVerticalState(Types::VS_IDLE);
				break;
			case Collider::COLLISION_LEFT:
			case Collider::COLLISION_RIGHT:
				FlipActorDirection();
				break;
			}
			break;
		case Types::AT_PLAYER:
			auto pPlayer = static_cast<CPlayer*>(pOther);
			if (pPlayer->GetPlayerState() == CPlayer::PS_SMALL)
			{
				pPlayer->SetPlayerState(CPlayer::PS_BIG);
			}
			SetActive(false);
			break;
		}

	};
	pCollider->SetOnCollision(colliderCallback);
	pCollider->SetSize(m_iObjectWidth * 0.5f, m_iObjectHeight * 0.5f);
	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//ImageRender 추가
	auto pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->SetSprite(TEXT("Mushroom")))
		return false;
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;
	

	return true;
}

void CMushroom::Init()
{
	CActor::Init();
	m_bActive = true;
}

void CMushroom::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CActor::Update(dDeltaTime);
	}

}

void CMushroom::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		GetComponent<ImageRender>().lock()->Draw(hDC);
	}

}

void CMushroom::ActorBehavior(double dDeltaTime)
{
	auto pPhysics = GetComponent<PhysicsComponent>().lock();
	float fWalkSpeed = pPhysics->GetSpeed();

	if (m_Direction == Types::DIR_LEFT)
	{
		pPhysics->SetCurSpeed(-1 * fWalkSpeed);
	}
	else if (m_Direction == Types::DIR_RIGHT)
	{
		pPhysics->SetCurSpeed(fWalkSpeed);
	}

	GetTransform().lock()->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);

}
