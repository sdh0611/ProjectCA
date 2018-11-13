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
	//기본 Actor의 속성 초기화
	if (data.iActorWidth > MAX_ACTOR_SIZE || data.iActorHeight > MAX_ACTOR_SIZE)
		return false;

	if (data.actorPoint.x < 0 || data.actorPoint.x > MAX_WIDTH || 
		data.actorPoint.y < 0 || data.actorPoint.y > MAX_HEIGHT)
		return false;

	m_iActorWidth					= data.iActorWidth;
	m_iActorHeight					= data.iActorHeight;
	//m_actorPoint = m_spawnPoint = data.actorPoint;
	m_actorType					= data.actorType;
	m_actorCurState				= data.actorState;
	m_actorCurVerticalState		= data.verticalState;
	m_actorHorizonalState		= data.horizonalState;
	m_direction						= data.direction;
	m_actorID						= data.actorID;
	m_strActorTag					= data.strActorTag;
	m_bActive						= data.bActive;
	
	m_pOwnerScene = pScene;
	
	
	//TransformComponent 추가
	//NOTE(11.01) : TransformComponent에서 ScreenPosition값을 계산하므로 
	//					다른 컴포넌트들의 동작이 수행 된 뒤 동작해야함.
	//					떄문에 마지막에 추가하는 것으로 변경함.
	auto pTransform = std::make_shared<TransformComponent>();
	if (!pTransform->PostInit(this, data.actorPoint))
		return false;
	pTransform->SetPivotRatio(0.5f, 1.f);

	printf("Position : (%f, %f), Pivot : (%f, %f)\n", pTransform->GetPosition().x, pTransform->GetPosition().y, pTransform->GetPivot().x, pTransform->GetPivot().y);

	if (!AddComponent(pTransform, pTransform->GetComponentTag()))
		return false;


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

	auto onCollisionDelegater = [](std::shared_ptr<CActor> pOwner, std::shared_ptr<CActor> pOther, Collider::CollisionType type)->void 
	{
		POSITION position = pOwner->GetComponent<TransformComponent>().lock()->GetLastPosition();
		std::shared_ptr<PhysicsComponent> pPhysics = pOwner->GetComponent<PhysicsComponent>().lock();

		switch (pOther->GetActorType()) 
		{
		case Types::AT_ENEMY:
			//pComponent = pOwner->GetComponent(TEXT("PhysicsComponent"));
			//point = static_cast<PhysicsComponent*>(pComponent)->GetLastActorPoint();
			//pOwner->SetActorPoint(point.x, point.y);
			//pOwner->GetOwnerScene()->ResetScene();
			switch (type)
			{
			case Collider::COLLISION_BOT:
				pOwner->SetActorVerticalState(Types::VS_JUMP);
				pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
				break;
			}
			break;
		case Types::AT_PROB:
			switch (type) {
			case Collider::COLLISION_BOT:
				pPhysics->SetGrounded(true);
				pPhysics->SetCurJumpForce(0.f);
				pOwner->SetActorState(Types::AS_IDLE);
				pOwner->SetActorVerticalState(Types::VS_IDLE);
				pOwner->SetActorPoint(pOwner->GetActorPoint().x, pOther->GetActorPoint().y - pOther->GetActorHeight());
				break;
			case Collider::COLLISION_TOP:
				pOwner->SetActorVerticalState(Types::VS_FALL);
				break;
			case Collider::COLLISION_LEFT:
			case Collider::COLLISION_RIGHT:
				pOwner->SetActorHorizonalState(Types::HS_IDLE);
				pOwner->SetActorPoint(position.x, pOwner->GetActorPoint().y);
				pPhysics->SetCurSpeed(0.f);
				break;
			}
			
			break;
		}
	};

	pCollider->SetDelegate(onCollisionDelegater);
	pCollider->SetSize(m_iActorWidth*0.45, m_iActorHeight*0.9);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//RenderComponent 추가
	std::shared_ptr<AnimationRender> pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"),TEXT("PlayerIdleRight"),m_iActorWidth, m_iActorHeight, false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerIdleLeft"), m_iActorWidth, m_iActorHeight, false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioBig"), TEXT("PlayerWalkRight"), m_iActorWidth, m_iActorHeight, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioBig"), TEXT("PlayerWalkLeft"), m_iActorWidth, m_iActorHeight, true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioBig"), TEXT("PlayerRunRight"), m_iActorWidth, m_iActorHeight, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioBig"), TEXT("PlayerRunLeft"), m_iActorWidth, m_iActorHeight, true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerLookupRight"), m_iActorWidth, m_iActorHeight, false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerLookupLeft"), m_iActorWidth, m_iActorHeight, false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerSitdownRight"), m_iActorWidth, m_iActorHeight, false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerSitdownLeft"), m_iActorWidth, m_iActorHeight, false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerJumpRight"), m_iActorWidth, m_iActorHeight, false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerJumpLeft"), m_iActorWidth, m_iActorHeight, false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerFalldownRight"), m_iActorWidth, m_iActorHeight, false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerFalldownLeft"), m_iActorWidth, m_iActorHeight, false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerRunJumpRight"), m_iActorWidth, m_iActorHeight, false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerRunJumpLeft"), m_iActorWidth, m_iActorHeight, false, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerTurnRight"), m_iActorWidth, m_iActorHeight, false, TEXT("TurnRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerTurnLeft"), m_iActorWidth, m_iActorHeight, false, TEXT("TurnLeft")))
		return false;

	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;


	return true;
}

