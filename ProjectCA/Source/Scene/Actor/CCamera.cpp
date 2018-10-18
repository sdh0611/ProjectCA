#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

bool CCamera::PostInit(std::shared_ptr<CActor> pOwner, UINT iWidth, UINT iHeight, Types::CameraID id)
{
	m_pOwner = pOwner;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iCameraID = id;

	m_cameraPosition = pOwner->GetActorPoint();

	return true;
}

void CCamera::Init()
{


}

void CCamera::Update(double dDeltaTime)
{


}

std::weak_ptr<CActor> CCamera::GetOwner()
{
	if(m_pOwner == nullptr)
		return std::weak_ptr<CActor>();

	return m_pOwner;
}

void CCamera::SetOwner(std::shared_ptr<CActor> pOwner)
{
	m_pOwner = pOwner;
}

Types::CameraID CCamera::GetCameraID() const
{
	return m_iCameraID;
}
