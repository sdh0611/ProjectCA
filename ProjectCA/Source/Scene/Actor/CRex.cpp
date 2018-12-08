#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CRex.h"
#include "..\..\..\Include\Scene\Actor\CBlock.h"
#include "..\..\..\Include\Scene\CScoreManager.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"

CRex::CRex()
{
}

CRex::~CRex()
{
}

bool CRex::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	//기본 Actor의 속성 초기화
	if (!CActor::PostInit(data, pScene))
		return false;

	//PhysicsComponent 초기화
	std::shared_ptr<PhysicsComponent> pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 200.f, 300.f, 1300.f, 700.f))
		return false;

	pPhysics->SetCurSpeed(pPhysics->GetSpeed());

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//Collider 초기화
	std::shared_ptr<ColliderBox> pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionDelegater = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {

		auto pPhysics = GetComponent<PhysicsComponent>().lock();

		switch (pOther->GetObjectType()) {
		case Types::OT_ENEMY:
			if (pOther->GetObjectState() == Types::OS_DAMAGED)
			{
				if (pOther->GetComponent<PhysicsComponent>().lock()->GetCurSpeed() != 0.f)
				{
					SetObjectState(Types::OS_DEAD);
					GetComponent<ColliderBox>().lock()->SetActive(false);
					pPhysics->SetCurJumpForce(300.f);
					pPhysics->SetGrounded(false);
					CScoreManager::GetInstance()->IncreaseScore(200);
				}
				else
				{
					if (type == Collider::COLLISION_LEFT)
					{
						SetObjectPosition(GetObjectPosition().x + fIntersectLength, GetObjectPosition().y);
					}
					else if (type == Collider::COLLISION_RIGHT)
					{
						SetObjectPosition(GetObjectPosition().x - fIntersectLength, GetObjectPosition().y);
					}
					FlipActorDirection();
				}

			}
			else  //otherActor != Types::OS_DAMAGED
			{
				if (type == Collider::COLLISION_LEFT)
				{
					SetObjectPosition(GetObjectPosition().x + fIntersectLength, GetObjectPosition().y);
				}
				else if (type == Collider::COLLISION_RIGHT)
				{
					SetObjectPosition(GetObjectPosition().x - fIntersectLength, GetObjectPosition().y);
				}
				FlipActorDirection();
			}
			break;
		case Types::OT_PROB:
			switch (type) {
			case Collider::COLLISION_BOT:
				pPhysics->SetGrounded(true);
				SetActorVerticalState(Types::VS_IDLE);
				SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
				break;
			case Collider::COLLISION_LEFT:
			case Collider::COLLISION_RIGHT:
				FlipActorDirection();
				break;
			}
			break;
		case Types::OT_GROUND:
			if (type == Collider::COLLISION_BOT)
			{
				pPhysics->SetGrounded(true);
				pPhysics->SetCurJumpForce(0.f);
				SetActorVerticalState(Types::VS_IDLE);
				SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
			}
			break;
		case Types::OT_BLOCK:
		{
			auto pBlock = static_cast<CBlock*>(pOther);
			if (!pBlock->IsHiding())
			{
				switch (type) {
				case Collider::COLLISION_BOT:
					pPhysics->SetGrounded(true);
					SetActorVerticalState(Types::VS_IDLE);
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
					break;
				case Collider::COLLISION_TOP:
					pPhysics->SetGrounded(false);
					pPhysics->SetCurJumpForce(-100.f);
					SetActorVerticalState(Types::VS_FALL);
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y + fIntersectLength);
					break;
				case Collider::COLLISION_LEFT:
					FlipActorDirection();
					if (m_ObjectState == Types::OS_DAMAGED)
					{
						pPhysics->SetCurSpeed(pPhysics->GetMaxSpeed());
					}
					break;
				case Collider::COLLISION_RIGHT:
					FlipActorDirection();
					if (m_ObjectState == Types::OS_DAMAGED)
					{
						pPhysics->SetCurSpeed(-1 * pPhysics->GetMaxSpeed());
					}
					break;
				}
			}
		}
		break;
		case Types::OT_PLAYER:
			switch (type) {
			case Collider::COLLISION_TOP:
				if (m_RexState != REX_DAMAGED)
				{
					m_RexState = REX_DAMAGED;
					GetComponent<ColliderBox>().lock()->SetCurRectHeight(GetObjectHeight() * 0.5f);
					GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("RexDamaged"));
				}
				else
				{
					SetObjectState(Types::OS_DEAD);
					GetComponent<ColliderBox>().lock()->SetActive(false);
					pPhysics->SetStatic(true);
					pPhysics->SetCurSpeed(0.f);
					//pPhysics->SetCurJumpForce(300.f);
					//pPhysics->SetGrounded(false);
					if (m_Direction == Types::DIR_RIGHT)
					{
						GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("DeadRight"));
					}
					else if (m_Direction == Types::DIR_LEFT)
					{
						GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("DeadLeft"));
					}
				}
				CScoreManager::GetInstance()->IncreaseScore(200);
				break;
			}
			break;
		case Types::OT_BULLET:
			SetObjectState(Types::OS_DEAD);
			GetComponent<ColliderBox>().lock()->SetActive(false);
			pPhysics->SetCurSpeed(0.f);
			pPhysics->SetCurJumpForce(300.f);
			pPhysics->SetGrounded(false);
			CScoreManager::GetInstance()->IncreaseScore(200);
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
	if (!pRender->AddAnimation(0.25f, TEXT("RexNormal"), TEXT("RexMoveRight"), true, TEXT("MoveRight")))
		return false;
	if (!pRender->AddAnimation(0.25f, TEXT("RexNormal"), TEXT("RexMoveLeft"), true, TEXT("MoveLeft")))
		return false;
	if (!pRender->AddAnimation(0.15f, TEXT("RexDamaged"), TEXT("RexDamagedMoveRight"), true, TEXT("MoveRight")))
		return false;
	if (!pRender->AddAnimation(0.15f, TEXT("RexDamaged"), TEXT("RexDamagedMoveLeft"), true, TEXT("MoveLeft")))
		return false;
	if (!pRender->AddAnimation(0.f, TEXT("RexDamaged"), TEXT("RexDeadRight"), false, TEXT("DeadRight")))
		return false;
	if (!pRender->AddAnimation(0.f, TEXT("RexDamaged"), TEXT("RexDeadLeft"), false, TEXT("DeadLeft")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	m_dElapsedTime	= 0.f;
	m_RexState			= REX_IDLE;

	return true;

}

void CRex::Init()
{
	CEnemy::Init();
	m_RexState = REX_IDLE;
	GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("RexNormal"), TEXT("MoveLeft"));
}

