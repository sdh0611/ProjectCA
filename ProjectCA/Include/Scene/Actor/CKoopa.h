#pragma once

#include "..\..\..\stdafx.h"
#include "CEnemy.h"


class CKoopa : public CEnemy {

public:
	CKoopa();
	virtual ~CKoopa();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual bool Init() override;
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);


private:
	virtual void ActorBehavior(double dDeltaTime) override;

};