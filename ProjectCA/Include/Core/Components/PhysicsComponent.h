#pragma once

#include "..\..\..\stdafx.h"
#include "..\Components\ComponentBase.h"


class PhysicsComponent :public ComponentBase {


public:
	PhysicsComponent();
	virtual ~PhysicsComponent();


public:
	virtual bool Init(CActor* pOwner, float fSpeed, float fMaxSpeed, float fGravity, 
		float fJumpForce, const Types::tstring& strTag = TEXT("PhysicsComponent"));
	virtual void Update(double fDeltaTime) override;


public:
	void RestoreActorPoint();

	
public:
	const float& GetSpeed() const { return m_fSpeed; }
	void SetSpeed(float speed) { if (speed >= 0) m_fSpeed = speed; }
	const float& GetGravity() const { return m_fGravity; }
	void SetGravity(float gravity) { if (gravity >= 0) m_fGravity = gravity; }
	const float& GetJumpForce() const { return m_fGravity; }
	void SetJumpForce(float jumpForce) { if (m_fJumpForce >= 0) m_fJumpForce = jumpForce; }
	const Types::Point& GetLastActorPoint() const { return m_lastActorPoint; }
	void SetGrounded(bool bGrounded) { m_bGrounded = bGrounded; }
	bool IsGrounded() const { return m_bGrounded; }


private:
	void Move(double dDeltaTime);
	void Gravity(double dDeltaTime);


private:
	bool					m_bGrounded;
	float					m_fGravity;
	float					m_fSpeed;
	float					m_fMaxSpeed;
	float					m_fJumpForce;
	float					m_fXSpeed;
	float					m_fYSpeed;
	double				m_dTimeElapsed;
	Types::Point			m_lastActorPoint;


};