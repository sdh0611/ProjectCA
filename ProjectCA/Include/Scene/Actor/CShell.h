#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CShell : public CActor{

public:
	CShell();
	virtual ~CShell();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual bool Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void ActorBehavior(double dDeltaTime) override;


public:
	void SetLimitTime(double dTime);



public:
	double GetLimitTime() const;



private:
	double			m_dTimeElapsed;
	double			m_dLimitTime;

};