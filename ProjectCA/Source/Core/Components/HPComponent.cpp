#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\HPComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"

HPComponent::HPComponent()
{
}

HPComponent::~HPComponent()
{
}

bool HPComponent::PostInit(std::shared_ptr<CActor> pOwner, float fHP, float fMaxHP, 
	const Types::tstring & strTag)
{
	m_pOwner = pOwner;
	m_strComponentTag = strTag;
	
	m_bIsDead = false;
	m_fHP = fHP;
	m_fMaxHP = fMaxHP;

	return true;
}

void HPComponent::Update(double fDeltaTime)
{


}

void HPComponent::LateUpdate(double dDeltaTime)
{
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
			m_bIsDead = true;
		}
	}

	

}
