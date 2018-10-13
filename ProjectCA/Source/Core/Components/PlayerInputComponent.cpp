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
	//Types::ObjectState state = m_pOwner->GetActorState();
	Types::Direction dir = m_pOwner->GetActorDirection();
	RenderComponent* pRender = static_cast<RenderComponent*>( m_pOwner->GetComponent(TEXT("RenderComponent")) );	
	//PhysicsComponent* pPhysics = static_cast<PhysicsComponent*>(m_pOwner->GetComponent(TEXT("PhysicsComponent")));

	//if (KEY_DOWN(VK_LEFT)) {
	//	if (KEY_DOWN('A'))
	//	{
	//		m_pOwner->SetActorState(Types::OS_RUN);
	//		//pPhysics->SetAcceled(true);
	//	}
	//	else
	//	{
	//		m_pOwner->SetActorState(Types::OS_WALK);
	//		//pPhysics->SetAcceled(false);
	//	}
	//	
	//	//if (m_pOwner->GetActorState() != Types::OS_RUN) 
	//	//{
	//	//	m_pOwner->SetActorState(Types::OS_WALK);
	//	//}
	//	m_pOwner->SetActorVector(-1.f, 0.f);
	//	m_pOwner->SetActorDirection(Types::DIR_LEFT);
	//}
	//else if (KEY_DOWN(VK_RIGHT)) {
	//	if (KEY_DOWN('A')) 
	//	{
	//		m_pOwner->SetActorState(Types::OS_RUN);
	//		//pPhysics->SetAcceled(true);
	//	}
	//	else
	//	{
	//		m_pOwner->SetActorState(Types::OS_WALK);
	//		//pPhysics->SetAcceled(false);
	//	}

	//	//if (m_pOwner->GetActorState() != Types::OS_RUN)
	//	//{
	//	//	m_pOwner->SetActorState(Types::OS_WALK);
	//	//}
	//	m_pOwner->SetActorVector(1.f, 0.f);
	//	m_pOwner->SetActorDirection(Types::DIR_RIGHT);
	//}
	//else {
	//	//pPhysics->SetAcceled(false);
	//	m_pOwner->SetActorState(Types::OS_IDLE);			
	//	//pRender->ChangeAnimationCilp(Types::OS_IDLE);
	//	//m_pOwner->SetActorVector(0.f, 0.f);
	//}

	//if (KEY_DOWN(VK_DOWN)) {
	//	if (m_pOwner->GetActorJumpState() == Types::JS_IDLE)
	//	{
	//		m_pOwner->SetActorState(Types::OS_SITDOWN);
	//		//pRender->ChangeAnimationCilp(Types::OS_SITDOWN);
	//	}
	//	//m_pOwner->SetActorVector(0.f, 1.f);
	//}
	//else if (KEY_DOWN(VK_UP)) {
	//	if(m_pOwner->GetActorJumpState() == Types::JS_IDLE)
	//	{
	//		m_pOwner->SetActorState(Types::OS_LOOKUP);
	//		//pRender->ChangeAnimationCilp(Types::OS_LOOKUP);
	//		//m_pOwner->SetActorVector(0.f, -1.f);

	//	}
	//}

	//if (KEY_DOWN('X')) {
	//	if (m_pOwner->GetActorJumpState() == Types::JS_IDLE)
	//	{
	//			m_pOwner->SetActorJumpState(Types::JS_JUMP);
	//	}
	//}
	//else if (KEY_UP('X')) {
	//	if (m_pOwner->GetActorJumpState() == Types::JS_JUMP)
	//	{
	//		m_pOwner->SetActorJumpState(Types::JS_FALL);
	//	}
	//}


	if (GetKeyDown(TEXT("LEFT"))) {
		if (GetKeyDown(TEXT("ACCEL")))
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
	else if (GetKeyDown(TEXT("RIGHT"))) {
		if (GetKeyDown(TEXT("ACCEL")))
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
	else {
		m_pOwner->SetActorState(Types::OS_IDLE);
	}

	if (GetKeyDown(TEXT("DOWN"))) {
		if (m_pOwner->GetActorJumpState() == Types::JS_IDLE)
		{
			//if (m_pOwner->GetActorState() != Types::OS_WALK ||
			//	m_pOwner->GetActorState() != Types::OS_RUN)
			//{
				m_pOwner->SetActorState(Types::OS_SITDOWN);
			//}
		}
	}
	else if (GetKeyDown(TEXT("UP"))) {
		if (m_pOwner->GetActorJumpState() == Types::JS_IDLE)
		{
			//if (m_pOwner->GetActorState() != Types::OS_WALK ||
			//	m_pOwner->GetActorState() != Types::OS_RUN)
			//{
				m_pOwner->SetActorState(Types::OS_LOOKUP);
			//}

		}
	}

	if (GetKeyDown(TEXT("JUMP"))) {
		if (m_pOwner->GetActorJumpState() == Types::JS_IDLE)
		{
			m_pOwner->SetActorJumpState(Types::JS_JUMP);
		}
	}
	else {
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

	if (m_pOwner->GetActorState() == Types::OS_SITDOWN)
		puts("SITDOWN");

}
