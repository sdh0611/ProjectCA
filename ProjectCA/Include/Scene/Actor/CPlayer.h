#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CPlayer : public CActor {

public:
	CPlayer();
	virtual ~CPlayer();


public:
	//virtual bool Init(const Types::ActorData&) override;
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual bool Init() override;
	virtual void Update(double fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void ActorBehavior(double dDeltaTime) override;


private:



private:



};