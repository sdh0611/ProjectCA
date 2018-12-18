#pragma once

#include "..\..\..\stdafx.h"
#include "CRandomBlock.h"


class CCoinBlock : public CRandomBlock
{
public:
	CCoinBlock();
	virtual ~CCoinBlock();


public:
	virtual bool PostInit(const OBJECT_DATA& objectData, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC) override;


public:
	void SetLimitTime(double dTime);


private:
	void Behavior(double dDeltaTime);
	virtual void HandlingEvent(EVENT_TYPE type) override;



private:
	double		m_dTimeElapsed;
	double		m_dLimitTime;
	POSITION	m_DrawPosition;

};