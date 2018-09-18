#pragma once

#include "..\..\..\stdafx.h"
#include "..\Components\ComponentBase.h"
#include "Collider.h"
#include "ColliderBox.h"

class PhysicsComponent :public ComponentBase {
	friend void Collider::OnCollision(Types::ObjectType type, Collider::CollisionType collision);
	friend void Collider::OnCollision(Types::ObjectType type);
	friend void ColliderBox::OnCollision(Types::ObjectType type, Collider::CollisionType collision);

public:
	PhysicsComponent();
	virtual ~PhysicsComponent();


public:
	virtual bool Init(std::shared_ptr<CActor> pOwner, float fSpeed, float fGravity, float fJumpForce, 
		const Types::tstring& strTag = TEXT("PhysicsComponent"));
	virtual void Update(float fDeltaTime) override;


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
	

private:
	void Move(float fDeltaTime);
	void Gravity(float fDeltaTime);

	
private:
	/*
		NOTE:	컴포넌트간의 상호작용을 위해 정의한 메소드들
	*/
	void ResetJumpForce() { m_fJumpForce = -350.f; }
	void ResetSpeed() { }


private:
	float					m_fGravity;
	float					m_fSpeed;
	float					m_fJumpForce;
	Types::Point			m_lastActorPoint;


};