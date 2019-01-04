#pragma once

#include "..\..\..\stdafx.h"
#include "CPickup.h"

//NOTE: CPickup을 상속받아 작성한 Pickup Class
//			Player가 습득하게 될 경우 FireMario가 됨.

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