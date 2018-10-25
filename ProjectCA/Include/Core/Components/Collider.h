#pragma once

#include "..\..\..\stdafx.h"
#include "ComponentBase.h"

//ColliderType에 Trigger도 추가해보자.

class Collider : public ComponentBase {
public:
	enum ColliderType { CT_BOX, CT_CIRCLE, CT_BOX_TRIGGER, CT_CIRCLE_TRIGGER };
	enum CollisionType {
		COLLISION_IDLE, COLLISION_TOP,
		COLLISION_BOT, COLLISION_RIGHT, COLLISION_LEFT
	};

	using Delegate = std::function<void(std::shared_ptr<CActor>, std::shared_ptr<CActor>, CollisionType type)>;


public:
	Collider(ColliderType);
	virtual ~Collider();


public:
	virtual bool PostInit(CActor* pOwner, const Types::tstring& strTag = TEXT("Collider")) = 0;
	virtual void Update(double dDeltaTime) = 0;
	virtual void LateUpdate(double dDeltaTime) override;
	virtual void ResolveCollision(std::shared_ptr<CActor> pOther);
	virtual void DrawCollider(const HDC& hDC) = 0;


public:
	void SetColliderType(ColliderType type);
	void SetCollisionType(CollisionType type);
	void SetIsCollision(bool bCollision);
	void SetColliderPoint(POSITION position);
	void SetColliderPoint(float fx, float fy);
	void SetUseTriggered(bool bUseTrigger);
	void SetDelegate(Delegate pDelegate);
	void SetOnCollision(Delegate pCollision);
	void SetOnTrigerr(Delegate pTrigger);


public:
	ColliderType			GetColliderType() const;
	CollisionType			GetCollisionType() const;
	bool						IsCollision() const;
	bool						IsTriggered() const;
	const Types::Point&	GetColliderPoint() const;
	

private:
	virtual void OnCollision(std::shared_ptr<CActor> pOther);
	virtual void OnTriggered(std::shared_ptr<CActor> pOther);


protected:
	ColliderType				m_ColliderType;
	CollisionType				m_CollisionType;
	Types::Point					m_ColliderPoint;
	Types::Point					m_CurColliderPoint;
	bool							m_bCollision;
	bool							m_bUseTrigger;


private:
	Delegate						m_pOnCollision;
	Delegate						m_pOnTrigger;


};