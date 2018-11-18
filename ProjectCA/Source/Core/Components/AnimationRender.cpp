#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\Collider.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Core\Graphic\CAnim.h"



AnimationRender::AnimationRender()
	:RenderComponent(), 
	m_AnimationState(Types::AM_IDLE)
{
}

AnimationRender::~AnimationRender()
{
	m_AnimationTable.clear();
}


bool AnimationRender::PostInit(CObject * pOwner, const Types::tstring & strTag)
{
	if (!RenderComponent::PostInit(pOwner, strTag))
		return false;

	m_pActor = static_cast<CActor*>(pOwner);

	m_OwnerState = m_pActor->GetActorState();
	m_OwnerVerticalState = m_pActor->GetActorVerticalState();
	m_OwnerHorizonalState = m_pActor->GetActorHorizonalState();
	m_OwnerDirection = m_pActor->GetActorDirection();

	m_AnimationState = m_PreAnimationState = Types::AM_IDLE;

	return true;
}

void AnimationRender::Init()
{
	RenderComponent::Init();

	m_AnimationState = m_PreAnimationState = Types::AM_IDLE;
}

void AnimationRender::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		m_OwnerState = m_pActor->GetActorState();
		m_OwnerVerticalState = m_pActor->GetActorVerticalState();
		m_OwnerHorizonalState = m_pActor->GetActorHorizonalState();
		m_OwnerDirection = m_pActor->GetActorDirection();

		UpdateAnimationMotion();
		if (m_pCurAnimation.lock()->IsReadyToChange())
			ChangeAnimationClip(m_AnimationState);

		m_pCurAnimation.lock()->Update(dDeltaTime);
	}
}

void AnimationRender::Draw(const HDC & hDC)
{
	if (m_bActive)
	{

		POSITION pivot = m_pOwner->GetComponent<TransformComponent>().lock()->GetScreenPivot();
		POSITION position = m_pOwner->GetComponent<TransformComponent>().lock()->GetScreenPosition();
		if (!m_pCurAnimation.expired())
			m_pCurAnimation.lock()->Draw(hDC, m_hRenderDC, pivot);

		std::shared_ptr<Collider> pCollider = STATIC_POINTER_CAST(Collider, m_pOwner->GetComponent(TEXT("Collider")).lock());
		std::shared_ptr<PhysicsComponent> pPhysics = STATIC_POINTER_CAST(PhysicsComponent, m_pOwner->GetComponent(TEXT("PhysicsComponent")).lock());

		if (pCollider)
		{
			pCollider->DrawCollider(hDC, position);

			if (pCollider->IsCollision())
			{
				TextOut(hDC, position.x, position.y, TEXT("TRUE"), sizeof(TEXT("TRUE")));
			}
			else
			{
				TextOut(hDC, position.x, position.y, TEXT("FALSE"), sizeof(TEXT("FALSE")));
			}

		}

		if (pPhysics)
		{
			if (pPhysics->IsGrounded())
			{
				TextOut(hDC, position.x, position.y + 20, TEXT("TRUE"), sizeof(TEXT("TRUE")));
			}
			else
			{
				TextOut(hDC, position.x, position.y + 20, TEXT("FALSE"), sizeof(TEXT("FALSE")));
			}

		}



	}
}

bool AnimationRender::AddAnimation(double dPlayTime, const TSTRING& strMapName, const TSTRING& strSpriteName, 
	UINT iWidth, UINT iHeight, bool bLoop, const Types::tstring & strAnimTag, bool bInterrupt)
{
	auto pAnim = std::make_shared<CAnim>();

	if (!pAnim->Init(strSpriteName, iWidth, iHeight, dPlayTime, bLoop, strAnimTag, bInterrupt))
		return false;

	if (m_pCurAnimation.expired())
	{
		m_pCurAnimation = pAnim;
		m_strCurTableName = strMapName;
	}

	auto tableIter = m_AnimationTable.find(strMapName);
	if (tableIter == m_AnimationTable.end())
	{
		AnimationMap map;
		map.insert(std::make_pair(strAnimTag, pAnim));
		m_AnimationTable.insert(std::make_pair(strMapName, map));
	}
	else
	{
		//테이블 안에 해당 Key값과 연결된 vector가 있는 경우
		if (tableIter->second.find(strAnimTag) != tableIter->second.end())
		{
			tableIter->second[strAnimTag] = pAnim;
		}
		else
		{
			//테이블 안에 해당 Key값과 연결된 vector가 없는 경우
			//해당 Key값과 연결된 Vector를 생성한 후, Anim 추가
			tableIter->second.insert(std::make_pair(strAnimTag, pAnim));
		}

	}

	return true;

}

