#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"



CPlayer::CPlayer()
	:CActor()
{
}

CPlayer::~CPlayer()
{
}

//bool CPlayer::Init(const Types::ActorData &)
//{
//	
//
//	return true;
//}

bool CPlayer::PostInit(const Types::ActorData& data, CGameScene* pScene)
{

	//TransformComponent 추가
	CActor::PostInit(data, pScene);
	//printf("Position : (%f, %f), Pivot : (%f, %f)\n", pTransform->GetPosition().x, pTransform->GetPosition().y, pTransform->GetPivot().x, pTransform->GetPivot().y);

	//PlayerInputComponent (InputComponent) 초기화
	std::shared_ptr<PlayerInputComponent> pInput = std::make_shared<PlayerInputComponent>();
	if (!pInput->PostInit(this))
		return false;

	if (!AddComponent(pInput, pInput->GetComponentTag()))
		return false;


	//PhysicsComponent 초기화
	std::shared_ptr<PhysicsComponent> pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 300.f, 700.f, 1300.f, 700.f))
		return false;

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;


	//Collider 초기화
	std::shared_ptr<ColliderBox> pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionDelegater = [](CObject* pOwner, CObject* pOther, Collider::CollisionType type)->void 
	{
		POSITION position = pOwner->GetComponent<TransformComponent>().lock()->GetLastPosition();
		auto pPhysics = pOwner->GetComponent<PhysicsComponent>().lock();
		auto pOwnerActor = static_cast<CActor*>(pOwner);

		switch (static_cast<CActor*>(pOther)->GetActorType()) 
		{
		case Types::AT_ENEMY:
			//pComponent = pOwnerActor->GetComponent(TEXT("PhysicsComponent"));
			//point = static_cast<PhysicsComponent*>(pComponent)->GetLastActorPoint();
			//pOwnerActor->SetActorPoint(point.x, point.y);
			//pOwnerActor->GetOwnerScene()->ResetScene();
			switch (type)
			{
			case Collider::COLLISION_BOT:
				pOwnerActor->SetActorVerticalState(Types::VS_JUMP);
				pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
				break;
			}
			break;
		case Types::AT_PROB:
			switch (type) {
			case Collider::COLLISION_BOT:
				pPhysics->SetGrounded(true);
				pPhysics->SetCurJumpForce(0.f);
				pOwnerActor->SetActorState(Types::AS_IDLE);
				pOwnerActor->SetActorVerticalState(Types::VS_IDLE);
				pOwnerActor->SetObjectPosition(pOwnerActor->GetObjectPosition().x, pOther->GetObjectPosition().y - pOther->GetObjectHeight());
				break;
			case Collider::COLLISION_TOP:
				pOwnerActor->SetActorVerticalState(Types::VS_FALL);
				break;
			case Collider::COLLISION_LEFT:
			case Collider::COLLISION_RIGHT:
				pOwnerActor->SetActorHorizonalState(Types::HS_IDLE);
				pOwnerActor->SetObjectPosition(position.x, pOwnerActor->GetObjectPosition().y);
				pPhysics->SetCurSpeed(0.f);
				break;
			}
			
			break;
		}
	};

	pCollider->SetDelegate(onCollisionDelegater);
	pCollider->SetSize(m_iObjectWidth*0.35, m_iObjectHeight*0.8);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//RenderComponent 추가
	std::shared_ptr<AnimationRender> pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"),TEXT("PlayerIdleRight"),m_iObjectWidth, m_iObjectHeight, false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerIdleLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioBig"), TEXT("PlayerWalkRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioBig"), TEXT("PlayerWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioBig"), TEXT("PlayerRunRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioBig"), TEXT("PlayerRunLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerLookupRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerLookupLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerSitdownRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerSitdownLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerJumpRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerJumpLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerFalldownRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerFalldownLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerRunJumpRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerRunJumpLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerTurnRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("TurnRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerTurnLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("TurnLeft")))
		return false;

	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;


	return true;
}

void CPlayer::Init()
{
	//m_ActorPoint = m_spawnPoint;
	m_pCamera->Init();

	for (auto& it : m_ComponentTable)
		it.second->Init();
}

