#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CMushroom.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"


CMushroom::CMushroom()
{
}

CMushroom::~CMushroom()
{
}

bool CMushroom::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	if (!CActor::PostInit(data, pScene))
		return false;

	auto pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->SetSprite(TEXT("Mushroom")))
		return false;
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;
	

	return true;
}

void CMushroom::Init()
{
}

void CMushroom::Update(double dDeltaTime)
{
}

void CMushroom::Render(const HDC & hDC)
{
}
