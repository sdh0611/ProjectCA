#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"


AIComponent::AIComponent()
	:m_pDelegate(nullptr)
{
}

AIComponent::~AIComponent()
{
}

void AIComponent::Init()
{
	m_bActive = true;
}

void AIComponent::Update(double fDeltaTime)
{
	BehaviorUpdate();
	
}

void AIComponent::LateUpdate(double dDeltaTime)
{
}

Callback AIComponent::SetDelegate(Callback pDelegater) {
	Callback pTemp = m_pDelegate;

	m_pDelegate = pDelegater;

	return pTemp;
}

void AIComponent::BehaviorUpdate()
{
	if(m_pDelegate != nullptr)
		m_pDelegate(static_cast<CActor*>(m_pOwner));

}

