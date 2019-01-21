#pragma once

#include "..\..\..\stdafx.h"
#include "CProp.h"

//NOTE: SpinBlock을 구현한 클래스.

class CSpinBlock : public CProp
{
public:
	CSpinBlock();
	virtual ~CSpinBlock();


public:
	virtual bool PostInit(const OBJECT_DATA&, CScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;


private:
	void BlockBehavior(double dDeltaTime);
	virtual void HandlingEvent(EVENT_TYPE type);


private:
	double m_dTimeElapsed;


};
