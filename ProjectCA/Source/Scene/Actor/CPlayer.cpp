#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Scene\CGameScene.h"



CPlayer::CPlayer()
	:CActor()
{
}

CPlayer::~CPlayer()
{
}

//bool CPlayer::Init(const Types::ActorData &)
//{
//	
//
//	return true;
//}

bool CPlayer::PostInit(const Types::ActorData& data, CGameScene* pScene)
{
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
	
	m_pOwnerScene = pScene;

	//PlayerInputComponent (InputComponent) 초기화
	PlayerInputComponent* pAI = new PlayerInputComponent;
	if (!pAI->PostInit(this))
		return false;

	if (!AddComponent(pAI, pAI->GetComponentTag()))
		return false;

	//PhysicsComponent 초기화
	PhysicsComponent* pPhysics = new PhysicsComponent;
	if (!pPhysics->PostInit(this, 300.f, 700.f, 1300.f, 700.f))
		return false;

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//Collider 초기화
	ColliderBox* pCollider = new ColliderBox;
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionDelegater = [](std::shared_ptr<CActor> pOwner, std::shared_ptr<CActor> pOther)->void 
	{
		ComponentBase* pComponent = nullptr;
		Types::Point point;

		switch (pOther->GetActorType()) 
		{
		case Types::AT_ENEMY:
			//pComponent = pOwner->GetComponent(TEXT("PhysicsComponent"));
			//point = static_cast<PhysicsComponent*>(pComponent)->GetLastActorPoint();
			//pOwner->SetActorPoint(point.x, point.y);
			//pOwner->GetOwnerScene()->ResetScene();
			break;
		case Types::AT_PROB:
			PhysicsComponent* pComponent = static_cast<PhysicsComponent*>(
				pOwner->GetComponent(TEXT("PhysicsComponent")));
			//point = static_cast<PhysicsComponent*>(pComponent)->GetLastActorPoint();
			//point = pComponent->GetLastActorPoint();
			pComponent->SetGrounded(true);
			pOwner->SetActorState(Types::AS_IDLE);
			pOwner->SetActorVerticalState(Types::VS_IDLE);
			//pOwner->SetActorPoint(point.x, point.y); // 이부분 떄문에 PROB위에서 움직이질못함.
			break;
		}
	};

	pCollider->SetDelegate(onCollisionDelegater);
	pCollider->SetSize(m_iActorWidth*0.45, m_iActorHeight*0.9);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;

	//RenderComponent 추가
	RenderComponent* pRender = new RenderComponent;
	if (!pRender->PostInit(this))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerIdleRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerIdleLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnim(0.2f, TEXT("PlayerWalkRight"), m_iActorWidth, m_iActorHeight, true, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnim(0.2f, TEXT("PlayerWalkLeft"), m_iActorWidth, m_iActorHeight, true, true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnim(0.05f, TEXT("PlayerRunRight"), m_iActorWidth, m_iActorHeight, true, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnim(0.05f, TEXT("PlayerRunLeft"), m_iActorWidth, m_iActorHeight, true, true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerLookupRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerLookupLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerSitdownRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerSitdownLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerJumpRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerJumpLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerFalldownRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerFalldownLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerRunJumpRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerRunJumpLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerTurnRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("TurnRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerTurnLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("TurnLeft")))
		return false;

	pRender->SetUseOffset(true);

	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;


	return true;
}

bool CPlayer::Init()
{
	m_actorPoint = m_spawnPoint;
		
	for (auto& it : m_componentTable)
		it.second->Init();

	return true;
}

void CPlayer::Update(double dDeltaTime)
{
	CActor::Update(dDeltaTime);

}

void CPlayer::Render(const HDC & hDC)
{
	auto it = m_componentTable.find(TEXT("RenderComponent"));
	if (it != m_componentTable.end())
		static_cast<RenderComponent*>((*it).second)->Draw(hDC);

	auto pColliderIt = m_componentTable.find(TEXT("Collider"));
	if(pColliderIt != m_componentTable.end())
		static_cast<Collider*>((*pColliderIt).second)->DrawCollider(hDC);

	if (static_cast<Collider*>(GetComponent(TEXT("Collider")))->GetIsCollision()) {
		TextOut(hDC, m_actorPoint.x, m_actorPoint.y, TEXT("TRUE"), sizeof(TEXT("TRUE")));
	}
	else {
		TextOut(hDC, m_actorPoint.x, m_actorPoint.y, TEXT("FALSE"), sizeof(TEXT("FALSE")));
	}

	static TCHAR buf[15];
	wsprintf(buf, TEXT("Position : (%3ld, %3ld)"), (LONG)m_actorPoint.x, (LONG)m_actorPoint.y);
	TextOut(hDC, 0, 20, buf, sizeof(buf));

}

void CPlayer::ActorBehavior()
{








}
