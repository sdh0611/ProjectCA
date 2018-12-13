#pragma once

#include "..\..\..\stdafx.h"
#include "CRandomBlock.h"


class CPickupBlock : public CRandomBlock
{
public:
	CPickupBlock();
	virtual ~CPickupBlock();


public:
	virtual bool PostInit(const OBJECT_DATA& objectData, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);
	virtual void LateUpdate();



};