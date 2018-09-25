#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CProb : public CActor {

public:
	CProb();
	virtual ~CProb();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual bool Init() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void ActorBehavior() override;

private:




private:


};