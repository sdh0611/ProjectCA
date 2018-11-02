#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CProb.h" 
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"


CProb::CProb()
	:CActor()
{
}

CProb::~CProb()
{
}

bool CProb::PostInit(const Types::ActorData & data, CGameScene* pScene)
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
	m_actorVector = data.vector;
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
	printf("Position : (%f, %f), Pivot : (%f, %f)\n", pTransform->GetPosition().x, pTransform->GetPosition().y, pTransform->GetPivot().x, pTransform->GetPivot().y);

	if (!AddComponent(pTransform, pTransform->GetComponentTag()))
		return false;


	//AIComponent (InputComponent) 초기화
	AIComponent* pAI = new AIComponent;
	if (!pAI->PostInit(this))
		return false;

	if (!AddComponent(pAI, pAI->GetComponentTag()))
		return false;

	//PhysicsComponent 초기화
	PhysicsComponent* pPhysics = new PhysicsComponent;
	if (!pPhysics->PostInit(this, 0.f, 0.f, 0.f, 0.f))
		return false;

	//if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
	//	return false;

	//Collider 초기화
	ColliderBox* pCollider = new ColliderBox;
	if (!pCollider->PostInit(this))
		return false;

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;
	




	return true;
}

bool CProb::Init()
{
	return true;
}

void CProb::Update(double fDeltaTime)
{
	CActor::Update(fDeltaTime);
}

void CProb::Render(const HDC & hDC)
{
	POSITION pivot = GetComponent<TransformComponent>()->GetScreenPivot();

	Rectangle(hDC, pivot.x, pivot.y, pivot.x + m_iActorWidth, pivot.y + m_iActorHeight);

}

void CProb::ActorBehavior(double dDeltaTime)
{
}
