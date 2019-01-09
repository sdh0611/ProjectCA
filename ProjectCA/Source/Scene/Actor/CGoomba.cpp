#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CGoomba.h"
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


CGoomba::CGoomba()
{
}

CGoomba::~CGoomba()
{
}

bool CGoomba::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	//기본 Actor의 속성 초기화
	if (!CEnemy::PostInit(data, pScene))
		return false;

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
			}
			break;
		case Types::OT_GROUND:
			if (type == Collider::COLLISION_BOT)
			{
				if (GetObjectPosition().y > pOther->GetObjectPosition().y + fIntersectLength
					|| pPhysics->GetCurJumpForce() > 0.f)
					return;
				pPhysics->SetGrounded(true);
				pPhysics->SetCurJumpForce(0.f);
				SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
			}
			break;
		case Types::OT_BLOCK:
		{
			auto pBlock = static_cast<CRandomBlock*>(pOther);
			if (!pBlock->IsHiding())
			{
				switch (type) {
				case Collider::COLLISION_BOT:
					pPhysics->SetGrounded(true);
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
					break;
				case Collider::COLLISION_TOP:
					pPhysics->SetGrounded(false);
					pPhysics->SetCurJumpForce(-100.f);
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y + fIntersectLength);
					break;
				case Collider::COLLISION_LEFT:
					SetObjectPosition(GetObjectPosition().x + fIntersectLength, GetObjectPosition().y);
					FlipActorDirection();
					break;
				case Collider::COLLISION_RIGHT:
					SetObjectPosition(GetObjectPosition().x - fIntersectLength, GetObjectPosition().y);
					FlipActorDirection();
					break;
				}
			}
		}
		break;
		case Types::OT_PLAYER:
			switch (type) {
			case Collider::COLLISION_TOP:
				if (static_cast<CActor*>(pOther)->GetActorAct() != Types::ACT_DESTROY)
				{
					HandlingEvent(Types::EVENT_DEAD);
				}
				else
				{
					HandlingEvent(Types::EVENT_DESTROY);
				}
				break;
			}
			break;
			case Types::OT_BULLET:
				HandlingEvent(Types::EVENT_DEAD);
				break;
			case Types::OT_PICKABLE:
				if (static_cast<CActor*>(pOther)->GetActorAct() == Types::ACT_ATTACK)
				{
					HandlingEvent(Types::EVENT_DEAD);
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
	if (!pRender->AddAnimation(0.25f, TEXT("GoombaNormal"), TEXT("EffectDestroyEnemy"), false,TEXT("Destroy"), false))
		return false;
	if (!pRender->AddAnimation(0.25f, TEXT("GoombaDamaged"), TEXT("GoombaTurnOverLeft"), true, TEXT("TurnOverLeft")))
		return false;
	if (!pRender->AddAnimation(0.25f, TEXT("GoombaDamaged"), TEXT("GoombaTurnOverRight"), true, TEXT("TurnOverRight")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;


	return true;
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
	
	switch (m_ObjectState)
	{
	case Types::OS_DEAD:
		{
			auto pCamera = CCameraManager::GetInstance()->GetMainCamera().lock();
			POSITION position = pTransform->GetScreenPosition();
			if (position.y > pCamera->GetCameraHeight() + m_iEntityHeight)
			{
				m_bActive = false;
				return;
			}
			if (position.x < 0.f - m_iEntityWidth || position.x > pCamera->GetCameraWidth() + m_iEntityWidth)
			{
				m_bActive = false;
				return;
			}
		}
		break;
	case Types::OS_DESTROYED:
		if (GetComponent<AnimationRender>().lock()->IsCurAnimationEnd())
		{
			SetActive(false);
		}
		break;
	default:
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
		break;
	}

	GetTransform().lock()->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);
}

void CGoomba::DeadProcess(double dDeltaTime)
{
}

void CGoomba::HandlingEvent(EVENT_TYPE type)
{
	auto pPhysics = GetComponent<PhysicsComponent>().lock();
	switch (type)
	{
	case Types::EVENT_DAMAGED:
		GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("GoombaDamaged"));
		pPhysics->SetCurSpeed(0.f);
		SetObjectState(Types::OS_DAMAGED);
		m_ObjectType = Types::OT_PICKABLE;
		CSoundManager::GetInstance()->SoundPlay(TEXT("SFXStomp"));
		break;
	case Types::EVENT_DEAD:
		SetObjectState(Types::OS_DEAD);
		GetComponent<ColliderBox>().lock()->SetActive(false);
		pPhysics->SetCurSpeed(0.f);
		pPhysics->SetCurJumpForce(300.f);
		pPhysics->SetGrounded(false);
		CScoreManager::GetInstance()->IncreaseScore(200);
		CSoundManager::GetInstance()->SoundPlay(TEXT("SFXStomp"));
		break;
	case Types::EVENT_DESTROY:
		SetObjectState(Types::OS_DESTROYED);
		pPhysics->SetActive(false);
		GetComponent<ColliderBox>().lock()->SetActive(false);
		GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("GoombaNormal"), TEXT("Destroy"));
		CScoreManager::GetInstance()->IncreaseScore(200);
		CSoundManager::GetInstance()->SoundPlay(TEXT("SFXSpinStomp"));
		break;
	}

}
