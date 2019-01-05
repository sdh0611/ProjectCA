#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CKoopa.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Scene\Actor\CRandomBlock.h"
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
#include "..\..\..\Include\Core\Sound\CSoundManager.h"


CKoopa::CKoopa()
	:CEnemy()
{
}

CKoopa::~CKoopa()
{
}

bool CKoopa::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	//기본 Actor의 속성 초기화
	if (!CEnemy::PostInit(data, pScene))
		return false;

	//PhysicsComponent 초기화
	auto pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 200.f, 750.f, 1300.f, 850.f))
		return false;

	pPhysics->SetCurSpeed(pPhysics->GetSpeed());

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//Collider 초기화
	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionDelegater = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {

		auto pPhysics = GetComponent<PhysicsComponent>().lock();

		if (m_pOwnerObject.expired())
		{
			switch (pOther->GetObjectType()) {
			case Types::OT_ENEMY:
				if (GetKoopaState() != KOOPA_SHELL)
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
				if (GetKoopaState() == KOOPA_SHELL)
				{
					if (type != Collider::COLLISION_BOT)
					{
						CSoundManager::GetInstance()->SoundPlay(TEXT("SFXShellRicochet"));
					}
				}
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
					if (GetKoopaState() == KOOPA_SHELL)
					{
						pPhysics->SetCurSpeed(pPhysics->GetMaxSpeed());
					}
					break;
				case Collider::COLLISION_RIGHT:
					SetObjectPosition(GetObjectPosition().x - fIntersectLength, GetObjectPosition().y);
					FlipActorDirection();
					if (GetKoopaState() == KOOPA_SHELL)
					{
						pPhysics->SetCurSpeed(-1 * pPhysics->GetMaxSpeed());
					}
					break;
				}
				break;
			case Types::OT_BLOCK:
			{
				auto pBlock = static_cast<CRandomBlock*>(pOther);
				if (!pBlock->IsHiding())
				{
					if (GetKoopaState() == KOOPA_SHELL)
					{
						if (type != Collider::COLLISION_BOT)
						{
							CSoundManager::GetInstance()->SoundPlay(TEXT("SFXShellRicochet"));
						}
					}
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
						if (GetKoopaState() == KOOPA_SHELL)
						{
							pPhysics->SetCurSpeed(pPhysics->GetMaxSpeed());
						}
						break;
					case Collider::COLLISION_RIGHT:
						SetObjectPosition(GetObjectPosition().x - fIntersectLength, GetObjectPosition().y);
						FlipActorDirection();
						if (GetKoopaState() == KOOPA_SHELL)
						{
							pPhysics->SetCurSpeed(-1 * pPhysics->GetMaxSpeed());
						}
						break;
					}
				}
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
			case Types::OT_PLAYER:
				switch (type) {
				case Collider::COLLISION_TOP:
					if (static_cast<CActor*>(pOther)->GetActorAct() == Types::ACT_DESTROY)
					{
						HandlingEvent(Types::EVENT_DESTROY);
					}
					else
					{
						HandlingEvent(Types::EVENT_DAMAGED);
					}
					break;
				case Collider::COLLISION_RIGHT:
					if (pOther->GetObjectState() != Types::OS_DAMAGED)
					{
						HandlingEvent(Types::EVENT_DAMAGED);
					}
					break;
				case Collider::COLLISION_LEFT:
					if (pOther->GetObjectState() != Types::OS_DAMAGED)
					{
						HandlingEvent(Types::EVENT_DAMAGED);
					}
					break;
				}
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
		}
	};
	pCollider->SetDelegate(onCollisionDelegater);
	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//RenderComponent 초기화
	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;

	//평소 상태 animation
	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"),TEXT("KoopaGreenWalkRight"),   true, TEXT("WalkRight")))
		return false;
	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkLeft"),   true, TEXT("WalkLeft")))
		return false;
	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("EffectDestroyEnemy"), false, TEXT("Destroy"), false))
		return false;
	//Shell 상태 animation
	if (!pRender->AddAnimation(0.2f, TEXT("KoopaShell"), TEXT("KoopaGreenThrownRight"),   true, TEXT("ThrownRight")))
		return false;
	if (!pRender->AddAnimation(0.2f, TEXT("KoopaShell"), TEXT("KoopaGreenThrownLeft"),   true, TEXT("ThrownLeft")))
		return false;
	if (!pRender->AddAnimation(0.25f, TEXT("KoopaShell"), TEXT("KoopaGreenShell"),   true, TEXT("ShellIdle")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	m_KoopaState = KOOPA_IDLE;
	
	return true;
}

void CKoopa::Init()
{
	CEnemy::Init();
	m_KoopaState = KOOPA_IDLE;
	m_pOwnerObject.reset();
	GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("KoopaNormal"), TEXT("WalkLeft"));
}

