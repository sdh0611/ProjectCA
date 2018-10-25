#pragma once

#include "..\..\..\stdafx.h"
#include "..\Components\ComponentBase.h"


class PhysicsComponent :public ComponentBase {


public:
	PhysicsComponent();
	virtual ~PhysicsComponent();


public:
	virtual bool PostInit(CActor* pOwner, float fSpeed, float fMaxSpeed, float fGravity, 
		float fJumpForce, const Types::tstring& strTag = TEXT("PhysicsComponent"));
	virtual void Init();
	virtual void Update(double fDeltaTime) override;
	virtual void LateUpdate(double dDeltaTime) override;

	
public:
	void	SetCurSpeed(float fSpeed);
	void	SetSpeed(float fSpeed);
	void	SetMass(float fMass);
	void	SetGravity(float fGravity);
	void	SetCurJumpForce(float fJumpForce);
	void	SetJumpForce(float fJumpForce);
	void	SetGrounded(bool bGrounded);
	void	SetAcceled(bool bAccel);
	void	SetStatic(bool bStatic);
	void	AddForceX(float fForce);
	void	AddForceY(float fForce);


public:
	bool	IsGrounded() const;
	bool	IsAcceled() const;
	bool	IsStatic() const; 
	float	GetCurSpeed() const;
	float	GetMaxSpeed() const;
	float	GetSpeed() const;
	float	GetMass() const;
	float	GetCurJumpForce() const;
	float	GetGravity() const;
	float	GetJumpForce() const;


private:
	void Gravity(double dDeltaTime);


private:
	bool					m_bGrounded;
	bool					m_bAccel;
	bool					m_bStatic;
	float					m_fSpeed;
	float					m_fMaxSpeed;
	float					m_fXSpeed;
	float					m_fMass;
	float					m_fGravity;
	float					m_fJumpForce;
	float					m_fYSpeed;
	double				m_dTimeElapsed;


private:
	const float			m_fMaxYSpeed = 800.f;

};