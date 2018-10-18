#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\Actor\CActor.h"
#include "..\..\Include\Scene\Actor\CCamera.h"

void CCameraManager::Update(double dDeltaTime)
{
}

WeakCameraPtr CCameraManager::CreateCamera(std::shared_ptr<CActor> pOwner, UINT iWidth, UINT iHeight, Types::CameraID id)
{
	StrongCameraPtr pCamera = StrongCameraPtr(new CCamera);

	if (pCamera->PostInit(pOwner, iWidth, iHeight, id))
	{
		return StrongCameraPtr();
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

WeakCameraPtr CCameraManager::GetMainCameara()
{
	return m_pMainCamera.lock();
}

void CCameraManager::ChangeMainCamera(StrongCameraPtr pCamera)
{
	m_pMainCamera = pCamera;
}