void CPlayer::Update(double dDeltaTime)
{
	//InputComponent* pInput = GetComponent<PlayerInputComponent>();
	//if (pInput)
	//{
	//	pInput->Update(dDeltaTime);
	//}

	//ActorBehavior(dDeltaTime);
	//
	//for (auto& component : m_ComponentTable)
	//{
	//	if (component.first == TEXT("InputComponent"))
	//	{
	//		continue;
	//	}
	//	component.second->Update(dDeltaTime);
	//}
	CActor::Update(dDeltaTime);
	//m_pCamera->Update(dDeltaTime);
	
}

void CPlayer::Render(const HDC & hDC)
{
	auto pRender = GetComponent(TEXT("RenderComponent"));
	if (!pRender.expired())
		STATIC_POINTER_CAST(RenderComponent, pRender.lock())->Draw(hDC);

}

void CPlayer::ActorBehavior(double dDeltaTime)
{
	auto pTransform		= GetComponent<TransformComponent>().lock();
	auto pPhysics			= GetComponent<PhysicsComponent>().lock();

	float fCurSpeed		= pPhysics->GetCurSpeed();
	float fMaxSpeed		= pPhysics->GetMaxSpeed();
	float fWalkSpeed		= pPhysics->GetSpeed();
	float fCurJumpForce	= pPhysics->GetCurJumpForce();

	if (m_Direction == Types::DIR_LEFT)
	{
		if (m_ActorHorizonalState == Types::HS_RUN)
		{
			if (fCurSpeed > -1 * fMaxSpeed)
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 10.f);
		}
		else if (m_ActorHorizonalState == Types::HS_WALK)
		{
			if (fCurSpeed < -1 * fWalkSpeed)
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 10.f);
			else if (pPhysics->GetCurSpeed() > -1 * fWalkSpeed)
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 5.f);
			else if (pPhysics->GetCurSpeed() <= -1 * fWalkSpeed)
				pPhysics->SetCurSpeed(-1.f * pPhysics->GetSpeed());

		}
		else if (m_ActorHorizonalState == Types::HS_IDLE)
		{
			if (fCurSpeed < 0.f)
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 5.f);
				if (pPhysics->GetCurSpeed() > 0.f)
					pPhysics->SetCurSpeed(0.f);
			}
			else if (fCurSpeed > 0.f)
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 5.f);
				if (pPhysics->GetCurSpeed() < 0.f)
					pPhysics->SetCurSpeed(0.f);
			}
		}
	}
	else if (m_Direction == Types::DIR_RIGHT)
	{
		if (m_ActorHorizonalState == Types::HS_RUN)
		{
			if (fCurSpeed < fMaxSpeed)
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 10.f);
		}
		else if (m_ActorHorizonalState == Types::HS_WALK)
		{
			if (fCurSpeed > fWalkSpeed)
				pPhysics->SetCurSpeed(fCurSpeed - 10.f);
			else if (fCurSpeed < fWalkSpeed)
				pPhysics->SetCurSpeed(fCurSpeed + 5.f);
			else if (fCurSpeed >= fWalkSpeed)
				pPhysics->SetCurSpeed(fWalkSpeed);
		}
		else if (m_ActorHorizonalState == Types::HS_IDLE)
		{
			if (fCurSpeed > 0.f)
			{
				pPhysics->SetCurSpeed(fCurSpeed - 5.f);
				if (pPhysics->GetCurSpeed() < 0.f)
					pPhysics->SetCurSpeed(0.f);
			}
			else if (fCurSpeed < 0.f)
			{
				pPhysics->SetCurSpeed(fCurSpeed + 5.f);
				if (pPhysics->GetCurSpeed() > 0.f)
					pPhysics->SetCurSpeed(0.f);
			}
		}
	}

	if (pPhysics->IsGrounded())
	{
		if (m_ActorCurVerticalState == Types::VS_JUMP)
		{
			pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
			pPhysics->SetGrounded(false);
		}
	}

	pTransform->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);

	auto pCollider = GetComponent<ColliderBox>().lock();
	if (m_ActorCurState == Types::AS_SITDOWN)
	{
		pCollider->SetCurRectHeight(pCollider->GetHeight() / 2.f);
	}
	else
	{
		pCollider->SetCurRectHeight(pCollider->GetHeight());
	}
}

bool CPlayer::AttachCamera(std::shared_ptr<CCamera> pCamera)
{
	if(m_pCamera)
		return false;

	m_pCamera = pCamera;

	return true;
}

std::weak_ptr<CCamera> CPlayer::GetCamera()
{
	return m_pCamera;
}
