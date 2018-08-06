#pragma once

#include "..\..\..\stdafx.h"
#include "CObject.h"


class CPlayer : public CObject{

public:
	CPlayer(UINT iWidth, UINT iHeight, Types::Point startPoint);
	virtual ~CPlayer();


public:
	virtual bool Init(Types::Point point, UINT iWidth, UINT iHeight,
		Types::ObjectState state = Types::OS_IDLE, Types::Direction dir = Types::DIR_IDLE) override;
	virtual void Update(float fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	//virtual void LateUpdate(float fDeltaTime) override;

	
public:


private:
	


private:
	Types::Point			m_StartPoint;

};