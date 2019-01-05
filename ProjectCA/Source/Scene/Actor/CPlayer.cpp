#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\..\Include\Scene\Actor\CPickup.h"
#include "..\..\..\Include\Scene\Actor\CRandomBlock.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScoreManager.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
//#include "..\..\..\Include\Core\Components\HPComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\CInputManager.h"
#include "..\..\..\Include\Core\Sound\CSoundManager.h"



CPlayer::CPlayer()
	:CActor()
{
}

CPlayer::~CPlayer()
{
	m_FireballPool.clear();
}


bool CPlayer::PostInit(const Types::ActorData& data, CGameScene* pScene)
{
	//TransformComponent 추가
	if (!CActor::PostInit(data, pScene))
	{
		return false;
	}

	m_pSoundManager	= CSoundManager::GetInstance();

	m_dTimeElapsed		= 0.f;
	m_iSmallStateWidth	= m_iEntityWidth;
	m_iSmallStateHeight	= m_iEntityHeight * 0.6;

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

		switch (pOther->GetObjectType())
		{
		case Types::OT_ENEMY:
			switch (type)
			{
			case Collider::COLLISION_BOT:
				if (pOther->GetObjectName() != TEXT("Piranha"))
				{
					SetActorVerticalState(Types::VS_JUMP);
					pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
				}
				else
				{
					if (GetActorAct() == Types::ACT_DESTROY)
					{
						SetActorVerticalState(Types::VS_JUMP);
						pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
						m_pSoundManager->SoundPlay(TEXT("SFXStompNoDamage"));
					}
				}
				break;
			default:
				if (GetObjectState() != Types::OS_DAMAGED)
				{
					HandlingEvent(Types::EVENT_DAMAGED);			
				}
				break;
			}
			break;
		case Types::OT_PROB:
		switch (type) {
			m_pSoundManager->SoundPlay(TEXT("SFXSheelRicochet"));
		case Collider::COLLISION_BOT:
				pPhysics->SetGrounded(true);
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
		case Types::OT_BLOCK:
		{
			auto pBlock = static_cast<CRandomBlock*>(pOther);

			if (pBlock->IsHiding())
			{
				if (m_ActorCurVerticalState != Types::VS_FALL)
				{
					if (type == Collider::COLLISION_TOP)
					{
						m_pSoundManager->SoundPlay(TEXT("SFXSheelRicochet"));
						pPhysics->SetCurJumpForce(0.f);
						SetActorVerticalState(Types::VS_FALL);
						SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y + fIntersectLength);
						break;
					}
				}
			}
			else
			{
				switch (type) {
				case Collider::COLLISION_TOP:
					SetActorVerticalState(Types::VS_FALL);
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y + fIntersectLength);
					m_pSoundManager->SoundPlay(TEXT("SFXSheelRicochet"));
					break;
				case Collider::COLLISION_BOT:
					pPhysics->SetGrounded(true);
					//pPhysics->SetCurJumpForce(0.f);
					SetActorVerticalState(Types::VS_IDLE);
					SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
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
			}
		}
		break;
		case Types::OT_GROUND:
			if (type == Collider::COLLISION_BOT)
			{
				if (GetObjectPosition().y > pOther->GetObjectPosition().y+ fIntersectLength
					|| GetActorVerticalState() == Types::VS_JUMP)
					return;
				pPhysics->SetGrounded(true);
				pPhysics->SetCurJumpForce(0.f);
				SetActorVerticalState(Types::VS_IDLE);
				SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
			}
			break;
		case Types::OT_PICKUP:
			if (pOther->GetObjectName() != TEXT("Coin") && pOther->GetObjectName() != TEXT("EndPickup"))
			{
				switch (m_PlayerState)
				{
				case PS_SMALL:
					if (pOther->GetObjectName() == TEXT("Mushroom"))
					{
						SetPlayerState(PS_BIG);
					}
					else if (pOther->GetObjectName() == TEXT("Flower"))
					{
						SetPlayerState(PS_FLOWER);
					}
					m_pCurPickupPtr = STATIC_POINTER_CAST(CObject, m_pOwnerScene->FindEntityFromScene(pOther->GetObjectID()).lock());
					break;
				case PS_BIG:
					if (pOther->GetObjectName() == TEXT("Flower"))
					{
						SetPlayerState(PS_FLOWER);
					}
					SetStoredPickup(m_pCurPickupPtr.lock());
					m_pCurPickupPtr = STATIC_POINTER_CAST(CObject, m_pOwnerScene->FindEntityFromScene(pOther->GetObjectID()).lock());
					break;
				case PS_FLOWER:
					if (pOther->GetObjectName() != TEXT("Coin"))
					{
						SetStoredPickup(STATIC_POINTER_CAST(CObject, m_pOwnerScene->FindEntityFromScene(pOther->GetObjectID()).lock()));
					}
					break;
				}
				m_pSoundManager->SoundPlay(TEXT("SFXPowerUp"));
			}
			break;
		case Types::OT_PICKABLE:
			if (pOther->GetComponent<PhysicsComponent>().lock()->GetCurSpeed() != 0.f)
			{
				if (type == Collider::COLLISION_BOT)
				{
					SetActorVerticalState(Types::VS_JUMP);
					pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
				}
				else
				{
					if (static_cast<CActor*>(pOther)->GetActorAct() == Types::ACT_ATTACK)
					{
						if (GetObjectState() != Types::OS_DAMAGED)
						{
							HandlingEvent(Types::EVENT_DAMAGED);
						}
					}
				}
			}
			else
			{
				if (m_pPickObjectPtr.expired())
				{
					if (pOther->GetComponent<PhysicsComponent>().lock()->GetCurSpeed() == 0.f && m_ActType != Types::ACT_DESTROY)
					{
						if (CInputManager::GetInstance()->IsKeyDown(TEXT("ACCEL")))
						{
							m_pPickObjectPtr = STATIC_POINTER_CAST(CObject, m_pOwnerScene->FindEntityFromScene(pOther->GetObjectID()).lock());
							m_pPickObjectPtr.lock()->SetOwnerObject(STATIC_POINTER_CAST(CObject, m_pOwnerScene->FindEntityFromScene(m_EntityID).lock()));
							m_pPickObjectPtr.lock()->GetComponent<PhysicsComponent>().lock()->SetStatic(true);
						}
					}
					else if (type == Collider::COLLISION_BOT)
					{
						SetActorVerticalState(Types::VS_JUMP);
						pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
						break;
					}
				}
			}
			break;
		}
	};

	pCollider->SetDelegate(onCollisionDelegater);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//RenderComponent 추가
	std::shared_ptr<AnimationRender> pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);

	//Small state image
	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallIdleRight"), false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallIdleLeft"), false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.15f, TEXT("MarioSmall"), TEXT("PlayerSmallWalkRight"), true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnimation(0.15f, TEXT("MarioSmall"), TEXT("PlayerSmallWalkLeft"), true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioSmall"), TEXT("PlayerSmallRunRight"),   true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioSmall"), TEXT("PlayerSmallRunLeft"),   true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallLookupRight"),   false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallLookupLeft"),   false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallSitdownRight"),   false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallSitdownLeft"),   false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallJumpRight"),   false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallJumpLeft"),   false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallFalldownRight"),   false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallFalldownLeft"),   false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallRunJumpRight"),   false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallRunJumpLeft"),   false, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallTurnRight"),   false, TEXT("TurnRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallTurnLeft"),   false, TEXT("TurnLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallPickIdleRight"),   false, TEXT("PickIdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallPickIdleLeft"),   false, TEXT("PickIdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("MarioSmall"), TEXT("PlayerSmallPickMoveRight"),   true, TEXT("PickWalkRight")))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("MarioSmall"), TEXT("PlayerSmallPickMoveLeft"),   true, TEXT("PickWalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallPickLookupRight"),   false, TEXT("PickLookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallPickLookupLeft"),   false, TEXT("PickLookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallPickSitdownRight"),   false, TEXT("PickSitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallPickSitdownLeft"),   false, TEXT("PickSitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallPickJumpRight"),   false, TEXT("PickJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerSmallPickJumpLeft"),   false, TEXT("PickJumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioSmall"), TEXT("PlayerSmallSpin"),   true, TEXT("Spin"), false))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioSmall"), TEXT("PlayerDeadImage"),   false, TEXT("DeadImage")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioSmall"), TEXT("PlayerDeadAnimation"),   true, TEXT("DeadAnimation")))
		return false;


	//Big state image
	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"),TEXT("PlayerBigIdleRight"),  false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigIdleLeft"),   false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioBig"), TEXT("PlayerBigWalkRight"),   true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioBig"), TEXT("PlayerBigWalkLeft"),   true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioBig"), TEXT("PlayerBigRunRight"),   true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioBig"), TEXT("PlayerBigRunLeft"),   true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigLookupRight"),   false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigLookupLeft"),   false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigSitdownRight"),   false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigSitdownLeft"),   false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigJumpRight"),   false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigJumpLeft"),   false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigFalldownRight"),   false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigFalldownLeft"),   false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigRunJumpRight"),   false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigRunJumpLeft"),   false, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigTurnRight"),   false, TEXT("TurnRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigTurnLeft"),   false, TEXT("TurnLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigPickIdleRight"),   false, TEXT("PickIdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigPickIdleLeft"),   false, TEXT("PickIdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("MarioBig"), TEXT("PlayerBigPickMoveRight"),   true, TEXT("PickWalkRight")))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("MarioBig"), TEXT("PlayerBigPickMoveLeft"),   true, TEXT("PickWalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigPickLookupRight"),   false, TEXT("PickLookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigPickLookupLeft"),   false, TEXT("PickLookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigPickSitdownRight"),   false, TEXT("PickSitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigPickSitdownLeft"),   false, TEXT("PickSitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigPickJumpRight"),   false, TEXT("PickJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioBig"), TEXT("PlayerBigPickJumpLeft"),   false, TEXT("PickJumpLeft")))
		return false;
	
	if (!pRender->AddAnimation(0.05f, TEXT("MarioBig"), TEXT("PlayerBigSpin"),   true, TEXT("Spin"), false))
		return false;

	//Flower state image
	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerIdleRight"),   false, TEXT("IdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerIdleLeft"),   false, TEXT("IdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioFlower"), TEXT("PlayerFlowerWalkRight"),   true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("MarioFlower"), TEXT("PlayerFlowerWalkLeft"),   true, TEXT("WalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioFlower"), TEXT("PlayerFlowerRunRight"),   true, TEXT("RunRight")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioFlower"), TEXT("PlayerFlowerRunLeft"),   true, TEXT("RunLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerLookupRight"),   false, TEXT("LookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerLookupLeft"),   false, TEXT("LookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerSitdownRight"),   false, TEXT("SitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerSitdownLeft"),   false, TEXT("SitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerJumpRight"),   false, TEXT("JumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerJumpLeft"),   false, TEXT("JumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerFalldownRight"),   false, TEXT("FalldownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerFalldownLeft"),   false, TEXT("FalldownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerRunJumpRight"),   false, TEXT("RunJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerRunJumpLeft"),   false, TEXT("RunJumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerTurnRight"),   false, TEXT("TurnRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerTurnLeft"),   false, TEXT("TurnLeft")))
		return false;

	if (!pRender->AddAnimation(0.12f, TEXT("MarioFlower"), TEXT("PlayerFlowerAttackRight"),   false, TEXT("AttackRight"), false))
		return false;

	if (!pRender->AddAnimation(0.12f, TEXT("MarioFlower"), TEXT("PlayerFlowerAttackLeft"),   false, TEXT("AttackLeft"), false))
		return false;

	if (!pRender->AddAnimation(0.12f, TEXT("MarioFlower"), TEXT("PlayerFlowerJumpAttackRight"),   false, TEXT("JumpAttackRight"), false))
		return false;

	if (!pRender->AddAnimation(0.12f, TEXT("MarioFlower"), TEXT("PlayerFlowerJumpAttackLeft"),   false, TEXT("JumpAttackLeft"), false))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerPickIdleRight"),   false, TEXT("PickIdleRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerPickIdleLeft"),   false, TEXT("PickIdleLeft")))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("MarioFlower"), TEXT("PlayerFlowerPickMoveRight"),   true, TEXT("PickWalkRight")))
		return false;

	if (!pRender->AddAnimation(0.1f, TEXT("MarioFlower"), TEXT("PlayerFlowerPickMoveLeft"),   true, TEXT("PickWalkLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerPickLookupRight"),   false, TEXT("PickLookupRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerPickLookupLeft"),   false, TEXT("PickLookupLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerPickSitdownRight"),   false, TEXT("PickSitdownRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerPickSitdownLeft"),   false, TEXT("PickSitdownLeft")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerPickJumpRight"),   false, TEXT("PickJumpRight")))
		return false;

	if (!pRender->AddAnimation(0.f, TEXT("MarioFlower"), TEXT("PlayerFlowerPickJumpLeft"),   false, TEXT("PickJumpLeft")))
		return false;

	if (!pRender->AddAnimation(0.05f, TEXT("MarioFlower"), TEXT("PlayerFlowerSpin"),   true, TEXT("Spin"), false))
		return false;

	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	//Fireball 생성
	if (!GenerateFireball())
		return false;

	m_PlayerState = PS_SMALL;
	pRender->ChangeAnimationTable(TEXT("MarioSmall"));
	pCollider->SetHeight(m_iSmallStateHeight);

	return true;
}

void CPlayer::Init()
{
	m_ActType						= Types::ACT_IDLE;
	m_ObjectState					= Types::OS_IDLE;
	m_dTimeElapsed				= 0.f;
	m_iAvailableFireballCount	= 3;

	m_pCurPickupPtr.reset();
	m_pStoredPickupPtr.reset();
	m_pPickObjectPtr.reset();

	for (const auto& it : m_ComponentTable)
	{
		it.second->Init();
	}

	SetPlayerState(PS_SMALL);

	for (const auto& fire : m_FireballPool)
	{
		fire->Init();
	}

	m_bActive = true;
}

void CPlayer::Update(double dDeltaTime)
{
	if (m_ObjectState != Types::OS_DEAD)
	{
		//무적시간 관련 연산
		if (GetObjectState() == Types::OS_DAMAGED)
		{
			GetComponent<AnimationRender>().lock()->SwitchBlending();
			m_dTimeElapsed += dDeltaTime;
			if (m_dTimeElapsed > 3.f)
			{
				GetComponent<AnimationRender>().lock()->SetRenderMode(RenderComponent::RenderMode::RENDER_DEFAULT);
				m_dTimeElapsed = 0.f;
				SetObjectState(Types::OS_IDLE);
			}
		}
		
		//Player Update
		CActor::Update(dDeltaTime);

		//Fireball Update
		for (const auto& fire : m_FireballPool)
		{
			fire->Update(dDeltaTime);
		}

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
		if (fire->IsActive())
		{
			fire->Render(hDC);
		}
	}

}

void CPlayer::LateUpdate()
{
	if (!IsDead())
	{
		if (GetEntityPosition().y > MAX_HEIGHT * 1.5f)
		{
			SetPlayerDead();
			return;
		}
	}
	CObject::LateUpdate();
	for (const auto& fire : m_FireballPool)
	{
		fire->LateUpdate();
	}

}

void CPlayer::SetPlayerDead()
{
	m_pSoundManager->ChangeBGM(TEXT("BGMDead"));
	m_ObjectState = Types::OS_DEAD;
	GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("MarioSmall"), TEXT("DeadAnimation"));
	GetComponent<AnimationRender>().lock()->SetPauseAnimation(true);
	GetComponent<PhysicsComponent>().lock()->SetCurJumpForce(700.f);
	GetComponent<PhysicsComponent>().lock()->SetGrounded(false);
	GetComponent<ColliderBox>().lock()->SetActive(false);
}

