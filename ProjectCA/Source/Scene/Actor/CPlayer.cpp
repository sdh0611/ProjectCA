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
	m_actorState = data.actorState;
	m_direction = data.direction;
	m_actorVector = data.vector;
	m_actorID = data.actorID;
	m_strActorTag = data.strActorTag;
	m_bActive = data.bActive;
	
	m_pOwnerScene = pScene;

	//AIComponent (InputComponent) 초기화
	PlayerInputComponent* pAI = new PlayerInputComponent;
	if (!pAI->Init(this))
		return false;

	if (!AddComponent(pAI, pAI->GetComponentTag()))
		return false;

	//PhysicsComponent 초기화
	PhysicsComponent* pPhysics = new PhysicsComponent;
	if (!pPhysics->Init(this, 500.f, 1300.f, -600.f))
		return false;

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//Collider 초기화
	ColliderBox* pCollider = new ColliderBox;
	if (!pCollider->Init(this))
		return false;

	auto onCollisionDelegater = [](std::shared_ptr<CActor> pOwner, std::shared_ptr<CActor> pOther)->void {

		ComponentBase* pComponent = nullptr;
		Types::Point point;


		switch (pOther->GetActorType()) {
		case Types::OT_ENEMY:
			//pComponent = pOwner->GetComponent(TEXT("PhysicsComponent"));
			//point = static_cast<PhysicsComponent*>(pComponent)->GetLastActorPoint();
			//pOwner->SetActorPoint(point.x, point.y);
			pOwner->GetOwnerScene()->ResetScene();
			break;
		case Types::OT_PROB:
			pComponent = pOwner->GetComponent(TEXT("PhysicsComponent"));
			point = static_cast<PhysicsComponent*>(pComponent)->GetLastActorPoint();
			pOwner->SetActorPoint(point.x, point.y); // 이부분 떄문에 PROB위에서 움직이질못함.
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

	if (!pRender->AddAnim(3.f, TEXT("PlayerIdleRight"), true, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerIdleLeft"), true, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnim(0.2f, TEXT("PlayerMoveRight"), true, TEXT("MoveRight")))
		return false;

	if (!pRender->AddAnim(0.2f, TEXT("PlayerMoveLeft"), true, TEXT("MoveLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerLookupRight"), true, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerLookupLeft"), true, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerSitdownRight"), true, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerSitdownLeft"), true, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnim(0.05f, TEXT("PlayerRunRight"), true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnim(0.05f, TEXT("PlayerRunLeft"), true, TEXT("RunLeft")))
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
	//Code for test
	//RECT rect = {m_actorPoint.x, m_actorPoint.y, m_actorPoint.x + m_iActorWidth, m_actorPoint.y + m_iActorHeight};
	//HBRUSH hBrush = CreateSolidBrush(RGB(255.f, 0.f, 0.f));

	//FillRect(hDC, &rect, hBrush);

	//DeleteObject(hBrush);

	auto it = m_componentTable.find(TEXT("RenderComponent"));

	if (it != m_componentTable.end())
		static_cast<RenderComponent*>((*it).second)->Draw(hDC);

	//Rectangle(hDC, m_actorPoint.x, m_actorPoint.y, m_actorPoint.x + m_iActorWidth, m_actorPoint.y + m_iActorHeight);
	if (static_cast<Collider*>(GetComponent(TEXT("Collider")))->GetIsCollision()) {
		TextOut(hDC, m_actorPoint.x, m_actorPoint.y, TEXT("TRUE"), sizeof(TEXT("TRUE")));
	}
	else {
		TextOut(hDC, m_actorPoint.x, m_actorPoint.y, TEXT("FALSE"), sizeof(TEXT("FALSE")));
	}

}

void CPlayer::ActorBehavior()
{
}
