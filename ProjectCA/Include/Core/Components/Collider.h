#pragma once

#include "..\..\..\stdafx.h"
#include "ComponentBase.h"

//ColliderType에 Trigger도 추가해보자.

using Delegate = std::function<void(std::shared_ptr<CActor>, std::shared_ptr<CActor>)>;


class Collider : public ComponentBase {
public:
	enum ColliderType { CT_BOX, CT_CIRCLE, CT_BOX_TRIGGER, CT_CIRCLE_TRIGGER };


public:
	Collider(ColliderType);
	virtual ~Collider();


public:
	virtual bool Init(CActor* pOwner, const Types::tstring& strTag = TEXT("Collider")) = 0;
	virtual void Update(double fDeltaTime) = 0;
	//virtual void OnCollision(Types::ObjectType type);
	virtual void OnCollision(std::shared_ptr<CActor> pOther);
	virtual Delegate SetDelegate(Delegate dele);


public:
	ColliderType GetColliderType() const { return m_Type; }
	void SetColliderType(ColliderType type) { m_Type = type; }
	//bool GetIsTrigger() const { return m_bIsTrigger; }
	//void SetIsTrigger(bool bIsTrigger) { m_bIsTrigger = bIsTrigger; }
	bool GetIsCollision() const { return m_bIsCollision; }
	void SetIsCollision(bool isCollision) { m_bIsCollision = isCollision; }
	const Types::Point& GetColliderPoint() const { return m_ColliderPoint; }
	void SetColliderPoint(Types::Point point) { if (point.x > 0 && point.y > 0) m_ColliderPoint = point; }


protected:
	ColliderType				m_Type;
	Types::Point					m_ColliderPoint;
	//bool						m_bIsTrigger;	//Trigger활성화여부->그냥 따로 상속클래스 만들어서 추가해야되나?
	bool							m_bIsCollision;


private:
	Delegate						m_pDelegate;


};