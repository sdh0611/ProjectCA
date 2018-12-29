#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Graphic\CCoinParticle.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Sound\CSoundManager.h"



CCoinParticle::CCoinParticle()
{
}

CCoinParticle::~CCoinParticle()
{
}

bool CCoinParticle::PostInit(const ENTITY_DATA & data, CScene * pScene)
{
	if (!CParticle::PostInit(data, pScene))
	{
		return false;
	}

	if (!GetComponent<AnimationRender>().lock()->AddAnimation(0.5f, TEXT("Default"), TEXT("CoinAnimation"), true, TEXT("Idle")))
	{
		return false;
	}

	m_fYSpeed		= 400.f;
	m_dPlayTime	= 1.f;
	
	return true;
}

void CCoinParticle::Init()
{	
	CParticle::Init();
	m_fYSpeed = 400.f;
}

void CCoinParticle::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		GetComponent<AnimationRender>().lock()->Draw(hDC);
	}

}

void CCoinParticle::LateUpdate()
{
	if (m_bActive)
	{
		CEntity::LateUpdate();
	}
}

void CCoinParticle::ParticleUpdate(double dDeltaTime)
{
	auto pTransform = GetComponent<TransformComponent>().lock();
	pTransform->Move(m_fXSpeed, m_fYSpeed * dDeltaTime);
	if (m_fYSpeed > 0.f)
	{
		m_fYSpeed -= 10.f;
	}
	else
	{
		m_fYSpeed = 0.f;
	}

}
