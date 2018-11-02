#pragma once

#include "..\..\..\stdafx.h"


class CActor;

class CCamera {

public:
	CCamera();
	~CCamera();


public:
	bool PostInit(std::shared_ptr<CActor> pOwner, UINT iWidth, UINT iHeight, Types::CameraID id);
	void Init();
	void Update(double dDeltaTime);

public:
	void SetActive(bool bActive);
	void SetCameraSize(UINT iWidth, UINT iHeight);
	void SetOwner(std::shared_ptr<CActor> pOwner);
	void Move(float fx, float fy);
	void SetCameraPosition(const POSITION& position);
	void SetCameraPosition(float fx, float fy);
	void SetCameraPositionX(float fx);
	void SetCameraPositionY(float fy);



public:
	bool							IsActive() const;
	UINT							GetCameraWidth() const;
	UINT							GetCameraHeight() const;
	Types::CameraID			GetCameraID() const;
	const POSITION&			GetCameraPosition() const;
	std::weak_ptr<CActor>	GetOwner();


public:
	void AdjustCameraPosition(double dDeltaTime);


private:
	bool								m_bActive;
	UINT								m_iWidth;
	UINT								m_iHeight;
	POSITION						m_CameraPosition;
	std::shared_ptr<CActor>		m_pOwner;
	Types::CameraID				m_iCameraID;


};