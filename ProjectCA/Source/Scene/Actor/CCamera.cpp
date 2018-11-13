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
	m_bActive						= true;
	m_fDestPosition				= 0.f;
	m_fCameraMoveSpeed		= pOwner->GetComponent<PhysicsComponent>().lock()->GetMaxSpeed() * 2.f;
	m_iWidth						= iWidth;
	m_iHeight						= iHeight;
	m_iCameraID					= id;
	m_pOwner						= pOwner;
	m_CameraMode				= CM_DEFAULT;

	m_CameraPosition				= std::move(POSITION(0.f - m_iWidth/2.f, 0));

	return true;
}

void CCamera::Init()
{
	m_CameraPosition.x = 0.f - m_iWidth / 2.f;
	m_CameraPosition.y = 0.f;

}

void CCamera::Update(double dDeltaTime)
{
	//PhysicsComponent* pPhysics = m_pOwner->GetComponent<PhysicsComponent>();
	//if(pPhysics)
	//{
	//	m_fCameraMoveSpeed = pPhysics->GetMaxSpeed() + pPhysics->GetSpeed();
	//	
	//}
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
	switch (m_CameraMode)
	{
	case CM_DEFAULT:
		ScrollDefault(dDeltaTime);
		break;
	case CM_SCROLL_HOR:
		ScrollHorizon(dDeltaTime);
		break;
	case CM_SCROLL_VER:
		ScrollVertical(dDeltaTime);
		break;
	case CM_AUTO:
		ScrollAuto(dDeltaTime);
		break;
	}

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

void CCamera::SetCameraMode(CameraMode mode)
{
	m_CameraMode = mode;
}

void CCamera::ScrollDefault(double dDeltaTime)
{
}

void CCamera::ScrollHorizon(double dDeltaTime)
{
	POSITION screenPosition = m_pOwner->GetComponent<TransformComponent>().lock()->GetScreenPosition();
	float fCurSpeed = m_pOwner->GetComponent<PhysicsComponent>().lock()->GetCurSpeed();
	float fCurJumpForce = m_pOwner->GetComponent<PhysicsComponent>().lock()->GetCurJumpForce();
	float fUnit = m_iWidth * (1.f / 5.f);

	if (m_fDestPosition != 0.f)
	{
		if (m_fDestPosition == fUnit * 2.f)
		{
			if (screenPosition.x >= fUnit * 1.89f)
			{
				m_CameraPosition.x += m_fCameraMoveSpeed * dDeltaTime;
			}
			else
			{
				m_fDestPosition = 0.f;
			}

		}
		else if (m_fDestPosition == fUnit * 3.f)
		{
			if (screenPosition.x <= fUnit * 3.11f)
			{
				m_CameraPosition.x -= m_fCameraMoveSpeed * dDeltaTime;
			}
			else
			{
				m_fDestPosition = 0.f;
			}

		}

	}
	else
	{
		if (fCurSpeed > 0.f)
		{
			if (screenPosition.x > fUnit * 4.f)
			{
				m_fDestPosition = fUnit * 2.f;
			}
			else if (screenPosition.x <= fUnit * 2.f && screenPosition.x >= fUnit * 1.9f)
			{
				m_CameraPosition.x += fCurSpeed * dDeltaTime;
			}

		}
		else if (fCurSpeed < 0.f)
		{
			if (screenPosition.x < fUnit)
			{
				m_fDestPosition = fUnit * 3.f;
			}
			else if (screenPosition.x >= fUnit * 3.f && screenPosition.x <= fUnit * 3.1f)
			{
				m_CameraPosition.x += fCurSpeed * dDeltaTime;
			}

		}

	}
}

void CCamera::ScrollVertical(double dDeltaTime)
{
}

void CCamera::ScrollAuto(double dDeltaTime)
{
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
