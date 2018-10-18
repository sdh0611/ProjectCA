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

bool InputComponent::PostInit(CActor* pOwner, const Types::tstring & strTag)
{
	auto pStrongOwenr = std::shared_ptr<CActor>(pOwner);

	m_pOwner = pStrongOwenr;
	m_strComponentTag = strTag;

	return true;
}

