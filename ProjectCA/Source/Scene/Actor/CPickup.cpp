#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPickup.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
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

	m_ObjectType	= Types::OT_PICKUP;
	m_bStored		= false;

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

void CPickup::LateUpdate()
{
	CObject::LateUpdate();

	POSITION onScreenPosition = GetTransform().lock()->GetScreenPosition();

	if (onScreenPosition.x <  0.f - MAX_WIDTH / 2.f || onScreenPosition.x >CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraWidth() + MAX_WIDTH / 2.f)
	{
		SetActive(false);
		return;
	}
	else if (onScreenPosition.y <  0.f - MAX_HEIGHT / 2.f || onScreenPosition.y >CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraHeight() + MAX_HEIGHT / 2.f)
	{
		SetActive(false);
		return;
	}

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
