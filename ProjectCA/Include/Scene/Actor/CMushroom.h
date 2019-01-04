#pragma once

#include "..\..\..\stdafx.h"
#include "CPickup.h"

//NOTE: CPickup을 상속받아 작성한 Pickup Class
//			Player가 습득할 경우 BigMario가 됨.

class CMushroom : public CPickup {

public:
	CMushroom();
	virtual ~CMushroom();


public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);

	
private:
	virtual void ActorBehavior(double dDeltaTime) override;
	virtual void HandlingEvent(EVENT_TYPE type) override;


};