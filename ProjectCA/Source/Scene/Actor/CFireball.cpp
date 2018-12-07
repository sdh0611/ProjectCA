#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CFireball.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Scene\Actor\CBlock.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"


CFireball::CFireball()
	:CActor()
{
}

CFireball::~CFireball()
{
	puts("Destroy Fireball");
}

bool CFireball::PostInit(const Types::ActorData & data, CGameScene* pScene)
{
	if (!CActor::PostInit(data, pScene))
		return false;

	m_ObjectType = Types::OT_BULLET;

	//Physics 추가
	auto pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 600.f, 600.f, 1300.f, 300.f))
		return false;
	//pPhysics->SetCurSpeed(pPhysics->GetSpeed());
	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;


	//Collider 추가
	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;
	pCollider->SetSize(m_iObjectWidth * 0.23f, m_iObjectHeight * 0.23f);

	auto onCollision = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {

		CActor* pOtherActor = static_cast<CActor*>(pOther);

		switch (pOtherActor->GetObjectType())
		{
		case Types::OT_BLOCK:
			if (!static_cast<CBlock*>(pOther)->IsHiding())
			{
				switch (type)
				{
				case Collider::COLLISION_BOT:
					GetComponent<PhysicsComponent>().lock()->SetCurJumpForce(GetComponent<PhysicsComponent>().lock()->GetJumpForce());
					break;
				default:
					SetFireballInactive();
					//SetActive(false);
					break;
				}
			}
			break;
		case Types::OT_PROB:
			switch (type)
			{
			case Collider::COLLISION_BOT:
				GetComponent<PhysicsComponent>().lock()->SetCurJumpForce(GetComponent<PhysicsComponent>().lock()->GetJumpForce());
				break;
			default:
				SetFireballInactive();
				//SetActive(false);
				break;
			}
			break;
		case Types::OT_GROUND:
			if (type == Collider::COLLISION_BOT)
			{
				GetComponent<PhysicsComponent>().lock()->SetCurJumpForce(GetComponent<PhysicsComponent>().lock()->GetJumpForce());
			}
			break;
		case Types::OT_ENEMY:
			SetFireballInactive();
			//SetActive(false);
			break;
		}

	};
	pCollider->SetOnCollision(onCollision);
	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//Render 추가
	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->AddAnimation(0.3f, TEXT("Default"), TEXT("FireballRight"),   true, TEXT("AttackRight")))
		return false;
	if (!pRender->AddAnimation(0.3f, TEXT("Default"), TEXT("FireballLeft"),   true, TEXT("AttackLeft")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	m_bActive = false;
	return true;
}

void CFireball::Init()
{
	CActor::Init();
	m_bActive = false;
}

void CFireball::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		ActorBehavior(dDeltaTime);
		for (auto& component : m_ComponentTable)
		{
			component.second->Update(dDeltaTime);
		}

	}

}

void CFireball::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		auto pRender = GetComponent<AnimationRender>();
		if (!pRender.expired())
			pRender.lock()->Draw(hDC);
	}
}

void CFireball::LateUpdate()
{
	if (m_bActive)
	{
		GetTransform().lock()->AdjustScreenPosition();
		GetComponent<AnimationRender>().lock()->LateUpdate();

		auto pCamera = CCameraManager::GetInstance()->GetMainCamera().lock();
		auto screenPosition = GetTransform().lock()->GetScreenPosition();

		if (screenPosition.x < 0.f || screenPosition.x >(float)pCamera->GetCameraWidth())
		{
			SetFireballInactive();
			return;
		}
		if (screenPosition.y < 0.f || screenPosition.y >(float)pCamera->GetCameraHeight())
		{
			SetFireballInactive();
			return;
		}
	}
}
void CFireball::SetFireballActive()
{
	m_Direction = m_pOwnerActor->GetActorDirection();
	
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
	GetTransform().lock()->SetPosition(m_pOwnerActor->GetObjectPosition().x, m_pOwnerActor->GetObjectPosition().y - m_pOwnerActor->GetObjectHeight() / 2.f);
	SetActive(true);
}

void CFireball::SetFireballInactive()
{
	static_cast<CPlayer*>(m_pOwnerActor)->IncreaseAvailableFireballCount();
	GetComponent<ColliderBox>().lock()->SetActive(false);
	SetActive(false);
}


void CFireball::ActorBehavior(double dDeltaTime)
{
	auto pPhysics = GetComponent<PhysicsComponent>().lock();

	GetTransform().lock()->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);

	if (m_Direction == Types::DIR_RIGHT)
	{
		GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("AttackRight"));
	}
	else if (m_Direction == Types::DIR_LEFT)
	{
		GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("AttackLeft"));
	}

}

void CFireball::SetOwnerObject(std::shared_ptr<CObject> pOwner)
{
}
