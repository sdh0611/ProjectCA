#pragma once


#include "..\..\..\stdafx.h"
#include "CActor.h"


class CFireball : public CActor {

public:
	CFireball();
	virtual ~CFireball();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;


public:
	void SetDamage(float fDamage) { if (fDamage < 0.f) return; m_fDamage = fDamage; }
	void SetOwnerActor(CActor* pActor) { m_pOwnerActor = pActor; }
	void SetFireballActive();


public:
	float				GetFireballDamage() const { return m_fDamage; }
	CActor * const	GetOwnerActor() const { return m_pOwnerActor; }


private:
	virtual void ActorBehavior(double dDeltaTime) override;


private:
	float		m_fDamage;
	CActor*	m_pOwnerActor;

};