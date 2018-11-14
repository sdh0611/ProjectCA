#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\CInputManager.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"




PlayerInputComponent::~PlayerInputComponent()
{
}

bool PlayerInputComponent::PostInit(CObject* pOwner, const Types::tstring & strTag)
{
	
	if (!InputComponent::PostInit(pOwner, strTag))
		return false;

	m_pInputManager = CInputManager::GetInstance();
	if (m_pInputManager == nullptr)
		return false;

	return true;
}

void PlayerInputComponent::Update(double fDeltaTime)
{
	KeyProcess();

}

void PlayerInputComponent::LateUpdate(double dDeltaTime)
{
}


void PlayerInputComponent::KeyProcess()
{
	std::shared_ptr<CActor> pOwner = STATIC_POINTER_CAST(CActor, m_pOwner);

	Types::Direction dir = pOwner->GetActorDirection();

	if (m_pInputManager->IsKeyDown(TEXT("LEFT")))
	{
		if (m_pInputManager->IsKeyDown(TEXT("ACCEL")))
		{
			pOwner->SetActorHorizonalState(Types::HS_RUN);
		}
		else
		{
			pOwner->SetActorHorizonalState(Types::HS_WALK);
		}

		pOwner->SetActorDirection(Types::DIR_LEFT);

	}
	else if (m_pInputManager->IsKeyDown(TEXT("RIGHT")))
	{
		if (m_pInputManager->IsKeyDown(TEXT("ACCEL")))
		{
			pOwner->SetActorHorizonalState(Types::HS_RUN);
		}
		else
		{
			pOwner->SetActorHorizonalState(Types::HS_WALK);
		}

		pOwner->SetActorDirection(Types::DIR_RIGHT);
	}
	else 
	{
		if (pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			pOwner->SetActorState(Types::AS_IDLE);
		}

		pOwner->SetActorHorizonalState(Types::HS_IDLE);

	}

	if (m_pInputManager->IsKeyDown(TEXT("DOWN")))
	{
		if (pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			pOwner->SetActorState(Types::AS_SITDOWN);
			pOwner->SetActorHorizonalState(Types::HS_IDLE);
		}
	}
	else if (m_pInputManager->IsKeyDown(TEXT("UP")))
	{
		if (pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			pOwner->SetActorState(Types::AS_LOOKUP);
			pOwner->SetActorHorizonalState(Types::HS_IDLE);
		}

	}

	if (m_pInputManager->IsKeyDown(TEXT("JUMP")))
	{
		if (pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			pOwner->SetActorVerticalState(Types::VS_JUMP);
		}

	}
	else 
	{
		if (pOwner->GetActorVerticalState() == Types::VS_JUMP)
		{
			pOwner->SetActorVerticalState(Types::VS_FALL);
		}

	}

	if (KEY_ONCE_PRESS(VK_ESCAPE))
	{
		puts("reset");
		pOwner->GetOwnerScene()->ResetScene();
	}

}
