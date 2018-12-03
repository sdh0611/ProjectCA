#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\HPComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"

HPComponent::HPComponent()
{
}

HPComponent::~HPComponent()
{
}

bool HPComponent::PostInit(CObject* pOwner, float fHP, float fMaxHP, int iLifeCount,
	const TSTRING & strTag)
{
	m_pOwner = pOwner;
	m_strComponentTag = strTag;
	
	m_bDead = false;
	m_iLife = m_iInitialLife =iLifeCount;
	m_fHP = fHP;
	m_fMaxHP = fMaxHP;

	m_bActive = true;

	return true;
}

void HPComponent::Init()
{
	m_fHP = m_fMaxHP;
	m_iLife = m_iInitialLife;
	m_bDead = false;
	m_bActive = true;
}

void HPComponent::Update(double dDeltaTime)
{
	if (m_fHP <= 0.f)
	{
		DecreaseLife(1);
	}

	if (m_iLife <= 0)
	{
		m_bDead = true;
	}

}

void HPComponent::LateUpdate()
{
}

void HPComponent::SetDead(bool bDead)
{
	m_bDead = bDead;
}

void HPComponent::IncreaseHP(float fHeal)
{
	if (m_fHP < m_fMaxHP) {
		m_fHP += fHeal;
		if (m_fHP > m_fMaxHP)
			m_fHP = m_fMaxHP;
	}

}

void HPComponent::DecreaseHP(float fDamage)
{
	if (m_fHP > 0) {
		m_fHP -= fDamage;
		if (m_fHP <= 0) {
			m_fHP = 0;
			m_bDead = true;
		}
	}

}

void HPComponent::IncreaseLife(UINT iValue)
{
	m_iLife += iValue;
	
}

void HPComponent::DecreaseLife(UINT iValue)
{
	if (m_iLife > 0)
	{
		m_iLife -= iValue;
	}

	if (m_iLife < 0)
	{
		m_iLife = 0;
	}

}
