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
	//virtual bool Init(const Types::ActorData&) override;
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Update(double fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;


public:
	float GetEnemyDamage() const { return m_fDamage; }
	void SetEnemyDamage(float fDamage) { if (fDamage < 0.f) return; m_fDamage = fDamage; }


protected:
	virtual void ActorBehavior(double dDeltaTime) override;


protected:
	float m_fDamage;



};