#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"



class CPickup : public CActor 
{
public:
	CPickup();
	virtual ~CPickup();


public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC) override = 0 ;


private:
	virtual void ActorBehavior(double dDeltaTime) override;


private:
	bool m_bStored;


};