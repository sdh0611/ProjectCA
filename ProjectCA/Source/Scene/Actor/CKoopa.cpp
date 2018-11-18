#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CKoopa.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
//#include "..\..\..\Include\Core\Components\HPComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"


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
	CActor::PostInit(data, pScene);

	//AIComponent (InputComponent) 초기화
	std::shared_ptr<AIComponent> pAI = std::make_shared<AIComponent>();
	if (!pAI->PostInit(this))
		return false;

	auto enemyBehavior = [](CActor* pActor) ->void {

		if (pActor->GetObjectPosition().x < 100.f) 
		{
			pActor->SetActorDirection(Types::DIR_RIGHT);
		}
		else if (pActor->GetObjectPosition().x > 1000.f)
		{
			pActor->SetActorDirection(Types::DIR_LEFT);
		}

	};
	pAI->SetDelegate(enemyBehavior);
	if (!AddComponent(pAI, pAI->GetComponentTag()))
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
		auto pOtherActor = static_cast<CActor*>(pOther);

		switch (pOtherActor->GetActorType()) {
		case Types::AT_ENEMY:
			if (m_ActorCurState != Types::AS_DAMAGED)
			{
				if (pOtherActor->GetActorState() == Types::AS_DAMAGED)
				{
					if (pOtherActor->GetComponent<PhysicsComponent>().lock()->GetCurSpeed() != 0.f)
					{
						SetActorState(Types::AS_DEAD);
						GetComponent<ColliderBox>().lock()->SetActive(false);
						pPhysics->SetCurJumpForce(300.f);
						pPhysics->SetGrounded(false);
						//SetActive(false);
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
				else//otherActor != Types::AS_DAMAGED
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
			else//Owner == DAMAGED
			{
				if (pOtherActor->GetActorState() == Types::AS_DAMAGED)
				{
					if (pOtherActor->GetComponent<PhysicsComponent>().lock()->GetCurSpeed() != 0.f)
					{
						SetActorState(Types::AS_DEAD);
						pPhysics->AddForceY(300.f);
						pPhysics->SetGrounded(false);
						GetComponent<ColliderBox>().lock()->SetActive(false);
						//SetActive(false);
					}

				}
			}
			break;
		case Types::AT_PROB:
			switch (type){
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
		case Types::AT_PLAYER:
			switch (type){
			case Collider::COLLISION_TOP:
				SetActorState(Types::AS_DAMAGED);
				GetComponent<ColliderBox>().lock()->SetCurRectHeight(GetComponent<ColliderBox>().lock()->GetHeight()/2.f);
				pPhysics->SetCurSpeed(0.f);
				//GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("KoopaNormal"), TEXT("Damaged"));
				break;
			case Collider::COLLISION_RIGHT:
				if (GetActorState() == Types::AS_DAMAGED)
				{
					if (pPhysics->GetCurSpeed() == 0.f)
					{
						pPhysics->AddForceX(-750.f);
					}

				}
				else
				{
					FlipActorDirection();
				}
				break;
			case Collider::COLLISION_LEFT:
				if (GetActorState() == Types::AS_DAMAGED)
				{
					if (pPhysics->GetCurSpeed() == 0.f)
					{
						pPhysics->AddForceX(750.f);
					}

				}
				else
				{
					FlipActorDirection();
				}
				break;
			
			}
			break;
		//case Types::AT_THROWN:
		//	SetActorState(Types::AS_DAMAGED);
		//	GetComponent<ColliderBox>().lock()->SetCurRectHeight(GetComponent<ColliderBox>().lock()->GetHeight() / 2.f);
		//	pPhysics->SetCurSpeed(0.f);
		//	break;

		}

	};

	pCollider->SetDelegate(onCollisionDelegater);
	pCollider->SetSize(m_iObjectWidth*0.45, m_iObjectHeight*0.8);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//RenderComponent 초기화
	std::shared_ptr<AnimationRender> pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"),TEXT("KoopaGreenWalkRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenRunRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenRunLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenRunRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenRunLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("KoopaNormal"), TEXT("KoopaGreenDamaged"), m_iObjectWidth, m_iObjectHeight, false, TEXT("Damaged")))
		return false;


	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	
	return true;
}

void CKoopa::Init()
{
	CEnemy::Init();
}

void CKoopa::Update(double dDeltaTime)
{
	CEnemy::Update(dDeltaTime);
	if (!m_bActive)
	{
		puts("inactive");
	}
}

void CKoopa::Render(const HDC & hDC)
{
	auto pRender = GetComponent(TEXT("RenderComponent"));
	if (!pRender.expired())
		STATIC_POINTER_CAST(RenderComponent, pRender.lock())->Draw(hDC);

}

void CKoopa::ActorBehavior(double dDeltaTime)
{
	auto pPhysics = GetComponent<PhysicsComponent>().lock();
	auto pTransform = GetTransform().lock();
	if(m_ActorCurState != Types::AS_DEAD)
	{
		if (m_ActorCurState != Types::AS_DAMAGED)
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
