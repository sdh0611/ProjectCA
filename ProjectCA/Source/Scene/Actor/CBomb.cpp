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

bool CBomb::Init()
{
	return true;
}

void CBomb::Update(double fDeltaTime)
{

}

void CBomb::Render(const HDC & hDC)
{
	auto it = m_componentTable.find(TEXT("RenderComponent"));

	if (it != m_componentTable.end())
		static_cast<RenderComponent*>((*it).second)->Draw(hDC);

}

void CBomb::Destroy()
{

}

void CBomb::ActorBehavior(double dDeltaTime)
{
}
