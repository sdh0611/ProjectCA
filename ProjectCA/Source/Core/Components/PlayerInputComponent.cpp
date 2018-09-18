#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"

PlayerInputComponent::~PlayerInputComponent()
{
}

bool PlayerInputComponent::Init(std::shared_ptr<CActor> pOwner, const Types::tstring & strTag)
{
	m_pOwner = pOwner;
	m_strComponentTag = strTag;

	return true;
}

void PlayerInputComponent::Update(float fDeltaTime)
{
	KeyProcess();

}

void PlayerInputComponent::KeyProcess()
{
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
