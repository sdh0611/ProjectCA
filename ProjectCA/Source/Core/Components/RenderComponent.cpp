#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\Collider.h"
#include	"..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Core\Graphic\CAnim.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"



RenderComponent::RenderComponent()
	:m_bVisible(true), m_bChangeAnim(false)
{
}

RenderComponent::~RenderComponent()
{
	m_animationTable.clear();
}

bool RenderComponent::PostInit(CActor * pOwner, const Types::tstring & strTag)
{
	std::shared_ptr<CActor> pActor = std::shared_ptr<CActor>(pOwner);
	m_pOwner					= pActor;

	m_strComponentTag		= strTag;
	
	m_ownerState				= m_pOwner->GetActorState();
	m_ownerVerticalState		= m_pOwner->GetActorVerticalState();
	m_ownerDirection			= m_pOwner->GetActorDirection();

	m_animationState			= Types::AM_IDLE;

	return true;
}

void RenderComponent::Init() 
{

}


void RenderComponent::Update(double dDeltaTime)
{
	m_ownerState				= m_pOwner->GetActorState();
	m_ownerDirection			= m_pOwner->GetActorDirection();
	m_ownerVerticalState		= m_pOwner->GetActorVerticalState();

	UpdateAnimationMotion();
	ChangeAnimationCilp(m_animationState);
	m_pWeakCurAnim.lock()->Update(dDeltaTime);

}

void RenderComponent::LateUpdate(double dDeltaTime)
{
}

//DC핸들값은 BackBuffer의 DC를 받아와야 한다.
void RenderComponent::Draw(const HDC & hDC)
{
	if (m_bVisible) 
	{
		POSITION pivot = m_pOwner->GetComponent<TransformComponent>()->GetScreenPivot();
		POSITION position = m_pOwner->GetComponent<TransformComponent>()->GetScreenPosition();
		m_pWeakCurAnim.lock()->Draw(hDC, pivot);

		Collider* pCollider = static_cast<Collider*>(m_pOwner->GetComponent(TEXT("Collider")));
		PhysicsComponent* pPhysics = m_pOwner->GetComponent<PhysicsComponent>();

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

		//BUG(11.02) : 이 부분에서 버그가 발생했었음 -> CameraManager내부 포인터가 가리키는 위치를 변경시켜버림..
		//static TCHAR buf[15];
		//wsprintf(buf, TEXT("Position : (%3ld, %3ld)"), (LONG)position.x, (LONG)position.y);
		//TextOut(hDC, 0, 20, buf, sizeof(buf));


	}		

}

bool RenderComponent::AddAnim(double dPlayTime, const Types::tstring & strSpriteName,
	UINT iWidth, UINT iHeight,	bool bLoop, bool bAnimate, const Types::tstring & strAnimTag)
{
	std::shared_ptr<CAnim> pAnim = std::make_shared<CAnim>();

	if (!pAnim->Init(m_pOwner, strSpriteName, iWidth, iHeight, dPlayTime, bLoop, bAnimate, strAnimTag))
		return false;

	if (m_pWeakCurAnim.expired())
		m_pWeakCurAnim = pAnim;

	//테이블 안에 해당 Key값과 연결된 vector가 있는 경우
	if (m_animationTable.find(strAnimTag) != m_animationTable.end())
	{ 
		m_animationTable[strAnimTag] = pAnim;
	}
	else 
	{	
		//테이블 안에 해당 Key값과 연결된 vector가 없는 경우
		//해당 Key값과 연결된 Vector를 생성한 후, Anim 추가
		m_animationTable.insert(std::make_pair(strAnimTag, pAnim));
	}

	return true;
}

bool RenderComponent::ChangeAnimation(const Types::tstring & strAnimTag)
{
	auto it = m_animationTable.find(strAnimTag);
	if ( it == m_animationTable.end())
		return false;

	if ((*it).second == nullptr)
		return false;

	if (!m_pWeakCurAnim.expired()) {
		if (m_pWeakCurAnim.lock()->GetAnimTag() == strAnimTag)
			return true;

		m_pWeakCurAnim.lock()->ClearEleapsedTime();
	}
	m_pWeakCurAnim = (*it).second;

	return true;
}

