#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CShell.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"




CShell::CShell()
	:m_dTimeElapsed(0.f), m_dLimitTime(0.f)
{
}

CShell::~CShell()
{
}

bool CShell::PostInit(const Types::ActorData & data, CGameScene * pScene)
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

bool CShell::Init()
{
	return false;
}

void CShell::Update(double dDeltaTime)
{
	m_dTimeElapsed += dDeltaTime;

	CActor::Update(dDeltaTime);

}

void CShell::Render(const HDC & hDC)
{
	auto pRender = GetComponent<RenderComponent>();
	if (pRender)
	{
		pRender->Draw(hDC);
	}

}

void CShell::ActorBehavior(double dDeltaTime)
{
	
	


}

void CShell::SetLimitTime(double dTime)
{
	if (dTime < 0.f)
		return;

	m_dLimitTime = dTime;

}

double CShell::GetLimitTime() const
{
	return m_dLimitTime;
}
