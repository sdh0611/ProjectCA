#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"

PlayerInputComponent::~PlayerInputComponent()
{
}

bool PlayerInputComponent::Init(CActor* pOwner, const Types::tstring & strTag)
{
	auto pActor = std::shared_ptr<CActor>(pOwner);

	m_pOwner = pActor;
	m_strComponentTag = strTag;

	return true;
}

void PlayerInputComponent::Update(double fDeltaTime)
{
	KeyProcess();

}

void PlayerInputComponent::KeyProcess()
{
	Types::ObjectState state = m_pOwner->GetActorState();
	Types::Direction dir = m_pOwner->GetActorDirection();
	RenderComponent* pRender = static_cast<RenderComponent*>( m_pOwner->GetComponent(TEXT("RenderComponent")) );


	if (KEY_DOWN(VK_LEFT)) {
		//m_pOwner->SetActorDirection(Types::DIR_LEFT);
		if (state != Types::OS_JUMP || state != Types::OS_FALL) {
			m_pOwner->SetActorState(Types::OS_MOVE);
			if (KEY_DOWN('A'))
				pRender->ChangeAnimation(TEXT("RunLeft"));
			else
				pRender->ChangeAnimation(TEXT("MoveLeft"));
		}
		m_pOwner->SetActorVector(-1.f, 0.f);
		m_pOwner->SetActorDirection(Types::DIR_LEFT);
	}
	else if (KEY_DOWN(VK_RIGHT)) {
		//m_pOwner->SetActorDirection(Types::DIR_RIGHT);
		if (state != Types::OS_JUMP || state != Types::OS_FALL) {
			m_pOwner->SetActorState(Types::OS_MOVE);
			if (KEY_DOWN('A'))
				pRender->ChangeAnimation(TEXT("RunRight"));
			else
			pRender->ChangeAnimation(TEXT("MoveRight"));
		}
		m_pOwner->SetActorVector(1.f, 0.f);
		m_pOwner->SetActorDirection(Types::DIR_RIGHT);
	}
	else if (KEY_DOWN(VK_UP)) {
		//m_pOwner->SetActorDirection(Types::DIR_UP);
		if (state != Types::OS_JUMP || state != Types::OS_FALL) {
			m_pOwner->SetActorState(Types::OS_MOVE);
			if (dir == Types::DIR_LEFT)
				pRender->ChangeAnimation(TEXT("LookupLeft"));
			else if (dir == Types::DIR_RIGHT)
				pRender->ChangeAnimation(TEXT("LookupRight"));
		}
		m_pOwner->SetActorVector(0.f, -1.f);
	}
	else if (KEY_DOWN(VK_DOWN)) {
		//m_pOwner->SetActorDirection(Types::DIR_DOWN);
		if (state != Types::OS_JUMP || state != Types::OS_FALL) {
			m_pOwner->SetActorState(Types::OS_MOVE);
			if (dir == Types::DIR_LEFT)
				pRender->ChangeAnimation(TEXT("SitdownLeft"));
			else if (dir == Types::DIR_RIGHT)
				pRender->ChangeAnimation(TEXT("SitdownRight"));
		}
		m_pOwner->SetActorVector(0.f, 1.f);
	}
	else {
		//m_pOwner->SetActorDirection(Types::DIR_IDLE);
		if (state != Types::OS_JUMP || state != Types::OS_FALL) {
			m_pOwner->SetActorState(Types::OS_IDLE);
			if(dir == Types::DIR_LEFT)
				pRender->ChangeAnimation(TEXT("IdleLeft"));
			else if(dir == Types::DIR_RIGHT)
				pRender->ChangeAnimation(TEXT("IdleRight"));

		}
		m_pOwner->SetActorVector(0.f, 0.f);
	}

}
