#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CGoomba.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScoreManager.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Scene\Actor\CBlock.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
//#include "..\..\..\Include\Core\Components\HPComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"


CGoomba::CGoomba()
{
}

CGoomba::~CGoomba()
{
}

bool CGoomba::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	//기본 Actor의 속성 초기화
	if (!CActor::PostInit(data, pScene))
		return false;

	//AIComponent (InputComponent) 초기화
	std::shared_ptr<AIComponent> pAI = std::make_shared<AIComponent>();
	if (!pAI->PostInit(this))
		return false;

	//auto enemyBehavior = [](CActor* pActor) ->void {

	//	if (pActor->GetObjectPosition().x < 100.f)
	//	{
	//		pActor->SetActorDirection(Types::DIR_RIGHT);
	//	}
	//	else if (pActor->GetObjectPosition().x > 1000.f)
	//	{
	//		pActor->SetActorDirection(Types::DIR_LEFT);
	//	}

	//};
	//pAI->SetDelegate(enemyBehavior);
	//if (!AddComponent(pAI, pAI->GetComponentTag()))
	//	return false;

	//PhysicsComponent 초기화
	std::shared_ptr<PhysicsComponent> pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 200.f, 750.f, 1300.f, 700.f))
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
			if (m_ObjectState != Types::OS_DAMAGED)
			{
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
				SetObjectState(Types::OS_DEAD);
				GetComponent<ColliderBox>().lock()->SetActive(false);
				pPhysics->SetCurSpeed(0.f);
				pPhysics->SetCurJumpForce(300.f);
				pPhysics->SetGrounded(false);
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
	if (!pRender->AddAnimation(0.25f, TEXT("GoombaNormal"), TEXT("GoombaWalkRight"),   true, TEXT("WalkRight")))
		return false;
	if (!pRender->AddAnimation(0.25f, TEXT("GoombaNormal"), TEXT("GoombaWalkLeft"),   true, TEXT("WalkLeft")))
		return false;
	if (!pRender->AddAnimation(0.25f, TEXT("GoombaNormal"), TEXT("GoombaWalkRight"),   true, TEXT("JumpRight")))
		return false;
	if (!pRender->AddAnimation(0.25f, TEXT("GoombaNormal"), TEXT("GoombaWalkLeft"),   true, TEXT("JumpLeft")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;


	return true;
}

void CGoomba::Init()
{
	CEnemy::Init();
}

void CGoomba::Update(double dDeltaTime)
{
	CEnemy::Update(dDeltaTime);
}

void CGoomba::Render(const HDC & hDC)
{
	auto pRender = GetComponent<AnimationRender>().lock();
	if (pRender->IsActive())
		pRender->Draw(hDC);

}

void CGoomba::ActorBehavior(double dDeltaTime)
{
	auto pPhysics = GetComponent<PhysicsComponent>().lock();
	auto pTransform = GetTransform().lock();
	if (m_ObjectState != Types::OS_DEAD)
	{
		if (m_ObjectState != Types::OS_DAMAGED)
		{
			float fWalkSpeed = pPhysics->GetSpeed();

			if (m_Direction == Types::DIR_LEFT)
			{
				pPhysics->SetCurSpeed(-1 * fWalkSpeed);
				GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("WalkLeft"));
			}
			else if (m_Direction == Types::DIR_RIGHT)
			{
				pPhysics->SetCurSpeed(fWalkSpeed);
				GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("WalkRight"));
			}

		}

	}
	else
	{
		auto pCamera = CCameraManager::GetInstance()->GetMainCamera().lock();
		POSITION position = pTransform->GetScreenPosition();
		if (position.y > pCamera->GetCameraHeight() + m_iObjectHeight)
		{
			m_bActive = false;
			return;
		}
		if (position.x < 0.f - m_iObjectWidth || position.x > pCamera->GetCameraWidth() + m_iObjectWidth)
		{
			m_bActive = false;
			return;
		}

	}

	GetTransform().lock()->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);

}

void CGoomba::DeadProcess(double dDeltaTime)
{
}