bool AnimationRender::SetAnimationPlaySpeed(double dSpeed)
{
	return m_pCurAnimation.lock()->SetPlaySpeed(dSpeed);
}

void AnimationRender::SetAnimationMotion(ANIM_MOTION motion)
{
	m_AnimationState = motion;
}

void AnimationRender::SetCurAnimationTable(const TSTRING & strTableName)
{
	m_strCurTableName = strTableName;
	m_pCurAnimation = m_AnimationTable.at(strTableName).at(m_pCurAnimation.lock()->GetAnimTag());
}

const TSTRING AnimationRender::GetAnimTag() const
{
	return m_pCurAnimation.lock()->GetAnimTag();
}

void AnimationRender::UpdateAnimationMotion()
{
	auto pPhysics = m_pOwner->GetComponent<PhysicsComponent>().lock();
	if (pPhysics == nullptr)
		return;

	float fCurSpeed = std::fabsf(pPhysics->GetCurSpeed());
	float fSpeed = pPhysics->GetSpeed();
	float fMaxSpeed = pPhysics->GetMaxSpeed();

	if (m_OwnerState == Types::AS_DAMAGED)
	{
		m_AnimationState = Types::AM_DAMAGED;
		return;
	}

	if (m_OwnerState == Types::AS_ATTACK)
	{
		m_AnimationState = Types::AM_ATTACK;
		return;
	}

	if (m_OwnerState == Types::AS_SITDOWN)
	{
		m_AnimationState = Types::AM_SITDOWN;
		return;
	}
	else if (m_OwnerState == Types::AS_LOOKUP)
	{
		m_AnimationState = Types::AM_LOOKUP;
		return;
	}

	if (fCurSpeed > 0.f && fCurSpeed < fMaxSpeed)
	{
		m_AnimationState = Types::AM_WALK;

	}
	else if (fCurSpeed >= fMaxSpeed)
	{
		m_AnimationState = Types::AM_RUN;
	}
	else
	{
		m_AnimationState = Types::AM_IDLE;
	}

	if (m_OwnerVerticalState == Types::VS_IDLE)
	{
		if (pPhysics->GetCurSpeed() < 0.f)
		{
			if (m_OwnerDirection == Types::DIR_RIGHT && m_OwnerHorizonalState != Types::HS_IDLE)
				m_AnimationState = Types::AM_TURN;
		}
		else if (pPhysics->GetCurSpeed() > 0.f)
		{
			if (m_OwnerDirection == Types::DIR_LEFT && m_OwnerHorizonalState != Types::HS_IDLE)
				m_AnimationState = Types::AM_TURN;
		}
	}

	if (fCurSpeed < fSpeed / 3)
		SetAnimationPlaySpeed(0.3f);
	else if (fCurSpeed < (fSpeed / 3) * 2)
		SetAnimationPlaySpeed(0.6f);
	else
		SetAnimationPlaySpeed(1.f);

}

bool AnimationRender::ChangeAnimation(const TSTRING & strAnimTag)
{
	auto animIter = m_AnimationTable.at(m_strCurTableName).find(strAnimTag);
	if (animIter == m_AnimationTable.at(m_strCurTableName).end())
	{
		return false;
	}
	//같은 animation인지 판별
	if (!m_pCurAnimation.expired())
	{
		if (m_pCurAnimation.lock()->GetAnimTag() == strAnimTag )
			return true;

		m_pCurAnimation.lock()->ClearEleapsedTime();
	}

	m_pCurAnimation = animIter->second;

	return true;
}

