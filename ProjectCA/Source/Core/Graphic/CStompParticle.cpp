#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Graphic\CCoinParticle.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Sound\CSoundManager.h"
#include "..\..\..\Include\Core\Graphic\CStompParticle.h"

CStompParticle::CStompParticle()
{
}

CStompParticle::~CStompParticle()
{
}

bool CStompParticle::PostInit(const ENTITY_DATA & data, CScene * pScene)
{
	if (!CParticle::PostInit(data, pScene))
	{
		return false;
	}

	auto pRender = GetComponent<AnimationRender>().lock();
	if (!pRender->AddAnimation(0.1f, TEXT("Default"), TEXT("EffectStomp"), true, TEXT("Idle")))
	{
		return false;
	}
	pRender->SetPivotRatio(0.5f, 0.75f);

	m_dPlayTime = 0.1f;

	return true;
}

void CStompParticle::Init()
{
	CParticle::Init();
	m_dPlayTime = 0.1f;
}

void CStompParticle::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		GetComponent<AnimationRender>().lock()->Draw(hDC);
	}
}


void CStompParticle::ParticleUpdate(double dDeltaTime)
{
}
