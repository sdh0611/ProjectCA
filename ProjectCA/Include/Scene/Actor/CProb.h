#pragma once

#include "..\..\..\stdafx.h"
#include "..\CObject.h"


class CProb : public CObject {

public:
	CProb();
	virtual ~CProb();


public:
	virtual bool PostInit(const OBJECT_DATA&, CScene*) override;
	virtual void Init() override;
	virtual void Update(double fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;


private:


	
private:



};