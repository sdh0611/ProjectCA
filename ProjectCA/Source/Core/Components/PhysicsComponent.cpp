#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
//#include "..\..\..\Include\Core\Components\Collider.h"



PhysicsComponent::PhysicsComponent()
	:m_bGrounded(false), m_bAccel(false), m_fGravity(0.f), m_fSpeed(0.f), m_fMaxSpeed(0.f),
	m_fJumpForce(0.f), m_fXSpeed(0.f), m_fYSpeed(0.f), m_dTimeElapsed(0.f)
{
	
}

PhysicsComponent::~PhysicsComponent()
{
}

bool PhysicsComponent::Init(CActor* pOwner, float fSpeed, float fMaxSpeed, float fGravity, 
	float fJumpForce, const Types::tstring& strTag)
{
	auto pActor= std::shared_ptr<CActor>(pOwner);

	m_pOwner = pActor;
	m_lastActorPoint = m_pOwner->GetActorPoint();

	if (fSpeed < 0.f || fMaxSpeed < 0.f || fGravity < 0.f)
		return false;

	m_fGravity					= fGravity;
	m_fXSpeed					= 0.f;
	m_fSpeed					= fSpeed;
	m_fMaxSpeed				= fMaxSpeed;
	m_fYSpeed					= 0.f;
	m_fJumpForce				= fJumpForce;
	m_strComponentTag		= strTag;
	m_bGrounded				= false;
	m_bAccel					= false;

	return true;
}

void PhysicsComponent::Update(double dDeltaTime)
{
	m_dTimeElapsed += dDeltaTime;
	m_lastActorPoint = m_pOwner->GetActorPoint();
	Gravity(dDeltaTime);
	Move(dDeltaTime);

	//if (m_pOwner->GetActorDirection() == Types::DIR_LEFT)
	//{
		m_pOwner->SetActorPoint(m_pOwner->GetActorPoint().x + (m_fXSpeed * dDeltaTime),
			m_pOwner->GetActorPoint().y - m_fYSpeed * dDeltaTime);
	//}
	//else if (m_pOwner->GetActorDirection() == Types::DIR_RIGHT)
	//{
	//	m_pOwner->SetActorPoint(m_pOwner->GetActorPoint().x + (m_fXSpeed * dDeltaTime),
	//		m_pOwner->GetActorPoint().y - m_fYSpeed * dDeltaTime);
	//}
	m_bGrounded = false;
}

void PhysicsComponent::RestoreActorPoint()
{
	m_pOwner->SetActorPoint(m_lastActorPoint.x, m_lastActorPoint.y);

}

void PhysicsComponent::Move(double dDeltaTime)
{
	Types::HorizonalState horizonal = m_pOwner->GetActorHorizonalState();
	Types::Direction dir = m_pOwner->GetActorDirection();

	if (dir == Types::DIR_LEFT)
	{
		if (horizonal == Types::HS_RUN)
		{
			if (m_fXSpeed > -1*m_fMaxSpeed)
				m_fXSpeed -= 10;
		}
		else if (horizonal == Types::HS_WALK)
		{
			if (m_fXSpeed < -1*m_fSpeed)
				m_fXSpeed += 10;
			else if (m_fXSpeed > -1*m_fSpeed)
				m_fXSpeed -= 5;
			else if (m_fXSpeed <= -1*m_fSpeed)
				m_fXSpeed = -1*m_fSpeed;

		}
		else if (horizonal == Types::HS_IDLE)
		{
			if (m_fXSpeed < 0.f)
				m_fXSpeed += 10;
			if (m_fXSpeed > 0.f)
				m_fXSpeed = 0.f;
		}
	}
	else if (dir == Types::DIR_RIGHT)
	{
		if (horizonal == Types::HS_RUN)
		{
			if (m_fXSpeed < m_fMaxSpeed)
				m_fXSpeed += 10;
		}
		else if (horizonal == Types::HS_WALK)
		{
			if (m_fXSpeed > m_fSpeed)
				m_fXSpeed -= 10;
			else if (m_fXSpeed < m_fSpeed)
				m_fXSpeed += 5;
			else if (m_fXSpeed >= m_fSpeed)
				m_fXSpeed = m_fSpeed;
		}
		else if (horizonal == Types::HS_IDLE)
		{
			if (m_fXSpeed > 0.f)
				m_fXSpeed -= 10;
			if (m_fXSpeed < 0.f)
				m_fXSpeed = 0.f;
		}
	}


}


void PhysicsComponent::Gravity(double dDeltaTime)
{
	Types::VerticalState state = m_pOwner->GetActorVerticalState();

	if (m_bGrounded)
		m_fYSpeed = 0.f;

	if (m_pOwner->GetActorPreVerticalState() == Types::VS_JUMP)
		if (state == Types::VS_FALL)
			m_fYSpeed = -50.f;

	if (state != Types::VS_FALL && m_fYSpeed < 0.f)
		m_pOwner->SetActorVerticalState(Types::VS_FALL);
	
	if (state != Types::VS_JUMP && !m_bGrounded) 
	{
		m_pOwner->SetActorVerticalState(Types::VS_FALL);
	}
	else if (state == Types::VS_JUMP) 
	{
		if (m_pOwner->GetActorPreVerticalState() != Types::VS_JUMP) {
			m_fYSpeed = m_fJumpForce;
		}
	}

	if(!m_bGrounded)
		if(m_fYSpeed > -800.f)
			m_fYSpeed -= m_fGravity * dDeltaTime;

}
