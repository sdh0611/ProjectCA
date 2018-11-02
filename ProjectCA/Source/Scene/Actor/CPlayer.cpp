#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
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
	m_actorVector					= data.vector;
	m_actorID						= data.actorID;
	m_strActorTag					= data.strActorTag;
	m_bActive						= data.bActive;
	
	m_pOwnerScene = pScene;
	
	
	//TransformComponent 추가
	//NOTE(11.01) : TransformComponent에서 ScreenPosition값을 계산하므로 
	//					다른 컴포넌트들의 동작이 수행 된 뒤 동작해야함.
	//					떄문에 마지막에 추가하는 것으로 변경함.
	TransformComponent* pTransform = new TransformComponent;
	if (!pTransform->PostInit(this, data.actorPoint))
		return false;
	pTransform->SetPivotRatio(0.5f, 1.f);

	printf("Position : (%f, %f), Pivot : (%f, %f)\n", pTransform->GetPosition().x, pTransform->GetPosition().y, pTransform->GetPivot().x, pTransform->GetPivot().y);

	if (!AddComponent(pTransform, pTransform->GetComponentTag()))
		return false;


	//PlayerInputComponent (InputComponent) 초기화
	PlayerInputComponent* pAI = new PlayerInputComponent;
	if (!pAI->PostInit(this))
		return false;

	if (!AddComponent(pAI, pAI->GetComponentTag()))
		return false;


	//PhysicsComponent 초기화
	PhysicsComponent* pPhysics = new PhysicsComponent;
	if (!pPhysics->PostInit(this, 300.f, 700.f, 1300.f, 700.f))
		return false;

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;


	//Collider 초기화
	ColliderBox* pCollider = new ColliderBox;
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionDelegater = [](std::shared_ptr<CActor> pOwner, std::shared_ptr<CActor> pOther, Collider::CollisionType type)->void 
	{
		ComponentBase* pComponent = nullptr;
		POSITION position = pOwner->GetComponent<TransformComponent>()->GetLastPosition();
		PhysicsComponent* pPhysics = pOwner->GetComponent<PhysicsComponent>();

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
	RenderComponent* pRender = new RenderComponent;
	if (!pRender->PostInit(this))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerIdleRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerIdleLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnim(0.2f, TEXT("PlayerWalkRight"), m_iActorWidth, m_iActorHeight, true, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnim(0.2f, TEXT("PlayerWalkLeft"), m_iActorWidth, m_iActorHeight, true, true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnim(0.05f, TEXT("PlayerRunRight"), m_iActorWidth, m_iActorHeight, true, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnim(0.05f, TEXT("PlayerRunLeft"), m_iActorWidth, m_iActorHeight, true, true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerLookupRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerLookupLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerSitdownRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerSitdownLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerJumpRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerJumpLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerFalldownRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerFalldownLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerRunJumpRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerRunJumpLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerTurnRight"), m_iActorWidth, m_iActorHeight, false, false, TEXT("TurnRight")))
		return false;

	if (!pRender->AddAnim(3.f, TEXT("PlayerTurnLeft"), m_iActorWidth, m_iActorHeight, false, false, TEXT("TurnLeft")))
		return false;

	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;




	return true;
}

bool CPlayer::Init()
{
	//m_actorPoint = m_spawnPoint;
		
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
	const POSITION& position = GetComponent<TransformComponent>()->GetPosition();

	auto it = m_componentTable.find(TEXT("RenderComponent"));
	if (it != m_componentTable.end())
		static_cast<RenderComponent*>((*it).second)->Draw(hDC);

}

void CPlayer::ActorBehavior(double dDeltaTime)
{
	TransformComponent* pTransform = GetComponent<TransformComponent>();
	PhysicsComponent* pPhysics = GetComponent<PhysicsComponent>();

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
