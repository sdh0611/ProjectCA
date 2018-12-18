#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Graphic\CEffect.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"


CEffect::CEffect()
{
}

CEffect::~CEffect()
{
}

bool CEffect::Init(CObject* pOwner)
{
	m_pOwnerObject = pOwner;

	m_pAnimationRender = std::make_unique<AnimationRender>();
	if (!m_pAnimationRender->PostInit(pOwner))
		return false;

	
	return true;
}

void CEffect::Update(double dDeltaTime)
{
}

void CEffect::Render(const HDC & hDC)
{
}

void CEffect::SetAnimation()
{
}
