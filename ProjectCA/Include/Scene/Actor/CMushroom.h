#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CMushroom : public CActor {

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