#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\Actor\CActor.h"
#include "..\..\Include\Scene\Actor\CCamera.h"

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
	puts("CameraManager Destroy");
}


bool CCameraManager::Init()
{
	m_iLastCameraID = 0;
	

	return true;
}


void CCameraManager::Update(double dDeltaTime)
{
}

WeakCameraPtr CCameraManager::CreateCamera(std::shared_ptr<CActor> pOwner, UINT iWidth, UINT iHeight)
{
	StrongCameraPtr pCamera = StrongCameraPtr(new CCamera);

	if (!pCamera->PostInit(pOwner, iWidth, iHeight, m_iLastCameraID++))
	{
		return StrongCameraPtr();
	}

	AddCamera(pCamera);

	if (m_pMainCamera.lock() == nullptr)
	{
		m_pMainCamera = pCamera;
	}

	return pCamera;
}

StrongCameraPtr CCameraManager::GetCamera(Types::CameraID id)
{
	for (auto& it : m_cameraList)
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
	m_cameraList.emplace_back(pCamera);

}

bool CCameraManager::DeleteCamera(StrongCameraPtr pCamera)
{
	for (auto it = m_cameraList.begin() ; it != m_cameraList.end(); ++it)
	{
		if ((*it) == pCamera)
		{
			m_cameraList.erase(it);
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
	m_pMainCamera = pCamera;
}
