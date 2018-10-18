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
	std::weak_ptr<CActor> GetOwner();
	void SetOwner(std::shared_ptr<CActor> pOwner);
	Types::CameraID GetCameraID() const;


private:
	UINT								m_iWidth;
	UINT								m_iHeight;
	Types::POSITION				m_cameraPosition;
	std::shared_ptr<CActor>		m_pOwner;
	Types::CameraID				m_iCameraID;


};