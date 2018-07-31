#pragma once

#include "..\..\..\stdafx.h"
#include "ComponentBase.h"

//ColliderType에 Trigger도 추가해보자.

class Collider : public ComponentBase {
public:
	enum ColliderType{ CT_BOX, CT_CIRCLE, 
		CT_BOX_TRIGGER, CT_CIRCLE_TRIGGER };
	enum CollisionType { COLLISION_IDLE, COLLISION_SIDE, 
		COLLISION_TOP, COLLISION_BOT };


public:
	Collider(CObject* owner, ColliderType type);
	virtual ~Collider();


public:
	virtual void Init() = 0;
	virtual void Update(float fDeltaTime) = 0;
	virtual void ResolveCollision(Types::ObjectType type, CollisionType collision);
	virtual void ResolveCollision(Types::ObjectType type);


public:
	const ColliderType& GetColliderType() const { return m_Type; }
	void SetColliderType(ColliderType type) { m_Type = type; }
	//bool GetIsTrigger() const { return m_bIsTrigger; }
	//void SetIsTrigger(bool bIsTrigger) { m_bIsTrigger = bIsTrigger; }
	bool GetIsCollision() const { return m_bIsCollision; }
	void SetIsCollision(bool isCollision) { m_bIsCollision = isCollision; }



protected:
	ColliderType		m_Type;
	//bool				m_bIsTrigger;	//Trigger활성화여부->그냥 따로 상속클래스 만들어서 추가해야되나?
	bool				m_bIsCollision;

private:



};