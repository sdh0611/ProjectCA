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

	////PhysicsComponent Ãß°¡
	//auto pPhysics = std::make_shared<PhysicsComponent>();
	//if (!pPhysics->PostInit(this, 0.f, 0.f, 1300.f, 0.f))
	//	return false;
	//if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
	//	return false;

	m_ObjectType = Types::OT_PICKUP;
	m_bStored = false;
	m_iScore = 10;

	return true;
}

void CPickup::Init()
{
	CActor::Init();
	m_bActive = true;
	m_bStored = false;
}

void CPickup::Update(double dDeltaTime)
{
	CActor::Update(dDeltaTime);
}

void CPickup::SetStored()
{
	printf("Stored : %d\n", m_ObjectID);
	if (!GetComponent<PhysicsComponent>().expired())
	{
		GetComponent<PhysicsComponent>().lock()->SetActive(false);
	}

	m_bStored = true;
}

bool CPickup::IsStored() const
{
	return m_bStored;
}

CPickup::PickupType CPickup::GetPickupType() const
{
	return m_Type;
}


void CPickup::ActorBehavior(double dDeltaTime)
{
}
