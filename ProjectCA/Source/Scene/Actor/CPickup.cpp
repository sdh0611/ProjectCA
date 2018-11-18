#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPickup.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"

CPickup::CPickup()
{
}

CPickup::~CPickup()
{
}

bool CPickup::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	if (!CActor::PostInit(data, pScene))
		return false;

	//PhysicsComponent Ãß°¡
	auto pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 0.f, 0.f, 1300.f, 0.f))
		return false;
	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	m_ActorType	= Types::AT_PICKUP;
	m_bStored		= false;
	
	return true;
}

void CPickup::Init()
{
	CActor::Init();
	m_bStored = false;
}

void CPickup::Update(double dDeltaTime)
{
	CActor::Update(dDeltaTime);
}


void CPickup::ActorBehavior(double dDeltaTime)
{
}
