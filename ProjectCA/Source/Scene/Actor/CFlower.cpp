#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CFlower.h"
#include "..\..\..\Include\Scene\Actor\CBlock.h"
#include "..\..\..\Include\Scene\CScoreManager.h"
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
	if (!CPickup::PostInit(data, pScene))
		return false;

	m_iScore		= 1000;
	m_Type		= PT_FLOWER;

	//PhysicsComponent 추가
	auto pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 0.f, 0.f, 1300.f, 0.f))
		return false;
	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//BoxCollider 추가
	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;
	auto colliderCallback = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {

		auto pPhysics = GetComponent<PhysicsComponent>().lock();

		switch (pOther->GetObjectType())
		{
		case Types::OT_PROB:
			if (!IsStored())
			{
				switch (type)
				{
				case Collider::COLLISION_BOT:
					pPhysics->SetGrounded(true);
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
					SetActorVerticalState(Types::VS_IDLE);
					break;
				case Collider::COLLISION_TOP:
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y + fIntersectLength);
					break;
				}
			}
			break;
		case Types::OT_GROUND:
			if (!IsStored())
			{
				if (type == Collider::COLLISION_BOT)
				{
					pPhysics->SetGrounded(true);
					pPhysics->SetCurJumpForce(0.f);
					SetActorVerticalState(Types::VS_IDLE);
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
				}

			}
			break;
		case Types::OT_BLOCK:
		{
			if (!IsStored())
			{
				auto pBlock = static_cast<CBlock*>(pOther);
				if (!pBlock->IsHiding())
				{
					switch (type) {
					case Collider::COLLISION_BOT:
						pPhysics->SetGrounded(true);
						SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
						SetActorVerticalState(Types::VS_IDLE);
						break;
					case Collider::COLLISION_TOP:
						SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y + fIntersectLength);
						break;
					}
				}
			}
			break;
		}
		case Types::OT_PLAYER:
			if (!IsStored())
			{
				CScoreManager::GetInstance()->IncreaseScore(m_iScore);
				SetStored();
			}
			SetActive(false);
			break;
		}

	};
	pCollider->SetOnCollision(colliderCallback);
	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//AnimationRender 추가
	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("FlowerAnimation"),   true, TEXT("IdleLeft")))
		return false;
	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("FlowerAnimation"),   true, TEXT("IdleRight")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;


	return true;
}

void CFlower::Init()
{
	CPickup::Init();
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
	if (!IsStored())
	{
		auto pPhysics = GetComponent<PhysicsComponent>().lock();
		GetTransform().lock()->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);
	}
	else
	{
		GetTransform().lock()->Move(0.f, -200.f * dDeltaTime);
	}
}
