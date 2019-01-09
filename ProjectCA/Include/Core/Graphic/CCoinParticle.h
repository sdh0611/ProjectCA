#pragma once

#include "..\..\..\stdafx.h"
#include "CParticle.h"

//NOTE: CParticle을 상속받은 Particle객체
//			Coin을 습득하거나 CoinBlock을 건드렸을 때 발생하는 Particle

class CCoinParticle : public CParticle
{
public:
	CCoinParticle();
	virtual ~CCoinParticle();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene) override;
	virtual void Init() override;
	virtual void Render(const HDC& hDC) override;


private:
	virtual void ParticleUpdate(double dDeltaTime) override;


};
