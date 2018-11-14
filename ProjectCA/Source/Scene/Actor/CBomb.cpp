#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CBomb.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"



CBomb::CBomb()
	:CActor()
{
}

CBomb::~CBomb()
{
}

bool CBomb::PostInit(const Types::ActorData &, CGameScene* pScene)
{
	return true;
}

void CBomb::Init()
{
}

void CBomb::Update(double fDeltaTime)
{

}

void CBomb::Render(const HDC & hDC)
{
	auto pRender = GetComponent<RenderComponent>();
	if (!pRender.expired())
		pRender.lock()->Draw(hDC);

}


void CBomb::ActorBehavior(double dDeltaTime)
{
}
