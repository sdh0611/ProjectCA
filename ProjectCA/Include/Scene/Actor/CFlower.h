#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CFlower : public CActor {

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