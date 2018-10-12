#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Scene\CGameScene.h"
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
	//Types::ObjectState state = m_pOwner->GetActorState();
	Types::Direction dir = m_pOwner->GetActorDirection();
	//RenderComponent* pRender = static_cast<RenderComponent*>( m_pOwner->GetComponent(TEXT("RenderComponent")) );	
	
	if (KEY_DOWN(VK_DOWN)) {
		if (m_pOwner->GetActorJumpState() != Types::JS_JUMP
			&& m_pOwner->GetActorJumpState() != Types::JS_FALL) 
		{
			m_pOwner->SetActorState(Types::OS_SITDOWN);
		}
		m_pOwner->SetActorVector(0.f, 1.f);
	}
	else if (KEY_DOWN(VK_LEFT)) {
		if (KEY_DOWN('A'))
		{
			m_pOwner->SetActorState(Types::OS_RUN);
		}
		else
		{
			m_pOwner->SetActorState(Types::OS_WALK);
		}
		m_pOwner->SetActorVector(-1.f, 0.f);
		m_pOwner->SetActorDirection(Types::DIR_LEFT);
	}
	else if (KEY_DOWN(VK_RIGHT)) {
		if (KEY_DOWN('A')) 
		{
			m_pOwner->SetActorState(Types::OS_RUN);
		}
		else
		{
			m_pOwner->SetActorState(Types::OS_WALK);
		}
		m_pOwner->SetActorVector(1.f, 0.f);
		m_pOwner->SetActorDirection(Types::DIR_RIGHT);
	}
	else if (KEY_DOWN(VK_UP)) {
		m_pOwner->SetActorState(Types::OS_LOOKUP);
		m_pOwner->SetActorVector(0.f, -1.f);
	}
	else {
		m_pOwner->SetActorState(Types::OS_IDLE);			
		//m_pOwner->SetActorVector(0.f, 0.f);
	}

	if (KEY_DOWN('X')) {
		if (m_pOwner->GetActorJumpState() == Types::JS_IDLE)
		{
				m_pOwner->SetActorJumpState(Types::JS_JUMP);
		}
	}
	else if (KEY_UP('X')) {
		if (m_pOwner->GetActorJumpState() == Types::JS_JUMP)
		{
			m_pOwner->SetActorJumpState(Types::JS_FALL);
		}
	}

	if (KEY_ONCE_PRESS(VK_ESCAPE)) 
	{
		puts("reset");
		m_pOwner->GetOwnerScene()->ResetScene();
	}


}
