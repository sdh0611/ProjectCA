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

	using Callback = std::function<void(CObject*, CollisionType type, float fIntersectLength)>;


public:
	Collider(ColliderType);
	virtual ~Collider();


public:
	virtual bool PostInit(CObject* pOwner, const Types::tstring& strTag = TEXT("Collider"));
	virtual void Update(double dDeltaTime) = 0;
	virtual void LateUpdate(double dDeltaTime) override;
	virtual void ResolveCollision(CObject* pOther, float fintersectLength);
	virtual void DrawCollider(const HDC& hDC, const POSITION& position) = 0;


public:
	void SetColliderType(ColliderType type);
	void SetCollisionType(CollisionType type);
	void SetIsCollision(bool bCollision);
	void SetColliderPoint(POSITION position);
	void SetColliderPoint(float fx, float fy);
	void SetUseTriggered(bool bUseTrigger);
	void SetDelegate(Callback pDelegate);
	void SetOnCollision(Callback pCollision);
	void SetOnTrigerr(Callback pTrigger);


public:
	ColliderType			GetColliderType() const;
	CollisionType			GetCollisionType() const;
	bool						IsCollision() const;
	bool						IsTriggered() const;
	const Types::Point&	GetColliderPoint() const;
	

private:
	virtual void OnCollision(CObject* pOther, float fintersectLength);
	virtual void OnTriggered(CObject* pOther, float fintersectLength);


protected:
	ColliderType				m_ColliderType;
	CollisionType				m_CollisionType;
	POSITION					m_ColliderPoint;
	POSITION					m_CurColliderPoint;
	bool							m_bCollision;
	bool							m_bUseTrigger;


private:
	Callback						m_pOnCollision;
	Callback						m_pOnTrigger;


};