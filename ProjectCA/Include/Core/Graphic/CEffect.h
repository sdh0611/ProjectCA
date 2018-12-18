#pragma once

#include "..\..\..\stdafx.h"

class CObject;

class CEffect
{
public:
	CEffect();
	~CEffect();


public:
	bool Init(CObject* pOwner);
	void Update(double dDeltaTime);
	void Render(const HDC& hDC);


public:
	void SetAnimation();


private:
	TSTRING												m_strEffectName;
	CObject*												m_pOwnerObject;
	std::unique_ptr<class AnimationRender>	m_pAnimationRender;

};