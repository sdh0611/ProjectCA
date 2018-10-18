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
	virtual bool PostInit(CActor* pOwner, const Types::tstring& strTag = TEXT("Collider")) = 0;
	virtual void Update(double dDeltaTime) = 0;
	//virtual void OnCollision(Types::ObjectType type);
	virtual void OnCollision(std::shared_ptr<CActor> pOther);
	virtual void OnTriggered(std::shared_ptr<CActor> pOther);
	virtual Delegate SetDelegate(Delegate dele);
	virtual void DrawCollider(const HDC& hDC) = 0;


public:
	ColliderType GetColliderType() const { return m_Type; }
	void SetColliderType(ColliderType type) { m_Type = type; }
	//bool GetIsTrigger() const { return m_bIsTrigger; }
	//void SetIsTrigger(bool bIsTrigger) { m_bIsTrigger = bIsTrigger; }
	bool GetIsCollision() const { return m_bIsCollision; }
	void SetIsCollision(bool isCollision) { m_bIsCollision = isCollision; }
	const Types::Point& GetColliderPoint() const { return m_ColliderPoint; }
	void SetColliderPoint(float fx, float fy) { m_ColliderPoint.x = fx; m_ColliderPoint.y = fy; m_CurColliderPoint = m_ColliderPoint; }
	bool IsTriggered() const { return m_bTriggered; }
	void SetTriggered(bool bTriggered) { m_bTriggered = bTriggered; }
	


protected:
	ColliderType				m_Type;
	Types::Point					m_ColliderPoint;
	Types::Point					m_CurColliderPoint;
	//bool						m_bIsTrigger;	//Trigger활성화여부->그냥 따로 상속클래스 만들어서 추가해야되나?
	bool							m_bIsCollision;
	bool							m_bTriggered;


private:
	Delegate						m_pDelegate;


};