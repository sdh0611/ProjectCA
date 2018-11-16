#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Scene\Actor\CFireBall.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"



CPlayer::CPlayer()
	:CActor()
{
}

CPlayer::~CPlayer()
{
}


bool CPlayer::PostInit(const Types::ActorData& data, CGameScene* pScene)
{

	//TransformComponent 추가
	CActor::PostInit(data, pScene);

	m_iSmallStateWidth = m_iObjectWidth;
	m_iSmallStateHeight = m_iObjectHeight * 0.6;

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

	auto onCollisionDelegater = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void 
	{
		auto pPhysics = GetComponent<PhysicsComponent>().lock();
		auto pOtherActor = static_cast<CActor*>(pOther);

		switch (static_cast<CActor*>(pOther)->GetActorType()) 
		{
		case Types::AT_ENEMY:
			switch (type)
			{
			case Collider::COLLISION_BOT:
				SetActorVerticalState(Types::VS_JUMP);
				pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
				break;
			default:
				if (m_PlayerState != PS_SMALL)
				{
					SetPlayerState(PS_SMALL);
				}
				else //Player die
				{
					
				}

				break;
			}
			break;
		case Types::AT_PROB:
			switch (type) {
			case Collider::COLLISION_BOT:
				pPhysics->SetGrounded(true);
				pPhysics->SetCurJumpForce(0.f);
				SetActorVerticalState(Types::VS_IDLE);
				SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
				break;
			case Collider::COLLISION_TOP:
				SetActorVerticalState(Types::VS_FALL);
				SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y + fIntersectLength);
				break;
			case Collider::COLLISION_LEFT:
				SetActorHorizonalState(Types::HS_IDLE);
				SetObjectPosition(GetObjectPosition().x + fIntersectLength, GetObjectPosition().y);
				pPhysics->SetCurSpeed(0.f);
				break;
			case Collider::COLLISION_RIGHT:
				SetActorHorizonalState(Types::HS_IDLE);
				SetObjectPosition(GetObjectPosition().x - fIntersectLength, GetObjectPosition().y);
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

	//Small state image
	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallIdleRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallIdleLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.15f, TEXT("MarioSmall"), TEXT("PlayerSmallWalkRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnimation(0.15f, TEXT("MarioSmall"), TEXT("PlayerSmallWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioSmall"), TEXT("PlayerSmallRunRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioSmall"), TEXT("PlayerSmallRunLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallLookupRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallLookupLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallSitdownRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallSitdownLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallJumpRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallJumpLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallFalldownRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallFalldownLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallRunJumpRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallRunJumpLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallTurnRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("TurnRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallTurnLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("TurnLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerDeadImage"), m_iObjectWidth, m_iObjectHeight, false, TEXT("DeadImage")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerDeadAnimation"), m_iObjectWidth, m_iObjectHeight, false, TEXT("DeadAnimation")))
		return false;


	//Big state image
	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"),TEXT("PlayerBigIdleRight"),m_iObjectWidth, m_iObjectHeight, false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigIdleLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioBig"), TEXT("PlayerBigWalkRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioBig"), TEXT("PlayerBigWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioBig"), TEXT("PlayerBigRunRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioBig"), TEXT("PlayerBigRunLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigLookupRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigLookupLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigSitdownRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigSitdownLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigJumpRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigJumpLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigFalldownRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigFalldownLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigRunJumpRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigRunJumpLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigTurnRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("TurnRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigTurnLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("TurnLeft")))
		return false;


	//Flower state image
	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerIdleRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerIdleLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioFlower"), TEXT("PlayerFlowerWalkRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioFlower"), TEXT("PlayerFlowerWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioFlower"), TEXT("PlayerFlowerRunRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioFlower"), TEXT("PlayerFlowerRunLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerLookupRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerLookupLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerSitdownRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerSitdownLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerJumpRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerJumpLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerFalldownRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerFalldownLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerRunJumpRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerRunJumpLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerTurnRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("TurnRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerTurnLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("TurnLeft")))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("MarioFlower"), TEXT("PlayerFlowerAttackRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("AttackRight"), false))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("MarioFlower"), TEXT("PlayerFlowerAttackLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("AttackLeft"), false))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("MarioFlower"), TEXT("PlayerFlowerJumpAttackRight"), m_iObjectWidth, m_iObjectHeight, false, TEXT("JumpAttackRight"), false))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("MarioFlower"), TEXT("PlayerFlowerJumpAttackLeft"), m_iObjectWidth, m_iObjectHeight, false, TEXT("JumpAttackLeft"), false))
		return false;

	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	//Fireball 생성
	if (!GenerateFireball())
		return false;

	m_PlayerState = PS_SMALL;
	pRender->SetCurAnimationTable(TEXT("MarioSmall"));
	pCollider->SetHeight(m_iSmallStateHeight);
	return true;
}

