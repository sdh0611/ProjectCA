#pragma once


#include "..\..\..\stdafx.h"
#include "CActor.h"

//NOTE: FireMario가 공격할 때 나오는 불꽃을 구현한 Class
//			Player객체가 관리함.

class CFireball : public CActor {

public:
	CFireball();
	virtual ~CFireball();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void LateUpdate() override;

public:
	void SetDamage(float fDamage) { if (fDamage < 0.f) return; m_fDamage = fDamage; }
	void SetOwnerActor(CActor* pActor) { m_pOwnerActor = pActor; }
	void SetFireballActive();
	void SetFireballInactive();


public:
	float				GetFireballDamage() const { return m_fDamage; }
	CActor * const	GetOwnerActor() const { return m_pOwnerActor; }


private:
	virtual void ActorBehavior(double dDeltaTime) override;

	
private:
	virtual void SetOwnerObject(std::shared_ptr<CObject> pOwner) override;


private:
	float		m_fDamage;
	CActor*	m_pOwnerActor;

};