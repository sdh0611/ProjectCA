#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CKoopa.h"
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


CKoopa::CKoopa()
	:CEnemy()
{
}

CKoopa::~CKoopa()
{
	puts("Koopa Destroy");
}

bool CKoopa::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	//기본 Actor의 속성 초기화
	CActor::PostInit(data, pScene);

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
					else//otherActor != Types::OS_DAMAGED
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
				else//state == DAMAGED
				{
					if (pOther->GetObjectState() == Types::OS_DAMAGED)
					{
						if (pOther->GetComponent<PhysicsComponent>().lock()->GetCurSpeed() != 0.f)
						{
							puts("Dead");
							SetObjectState(Types::OS_DEAD);
							pPhysics->AddForceY(300.f);
							pPhysics->SetGrounded(false);
							GetComponent<ColliderBox>().lock()->SetActive(false);
							CScoreManager::GetInstance()->IncreaseScore(200);
						}

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
			case Types::OT_GROUND:
				if (type == Collider::COLLISION_BOT)
				{
					if (GetObjectPosition().y > pOther->GetObjectPosition().y - pOther->GetObjectHeight() + fIntersectLength)
						return;
					pPhysics->SetGrounded(true);
					pPhysics->SetCurJumpForce(0.f);
					SetActorVerticalState(Types::VS_IDLE);
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
				}
				break;
			case Types::OT_PLAYER:
				switch (type) {
				case Collider::COLLISION_TOP:
					if (m_ObjectState != Types::OS_DAMAGED)
					{
						SetObjectState(Types::OS_DAMAGED);
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
							GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("KoopaShell"), TEXT("ShellIdle"));
							CScoreManager::GetInstance()->IncreaseScore(200);
						}
						else
						{
							if (pOther->GetObjectPosition().x < GetObjectPosition().x)
							{

								SetObjectPosition(pOther->GetObjectPosition().x + 70, GetObjectPosition().y);
								pPhysics->SetCurSpeed(pPhysics->GetMaxSpeed());
								m_Direction = Types::DIR_RIGHT;
							}
							else
							{
								SetObjectPosition(pOther->GetObjectPosition().x - 70, GetObjectPosition().y);
								pPhysics->SetCurSpeed(-1 * pPhysics->GetMaxSpeed());
								m_Direction = Types::DIR_LEFT;

							}
						}
					}
					break;
				case Collider::COLLISION_RIGHT:
					if (GetObjectState() == Types::OS_DAMAGED)
					{
						if (pPhysics->GetCurSpeed() == 0.f)
						{
							if (pPhysics->GetCurSpeed() == 0.f)
							{
								SetObjectPosition(pOther->GetObjectPosition().x - 70, GetObjectPosition().y);
								pPhysics->SetCurSpeed(-1 * pPhysics->GetMaxSpeed());
								m_Direction = Types::DIR_LEFT;
							}
						}
					}
					break;
				case Collider::COLLISION_LEFT:
					if (GetObjectState() == Types::OS_DAMAGED)
					{
						if (pPhysics->GetCurSpeed() == 0.f)
						{
							SetObjectPosition(pOther->GetObjectPosition().x + 70, GetObjectPosition().y);
							pPhysics->SetCurSpeed(pPhysics->GetMaxSpeed());
							m_Direction = Types::DIR_RIGHT;
						}

					}
					break;

				}
				break;

			}
		}
		//else
		//{
		//	if (pOther->GetObjectType() == Types::OT_ENEMY)
		//	{
		//		SetObjectState(Types::OS_DEAD);
		//		GetComponent<ColliderBox>().lock()->SetActive(false);
		//		pPhysics->SetCurJumpForce(300.f);
		//		pPhysics->SetGrounded(false);
		//		CScoreManager::GetInstance()->IncreaseScore(200);
		//	}

		//}

	};

	pCollider->SetDelegate(onCollisionDelegater);
	//pCollider->SetSize(m_iObjectWidth*0.45, m_iObjectHeight*0.8);

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

	
	return true;
}

void CKoopa::Init()
{
	CEnemy::Init();
	m_pOwnerObject.reset();
	GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("KoopaNormal"), TEXT("WalkLeft"));
}

void CKoopa::Update(double dDeltaTime)
{
	CEnemy::Update(dDeltaTime);

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

void CKoopa::ChangeAnimationClip()
{


}

void CKoopa::ActorBehavior(double dDeltaTime)
{
	auto pPhysics = GetComponent<PhysicsComponent>().lock();
	auto pTransform = GetTransform().lock();
	auto pRender = GetComponent<AnimationRender>().lock();
	float fSpeed = 0.f;
	if(m_ObjectState != Types::OS_DEAD)
	{
		if (m_ObjectState != Types::OS_DAMAGED)
		{
			fSpeed = pPhysics->GetSpeed();

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
			fSpeed = pPhysics->GetMaxSpeed();
			if (pPhysics->GetCurSpeed() != 0.f)
			{
				if (m_Direction == Types::DIR_LEFT)
				{
					//pPhysics->SetCurSpeed(-1 * fSpeed);
					pRender->ChangeAnimation(TEXT("ThrownLeft"));
				}
				else if (m_Direction == Types::DIR_RIGHT)
				{
					//pPhysics->SetCurSpeed(fSpeed);
					pRender->ChangeAnimation(TEXT("ThrownRight"));
				}
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

void CKoopa::DeadProcess(double dDeltaTime)
{
	auto pPhysics = GetComponent<PhysicsComponent>().lock();
	auto pCamera = CCameraManager::GetInstance()->GetMainCamera().lock();
	POSITION position = GetTransform().lock()->GetScreenPosition();
	if (position.y > pCamera->GetCameraHeight() + m_iObjectHeight)
	{
		m_bActive = false;
		return;
	}
	GetTransform().lock()->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);


}
