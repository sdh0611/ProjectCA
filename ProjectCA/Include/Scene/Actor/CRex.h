#pragma once

#include "..\..\..\stdafx.h"
#include "CEnemy.h"


class CRex : public CEnemy
{
	enum RexState {
		REX_IDLE, REX_DAMAGED
	};

public:
	CRex();
	virtual ~CRex();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;


private:
	virtual void ActorBehavior(double dDeltaTime) override;
	virtual void DeadProcess(double dDeltaTime) override;
	virtual void HandlingEvent(EVENT_TYPE type) override;


private:
	RexState GetRexState() const;


private:
	double	m_dElapsedTime;
	RexState	m_RexState;
	

};