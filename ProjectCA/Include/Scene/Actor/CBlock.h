#pragma once

#include "..\..\..\stdafx.h"
#include "..\CObject.h"


class CBlock : public CObject
{
public:
	CBlock();
	virtual ~CBlock();


public:
	virtual bool PostInit(const OBJECT_DATA&, CScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;







};