#pragma once

#include "..\..\..\stdafx.h"
#include "..\..\Scene\CEntity.h"

class CEntity;

class CParticle
{
public:
	CParticle();
	~CParticle();


public:
	bool Init(CEntity* pOwner);
	void Update(double dDeltaTime);
	void Render(const HDC& hDC);


public:
	void SetAnimation();


private:
	TSTRING												m_strEffectName;
	CEntity*												m_pOwnerObject;
	std::unique_ptr<class AnimationRender>	m_pAnimationRender;

};