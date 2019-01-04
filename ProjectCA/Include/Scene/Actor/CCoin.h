#pragma once

#include "..\..\..\stdafx.h"
#include "CPickup.h"

//NOTE: CPickup을 상속받아 작성한 Pickup Class.
//			동전을 구현함.

class CCoin : public CPickup
{
public:
	CCoin();
	virtual ~CCoin();


public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;




};