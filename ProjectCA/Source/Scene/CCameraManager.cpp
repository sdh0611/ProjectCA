#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CCameraManager.h"


CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
	Clear();
}


bool CCameraManager::Init()
{
	m_LastCameraID = 0;
	

	return true;
}


void CCameraManager::Update(double dDeltaTime)
{
}

void CCameraManager::ResetCameraList()
{
	for (const auto& camera : m_CameraList)
	{
		camera->Init();
	}
}

WeakCameraPtr CCameraManager::CreateCamera(std::shared_ptr<CEntity> pOwner, UINT iWidth, UINT iHeight)
{
	StrongCameraPtr pCamera = StrongCameraPtr(new CCamera);

	if (!pCamera->PostInit(pOwner, iWidth, iHeight, m_LastCameraID++))
	{
		return StrongCameraPtr();
	}

	AddCamera(pCamera);

	if (m_pMainCamera.expired())
	{
		m_pMainCamera = pCamera;
	}

	return pCamera;
}

StrongCameraPtr CCameraManager::GetCamera(CAMERA_ID id)
{
	for (auto& it : m_CameraList)
	{
		if (it->GetCameraID() == id)
		{
			return it;
		}
	}

	return StrongCameraPtr();
}

void CCameraManager::AddCamera(StrongCameraPtr pCamera)
{
	m_CameraList.emplace_back(pCamera);

}

bool CCameraManager::DeleteCamera(StrongCameraPtr pCamera)
{
	for (auto it = m_CameraList.begin() ; it != m_CameraList.end(); ++it)
	{
		if ((*it) == pCamera)
		{
			m_CameraList.erase(it);
			return true;
		}
	}

	return false;
}

WeakCameraPtr CCameraManager::GetMainCamera()
{
	if (m_pMainCamera.expired())
	{
		return WeakCameraPtr();
	}
	return m_pMainCamera.lock();

}

void CCameraManager::ChangeMainCamera(StrongCameraPtr pCamera)
{
	puts("ChangeMainCamera");
	m_pMainCamera = pCamera;
}

void CCameraManager::ChangeMainCameraMode(CCamera::CameraMode mode)
{
	m_pMainCamera.lock()->SetCameraMode(mode);
}

void CCameraManager::Clear()
{
	m_CameraList.clear();
	m_pMainCamera.reset();
	m_LastCameraID = 0;
}
