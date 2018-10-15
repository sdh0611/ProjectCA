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

	m_keyInfoList.push_back(KeyInfo(TEXT("UP"), VK_UP, false));
	m_keyInfoList.push_back(KeyInfo(TEXT("DOWN"), VK_DOWN, false));
	m_keyInfoList.push_back(KeyInfo(TEXT("RIGHT"), VK_RIGHT, false));
	m_keyInfoList.push_back(KeyInfo(TEXT("LEFT"), VK_LEFT, false));
	m_keyInfoList.push_back(KeyInfo(TEXT("JUMP"), 'X', false));
	m_keyInfoList.push_back(KeyInfo(TEXT("ACCEL"), 'A', false));
	//m_keyInfoList.push_back(KeyInfo(TEXT("RESET"), VK_ESCAPE, false));

	return true;
}

void PlayerInputComponent::Update(double fDeltaTime)
{
	UpdateKeyDown();
	KeyProcess();

}

bool PlayerInputComponent::GetKeyDown(const Types::tstring & steKeyName)
{
	bool bPressed = false;

	for (auto& it : m_keyInfoList) 
	{
		if (steKeyName == it.m_strKeyName)
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

void PlayerInputComponent::UpdateKeyDown()
{
	for (auto& it : m_keyInfoList)
	{
		if (KEY_DOWN(it.m_iKeyKode))
			it.m_bPressed = true;
		else
			it.m_bPressed = false;

	}

}

void PlayerInputComponent::KeyProcess()
{
	Types::Direction dir = m_pOwner->GetActorDirection();
	RenderComponent* pRender = static_cast<RenderComponent*>( m_pOwner->GetComponent(TEXT("RenderComponent")) );	
	//PhysicsComponent* pPhysics = static_cast<PhysicsComponent*>(m_pOwner->GetComponent(TEXT("PhysicsComponent")));

	if (GetKeyDown(TEXT("LEFT"))) 
	{
		//if (m_pOwner->GetActorHorizonalState() != Types::HS_IDLE)
		//{
			if (GetKeyDown(TEXT("ACCEL")))
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
		m_pOwner->SetActorVector(-1.f, 0.f);
		//m_pOwner->SetActorState(Types::AS_MOVE);
		m_pOwner->SetActorDirection(Types::DIR_LEFT);
	}
	else if (GetKeyDown(TEXT("RIGHT"))) 
	{
		if (GetKeyDown(TEXT("ACCEL")))
		{
			m_pOwner->SetActorHorizonalState(Types::HS_RUN);
		}
		else
		{
			m_pOwner->SetActorHorizonalState(Types::HS_WALK);
		}

		m_pOwner->SetActorVector(1.f, 0.f);
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

	if (GetKeyDown(TEXT("DOWN"))) 
	{
		if (m_pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			m_pOwner->SetActorState(Types::AS_SITDOWN);
			m_pOwner->SetActorHorizonalState(Types::HS_IDLE);
		}
	}
	else if (GetKeyDown(TEXT("UP"))) 
	{
		if (m_pOwner->GetActorVerticalState() == Types::VS_IDLE)
		{
			m_pOwner->SetActorState(Types::AS_LOOKUP);
			m_pOwner->SetActorHorizonalState(Types::HS_IDLE);
		}
	}

	if (GetKeyDown(TEXT("JUMP"))) 
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
