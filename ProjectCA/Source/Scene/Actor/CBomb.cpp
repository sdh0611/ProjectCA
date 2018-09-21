#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CBomb.h"

CBomb::CBomb()
	: m_pOwnerActor(nullptr)
{
}

CBomb::~CBomb()
{
}

bool CBomb::Init(const Types::ActorData &)
{
	return true;
}

void CBomb::Update(float fDeltaTime)
{

}

void CBomb::Render(const HDC & hDC)
{

}

void CBomb::Destroy()
{

}