void AnimationRender::ChangeAnimationClip(ANIM_MOTION motion)
{
	if (m_OwnerVerticalState == Types::VS_JUMP) {
		switch (motion) {
		case Types::AM_IDLE:
		case Types::AM_WALK:
		case Types::AM_LOOKUP:
			if (m_OwnerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("JumpLeft"));
			}
			else if (m_OwnerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("JumpRight"));
			}
			break;
		case Types::AM_SITDOWN:
			if (m_OwnerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("SitdownLeft"));
			}
			else if (m_OwnerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("SitdownRight"));
			}
			break;
		case Types::AM_RUN:
			if (m_OwnerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("RunJumpLeft"));
			}
			else if (m_OwnerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("RunJumpRight"));
			}
			break;
		case Types::AM_ATTACK:
			if (m_OwnerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("JumpAttackLeft"));
			else if (m_OwnerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("JumpAttackRight"));
			break;
		case Types::AM_DAMAGED:
			if (m_OwnerDirection == Types::DIR_LEFT)
			{
				if (!ChangeAnimation(TEXT("DamagedLeft")))
				{
					ChangeAnimation(TEXT("Damaged"));
				}
			}
			else if (m_OwnerDirection == Types::DIR_RIGHT)
			{
				if (!ChangeAnimation(TEXT("DamagedRight")))
				{
					ChangeAnimation(TEXT("Damaged"));
				}
			}
			break;
		}

	}
	else if (m_OwnerVerticalState == Types::VS_FALL) {
		switch (motion) {
		case Types::AM_IDLE:
		case Types::AM_WALK:
		case Types::AM_LOOKUP:
			if (m_OwnerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("FalldownLeft"));
			}
			else if (m_OwnerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("FalldownRight"));
			}
			break;
		case Types::AM_SITDOWN:
			if (m_OwnerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("SitdownLeft"));
			}
			else if (m_OwnerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("SitdownRight"));
			}
			break;
		case Types::AM_RUN:
			if (m_OwnerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("RunJumpLeft"));
			}
			else if (m_OwnerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("RunJumpRight"));
			}
			break;
		case Types::AM_ATTACK:
			if (m_OwnerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("JumpAttackLeft"));
			else if (m_OwnerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("JumpAttackRight"));
			break;
		case Types::AM_DAMAGED:
			if (m_OwnerDirection == Types::DIR_LEFT)
			{
				if (!ChangeAnimation(TEXT("DamagedLeft")))
				{
					ChangeAnimation(TEXT("Damaged"));
				}
			}
			else if (m_OwnerDirection == Types::DIR_RIGHT)
			{
				if (!ChangeAnimation(TEXT("DamagedRight")))
				{
					ChangeAnimation(TEXT("Damaged"));
				}
			}
			break;
		}
	}
	else {
		switch (motion) {
		case Types::AM_IDLE:
			if (m_OwnerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("IdleLeft"));
			else if (m_OwnerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("IdleRight"));
			break;
		case Types::AM_WALK:
			if (m_OwnerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("WalkLeft"));
			else if (m_OwnerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("WalkRight"));
			break;
		case Types::AM_RUN:
			if (m_OwnerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("RunLeft"));
			else if (m_OwnerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("RunRight"));
			break;
		case Types::AM_TURN:
			if (m_OwnerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("TurnLeft"));
			else if (m_OwnerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("TurnRight"));
			break;
		case Types::AM_LOOKUP:
			if (m_OwnerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("LookupLeft"));
			else if (m_OwnerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("LookupRight"));
			break;
		case Types::AM_SITDOWN:
			if (m_OwnerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("SitdownLeft"));
			else if (m_OwnerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("SitdownRight"));
			break;
		case Types::AM_ATTACK:
			if (m_OwnerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("AttackLeft"));
			else if (m_OwnerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("AttackRight"));
			break;
		case Types::AM_DAMAGED:
			if (m_OwnerDirection == Types::DIR_LEFT)
			{
				if (!ChangeAnimation(TEXT("DamagedLeft")))
				{
					ChangeAnimation(TEXT("Damaged"));
				}
			}
			else if (m_OwnerDirection == Types::DIR_RIGHT)
			{
				if (!ChangeAnimation(TEXT("DamagedRight")))
				{
					ChangeAnimation(TEXT("Damaged"));
				}
			}
			break;

		}

	}
}



