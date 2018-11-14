#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CProb.h" 
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
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
	CActor::PostInit(data, pScene);

	//Collider 초기화
	std::shared_ptr<ColliderBox> pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;
	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//ImageRender 추가
	std::shared_ptr<ImageRender> pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	return true;
}

void CProb::Init()
{
}

void CProb::Update(double fDeltaTime)
{
	CActor::Update(fDeltaTime);
}

void CProb::Render(const HDC & hDC)
{
	POSITION pivot = GetComponent<TransformComponent>().lock()->GetScreenPivot();

	Rectangle(hDC, pivot.x, pivot.y, pivot.x + m_iObjectWidth, pivot.y + m_iObjectHeight);

}

void CProb::ActorBehavior(double dDeltaTime)
{
}
