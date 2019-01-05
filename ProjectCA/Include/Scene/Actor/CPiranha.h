#pragma once

#include "..\..\..\stdafx.h"
#include "CEnemy.h"


class CPiranha : public CEnemy
{

public:
	CPiranha();
	virtual ~CPiranha();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;


public:
	virtual void DeadProcess(double dDeltaTime) override;


private:
	virtual void ActorBehavior(double dDeltaTime) override;
	virtual void HandlingEvent(EVENT_TYPE type) override;


private:
	float	m_fPeak;

};