void RenderComponent::UpdateAnimationMotion()
{
	PhysicsComponent* pPhysics = static_cast<PhysicsComponent*>(m_pOwner->GetComponent(TEXT("PhysicsComponent")));
	if (pPhysics == nullptr)
		return;

	float fCurSpeed = std::fabsf(pPhysics->GetCurSpeed());
	float fSpeed = pPhysics->GetSpeed();
	float fMaxSpeed = pPhysics->GetMaxSpeed();

	if (m_ownerState == Types::AS_DAMAGED)
	{
		m_animationState = Types::AM_DAMAGED;
		return;
	}

	if (m_ownerState == Types::AS_SITDOWN)
	{
		m_animationState = Types::AM_SITDOWN;
		return;
	}
	else if (m_ownerState == Types::AS_LOOKUP)
	{
		m_animationState = Types::AM_LOOKUP;
		return;
	}

	if (fCurSpeed > 0.f && fCurSpeed < fMaxSpeed)
	{
		m_animationState = Types::AM_WALK;
		
	}
	else if (fCurSpeed >= fMaxSpeed)
	{
		m_animationState = Types::AM_RUN;
	}
	else
	{
		m_animationState = Types::AM_IDLE;
	}

	if (m_ownerVerticalState == Types::VS_IDLE)
	{
		if (pPhysics->GetCurSpeed() < 0.f)
		{
			if (m_ownerDirection == Types::DIR_RIGHT 
				&& m_pOwner->GetActorHorizonalState() != Types::HS_IDLE)
				m_animationState = Types::AM_TURN;
		}
		else if (pPhysics->GetCurSpeed() > 0.f)
		{
			if (m_ownerDirection == Types::DIR_LEFT
				&& m_pOwner->GetActorHorizonalState() != Types::HS_IDLE)
				m_animationState = Types::AM_TURN;
		}
	}

	if (fCurSpeed < fSpeed / 3)
		SetAnimationPlaySpeed(0.3f);
	else if (fCurSpeed < (fSpeed / 3) * 2)
		SetAnimationPlaySpeed(0.6f);
	else 
		SetAnimationPlaySpeed(1.f);

}

bool RenderComponent::SetAnimationPlaySpeed(double dSpeed)
{
	return m_pWeakCurAnim.lock()->SetPlaySpeed(dSpeed);
}

void RenderComponent::SetAnimation(Types::AnimationMotion motion)
{
	m_animationState = motion;

}


void RenderComponent::ChangeAnimationCilp(Types::AnimationMotion motion)
{
	if (m_ownerVerticalState == Types::VS_JUMP) {
		switch (motion) {
		case Types::AM_IDLE:
		case Types::AM_WALK:
		case Types::AM_LOOKUP:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("JumpLeft"));
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("JumpRight"));
			}
			break;
		case Types::AM_SITDOWN:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("SitdownLeft"));
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("SitdownRight"));
			}
			break;
		case Types::AM_RUN:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("RunJumpLeft"));
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("RunJumpRight"));
			}
			break;
		case Types::AM_ATTACK:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("AttackLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("AttackRight"));
			break;
		case Types::AM_DAMAGED:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				if (!ChangeAnimation(TEXT("DamagedLeft")))
				{
					ChangeAnimation(TEXT("Damaged"));
				}
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
			{
				if (!ChangeAnimation(TEXT("DamagedRight")))
				{
					ChangeAnimation(TEXT("Damaged"));
				}
			}
			break;
		}

	}
	else if (m_ownerVerticalState == Types::VS_FALL) {
		switch (motion) {
		case Types::AM_IDLE:
		case Types::AM_WALK:
		case Types::AM_LOOKUP:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("FalldownLeft"));
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("FalldownRight"));
			}
			break;
		case Types::AM_SITDOWN:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("SitdownLeft"));
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("SitdownRight"));
			}
			break;
		case Types::AM_RUN:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				ChangeAnimation(TEXT("RunJumpLeft"));
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
			{
				ChangeAnimation(TEXT("RunJumpRight"));
			}
			break;
		case Types::AM_ATTACK:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("AttackLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("AttackRight"));
			break;
		case Types::AM_DAMAGED:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				if (!ChangeAnimation(TEXT("DamagedLeft")))
				{
					ChangeAnimation(TEXT("Damaged"));
				}
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
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
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("IdleLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("IdleRight"));
			break;
		case Types::AM_WALK:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("WalkLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("WalkRight"));
			break;
		case Types::AM_RUN:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("RunLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("RunRight"));
			break;
		case Types::AM_TURN:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("TurnLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("TurnRight"));
			break;
		case Types::AM_LOOKUP:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("LookupLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("LookupRight"));
			break;
		case Types::AM_SITDOWN:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("SitdownLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("SitdownRight"));
			break;
		case Types::AM_ATTACK:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("AttackLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("AttackRight"));
			break;
		case Types::AM_DAMAGED:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				if (!ChangeAnimation(TEXT("DamagedLeft")))
				{
					ChangeAnimation(TEXT("Damaged"));
				}
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
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

