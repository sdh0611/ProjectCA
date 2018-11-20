#pragma once

#include "..\..\..\stdafx.h"
#include "CEnemy.h"


class CGoomba : public CEnemy
{

public:
	CGoomba();
	virtual ~CGoomba();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);


private:
	virtual void ActorBehavior(double dDeltaTime) override;




};