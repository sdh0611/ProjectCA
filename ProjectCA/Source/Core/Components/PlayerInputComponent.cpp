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

bool PlayerInputComponent::PostInit(CActor* pOwner, const Types::tstring & strTag)
{
	auto pActor = std::shared_ptr<CActor>(pOwner);

	m_pOwner = pActor;
	m_strComponentTag = strTag;

	m_keyInfoList.push_back(Types::KeyInfo(TEXT("UP"), VK_UP, false));
	m_keyInfoList.push_back(Types::KeyInfo(TEXT("DOWN"), VK_DOWN, false));
	m_keyInfoList.push_back(Types::KeyInfo(TEXT("RIGHT"), VK_RIGHT, false));
	m_keyInfoList.push_back(Types::KeyInfo(TEXT("LEFT"), VK_LEFT, false));
	m_keyInfoList.push_back(Types::KeyInfo(TEXT("JUMP"), 'X', false));
	m_keyInfoList.push_back(Types::KeyInfo(TEXT("ACCEL"), 'A', false));
	//m_keyInfoList.push_back(KeyInfo(TEXT("RESET"), VK_ESCAPE, false));

	m_pInputManager = CInputManager::GetInstance();
	if (m_pInputManager == nullptr)
		return false;

	return true;
}

void PlayerInputComponent::Update(double fDeltaTime)
{
	//UpdateKeyDown();
	KeyProcess();

}

void PlayerInputComponent::LateUpdate(double dDeltaTime)
{
}

bool PlayerInputComponent::GetKeyDown(const Types::tstring & strKeyName)
{
	bool bPressed = false;

	for (auto& it : m_keyInfoList) 
	{
		if (strKeyName == it.m_strKeyName)
		{
			bPressed = it.m_bPressed;
			break;
		}

	}
	//printf("%s : ", steKeyName.c_str());
	//if (bPressed)
	//	printf("true\n");
	//else
	//	printf("false\n");

	return bPressed;
}

//void PlayerInputComponent::UpdateKeyDown()
//{
//	for (auto& it : m_keyInfoList)
//	{
//		if (KEY_DOWN(it.m_iKeyCode))
//			it.m_bPressed = true;
//		else
//			it.m_bPressed = false;
//
//	}
//
//}

void PlayerInputComponent::KeyProcess()
{
	Types::Direction dir = m_pOwner->GetActorDirection();

	if (m_pInputManager->IsKeyDown(TEXT("LEFT")))
	{
		//if (m_pOwner->GetActorHorizonalState() != Types::HS_IDLE)
		//{
			if (m_pInputManager->IsKeyDown(TEXT("ACCEL")))
			{
				m_pOwner->SetActorHorizonalState(Types::HS_RUN);
			}
			else
			{
				m_pOwner->SetActorHorizonalState(Types::HS_WALK);
			}
		//}
		//else
		//{

		//}
		//m_pOwner->SetActorVector(-1.f, 0.f);
		//m_pOwner->SetActorState(Types::AS_MOVE);
		m_pOwner->SetActorDirection(Types::DIR_LEFT);
	}
	else if (m_pInputManager->IsKeyDown(TEXT("RIGHT")))
	{
		if (m_pInputManager->IsKeyDown(TEXT("ACCEL")))
		{
			m_pOwner->SetActorHorizonalState(Types::HS_RUN);
		}
		else
		{
			m_pOwner->SetActorHorizonalState(Types::HS_WALK);
		}

		//m_pOwner->SetActorVector(1.f, 0.f);
		//m_pOwner->SetActorState(Types::AS_MOVE);
		m_pOwner->SetActorDirection(Types::DIR_RIGHT);
	}
	else 
	{
		if (m_pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			m_pOwner->SetActorState(Types::AS_IDLE);
		}
		m_pOwner->SetActorHorizonalState(Types::HS_IDLE);
	}

	if (m_pInputManager->IsKeyDown(TEXT("DOWN")))
	{
		if (m_pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			m_pOwner->SetActorState(Types::AS_SITDOWN);
			m_pOwner->SetActorHorizonalState(Types::HS_IDLE);
		}
	}
	else if (m_pInputManager->IsKeyDown(TEXT("UP")))
	{
		if (m_pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			m_pOwner->SetActorState(Types::AS_LOOKUP);
			m_pOwner->SetActorHorizonalState(Types::HS_IDLE);
		}
	}

	if (m_pInputManager->IsKeyDown(TEXT("JUMP")))
	{
		if (m_pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			m_pOwner->SetActorVerticalState(Types::VS_JUMP);
		}
	}
	else 
	{
		if (m_pOwner->GetActorVerticalState() == Types::VS_JUMP)
		{
			m_pOwner->SetActorVerticalState(Types::VS_FALL);
		}
	}

	if (KEY_ONCE_PRESS(VK_ESCAPE))
	{
		puts("reset");
		m_pOwner->GetOwnerScene()->ResetScene();
	}

	//if (m_pOwner->GetActorState() == Types::AS_SITDOWN)
	//	puts("SITDOWN");

	//if (m_pOwner->GetActorState() == Types::AS_LOOKUP)
	//	puts("LOOKUP");

}
