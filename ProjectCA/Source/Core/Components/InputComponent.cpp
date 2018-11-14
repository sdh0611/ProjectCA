#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\InputComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"


InputComponent::InputComponent()
{

}

//void InputComponent::Init()
//{
//	//m_pOwner = owner;
//
//
//}

bool InputComponent::PostInit(CObject* pOwner, const Types::tstring & strTag)
{
	auto pActor = std::shared_ptr<CObject>(pOwner);

	m_pOwner = pActor;
	m_strComponentTag = strTag;

	return true;
}

