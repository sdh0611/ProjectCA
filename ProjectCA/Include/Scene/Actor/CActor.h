#pragma once

/*
	NOTE:
		게임 내 모든 Actor들의 기본 Class 정의.
		CWorld객체에서만 생성가능하게 설정.
*/

#include "..\..\..\stdafx.h"

typedef unsigned long ActorID;


class CActor {
	////Layer, World Class만 Actor의 생성, 파괴가 가능함.
	//friend class CWorld;
	//friend class CLayer;
	
	//08:17 : ActorFactory에서만 생성 가능하게 바꿈.
	//파괴의 경우엔 Actor 자체의 Destroy 메소드를 이용하여 자체적 파괴가 가능하게 수정.
	//friend class CActorFactory;
	friend class CActorManager;

private:
	CActor();
	virtual ~CActor();
	

public:
	virtual bool Init(const Types::ObjectData&);
	virtual void Update(float fDeltaTime);
	virtual void Render(const HDC& hDC);
	virtual void Destroy();


public:
	Types::ObjectType	GetActorType() const { return m_actorType; }
	Types::ObjectState GetActorState() const { return m_actorState; }
	inline void SetActorState(Types::ObjectState state) { m_actorState = state; }
	inline ActorID GetActorID() const { return m_actorID; }

private:
	//Actor, World, Layer클래스 마저 손보고, Git에다가 새로운 브랜치를 만들어서 Push하자.


protected:
	Types::Point				m_fActorPoint;
	Types::ObjectType		m_actorType;
	Types::ObjectState		m_actorState;
	Types::Direction		m_direction;
	ActorID					m_actorID;
	typedef std::unordered_map<Types::tstring, class ComponentBase*> ComponentTable;
	ComponentTable		m_componentTable;



};