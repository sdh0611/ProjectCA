#pragma once

#include "..\..\..\stdafx.h"
#include "..\CEntity.h"


class CInterface : public CEntity
{
public:
	CInterface();
	virtual ~CInterface();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);
	virtual void LateUpdate();


public:
	bool SetImage(const TSTRING& strImageName);

	
};