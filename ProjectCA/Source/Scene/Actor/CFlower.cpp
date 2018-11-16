#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CFlower.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"



CFlower::CFlower()
{
}

CFlower::~CFlower()
{
}

bool CFlower::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	if (!CActor::PostInit(data, pScene))
		return false;

	//PhysicsComponent 추가
	auto pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 0.f, 0.f, 900.f, 0.f))
		return false;
	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;


	//BoxCollider 추가
	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;
	auto colliderCallback = [&](CObject* pOther, Collider::CollisionType type)->void {

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
			if (pPlayer->GetPlayerState() != CPlayer::PS_FLOWER)
			{
				pPlayer->SetPlayerState(CPlayer::PS_FLOWER);
			}
			SetActive(false);
			break;
		}

	};
	pCollider->SetOnCollision(colliderCallback);
	pCollider->SetSize(m_iObjectWidth * 0.5f, m_iObjectHeight * 0.5f);
	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//AnimationRender 추가
	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("FlowerAnimation"), m_iObjectWidth, m_iObjectHeight, true, TEXT("IdleLeft")))
		return false;
	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("FlowerAnimation"), m_iObjectWidth, m_iObjectHeight, true, TEXT("IdleRight")))
		return false;
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	return true;
}

void CFlower::Init()
{
	CActor::Init();
}

void CFlower::Update(double dDeltaTime)
{
	CActor::Update(dDeltaTime);
}

void CFlower::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		GetComponent<AnimationRender>().lock()->Draw(hDC);
	}
}

void CFlower::ActorBehavior(double dDeltaTime)
{
	auto pPhysics = GetComponent<PhysicsComponent>().lock();

	GetTransform().lock()->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);
}
