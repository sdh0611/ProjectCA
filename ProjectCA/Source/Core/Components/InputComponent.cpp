#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\InputComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"


InputComponent::InputComponent(CActor* pOwner)
	:ComponentBase(pOwner, TEXT("InputComponent"))
{

}

//void InputComponent::Init()
//{
//	//m_pOwner = owner;
//
//
//}

bool InputComponent::Init(CActor * pOwner, const Types::tstring & strTag)
{
	m_pOwner = pOwner;
	m_strComponentTag = strTag;

	return true;
}

void InputComponent::Update(float fDeltaTime)
{
	//왠지 ObejctState에 Jump상태를 넣어놓으면 추후 
	//Damaged 상태로 전환할 떄 문제가 생길 것 같다.
	//bool형으로 따로 뺴는게 좋을듯(18.05.24)
	//if (m_pOwner->GetObjectState() != Types::OS_JUMP) {
	//	if (KEY_DOWN(VK_SPACE))
	//		m_pOwner->SetObjectState(Types::OS_JUMP);
	//}
	//else
	//	m_pOwner->SetObjectState(Types::OS_IDLE);

	KeyProcess();



}

void InputComponent::KeyProcess()
{
	//if(KEY_DOWN(VK_BACK))


	if (KEY_DOWN(VK_LEFT)) {
		m_pOwner->SetActorDirection(Types::DIR_LEFT);
		m_pOwner->SetActorState(Types::OS_MOVE);
	}
	else if (KEY_DOWN(VK_RIGHT)) {
		m_pOwner->SetActorDirection(Types::DIR_RIGHT);
		m_pOwner->SetActorState(Types::OS_MOVE);
	}
	else if (KEY_DOWN(VK_UP)) {
		m_pOwner->SetActorDirection(Types::DIR_UP);
		m_pOwner->SetActorState(Types::OS_MOVE);
	}
	else if (KEY_DOWN(VK_DOWN)) {
		m_pOwner->SetActorDirection(Types::DIR_DOWN);
		m_pOwner->SetActorState(Types::OS_MOVE);
	}
	else {
		m_pOwner->SetActorDirection(Types::DIR_IDLE);
		m_pOwner->SetActorState(Types::OS_IDLE);
	}
}