void CKoopa::Render(const HDC & hDC)
{
	auto pRender = GetComponent<AnimationRender>().lock();
	if (pRender->IsActive())
		pRender->Draw(hDC);

}

CKoopa::KoopaState CKoopa::GetKoopaState()
{
	return m_KoopaState;
}

void CKoopa::ActorBehavior(double dDeltaTime)
{
	auto pPhysics		= GetComponent<PhysicsComponent>().lock();
	auto pTransform	= GetTransform().lock();
	auto pRender		= GetComponent<AnimationRender>().lock();

	switch (m_ObjectState)
	{
	case Types::OS_DEAD:
		{
			auto pCamera = CCameraManager::GetInstance()->GetMainCamera().lock();
			POSITION position = pTransform->GetScreenPosition();
			if (position.y > pCamera->GetCameraHeight() + m_iEntityHeight)
			{
				SetActive(false);
				return;
			}
			if (position.x < 0.f - m_iEntityWidth || position.x > pCamera->GetCameraWidth() + m_iEntityWidth)
			{
				SetActive(false);
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
		if (m_KoopaState != KOOPA_SHELL)
		{
			float fSpeed = pPhysics->GetSpeed();

			if (m_Direction == Types::DIR_LEFT)
			{
				pPhysics->SetCurSpeed(-1 * fSpeed);
				pRender->ChangeAnimation(TEXT("WalkLeft"));
			}
			else if (m_Direction == Types::DIR_RIGHT)
			{
				pPhysics->SetCurSpeed(fSpeed);
				pRender->ChangeAnimation(TEXT("WalkRight"));
			}

		}
		else
		{
			if (pPhysics->GetCurSpeed() != 0.f)
			{
				if (m_Direction == Types::DIR_LEFT)
				{
					pRender->ChangeAnimation(TEXT("ThrownLeft"));
				}
				else if (m_Direction == Types::DIR_RIGHT)
				{
					pRender->ChangeAnimation(TEXT("ThrownRight"));
				}
			}
			else
			{
				if (pPhysics->GetCurJumpForce() == 0.f)
				{
					SetActorAct(Types::ACT_IDLE);
				}
			}
		}
		break;
	}

	GetTransform().lock()->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);

}

void CKoopa::DeadProcess(double dDeltaTime)
{
	if (GetComponent<AnimationRender>().lock()->IsCurAnimationEnd())
	{
		CScoreManager::GetInstance()->IncreaseScore(200);
		SetActive(false);
	}
	
}

void CKoopa::HandlingEvent(EVENT_TYPE type)
{
	auto pPlayer = static_cast<CGameScene*>(m_pOwnerScene)->GetPlayerPtr().lock();
	auto pPhysics = GetComponent<PhysicsComponent>().lock();

	switch (type)
	{
	case Types::EVENT_DAMAGED:
		if (m_KoopaState == KOOPA_IDLE)
		{
			m_KoopaState = KOOPA_SHELL;
			m_ObjectType = Types::OT_PICKABLE;
			GetComponent<ColliderBox>().lock()->SetCurRectHeight(GetComponent<ColliderBox>().lock()->GetHeight() / 2.f);
			pPhysics->SetCurSpeed(0.f);
			GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("KoopaShell"), TEXT("ShellIdle"));
			CScoreManager::GetInstance()->IncreaseScore(200);
		}
		else
		{
			if (pPhysics->GetCurSpeed() != 0.f)
			{
				pPhysics->SetCurSpeed(0.f);
				SetActorAct(Types::ACT_IDLE);
				GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("KoopaShell"), TEXT("ShellIdle"));
				CScoreManager::GetInstance()->IncreaseScore(200);
			}
			else
			{
				SetActorAct(Types::ACT_ATTACK);
				if (pPlayer->GetObjectPosition().x < GetObjectPosition().x)
				{
					SetObjectPosition(pPlayer->GetObjectPosition().x + 70, GetObjectPosition().y);
					pPhysics->SetCurSpeed(pPhysics->GetMaxSpeed());
					m_Direction = Types::DIR_RIGHT;
				}
				else
				{
					SetObjectPosition(pPlayer->GetObjectPosition().x - 70, GetObjectPosition().y);
					pPhysics->SetCurSpeed(-1 * pPhysics->GetMaxSpeed());
					m_Direction = Types::DIR_LEFT;
				}
			}
		}
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
		GetComponent<PhysicsComponent>().lock()->SetActive(false);
		GetComponent<ColliderBox>().lock()->SetActive(false);
		GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("KoopaNormal"), TEXT("Destroy"));
		CScoreManager::GetInstance()->IncreaseScore(200);
		CSoundManager::GetInstance()->SoundPlay(TEXT("SFXSpinStomp"));
		break;
	}
	
}
