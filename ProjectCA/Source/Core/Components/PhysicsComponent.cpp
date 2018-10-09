#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
//#include "..\..\..\Include\Core\Components\Collider.h"



PhysicsComponent::PhysicsComponent()
	:m_bGrounded(false), m_fGravity(0.f), m_fSpeed(0.f), m_fMaxSpeed(0.f),
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

	m_fGravity = fGravity;
	m_fXSpeed = m_fSpeed = fSpeed;
	m_fMaxSpeed = fMaxSpeed;
	m_fJumpForce = fJumpForce;
	m_strComponentTag = strTag;
	m_bGrounded = false;

	return true;
}

void PhysicsComponent::Update(double dDeltaTime)
{
	m_dTimeElapsed += dDeltaTime;
	m_lastActorPoint = m_pOwner->GetActorPoint();
	Gravity(dDeltaTime);
	Move(dDeltaTime);
	//if (!m_bGrounded) {
	//}
	//else {
	//	m_fYSpeed = m_fJumpForce;
	//}
	m_bGrounded = false;
}

void PhysicsComponent::RestoreActorPoint()
{
	m_pOwner->SetActorPoint(m_lastActorPoint.x, m_lastActorPoint.y);

}

//NOTE(07.30) : 현재 충돌인식은 잘 되고있으나, 그에 따른 반응이 제대로 안되는 상황. 고치자
void PhysicsComponent::Move(double dDeltaTime)
{
	if (m_pOwner->GetActorState() == Types::OS_RUN) {
		if(m_fXSpeed < m_fMaxSpeed)
			m_fXSpeed += 5;
	}
	else if (m_pOwner->GetActorState() == Types::OS_WALK) {
		if (m_fXSpeed > m_fSpeed)
			m_fXSpeed -= 10;
	}

	m_pOwner->SetActorPoint(m_pOwner->GetActorPoint().x + m_pOwner->GetActorVector().x * m_fXSpeed * dDeltaTime,
		m_pOwner->GetActorPoint().y - m_fYSpeed * dDeltaTime);
}


void PhysicsComponent::Gravity(double dDeltaTime)
{
	Types::JumpState state = m_pOwner->GetActorJumpState();

	if (m_bGrounded)
		m_fYSpeed = 0.f;

	if (m_pOwner->GetActorPreJumpState() == Types::JS_JUMP)
		if (state == Types::JS_FALL)
			m_fYSpeed = -50.f;

	if (state != Types::JS_FALL && m_fYSpeed < 0.f)
		m_pOwner->SetActorJumpState(Types::JS_FALL);
	
	if (state != Types::JS_JUMP && !m_bGrounded) {
		m_pOwner->SetActorJumpState(Types::JS_FALL);
	}
	else if (state == Types::JS_JUMP) {
		if (m_pOwner->GetActorPreJumpState() != Types::JS_JUMP) {
			m_fYSpeed = m_fJumpForce;
		}
	}

	if(!m_bGrounded)
		if(m_fYSpeed > -800.f)
			m_fYSpeed -= m_fGravity * dDeltaTime;

}

void PhysicsComponent::Jump(double dDeltaTime)
{
	m_pOwner->SetActorPoint(m_pOwner->GetActorPoint().
		x, m_pOwner->GetActorPoint().y + m_fYSpeed * dDeltaTime);



}

void PhysicsComponent::Down(double dDeltaTime)
{



}
