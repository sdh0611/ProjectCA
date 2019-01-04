#pragma once

/*
	NOTE:
		Enemy 객체들이 상속받게 될 Class
		
*/


#include "..\..\..\stdafx.h"
#include "CActor.h"



class CEnemy : public CActor {

public:
	CEnemy();
	virtual ~CEnemy();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Update(double fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void LateUpdate() override;


public:
	virtual void DeadProcess(double dDeltaTime) = 0;


protected:
	virtual void ActorBehavior(double dDeltaTime) = 0;


protected:
	double m_dTimeElapsed;


};