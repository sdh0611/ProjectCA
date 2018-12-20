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

bool InputComponent::PostInit(CEntity* pOwner, const Types::tstring & strTag)
{
	m_pOwner = pOwner;
	m_strComponentTag = strTag;
	m_bActive = true;

	return true;
}

