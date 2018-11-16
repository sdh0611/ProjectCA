#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CKoopa.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
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
	if (!pPhysics->PostInit(this, 200.f, 200.f, 1300.f, 700.f))
		return false;

	pPhysics->SetCurSpeed(pPhysics->GetSpeed());

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//Collider 초기화
	std::shared_ptr<ColliderBox> pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionDelegater = [&](CObject* pOther, Collider::CollisionType type)->void {

		auto pPhysics = GetComponent<PhysicsComponent>().lock();
		//auto pOwnerActor = static_cast<CActor*>(pOwner);
		//auto pPhysics = pOther->GetComponent<PhysicsComponent>().lock();
		auto pOtherActor = static_cast<CActor*>(pOther);

		switch (static_cast<CActor*>(pOther)->GetActorType()) {

		case Types::AT_PLAYER:
			switch (type)
			{
			case Collider::COLLISION_TOP:
				//puts("TOP");
				SetActorState(Types::AS_DAMAGED);
				pPhysics->SetCurSpeed(0.f);
				//pOwnerActor->SetActive(false);
				break;
			case Collider::COLLISION_RIGHT:
				if (GetActorState() == Types::AS_DAMAGED)
				{
					pPhysics->AddForceX(-600.f);
				}
				break;
			case Collider::COLLISION_LEFT:
				if (GetActorState() == Types::AS_DAMAGED)
				{
					pPhysics->AddForceX(600.f);
				}
				else
				{
					FlipActorDirection();
				}
				break;
			
			}
			break;
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

	//if (!pRender->AddAnim(0.25f, TEXT("KoopaGreenWalkRight"), m_iObjectWidth, m_iObjectHeight, true, true, TEXT("WalkRight")))
	//	return false;

	//if (!pRender->AddAnim(0.25f, TEXT("KoopaGreenWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, true, TEXT("WalkLeft")))
	//	return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"),TEXT("KoopaGreenWalkRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunLeft")))
		return false;

	//if (!pRender->AddAnim(0.25f, TEXT("KoopaGreenWalkRight"), m_iObjectWidth, m_iObjectHeight, true, true, TEXT("JumpRight")))
	//	return false;

	//if (!pRender->AddAnim(0.25f, TEXT("KoopaGreenWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, true, TEXT("JumpLeft")))
	//	return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunJumpLeft")))
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
}

void CKoopa::Render(const HDC & hDC)
{
	auto pRender = GetComponent(TEXT("RenderComponent"));
	if (!pRender.expired())
		STATIC_POINTER_CAST(RenderComponent, pRender.lock())->Draw(hDC);

}

void CKoopa::ActorBehavior(double dDeltaTime)
{
	std::shared_ptr<TransformComponent> pTransform = GetComponent<TransformComponent>().lock();
	std::shared_ptr<PhysicsComponent> pPhysics = GetComponent<PhysicsComponent>().lock();

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

	pTransform->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);

}