void CPlayer::Init()
{
	//m_ActorPoint = m_spawnPoint;
	m_pCamera->Init();

	for (const auto& it : m_ComponentTable)
		it.second->Init();

	SetPlayerState(PS_SMALL);

	for (const auto& fire : m_FireballPool)
	{
		fire->Init();
	}
}

void CPlayer::Update(double dDeltaTime)
{
	m_ActorCurState = Types::AS_IDLE;
	CActor::Update(dDeltaTime);
	for (const auto& fire : m_FireballPool)
	{
		fire->Update(dDeltaTime);
	}

}

void CPlayer::Render(const HDC & hDC)
{
	auto pRender = GetComponent<AnimationRender>();
	if (!pRender.expired())
	{
		pRender.lock()->Draw(hDC);
	}

	for (const auto& fire : m_FireballPool)
	{
		fire->Render(hDC);
	}

}

void CPlayer::LateUpdate()
{
	CObject::LateUpdate();
	for (const auto& fire : m_FireballPool)
	{
		fire->GetTransform().lock()->AdjustScreenPosition();
	}

}

void CPlayer::Attack()
{
	for (const auto& fire : m_FireballPool)
	{
		if (!fire->IsActive())
		{
			fire->SetFireballActive();
			break;
		}
	}

}

bool CPlayer::GenerateFireball()
{
	m_iAvailableFireballCount = 5;

	std::shared_ptr<CFireball> pFire;
	for (int i = 0; i < m_iAvailableFireballCount; ++i)
	{
		pFire = CObjectManager::GetInstance()->CreateActor<CFireball>(SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, GetObjectPosition().x, GetObjectPosition().y,
			Types::AT_THROWN, Types::AS_ATTACK, Types::VS_IDLE, Types::HS_IDLE, m_Direction, TEXT("Fireball"), static_cast<CGameScene*>(m_pOwnerScene));
		if (pFire == nullptr)
			return false;
		pFire->SetOwnerActor(this);
		m_FireballPool.emplace_back(std::move(pFire));
	}

	return true;
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

	if (!pPhysics->IsGrounded())
	{
		if (m_ActorCurVerticalState == Types::VS_FALL)
		{
			//m_bGrounded = false;
			if (pPhysics->GetCurJumpForce() > 0.f)
			{
				pPhysics->SetCurJumpForce(0.f);
			}
		}
	}
	auto pCollider = GetComponent<ColliderBox>().lock();
	if (m_ActorCurState == Types::AS_SITDOWN)
	{
		pCollider->SetCurRectHeight(pCollider->GetHeight() / 2.f);
	}
	else
	{
		pCollider->SetCurRectHeight(pCollider->GetHeight());
	}

	if (m_PlayerState == PS_FLOWER)
	{
		if (m_ActorCurState == Types::AS_ATTACK && m_iAvailableFireballCount > 0)
		{
			Attack();
		}
	}

}

bool CPlayer::AttachCamera(std::shared_ptr<CCamera> pCamera)
{
	if(m_pCamera)
		return false;

	m_pCamera = pCamera;

	return true;
}

void CPlayer::SetPlayerState(PlayerState state)
{
	auto pRender = GetComponent<AnimationRender>().lock();
	auto pCollider = GetComponent<ColliderBox>().lock();
	switch (state)
	{
	case PS_SMALL:
		if (m_PlayerState != PS_SMALL)
		{
			pRender->SetCurAnimationTable(TEXT("MarioSmall"));
			pCollider->SetHeight(m_iSmallStateHeight);
		}
		break;
	case PS_BIG:
		if (m_PlayerState == PS_SMALL)
		{
			pCollider->SetHeight(m_iObjectHeight*0.8);
		}

		if (m_PlayerState != PS_BIG)
		{
			pRender->SetCurAnimationTable(TEXT("MarioBig"));
		}
		break;
	case PS_FLOWER:
		if (m_PlayerState == PS_SMALL)
		{
			pCollider->SetHeight(m_iObjectHeight*0.8);
		}

		if (m_PlayerState != PS_BIG)
		{
			pRender->SetCurAnimationTable(TEXT("MarioFlower"));
		}
		break;
	}
	
	m_PlayerState = state;


}

CPlayer::PlayerState CPlayer::GetPlayerState()
{
	return m_PlayerState;
}

std::weak_ptr<CCamera> CPlayer::GetCamera()
{
	return m_pCamera;
}
