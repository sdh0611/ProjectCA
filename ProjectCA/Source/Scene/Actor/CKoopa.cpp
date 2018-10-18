#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CKoopa.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"


CKoopa::CKoopa()
	:CEnemy()
{
}

CKoopa::~CKoopa()
{
}

bool CKoopa::PostInit(const Types::ActorData & data, CGameScene * pScene)
{

	puts("Init Koopa");
	//기본 Actor의 속성 초기화
	if (data.iActorWidth > MAX_ACTOR_SIZE || data.iActorHeight > MAX_ACTOR_SIZE)
		return false;

	if (data.actorPoint.x < 0 || data.actorPoint.x > MAX_WIDTH ||
		data.actorPoint.y < 0 || data.actorPoint.y > MAX_HEIGHT)
		return false;


	m_iActorWidth = data.iActorWidth;
	m_iActorHeight = data.iActorHeight;
	m_actorPoint = m_spawnPoint = data.actorPoint;
	m_actorType = data.actorType;
	m_actorCurState = m_actorPreState = data.actorState;
	m_actorPreVerticalState = m_actorCurVerticalState = data.verticalState;
	m_actorHorizonalState = data.horizonalState;
	m_direction = data.direction;
	m_actorVector = data.vector;
	m_actorID = data.actorID;
	m_strActorTag = data.strActorTag;
	m_bActive = data.bActive;

	//AIComponent (InputComponent) 초기화
	AIComponent* pAI = new AIComponent;
	if (!pAI->PostInit(this))
		return false;

	auto enemyBehavior = [](std::weak_ptr<CActor> pActor) ->void {

		if (pActor.expired())
			return;

		auto pTemp = pActor.lock();

		//if (pTemp->GetActorPoint().x >= 600.f) 
		//{
		//	pTemp->SetActorDirection(Types::DIR_LEFT);
		//}

		if (pTemp->GetActorPoint().x < 100.f) 
		{
			pTemp->SetActorDirection(Types::DIR_RIGHT);
		}
	};
	pAI->SetDelegate(enemyBehavior);
	if (!AddComponent(pAI, pAI->GetComponentTag()))
		return false;

	//PhysicsComponent 초기화
	PhysicsComponent* pPhysics = new PhysicsComponent;
	if (!pPhysics->PostInit(this, 200.f, 200.f, 1300.f, 700.f))
		return false;

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//Collider 초기화
	ColliderBox* pCollider = new ColliderBox;
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionDelegater = [](std::shared_ptr<CActor> pOwner, std::shared_ptr<CActor> pOther)->void {

		switch (pOther->GetActorType()) {

		case Types::AT_PLAYER:
			puts("Collision with Player");
			pOwner->FlipActorDirection();
			break;
		case Types::AT_PROB:
			PhysicsComponent * pComponent = static_cast<PhysicsComponent*>(
				pOwner->GetComponent(TEXT("PhysicsComponent")));
			//point = static_cast<PhysicsComponent*>(pComponent)->GetLastActorPoint();
			//point = pComponent->GetLastActorPoint();
			pComponent->SetGrounded(true);
			pOwner->SetActorState(Types::AS_IDLE);
			pOwner->SetActorVerticalState(Types::VS_IDLE);
			break;
		}

	};

	pCollider->SetDelegate(onCollisionDelegater);
	pCollider->SetSize(m_iActorWidth*0.45, m_iActorHeight*0.8);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;

	//RenderComponent 초기화
	RenderComponent* pRender = new RenderComponent;
	if (!pRender->PostInit(this))
		return false;

	if (!pRender->AddAnim(0.25f, TEXT("KoopaGreenWalkRight"), m_iActorWidth, m_iActorHeight, true, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnim(0.25f, TEXT("KoopaGreenWalkLeft"), m_iActorWidth, m_iActorHeight, true, true, TEXT("WalkLeft")))
		return false;

	pRender->SetUseOffset(true);

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

	auto pColliderIt = m_componentTable.find(TEXT("Collider"));
	if (pColliderIt != m_componentTable.end())
		static_cast<Collider*>((*pColliderIt).second)->DrawCollider(hDC);

}
