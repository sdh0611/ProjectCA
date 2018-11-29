#pragma once

#include "..\..\..\stdafx.h"
#include "CEnemy.h"


class CKoopa : public CEnemy {

public:
	CKoopa();
	virtual ~CKoopa();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);


private:
	void ChangeAnimationClip();
	virtual void ActorBehavior(double dDeltaTime) override;
	virtual void DeadProcess(double dDeltaTime) override;


private:
	bool m_bDead;	


};