#pragma once

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"

class CCamera;
class CActor;

typedef std::shared_ptr<CCamera> StrongCameraPtr;
typedef std::weak_ptr<CCamera> WeakCameraPtr;
typedef std::list<StrongCameraPtr> StrongCameraList;

class CCameraManager : public Singleton<CCameraManager> {

	DECLARE_SINGLETON(CCameraManager)

public:
	bool Init();
	void Update(double dDeltaTime);


public:
	WeakCameraPtr CreateCamera(std::shared_ptr<CActor> pOwner, UINT iWidth, UINT iHeight);
	StrongCameraPtr GetCamera(Types::CameraID);
	void AddCamera(StrongCameraPtr pCamera);
	bool DeleteCamera(StrongCameraPtr pCamera);
	WeakCameraPtr GetMainCamera();
	void ChangeMainCamera(StrongCameraPtr pCamera);
	


private:



private:
	StrongCameraList m_cameraList;
	WeakCameraPtr	m_pMainCamera;
	Types::CameraID	m_iLastCameraID;

};