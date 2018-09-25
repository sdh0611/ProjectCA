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

//bool AIComponent::Init(std::shared_ptr<CActor> pOwner, const Types::tstring & strTag)
//{
//	return true;
//}

void AIComponent::Update(float fDeltaTime)
{
	BehaviorUpdate();
	
}

Delegater AIComponent::SetDelegate(Delegater pDelegater) {
	Delegater pTemp = m_pDelegate;

	m_pDelegate = pDelegater;

	return pTemp;
}

void AIComponent::BehaviorUpdate()
{
	m_pDelegate(m_pOwner);
	//auto pTemp = m_pOwner;

	//if (pTemp->GetActorPoint().y >= MAX_HEIGHT - pTemp->GetActorHeight())
	//	pTemp->SetActorDirection(Types::DIR_UP);

	//if (pTemp->GetActorPoint().y <= 0)
	//	pTemp->SetActorDirection(Types::DIR_DOWN);

}

