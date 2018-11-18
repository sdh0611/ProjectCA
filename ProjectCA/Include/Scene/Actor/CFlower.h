#pragma once

#include "..\..\..\stdafx.h"
#include "CPickup.h"


class CFlower : public CPickup {

public:
	CFlower();
	virtual ~CFlower();


public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);


private:
	virtual void ActorBehavior(double dDeltaTime) override;



};