void CPlayer::DeadProcess(double dDeltaTime)
{
	m_dTimeElapsed += dDeltaTime;
	
	if (m_dTimeElapsed > 0.5f)
	{
		GetComponent<AnimationRender>().lock()->SetPauseAnimation(false);
		if (m_dTimeElapsed < 4.0f)
		{
			GetComponent<AnimationRender>().lock()->Update(dDeltaTime);
			GetComponent<PhysicsComponent>().lock()->Update(dDeltaTime);
			GetTransform().lock()->Move(0.f, GetComponent<PhysicsComponent>().lock()->GetCurJumpForce()* dDeltaTime);
		}
		else
		{
			SetActive(false);
		}

	}
	LateUpdate();
}

void CPlayer::InterruptProecess(double dDeltaTime)
{




}

void CPlayer::IncreaseAvailableFireballCount()
{
	if (m_iAvailableFireballCount < 3)
	{
		++m_iAvailableFireballCount;
	}
}

void CPlayer::Attack()
{
	auto pRender = GetComponent<AnimationRender>().lock();

	//공격 모션
	if (m_ActType == Types::ACT_ATTACK && m_iAvailableFireballCount > 0)
	{
		if (m_ActorCurVerticalState == Types::VS_IDLE)
		{
			if (m_Direction == Types::DIR_RIGHT)
			{
				pRender->ChangeAnimation(TEXT("AttackRight"));
			}
			else if (m_Direction == Types::DIR_LEFT)
			{
				pRender->ChangeAnimation(TEXT("AttackLeft"));
			}
		}
		else
		{
			if (m_Direction == Types::DIR_RIGHT)
			{
				pRender->ChangeAnimation(TEXT("JumpAttackRight"));
			}
			else if (m_Direction == Types::DIR_LEFT)
			{
				pRender->ChangeAnimation(TEXT("JumpAttackLeft"));
			}
		}
		m_ObjectState = Types::OS_IDLE;
	}

	for (const auto& fire : m_FireballPool)
	{
		if (!fire->IsActive())
		{
			fire->SetFireballActive();
			--m_iAvailableFireballCount;
			break;
		}
	}

	m_ActType = Types::ACT_IDLE;
}