void CRex::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CEnemy::Update(dDeltaTime);
	}

}

void CRex::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		GetComponent<AnimationRender>().lock()->Draw(hDC);
	}
}

void CRex::ActorBehavior(double dDeltaTime)
{
	auto pPhysics = GetComponent<PhysicsComponent>().lock();
	auto pTransform = GetTransform().lock();
	if (m_ObjectState != Types::OS_DEAD)
	{
		float fSpeed;
		if (m_ObjectState != Types::OS_DAMAGED)
		{
			fSpeed = pPhysics->GetSpeed();
			if (m_Direction == Types::DIR_LEFT)
			{
				pPhysics->SetCurSpeed(-1 * fSpeed);
				GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("MoveLeft"));
			}
			else if (m_Direction == Types::DIR_RIGHT)
			{
				pPhysics->SetCurSpeed(fSpeed);
				GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("MoveRight"));
			}
		}
		else
		{
			fSpeed = pPhysics->GetMaxSpeed();
			if (m_Direction == Types::DIR_LEFT)
			{
				pPhysics->SetCurSpeed(-1 * fSpeed);
				GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("MoveLeft"));
			}
			else if (m_Direction == Types::DIR_RIGHT)
			{
				pPhysics->SetCurSpeed(fSpeed);
				GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("MoveRight"));
			}
		}
	}
	else
	{
		m_dElapsedTime += dDeltaTime;
		if (m_dElapsedTime > 0.5f)
		{
			m_dElapsedTime = 0.f;
			SetActive(false);
		}
	}

	GetTransform().lock()->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);


}

void CRex::DeadProcess(double dDeltaTime)
{
	
}
