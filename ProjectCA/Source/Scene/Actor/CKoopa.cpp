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
	if (data.iActorWidth > MAX_ACTOR_SIZE || data.iActorHeight > MAX_ACTOR_SIZE)
		return false;

	if (data.actorPoint.x < 0 || data.actorPoint.x > MAX_WIDTH ||
		data.actorPoint.y < 0 || data.actorPoint.y > MAX_HEIGHT)
		return false;


	m_iActorWidth = data.iActorWidth;
	m_iActorHeight = data.iActorHeight;
	//m_actorPoint = m_spawnPoint = data.actorPoint;
	m_actorType = data.actorType;
	m_actorCurState = data.actorState;
	m_actorCurVerticalState = data.verticalState;
	m_actorHorizonalState = data.horizonalState;
	m_direction = data.direction;
	m_actorID = data.actorID;
	m_strActorTag = data.strActorTag;
	m_bActive = data.bActive;
	
	m_pOwnerScene = pScene;


	//TransformComponent 추가
	//NOTE(11.01) : TransformComponent에서 ScreenPosition값을 계산하므로 
	//					다른 컴포넌트들의 동작이 수행 된 뒤 동작해야함.
	//					떄문에 마지막에 추가하는 것으로 변경함.
	TransformComponent* pTransform = new TransformComponent;
	if (!pTransform->PostInit(this, data.actorPoint))
		return false;
	pTransform->SetPivotRatio(0.5f, 1.f);

	if (!AddComponent(pTransform, pTransform->GetComponentTag()))
		return false;


	//AIComponent (InputComponent) 초기화
	AIComponent* pAI = new AIComponent;
	if (!pAI->PostInit(this))
		return false;

	auto enemyBehavior = [](std::weak_ptr<CActor> pActor) ->void {

		if (pActor.expired())
			return;

		auto pTemp = pActor.lock();

		if (pTemp->GetActorPoint().x < 100.f) 
		{
			pTemp->SetActorDirection(Types::DIR_RIGHT);
		}
		else if (pTemp->GetActorPoint().x > 1000.f)
		{
			pTemp->SetActorDirection(Types::DIR_LEFT);
		}

	};
	pAI->SetDelegate(enemyBehavior);
	if (!AddComponent(pAI, pAI->GetComponentTag()))
		return false;

	//PhysicsComponent 초기화
	PhysicsComponent* pPhysics = new PhysicsComponent;
	if (!pPhysics->PostInit(this, 200.f, 200.f, 1300.f, 700.f))
		return false;

	pPhysics->SetCurSpeed(pPhysics->GetSpeed());

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//Collider 초기화
	ColliderBox* pCollider = new ColliderBox;
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionDelegater = [](std::shared_ptr<CActor> pOwner, std::shared_ptr<CActor> pOther, Collider::CollisionType type)->void {

		PhysicsComponent * pPhysics = pOwner->GetComponent<PhysicsComponent>();

		switch (pOther->GetActorType()) {

		case Types::AT_PLAYER:
			switch (type)
			{
			case Collider::COLLISION_TOP:
				puts("TOP");
				pOwner->SetActorState(Types::AS_DAMAGED);
				pPhysics->SetCurSpeed(0.f);
				//pOwner->SetActive(false);
				break;
			case Collider::COLLISION_RIGHT:
			case Collider::COLLISION_LEFT:
				if (pOwner->GetActorState() == Types::AS_DAMAGED)
				{
					if (type == Collider::COLLISION_RIGHT)
					{
						pPhysics->AddForceX(-600.f);
					}
					else if (type == Collider::COLLISION_LEFT)
					{
						pPhysics->AddForceX(600.f);
					}
				}
				else
				{
					pOwner->FlipActorDirection();
				}

				break;
			}
		case Types::AT_PROB:
			//point = static_cast<PhysicsComponent*>(pComponent)->GetLastActorPoint();
			//point = pComponent->GetLastActorPoint();
			switch (type)
			{
			case Collider::COLLISION_BOT:
				pPhysics->SetGrounded(true);
				pOwner->SetActorPoint(pOwner->GetActorPoint().x, pOther->GetActorPoint().y - pOther->GetActorHeight());
				//pOwner->SetActorState(Types::AS_IDLE);
				pOwner->SetActorVerticalState(Types::VS_IDLE);
				break;
			case Collider::COLLISION_LEFT:
			case Collider::COLLISION_RIGHT:
				pOwner->FlipActorDirection();
				break;
			}
		}

	};

	pCollider->SetDelegate(onCollisionDelegater);
	pCollider->SetSize(m_iActorWidth*0.45, m_iActorHeight*0.8);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;

	//RenderComponent 초기화
	AnimationRender* pRender = new AnimationRender;
	if (!pRender->PostInit(this))
		return false;

	//if (!pRender->AddAnim(0.25f, TEXT("KoopaGreenWalkRight"), m_iActorWidth, m_iActorHeight, true, true, TEXT("WalkRight")))
	//	return false;

	//if (!pRender->AddAnim(0.25f, TEXT("KoopaGreenWalkLeft"), m_iActorWidth, m_iActorHeight, true, true, TEXT("WalkLeft")))
	//	return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"),TEXT("KoopaGreenWalkRight"), m_iActorWidth, m_iActorHeight, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkLeft"), m_iActorWidth, m_iActorHeight, true, TEXT("RunLeft")))
		return false;

	//if (!pRender->AddAnim(0.25f, TEXT("KoopaGreenWalkRight"), m_iActorWidth, m_iActorHeight, true, true, TEXT("JumpRight")))
	//	return false;

	//if (!pRender->AddAnim(0.25f, TEXT("KoopaGreenWalkLeft"), m_iActorWidth, m_iActorHeight, true, true, TEXT("JumpLeft")))
	//	return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkRight"), m_iActorWidth, m_iActorHeight, true, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkLeft"), m_iActorWidth, m_iActorHeight, true, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnimation(100.f, TEXT("KoopaNormal"), TEXT("KoopaGreenDamaged"), m_iActorWidth, m_iActorHeight, false, TEXT("Damaged")))
		return false;

	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	
	return true;
}

bool CKoopa::Init()
{
	return CEnemy::Init();
}

void CKoopa::Update(double dDeltaTime)
{
	CEnemy::Update(dDeltaTime);
}

void CKoopa::Render(const HDC & hDC)
{
	auto it = m_componentTable.find(TEXT("RenderComponent"));
	if (it != m_componentTable.end())
		static_cast<RenderComponent*>((*it).second)->Draw(hDC);

}

void CKoopa::ActorBehavior(double dDeltaTime)
{
	TransformComponent* pTransform = GetComponent<TransformComponent>();
	PhysicsComponent* pPhysics = GetComponent<PhysicsComponent>();

	if (m_actorCurState != Types::AS_DAMAGED)
	{
		float fWalkSpeed = pPhysics->GetSpeed();

		if (m_direction == Types::DIR_LEFT)
		{
			pPhysics->SetCurSpeed(-1 * fWalkSpeed);
		}
		else if (m_direction == Types::DIR_RIGHT)
		{
			pPhysics->SetCurSpeed(fWalkSpeed);
		}
	}

	pTransform->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);

}