bool CPlayer::GenerateFireball()
{
	m_iAvailableFireballCount = 3;

	std::shared_ptr<CFireball> pFire;
	for (int i = 0; i < m_iAvailableFireballCount; ++i)
	{
		pFire = CObjectManager::GetInstance()->CreateActor<CFireball>(SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, GetObjectPosition().x, GetObjectPosition().y,
			Types::OT_BULLET, Types::ACT_ATTACK, m_Direction, TEXT("Fireball"), static_cast<CGameScene*>(m_pOwnerScene));
		if (pFire == nullptr)
			return false;
		pFire->SetOwnerActor(this);
		m_FireballPool.emplace_back(std::move(pFire));
	}

	return true;
}

void CPlayer::ChangeAnimationClip(float fCurSpeed, float fWalkSpeed, float fMaxSpeed, float fCurJumpForce)
{
	auto pRender = GetComponent<AnimationRender>().lock();
	float fCurSpeedAbs = std::fabsf(fCurSpeed);

	if (m_pPickObjectPtr.expired())
	{
		//앉는 모션
		if (m_ActType == Types::ACT_SITDOWN)
		{
			if (m_Direction == Types::DIR_RIGHT)
			{
				pRender->ChangeAnimation(TEXT("SitdownRight"));
			}
			else if (m_Direction == Types::DIR_LEFT)
			{
				pRender->ChangeAnimation(TEXT("SitdownLeft"));
			}

			return;
		}

		//위를 보는 모션 모션
		if (m_ActorCurVerticalState == Types::VS_IDLE)
		{
			if (m_ActType == Types::ACT_LOOKUP)
			{
				if (m_Direction == Types::DIR_RIGHT)
				{
					pRender->ChangeAnimation(TEXT("LookupRight"));
				}
				else if (m_Direction == Types::DIR_LEFT)
				{
					pRender->ChangeAnimation(TEXT("LookupLeft"));
				}

				return;
			}
		}

		//이동 관련 Animation
		if (m_ActorCurVerticalState == Types::VS_JUMP)
		{
			if (GetActorAct() == Types::ACT_DESTROY)
			{
				pRender->ChangeAnimation(TEXT("Spin"));
			}
			else
			{
				if (fCurSpeedAbs >= fMaxSpeed)
				{
					if (m_Direction == Types::DIR_RIGHT)
					{
						pRender->ChangeAnimation(TEXT("RunJumpRight"));
					}
					else if (m_Direction == Types::DIR_LEFT)
					{
						pRender->ChangeAnimation(TEXT("RunJumpLeft"));
					}
				}
				else
				{
					if (m_Direction == Types::DIR_RIGHT)
					{
						pRender->ChangeAnimation(TEXT("JumpRight"));
					}
					else if (m_Direction == Types::DIR_LEFT)
					{
						pRender->ChangeAnimation(TEXT("JumpLeft"));
					}
				}
			}
		}
		else if (m_ActorCurVerticalState == Types::VS_FALL)
		{
			if (GetActorAct() == Types::ACT_DESTROY)
			{
				pRender->ChangeAnimation(TEXT("Spin"));
			}
			else
			{
				if (fCurSpeedAbs >= fMaxSpeed)
				{
					if (m_Direction == Types::DIR_RIGHT)
					{
						pRender->ChangeAnimation(TEXT("RunJumpRight"));
					}
					else if (m_Direction == Types::DIR_LEFT)
					{
						pRender->ChangeAnimation(TEXT("RunJumpLeft"));
					}
				}
				else
				{
					if (m_Direction == Types::DIR_RIGHT)
					{
						pRender->ChangeAnimation(TEXT("FalldownRight"));
					}
					else if (m_Direction == Types::DIR_LEFT)
					{
						pRender->ChangeAnimation(TEXT("FalldownLeft"));
					}
				}
			}
		}
		else
		{
			if (fCurSpeed < 0.f)	//힘의 방향이 왼쪽인 경우(LEFT)
			{
				if (m_Direction == Types::DIR_LEFT)
				{
					if (fCurSpeedAbs >= fMaxSpeed)
					{
						pRender->ChangeAnimation(TEXT("RunLeft"));
					}
					else if (fCurSpeedAbs > 0.f)
					{
						pRender->ChangeAnimation(TEXT("WalkLeft"));
					}
				}
				else if (m_Direction == Types::DIR_RIGHT)
				{
					if (m_ActorHorizonalState != Types::HS_IDLE)
					{
						pRender->ChangeAnimation(TEXT("TurnRight"));
					}
					else
					{
						pRender->ChangeAnimation(TEXT("WalkRight"));
					}
				}
			}
			else if (fCurSpeed > 0.f)		//힘의 방향이 오른쪽인 있는 경우(RIGHT)
			{
				if (m_Direction == Types::DIR_RIGHT)
				{
					if (fCurSpeedAbs >= fMaxSpeed)
					{
						pRender->ChangeAnimation(TEXT("RunRight"));
					}
					else if (fCurSpeedAbs > 0.f)
					{
						pRender->ChangeAnimation(TEXT("WalkRight"));
					}
				}
				else if (m_Direction == Types::DIR_LEFT)
				{
					if (m_ActorHorizonalState != Types::HS_IDLE)
					{
						pRender->ChangeAnimation(TEXT("TurnLeft"));
					}
					else
					{
						pRender->ChangeAnimation(TEXT("WalkLeft"));
					}
				}
			}
			else
			{
				if (m_Direction == Types::DIR_RIGHT)
				{
					pRender->ChangeAnimation(TEXT("IdleRight"));
				}
				else if (m_Direction == Types::DIR_LEFT)
				{
					pRender->ChangeAnimation(TEXT("IdleLeft"));
				}
			}
		}
	}
	else
	{
		//앉는 모션
		if (m_ActType == Types::ACT_SITDOWN)
		{
			if (m_Direction == Types::DIR_RIGHT)
			{
				pRender->ChangeAnimation(TEXT("PickSitdownRight"));
			}
			else if (m_Direction == Types::DIR_LEFT)
			{
				pRender->ChangeAnimation(TEXT("PickSitdownLeft"));
			}

			return;
		}

		//위를 보는 모션 모션
		if (m_ActorCurVerticalState == Types::VS_IDLE)
		{
			if (m_ActType == Types::ACT_LOOKUP)
			{
				if (m_Direction == Types::DIR_RIGHT)
				{
					pRender->ChangeAnimation(TEXT("PickLookupRight"));
				}
				else if (m_Direction == Types::DIR_LEFT)
				{
					pRender->ChangeAnimation(TEXT("PickLookupLeft"));
				}

				return;
			}
		}

		//이동 관련 Animation
		if (m_ActorCurVerticalState != Types::VS_IDLE)
		{
			if (m_Direction == Types::DIR_RIGHT)
			{
				pRender->ChangeAnimation(TEXT("PickJumpRight"));
			}
			else if (m_Direction == Types::DIR_LEFT)
			{
				pRender->ChangeAnimation(TEXT("PickJumpLeft"));

			}
		}
		else
		{
			if (fCurSpeedAbs > 0.f)
			{
				if (m_Direction == Types::DIR_LEFT)
				{
					pRender->ChangeAnimation(TEXT("PickWalkLeft"));
				}
				else if (m_Direction == Types::DIR_RIGHT)
				{
					pRender->ChangeAnimation(TEXT("PickWalkRight"));
				}
			}
			else
			{
				if (m_Direction == Types::DIR_LEFT)
				{
					pRender->ChangeAnimation(TEXT("PickIdleLeft"));
				}
				else if (m_Direction == Types::DIR_RIGHT)
				{
					pRender->ChangeAnimation(TEXT("PickIdleRight"));
				}
			}
		}
	}

	if (GetActorVerticalState() == Types::VS_IDLE)
	{
		if (fCurSpeedAbs > 0.f)
		{
			if (fCurSpeedAbs <= fWalkSpeed * 0.33f)
			{
				pRender->SetAnimationPlaySpeed(0.33f);
			}
			else if (fCurSpeedAbs <= fWalkSpeed * 0.66f)
			{
				pRender->SetAnimationPlaySpeed(0.66f);
			}
			else
			{
				pRender->SetAnimationPlaySpeed(1.f);
			}
		}
	}
	else
	{
		pRender->SetAnimationPlaySpeed(1.f);
	}

}

