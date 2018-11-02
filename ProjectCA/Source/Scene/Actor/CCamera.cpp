#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"



CCamera::CCamera()
{
}

CCamera::~CCamera()
{
	puts("Camera Destroy");
}

bool CCamera::PostInit(std::shared_ptr<CActor> pOwner, UINT iWidth, UINT iHeight, Types::CameraID id)
{
	m_bActive				= true;
	m_iWidth				= iWidth;
	m_iHeight				= iHeight;
	m_iCameraID			= id;
	m_pOwner				= pOwner;

	m_CameraPosition		= std::move(POSITION(0, 0));

	return true;
}

void CCamera::Init()
{
	m_CameraPosition.x = 0.f;
	m_CameraPosition.y = 0.f;

}

void CCamera::Update(double dDeltaTime)
{
	AdjustCameraPosition(dDeltaTime);
}

std::weak_ptr<CActor> CCamera::GetOwner()
{
	if(m_pOwner == nullptr)
		return std::weak_ptr<CActor>();

	return m_pOwner;
}

void CCamera::AdjustCameraPosition(double dDeltaTime)
{
	TransformComponent* pTransform = m_pOwner->GetComponent<TransformComponent>();
	float fCurSpeed = m_pOwner->GetComponent<PhysicsComponent>()->GetCurSpeed();
	float fCurJumpForce = m_pOwner->GetComponent<PhysicsComponent>()->GetCurJumpForce();

	//if (m_CameraPosition.x < 0.f)
	//{
	//	m_CameraPosition.x = 0.f;
	//}
	//else
	//{
		if (pTransform->GetScreenPosition().x >m_iWidth / 2.f)
		{
			m_CameraPosition.x += fCurSpeed*dDeltaTime;
		}
		
	//}
	

}

void CCamera::SetActive(bool bActive)
{
	m_bActive = bActive;
}

void CCamera::SetCameraSize(UINT iWidth, UINT iHeight)
{
	m_iWidth	= iWidth;
	m_iHeight	= iHeight;

}

void CCamera::SetOwner(std::shared_ptr<CActor> pOwner)
{
	m_pOwner = pOwner;
}

void CCamera::Move(float fx, float fy)
{
	m_CameraPosition.x += fx;
	m_CameraPosition.y -= fy;
}

void CCamera::SetCameraPosition(const POSITION & position)
{
	m_CameraPosition = position;

}

void CCamera::SetCameraPosition(float fx, float fy)
{
	m_CameraPosition.x = fx;
	m_CameraPosition.y = fy;
}

void CCamera::SetCameraPositionX(float fx)
{
	m_CameraPosition.x = fx;
}

void CCamera::SetCameraPositionY(float fy)
{
	m_CameraPosition.y = fy;
}

bool CCamera::IsActive() const
{
	return m_bActive;
}

UINT CCamera::GetCameraWidth() const
{
	return m_iWidth;
}

UINT CCamera::GetCameraHeight() const
{
	return m_iHeight;
}

Types::CameraID CCamera::GetCameraID() const
{
	return m_iCameraID;
}

const POSITION & CCamera::GetCameraPosition() const
{
	return m_CameraPosition;
}
