#pragma once

#include "..\..\..\stdafx.h"
#include "..\CObject.h"


class CInterface : public CObject
{
public:
	CInterface();
	virtual ~CInterface();


public:
	virtual bool PostInit(const OBJECT_DATA& data, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);
	virtual void LateUpdate();


public:
	bool SetImage(const TSTRING& strImageName);

	
};