void CPlayer::PopStoredPickup()
{
	if (m_pStoredPickupPtr.expired())
		return;
	auto pCamera = CCameraManager::GetInstance()->GetMainCamera().lock();
	POSITION position((2.f * pCamera->GetCameraPosition().x + pCamera->GetCameraWidth()) / 2.f, 70.f);

	m_pStoredPickupPtr.lock()->GetTransform().lock()->SetPosition(position);
	m_pStoredPickupPtr.lock()->SetObjectState(Types::OS_IDLE);
	m_pStoredPickupPtr.lock()->SetActive(true);
	m_pStoredPickupPtr.reset();
	m_pSoundManager->SoundPlay(TEXT("SFXReserveItemReleaseUp"));
}


void CPlayer::ActorBehavior(double dDeltaTime)
{
	//이동 및 물리처리 관련 Player로직

	auto pTransform		= GetComponent<TransformComponent>().lock();
	auto pPhysics			= GetComponent<PhysicsComponent>().lock();

	float fCurSpeed		= pPhysics->GetCurSpeed();
	float fMaxSpeed		= pPhysics->GetMaxSpeed();
	float fWalkSpeed		= pPhysics->GetSpeed();
	float fCurJumpForce	= pPhysics->GetCurJumpForce();

	//수직상의 이동에 대한 연산
	if (m_Direction == Types::DIR_LEFT)
	{
		if (m_ActorHorizonalState == Types::HS_RUN)
		{
			if (fCurSpeed > -1 * fMaxSpeed)
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 10.f);
			}
		}
		else if (m_ActorHorizonalState == Types::HS_WALK)
		{
			if (fCurSpeed < -1 * fWalkSpeed)
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 10.f);
			}
			else if (pPhysics->GetCurSpeed() > -1 * fWalkSpeed)
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 10.f);
			}
			else if (pPhysics->GetCurSpeed() <= -1 * fWalkSpeed)
			{
				pPhysics->SetCurSpeed(-1.f * pPhysics->GetSpeed());
			}
		}
		else if (m_ActorHorizonalState == Types::HS_IDLE)
		{
			if (fCurSpeed < 0.f)
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() + 10.f);
				if (pPhysics->GetCurSpeed() > 0.f)
				{
					pPhysics->SetCurSpeed(0.f);
				}
			}
			else if (fCurSpeed > 0.f)
			{
				pPhysics->SetCurSpeed(pPhysics->GetCurSpeed() - 10.f);
				if (pPhysics->GetCurSpeed() < 0.f)
				{
					pPhysics->SetCurSpeed(0.f);
				}
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
			{
				pPhysics->SetCurSpeed(fCurSpeed - 10.f);
			}
			else if (fCurSpeed < fWalkSpeed)
			{
				pPhysics->SetCurSpeed(fCurSpeed + 10.f);
			}
			else if (fCurSpeed >= fWalkSpeed)
			{
				pPhysics->SetCurSpeed(fWalkSpeed);
			}
		}
		else if (m_ActorHorizonalState == Types::HS_IDLE)
		{
			if (fCurSpeed > 0.f)
			{
				pPhysics->SetCurSpeed(fCurSpeed - 5.f);
				if (pPhysics->GetCurSpeed() < 0.f)
				{
					pPhysics->SetCurSpeed(0.f);
				}
			}
			else if (fCurSpeed < 0.f)
			{
				pPhysics->SetCurSpeed(fCurSpeed + 5.f);
				if (pPhysics->GetCurSpeed() > 0.f)
				{
					pPhysics->SetCurSpeed(0.f);
				}
			}
		}
	}

	//Jump관련 연산
	if (pPhysics->IsGrounded())
	{
		if (m_ActorCurVerticalState == Types::VS_JUMP)
		{
			pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
			pPhysics->SetGrounded(false);
		}
	}
	pTransform->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);

	//PickObject Update
	if (!m_pPickObjectPtr.expired())
	{
		if (CInputManager::GetInstance()->IsKeyDown(TEXT("ACCEL")))
		{
			if (m_Direction == Types::DIR_LEFT)
			{
				m_pPickObjectPtr.lock()->SetObjectPosition(GetObjectPosition().x - 20.f, GetObjectPosition().y - m_iEntityHeight * 0.15f);
			}
			else if (m_Direction == Types::DIR_RIGHT)
			{
				m_pPickObjectPtr.lock()->SetObjectPosition(GetObjectPosition().x + 20.f, GetObjectPosition().y - m_iEntityHeight * 0.15f);
			}
		}
		else
		{
			STATIC_POINTER_CAST(CActor, m_pPickObjectPtr.lock())->SetActorAct(Types::ACT_ATTACK);
			auto pPhysics = m_pPickObjectPtr.lock()->GetComponent<PhysicsComponent>().lock();
			pPhysics->SetStatic(false);
			if (CInputManager::GetInstance()->IsKeyDown(TEXT("UP")))
			{
				m_pPickObjectPtr.lock()->SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - m_iEntityHeight * 0.4f);
				pPhysics->SetCurJumpForce(pPhysics->GetJumpForce());
				m_pSoundManager->SoundPlay(TEXT("SFXKick"));

			}
			else if (CInputManager::GetInstance()->IsKeyDown(TEXT("DOWN")))
			{
				if (m_Direction == Types::DIR_LEFT)
				{
					m_pPickObjectPtr.lock()->SetObjectPosition(GetObjectPosition().x - 50.f, GetObjectPosition().y - m_iEntityHeight * 0.1f);
				}
				else if (m_Direction == Types::DIR_RIGHT)
				{
					m_pPickObjectPtr.lock()->SetObjectPosition(GetObjectPosition().x + 50.f, GetObjectPosition().y - m_iEntityHeight * 0.1f);
				}
			}
			else
			{
				if (m_Direction == Types::DIR_LEFT)
				{
					STATIC_POINTER_CAST(CActor, m_pPickObjectPtr.lock())->SetActorDirection(Types::DIR_LEFT);
					m_pPickObjectPtr.lock()->SetObjectPosition(GetObjectPosition().x - 50.f, GetObjectPosition().y - m_iEntityHeight * 0.15f);
					pPhysics->SetCurSpeed(-1 * pPhysics->GetMaxSpeed());
				}
				else if (m_Direction == Types::DIR_RIGHT)
				{
					STATIC_POINTER_CAST(CActor, m_pPickObjectPtr.lock())->SetActorDirection(Types::DIR_RIGHT);
					m_pPickObjectPtr.lock()->SetObjectPosition(GetObjectPosition().x + 50.f, GetObjectPosition().y - m_iEntityHeight * 0.15f);
					pPhysics->SetCurSpeed(pPhysics->GetMaxSpeed());
				}
				m_pSoundManager->SoundPlay(TEXT("SFXKick"));
			}
			m_pPickObjectPtr.lock()->SetOwnerObject(nullptr);
			m_pPickObjectPtr.reset();
		}
	}

	//Jump 관련 연산
	if (!pPhysics->IsGrounded())
	{
		if (pPhysics->GetCurJumpForce() < 0.f)
		{
			SetActorVerticalState(Types::VS_FALL);
		}
		else if (pPhysics->GetCurJumpForce() > 0.f)
		{
			if (m_ActorCurVerticalState == Types::VS_FALL)
			{
				pPhysics->SetCurJumpForce(0.f);
			}
		}
	}

	//Collider 변환 관련 Player로직
	{
		auto pCollider = GetComponent<ColliderBox>().lock();
		if (m_ActType == Types::ACT_SITDOWN)
		{
			pCollider->SetCurRectHeight(pCollider->GetHeight() / 2.f);
		}
		else
		{
			pCollider->SetCurRectHeight(pCollider->GetHeight());
		}
	}
	
	//공격 관련 로직
	//무언가를 들고있는 상태면 발사 불가능
	if (m_pPickObjectPtr.expired())
	{
		if (m_PlayerState == PS_FLOWER)
		{
			if (m_ActType == Types::ACT_ATTACK && m_iAvailableFireballCount > 0)
			{
				Attack();
				m_pSoundManager->SoundPlay(TEXT("SFXFireball"));
			}
		}
	}

	if (CInputManager::GetInstance()->IsKeyDown(TEXT("FUNC1")))
	{
		PopStoredPickup();
	}

	ChangeAnimationClip(fCurSpeed, fWalkSpeed, fMaxSpeed, fCurJumpForce);

}

