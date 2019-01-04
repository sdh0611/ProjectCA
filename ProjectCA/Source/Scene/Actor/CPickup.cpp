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

	//Score의 기본 값은 0
	m_iScore			= 0;
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
	if (m_ObjectState != Types::OS_DEAD && m_ObjectState != Types::OS_DESTROYED)
	{
		UINT cameraWidth = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraWidth();
		UINT cameraHeight = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraHeight();
		POSITION cameraPosition = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraPosition();
		POSITION position = GetObjectPosition();

		if (IsActive())
		{
			if (position.x <  cameraPosition.x - cameraWidth
				|| position.x > cameraPosition.x + 2 * cameraWidth)
			{
				SetActive(false);
				return;
			}
			else if (position.y < cameraPosition.y - cameraHeight
				|| position.y > cameraPosition.y + 2 * cameraHeight)
			{
				SetActive(false);
				SetObjectState(Types::OS_DEAD);
				return;
			}
		}
	}
}

void CPickup::SetStored()
{
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
