#pragma once

#include "..\..\..\stdafx.h"
#include "CPickup.h"


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


};