void CPlayer::HandlingEvent(EVENT_TYPE type)
{
	switch (type)
	{
	case Types::EVENT_DAMAGED:
		if (m_PlayerState != PS_SMALL)	//PlayerDamaged
		{
			SetPlayerState(PS_SMALL);
			PopStoredPickup();
			m_pCurPickupPtr.reset();
			SetObjectState(Types::OS_DAMAGED);
		}
		else //Player die
		{
			SetPlayerDead();
		}
		break;
	}

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
			pRender->ChangeAnimationTable(TEXT("MarioSmall"));
			pCollider->SetHeight(m_iSmallStateHeight);
			m_pSoundManager->SoundPlay(TEXT("SFXPipe"));
		}
		break;
	case PS_BIG:
		if (m_PlayerState == PS_SMALL)
		{
			pCollider->SetHeight(m_iEntityHeight*0.8);
		}

		if (m_PlayerState != PS_BIG)
		{
			pRender->ChangeAnimationTable(TEXT("MarioBig"));
		}
		break;
	case PS_FLOWER:
		if (m_PlayerState == PS_SMALL)
		{
			pCollider->SetHeight(m_iEntityHeight*0.8);
		}

		if (m_PlayerState != PS_FLOWER)
		{
			pRender->ChangeAnimationTable(TEXT("MarioFlower"));
		}
		break;
	}
	
	m_PlayerState = state;


}

void CPlayer::SetRequestInterrupt(bool bInterrupt)
{
	m_bInterrupt = bInterrupt;
}

void CPlayer::SetStoredPickup(std::shared_ptr<CObject> pPickup)
{
	m_pStoredPickupPtr = pPickup;
}

CPlayer::PlayerState CPlayer::GetPlayerState()
{
	return m_PlayerState;
}

bool CPlayer::IsRequestInterrupt()
{
	return m_bInterrupt;
}

bool CPlayer::IsPickingObject()
{
	return !m_pPickObjectPtr.expired();
}

std::weak_ptr<CObject> CPlayer::GetStoredPickup()
{
	return m_pStoredPickupPtr.lock();
}
