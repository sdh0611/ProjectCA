#pragma once

#include "..\..\..\stdafx.h"

//NOTE: Screen상에서 그려질 화면을 담는 Camera 객체 구현.
//			하나의 Owner Entity를 가질 수 있음.

class CEntity;

class CCamera {

public:
	enum CameraMode {
		CM_DEFAULT, CM_SCROLL_HOR, CM_SCROLL_HOR_VER, CM_AUTO, CM_STATIC
	};

public:
	CCamera();
	~CCamera();


public:
	bool PostInit(std::shared_ptr<CEntity> pOwner, UINT iWidth, UINT iHeight, CAMERA_ID id);
	void Init();
	void Update(double dDeltaTime);


public:
	void SetActive(bool bActive);
	void SetCameraSize(UINT iWidth, UINT iHeight);
	void SetOwner(std::shared_ptr<CEntity> pOwner);
	void Move(float fx, float fy);
	void SetCameraPosition(const POSITION& position);
	void SetCameraPosition(float fx, float fy);
	void SetCameraPositionX(float fx);
	void SetCameraPositionY(float fy);
	void SetCameraMode(CameraMode mode);


private:
	void ScrollDefault(double dDeltaTime);
	void ScrollHorizon(double dDeltaTime);
	void ScrollVertical(double dDeltaTime);
	void ScrollAuto(double dDeltaTime);


public:
	bool							IsActive() const;
	UINT							GetCameraWidth() const;
	UINT							GetCameraHeight() const;
	float							GetCameraCurSpeed() const;
	CAMERA_ID					GetCameraID() const;
	const POSITION&			GetCameraPosition() const;
	std::weak_ptr<CEntity>	GetOwner();


public:
	void AdjustCameraPosition(double dDeltaTime);


private:
	bool								m_bActive;
	float								m_fDestPosition;
	float								m_fCameraCurSpeed;
	float								m_fCameraMaxSpeed;
	float								m_fOwnerCurSpeed;
	float								m_fOwnerCurJumpForce;
	UINT								m_iWidth;
	UINT								m_iHeight;
	POSITION						m_CameraPosition;
	POSITION						m_OwnerScreenPosition;
	std::weak_ptr<CEntity>		m_pOwner;
	CAMERA_ID						m_iCameraID;
	CameraMode					m_CameraMode;


};