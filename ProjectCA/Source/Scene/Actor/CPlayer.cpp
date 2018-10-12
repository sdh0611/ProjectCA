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
	m_actorPreJumpState = m_actorJumpState = Types::JS_IDLE;
	m_direction = data.direction;
	m_actorVector = data.vector;
	m_actorID = data.actorID;
	m_strActorTag = data.strActorTag;
	m_bActive = data.bActive;
	
	m_pOwnerScene = pScene;

	//PlayerInputComponent (InputComponent) 초기화
	PlayerInputComponent* pAI = new PlayerInputComponent;
	if (!pAI->Init(this))
		return false;

	if (!AddComponent(pAI, pAI->GetComponentTag()))
		return false;

	//PhysicsComponent 초기화
	PhysicsComponent* pPhysics = new PhysicsComponent;
	if (!pPhysics->Init(this, 300.f, 700.f, 1300.f, 700.f))
		return false;

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//Collider 초기화
	ColliderBox* pCollider = new ColliderBox;
	if (!pCollider->Init(this))
		return false;

	auto onCollisionDelegater = [](std::shared_ptr<CActor> pOwner, std::shared_ptr<CActor> pOther)->void 
	{
		ComponentBase* pComponent = nullptr;
		Types::Point point;

		switch (pOther->GetActorType()) 
		{
		case Types::OT_ENEMY:
			//pComponent = pOwner->GetComponent(TEXT("PhysicsComponent"));
			//point = static_cast<PhysicsComponent*>(pComponent)->GetLastActorPoint();
			//pOwner->SetActorPoint(point.x, point.y);
			//pOwner->GetOwnerScene()->ResetScene();
			break;
		case Types::OT_PROB:
			PhysicsComponent* pComponent = static_cast<PhysicsComponent*>(
				pOwner->GetComponent(TEXT("PhysicsComponent")));
			//point = static_cast<PhysicsComponent*>(pComponent)->GetLastActorPoint();
			//point = pComponent->GetLastActorPoint();
			pComponent->SetGrounded(true);
			pOwner->SetActorState(Types::OS_IDLE);
			pOwner->SetActorJumpState(Types::JS_IDLE);
			//pOwner->SetActorPoint(point.x, point.y); // 이부분 떄문에 PROB위에서 움직이질못함.
			break;
		}
	};

	pCollider->SetDelegate(onCollisionDelegater);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;

	//RenderComponent 추가
	RenderComponent* pRender = new RenderComponent;
	if (!pRender->Init(this))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerIdleRight"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerIdleLeft"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnim(0.2f, TEXT("PlayerWalkRight"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, true, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnim(0.2f, TEXT("PlayerWalkLeft"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, true, true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnim(0.05f, TEXT("PlayerRunRight"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, true, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnim(0.05f, TEXT("PlayerRunLeft"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, true, true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerLookupRight"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerLookupLeft"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerSitdownRight"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerSitdownLeft"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerJumpRight"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerJumpLeft"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerFalldownRight"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerFalldownLeft"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerRunJumpRight"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerRunJumpLeft"), SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, false, false, TEXT("RunJumpLeft")))
		return false;


	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;


	return true;
}

bool CPlayer::Init()
{
	m_actorPoint = m_spawnPoint;
		

	return true;
}

void CPlayer::Update(double fDeltaTime)
{
	CActor::Update(fDeltaTime);

}

void CPlayer::Render(const HDC & hDC)
{
	auto it = m_componentTable.find(TEXT("RenderComponent"));

	if (it != m_componentTable.end())
		static_cast<RenderComponent*>((*it).second)->Draw(hDC);

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
