#pragma once

#include "..\..\..\stdafx.h"


class CObject;

class CCamera {

public:
	enum CameraMode {
		CM_DEFAULT, CM_SCROLL_HOR, CM_SCROLL_VER, CM_AUTO, CM_STATIC
	};

public:
	CCamera();
	~CCamera();


public:
	bool PostInit(std::shared_ptr<CObject> pOwner, UINT iWidth, UINT iHeight, Types::CameraID id);
	void Init();
	void Update(double dDeltaTime);


public:
	void SetActive(bool bActive);
	void SetCameraSize(UINT iWidth, UINT iHeight);
	void SetOwner(std::shared_ptr<CObject> pOwner);
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
	Types::CameraID			GetCameraID() const;
	const POSITION&			GetCameraPosition() const;
	std::weak_ptr<CObject>	GetOwner();


public:
	void AdjustCameraPosition(double dDeltaTime);


private:
	bool								m_bActive;
	float								m_fDestPosition;
	float								m_fCameraMoveSpeed;
	float								m_fOwnerCurSpeed;
	float								m_fOwnerCurJumpForce;
	UINT								m_iWidth;
	UINT								m_iHeight;
	POSITION						m_CameraPosition;
	POSITION						m_OwnerScreenPosition;
	std::weak_ptr<CObject>		m_pOwner;
	Types::CameraID				m_iCameraID;
	CameraMode					m_CameraMode;


};