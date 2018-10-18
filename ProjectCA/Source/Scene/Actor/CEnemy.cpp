#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CEnemy.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"



CEnemy::CEnemy()
	:CActor()
{
}

CEnemy::~CEnemy()
{
}

//bool CEnemy::Init(const Types::ActorData &)
//{
//
//
//
//	return true;
//}

bool CEnemy::PostInit(const Types::ActorData& data, CGameScene* pScene)
{

	////기본 Actor의 속성 초기화
	//if (data.iActorWidth > MAX_ACTOR_SIZE || data.iActorHeight > MAX_ACTOR_SIZE)
	//	return false;

	//if (data.actorPoint.x < 0 || data.actorPoint.x > MAX_WIDTH || 
	//	data.actorPoint.y < 0 || data.actorPoint.y > MAX_HEIGHT)
	//	return false;

	//
	//m_iActorWidth = data.iActorWidth;
	//m_iActorHeight = data.iActorHeight;
	//m_actorPoint = m_spawnPoint = data.actorPoint;
	//m_actorType = Types::AT_ENEMY;
	//m_actorCurState = m_actorPreState = data.actorState;
	//m_actorCurVerticalState = m_actorPreVerticalState = data.verticalState;
	//m_direction = data.direction;
	//m_actorVector = data.vector;
	//m_actorID = data.actorID;
	//m_strActorTag = data.strActorTag;
	//m_pOwnerScene = pScene;
	//m_bActive = data.bActive;

	////AIComponent (InputComponent) 초기화
	//AIComponent* pAI= new AIComponent;
	//if (!pAI->PostInit(this))
	//	return false;
	//
	//auto enemyBehavior = [](std::weak_ptr<CActor> pActor) ->void{
	//	
	//	if (pActor.expired())
	//		return;

	//	auto pTemp = pActor.lock();

	//	if (pTemp->GetActorPoint().y >= MAX_HEIGHT - pTemp->GetActorHeight()) {
	//		//pTemp->SetActorDirection(Types::DIR_UP);
	//		pTemp->FlipVector();
	//	}

	//	if (pTemp->GetActorPoint().y < 1.f) {
	//		//pTemp->SetActorDirection(Types::DIR_DOWN);
	//		pTemp->FlipVector();
	//	}
	//};
	//pAI->SetDelegate(enemyBehavior);
	//if (!AddComponent(pAI, pAI->GetComponentTag()))
	//	return false;

	////PhysicsComponent 초기화
	//PhysicsComponent* pPhysics = new PhysicsComponent;
	//if (!pPhysics->PostInit(this, 300.f, 400.f, 600.f, -1200.f))
	//	return false;

	//if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
	//	return false;

	////Collider 초기화
	//ColliderBox* pCollider = new ColliderBox;
	//if (!pCollider->PostInit(this))
	//	return false;

	//auto onCollisionDelegater = [](std::shared_ptr<CActor> pOwner, std::shared_ptr<CActor> pOther)->void {
	//		
	//	switch (pOther->GetActorType()) {

	//	case Types::AT_PLAYER:
	//		pOwner->FlipVector();
	//		break;
	//	case Types::AT_PROB:
	//		pOwner->FlipVector();
	//		break;
	//	}
	//
	//};

	//pCollider->SetDelegate(onCollisionDelegater);

	//if (!AddComponent(pCollider, pCollider->GetComponentTag()))
	//	return false;
	//
	return true;
}

bool CEnemy::Init()
{
	m_actorPoint = m_spawnPoint;
	

	return true;
}

void CEnemy::Update(double fDeltaTime)
{
	CActor::Update(fDeltaTime);

}

void CEnemy::Render(const HDC & hDC)
{
	Rectangle(hDC, m_actorPoint.x, m_actorPoint.y, m_actorPoint.x + m_iActorWidth, m_actorPoint.y + m_iActorHeight);
	if (static_cast<Collider*>(GetComponent(TEXT("Collider")))->GetIsCollision()) {
		TextOut(hDC, m_actorPoint.x, m_actorPoint.y, TEXT("TRUE"), sizeof(TEXT("TRUE")));
	}
	else {
		TextOut(hDC, m_actorPoint.x, m_actorPoint.y, TEXT("FALSE"), sizeof(TEXT("FALSE")));
	}

}

void CEnemy::ActorBehavior()
{

}


