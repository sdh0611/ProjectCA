#pragma once

#include "..\..\..\stdafx.h"

class CEntity;

class CEffect
{
public:
	CEffect();
	~CEffect();


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