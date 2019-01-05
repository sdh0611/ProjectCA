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
	CPlayer* pOwner			= static_cast<CPlayer*>(m_pOwner);
	DIRECTION dir				= pOwner->GetActorDirection();
	auto pSoundManager	= CSoundManager::GetInstance();
	auto pPhysics				= m_pOwner->GetComponent<PhysicsComponent>().lock();

	if (pPhysics->IsGrounded())
	{
		pOwner->SetActorAct(Types::ACT_IDLE);
	}

	//수평상의 움직임에 대한 입력
	if (m_pInputManager->IsKeyDown(TEXT("LEFT")))
	{
		pOwner->SetActorDirection(Types::DIR_LEFT);
		
		if (m_pInputManager->IsKeyDown(TEXT("ACCEL")))
		{
			if (pPhysics->GetCurSpeed() > -1 * pPhysics->GetMaxSpeed())
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 10.f);
			}
		}
		else
		{
			if (pPhysics->GetCurSpeed() > -1 * pPhysics->GetSpeed())
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 10.f);
			}
			else if (pPhysics->GetCurSpeed() < -1 * pPhysics->GetSpeed())
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 10.f);
				//오차 보정
				if (pPhysics->GetCurSpeed() > -1 * pPhysics->GetSpeed())
				{
					pPhysics->SetCurSpeed(-1 * pPhysics->GetSpeed());
				}
			}
		}
	}
	else if (m_pInputManager->IsKeyDown(TEXT("RIGHT")))
	{
		pOwner->SetActorDirection(Types::DIR_RIGHT);
		if (m_pInputManager->IsKeyDown(TEXT("ACCEL")))
		{
			if (pPhysics->GetCurSpeed() < pPhysics->GetMaxSpeed())
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 10.f);
			}
		}
		else
		{
			if (pPhysics->GetCurSpeed() < pPhysics->GetSpeed())
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 10.f);
			}
			else if (pPhysics->GetCurSpeed()  > pPhysics->GetSpeed())
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 10.f);
				//오차 보정
				if (pPhysics->GetCurSpeed() < pPhysics->GetSpeed())
				{
					pPhysics->SetCurSpeed( pPhysics->GetSpeed());
				}
			}
		}
	}
	else 
	{
		if (pPhysics->GetCurSpeed() < 0.f)
		{
			pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 10.f);
			if (pPhysics->GetCurSpeed() > 0.f)
			{
				pPhysics->SetCurSpeed(0.f);
			}
		}
		else if (pPhysics->GetCurSpeed() > 0.f)
		{
			pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 10.f);
			if (pPhysics->GetCurSpeed() < 0.f)
			{
				pPhysics->SetCurSpeed(0.f);
			}
		}
	}

	//위, 아래키에 대한 입력
	if (m_pInputManager->IsKeyDown(TEXT("DOWN")))
	{
		if (pPhysics->IsGrounded())
		{
			pOwner->SetActorAct(Types::ACT_SITDOWN);
			pPhysics->SetCurSpeed(0.f);
		}

	}
	else if (m_pInputManager->IsKeyDown(TEXT("UP")))
	{
		if (pPhysics->IsGrounded())
		{
			pOwner->SetActorAct(Types::ACT_LOOKUP);
			pPhysics->SetCurSpeed(0.f);
		}

	}

	//수직상의 움직임에 대한 입력
	if (m_pInputManager->IsKeyDown(TEXT("JUMP")))
	{
		if (pPhysics->IsGrounded())
		{
			pPhysics->SetGrounded(false);
			pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
			pSoundManager->SoundPlay(TEXT("SFXJump"));
		}
	}
	else if (m_pInputManager->IsKeyDown(TEXT("SPIN_JUMP")))
	{
		if (pPhysics->IsGrounded())
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
			pPhysics->SetGrounded(false);
			pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
		}
	}
	else 
	{
		if (pPhysics->GetCurJumpForce() > 50.f)
		{
			pPhysics->SetCurJumpForce(50.f);
		}

	}

	//공격키
	if (m_pInputManager->IsKeyDown(TEXT("ATTACK")))
	{
		if (pOwner->GetActorAct() != Types::ACT_SITDOWN && pOwner->GetActorAct() != Types::ACT_DESTROY)
			pOwner->SetActorAct(Types::ACT_ATTACK);
	}

}