bool CPlayer::Init()
{
	//m_actorPoint = m_spawnPoint;
	m_pCamera->Init();

	for (auto& it : m_componentTable)
		it.second->Init();

	return true;
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
	//for (auto& component : m_componentTable)
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
	std::shared_ptr<TransformComponent> pTransform = GetComponent<TransformComponent>().lock();
	std::shared_ptr<PhysicsComponent> pPhysics = GetComponent<PhysicsComponent>().lock();

	float fCurSpeed = pPhysics->GetCurSpeed();
	float fCurJumpForce = pPhysics->GetCurJumpForce();
	float fMaxSpeed = pPhysics->GetMaxSpeed();
	float fWalkSpeed = pPhysics->GetSpeed();

	if (m_direction == Types::DIR_LEFT)
	{
		if (m_actorHorizonalState == Types::HS_RUN)
		{
			if (fCurSpeed > -1 * fMaxSpeed)
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 10.f);
		}
		else if (m_actorHorizonalState == Types::HS_WALK)
		{
			if (fCurSpeed < -1 * fWalkSpeed)
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 10.f);
			else if (pPhysics->GetCurSpeed() > -1 * fWalkSpeed)
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 5.f);
			else if (pPhysics->GetCurSpeed() <= -1 * fWalkSpeed)
				pPhysics->SetCurSpeed(-1.f * pPhysics->GetSpeed());

		}
		else if (m_actorHorizonalState == Types::HS_IDLE)
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
	else if (m_direction == Types::DIR_RIGHT)
	{
		if (m_actorHorizonalState == Types::HS_RUN)
		{
			if (fCurSpeed < fMaxSpeed)
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 10.f);
		}
		else if (m_actorHorizonalState == Types::HS_WALK)
		{
			if (fCurSpeed > fWalkSpeed)
				pPhysics->SetCurSpeed(fCurSpeed - 10.f);
			else if (fCurSpeed < fWalkSpeed)
				pPhysics->SetCurSpeed(fCurSpeed + 5.f);
			else if (fCurSpeed >= fWalkSpeed)
				pPhysics->SetCurSpeed(fWalkSpeed);
		}
		else if (m_actorHorizonalState == Types::HS_IDLE)
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
		if (m_actorCurVerticalState == Types::VS_JUMP)
		{
			pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
			pPhysics->SetGrounded(false);
		}
	}

	pTransform->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);

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
