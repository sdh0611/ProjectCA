#pragma once

#include "..\..\..\stdafx.h"
#include "..\CObject.h"


class CBlock : public CObject
{
public:
	CBlock();
	virtual ~CBlock();


public:
	virtual bool PostInit(const OBJECT_DATA& objectData, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC) = 0;
	virtual void LateUpdate();


public:



private:



};