#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\CInputManager.h"
#include "..\..\..\Include\Core\Sound\CSoundManager.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"




PlayerInputComponent::~PlayerInputComponent()
{
}

bool PlayerInputComponent::PostInit(CEntity* pOwner, const Types::tstring & strTag)
{
	
	if (!InputComponent::PostInit(pOwner, strTag))
		return false;

	m_pInputManager = CInputManager::GetInstance();
	if (m_pInputManager == nullptr)
		return false;

	return true;
}

void PlayerInputComponent::Init()
{
	m_bActive = true;
}

void PlayerInputComponent::Update(double fDeltaTime)
{
	if (m_bActive)
	{
		KeyProcess();
	}
}

void PlayerInputComponent::LateUpdate()
{
}


void PlayerInputComponent::KeyProcess()
{
	CPlayer* pOwner = static_cast<CPlayer*>(m_pOwner);

	Types::Direction dir = pOwner->GetActorDirection();

	auto pSoundManager = CSoundManager::GetInstance();

	if (pOwner->GetActorVerticalState() == Types::VS_IDLE)
	{
		//pOwner->SetObjectState(Types::OS_IDLE);
		pOwner->SetActorAct(Types::ACT_IDLE);
	}

	//수평상의 움직임에 대한 입력
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
		pOwner->SetActorHorizonalState(Types::HS_IDLE);
	}

	//위, 아래키에 대한 입력
	if (m_pInputManager->IsKeyDown(TEXT("DOWN")))
	{
		if (pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			pOwner->SetActorAct(Types::ACT_SITDOWN);
			pOwner->SetActorHorizonalState(Types::HS_IDLE);
		}

	}
	else if (m_pInputManager->IsKeyDown(TEXT("UP")))
	{
		if (pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			pOwner->SetActorAct(Types::ACT_LOOKUP);
			pOwner->SetActorHorizonalState(Types::HS_IDLE);
		}

	}

	//수직상의 움직임에 대한 입력
	if (m_pInputManager->IsKeyDown(TEXT("JUMP")))
	{
		if (pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			pOwner->SetActorVerticalState(Types::VS_JUMP);
			pSoundManager->SoundPlay(TEXT("SFXJump"));
		}
	}
	else if (m_pInputManager->IsKeyDown(TEXT("SPIN_JUMP")))
	{
		if (pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			if (!static_cast<CPlayer*>(m_pOwner)->IsPickingObject())
			{
				pOwner->SetActorAct(Types::ACT_DESTROY);
				pSoundManager->SoundPlay(TEXT("SFXSpinJump"));
			}
			else
			{
				pSoundManager->SoundPlay(TEXT("SFXJump"));
			}
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

	//공격키
	if (m_pInputManager->IsKeyDown(TEXT("ATTACK")))
	{
		if (pOwner->GetActorAct() != Types::ACT_SITDOWN && pOwner->GetActorAct() != Types::ACT_DESTROY)
			pOwner->SetActorAct(Types::ACT_ATTACK);
	}

}
