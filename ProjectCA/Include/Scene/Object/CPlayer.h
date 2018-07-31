#pragma once

#include "..\..\..\stdafx.h"
#include "CObject.h"


class CPlayer : public CObject{

public:
	CPlayer(UINT iWidth, UINT iHeight);
	virtual ~CPlayer();


public:
	virtual bool Init() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	//virtual void LateUpdate(float fDeltaTime) override;

	
public:


private:
	


private:


};