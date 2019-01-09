#pragma once

#include "..\..\..\stdafx.h"
#include "CParticle.h"


class CStompParticle : public CParticle
{
public:
	CStompParticle();
	virtual ~CStompParticle();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene) override;
	virtual void Init() override;
	virtual void Render(const HDC& hDC);


private:
	virtual void ParticleUpdate(double dDeltaTime) override;


};