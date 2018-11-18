#pragma once

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"

class CCamera;
class CObject;

typedef std::shared_ptr<CCamera> StrongCameraPtr;
typedef std::weak_ptr<CCamera> WeakCameraPtr;
typedef std::list<StrongCameraPtr> StrongCameraList;

class CCameraManager : public Singleton<CCameraManager> {

	DECLARE_SINGLETON(CCameraManager)

public:
	bool Init();
	void Update(double dDeltaTime);


public:
	void					ResetCameraList();
	WeakCameraPtr	CreateCamera(std::shared_ptr<CObject> pOwner, UINT iWidth, UINT iHeight);
	void					AddCamera(StrongCameraPtr pCamera);
	bool					DeleteCamera(StrongCameraPtr pCamera);
	void					ChangeMainCamera(StrongCameraPtr pCamera);
	void					Clear();


public:
	WeakCameraPtr GetMainCamera();
	StrongCameraPtr GetCamera(Types::CameraID);


private:



private:
	StrongCameraList m_CameraList;
	WeakCameraPtr	m_pMainCamera;
	Types::CameraID	m_iLastCameraID;

};