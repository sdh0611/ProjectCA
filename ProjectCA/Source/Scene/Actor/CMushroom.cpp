#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CMushroom.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Scene\Actor\CRandomBlock.h"
#include "..\..\..\Include\Scene\CScoreManager.h"
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
	if (!CPickup::PostInit(data, pScene))
		return false;
	
	m_iScore		= 1000;
	m_Type		= PT_MUSHROOM_RED;

	//PhysicsComponent 추가
	auto pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 300.f, 300.f, 1300.f, 0.f))
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
					break;
				case Collider::COLLISION_LEFT:
					SetObjectPosition(GetObjectPosition().x + fIntersectLength, GetObjectPosition().y);
					FlipActorDirection();
					break;
				case Collider::COLLISION_RIGHT:
					SetObjectPosition(GetObjectPosition().x - fIntersectLength, GetObjectPosition().y);
					FlipActorDirection();
					break;
				case Collider::COLLISION_TOP:
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y + fIntersectLength);
					break;
				}
			}
			break;
		case Types::OT_BLOCK:
			if (!IsStored())
			{
				auto pBlock = static_cast<CRandomBlock*>(pOther);
				if (!pBlock->IsHiding())
				{
					switch (type)
					{
					case Collider::COLLISION_BOT:
						pPhysics->SetGrounded(true);
						SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
						break;
					case Collider::COLLISION_LEFT:
						SetObjectPosition(GetObjectPosition().x + fIntersectLength, GetObjectPosition().y);
						FlipActorDirection();
						break;
					case Collider::COLLISION_RIGHT:
						SetObjectPosition(GetObjectPosition().x - fIntersectLength, GetObjectPosition().y);
						FlipActorDirection();
						break;
					case Collider::COLLISION_TOP:
						SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y + fIntersectLength);
						break;
					}
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
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
				}

			}
			break;
		case Types::OT_PLAYER:
			HandlingEvent(Types::EVENT_DEAD);
			break;
		}
		
	};
	pCollider->SetOnCollision(colliderCallback);
	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//ImageRender 추가
	auto pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->SetSprite(TEXT("Mushroom")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;
	

	return true;
}

void CMushroom::Init()
{
	CPickup::Init();
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
	if (!IsStored())
	{
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
	else
	{
		GetTransform().lock()->Move(0.f, -200.f * dDeltaTime);
	}

}

void CMushroom::HandlingEvent(EVENT_TYPE type)
{
	switch (type)
	{
	case Types::EVENT_DEAD:
		if (!IsStored())
		{
			CScoreManager::GetInstance()->IncreaseScore(m_iScore);
			SetStored();
		}
		SetObjectState(Types::OS_DEAD);
		SetActive(false);
		break;
	}

}
