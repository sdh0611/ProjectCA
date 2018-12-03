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
	CActor::PostInit(data, pScene);

	//AIComponent (InputComponent) 초기화
	std::shared_ptr<AIComponent> pAI = std::make_shared<AIComponent>();
	if (!pAI->PostInit(this))
		return false;

	if (!AddComponent(pAI, pAI->GetComponentTag()))
		return false;

	//PhysicsComponent 초기화
	std::shared_ptr<PhysicsComponent> pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 0.f, 0.f, 0.f, 0.f))
		return false;

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//Collider 초기화
	std::shared_ptr<ColliderBox> pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto onCollision = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {




	};
	pCollider->SetOnCollision(onCollision);
	

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	return true;
}

void CShell::Init()
{
}

void CShell::Update(double dDeltaTime)
{
	m_dTimeElapsed += dDeltaTime;

	CActor::Update(dDeltaTime);

}

void CShell::Render(const HDC & hDC)
{
	auto pRender = GetComponent<RenderComponent>();
	if (!pRender.expired())
	{
		pRender.lock()->Draw(hDC);
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
