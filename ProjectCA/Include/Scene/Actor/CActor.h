#pragma once

/*
	NOTE:
		게임 내 모든 Actor들의 기본 Class 정의.
		CWorld객체에서만 생성가능하게 설정.
*/

#include "..\..\..\stdafx.h"
#include "..\CObject.h"
//#include "..\"
//#include "CActorManager.h"


class CWorld;
class CGameScene;
class ComponentBase;

//typedef std::shared_ptr<ComponentBase> StrongComponentPtr;

class CActor : public CObject{
	////Layer, World Class만 Actor의 생성, 파괴가 가능함.
	//friend class CWorld;
	//friend class CLayer;
	
	//08:17 : ActorFactory에서만 생성 가능하게 바꿈.
	//파괴의 경우엔 Actor 자체의 Destroy 메소드를 이용하여 내부 컴포넌트들을 정리하게끔 함.
	//friend class CActorFactory;
	//friend class로 ActorManager를 했던건 ActorManager에서만 생성 가능하게끔 하려는 의도였으나,
	//스마트포인터 사용할 때 생기는 문제를 도저히 못고치겠어서 걍 생성자를 Public으로 풀었다..(09.15)
	friend class CActorManager;

public:
	CActor();
	virtual ~CActor();
	

public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC) = 0;
	virtual void LateUpdate(double dDeltaTime);
	virtual void Destroy();


public:
	void SetActorState(Types::ActorState state);
	void SetActorDirection(Types::Direction dir);
	void SetActorVerticalState(Types::VerticalState vertical);
	void SetActorHorizonalState(Types::HorizonalState horizonal);
	//void SetOwnerWorld(CWorld* pWorld);


public:
	Types::ActorType		GetActorType() const;
	Types::ActorState		GetActorState() const;
	Types::Direction		GetActorDirection() const;
	UINT						GetActorWidth() const;
	UINT						GetActorHeight() const;
	Types::ActorID			GetActorID() const;
	Types::VerticalState	GetActorVerticalState() const;
	Types::HorizonalState GetActorHorizonalState() const;
	//CWorld* const			GetOwnerWorld() const;


public:
	void FlipActorDirection();


private:
	virtual void ActorBehavior(double dDeltaTime) = 0;


protected:
	Types::ActorType			m_ActorType;
	Types::ActorState			m_ActorCurState;
	Types::VerticalState		m_ActorCurVerticalState;
	Types::HorizonalState		m_ActorHorizonalState;
	Types::Direction			m_Direction;
	Types::ActorID				m_ActorID;
	//CWorld*					m_pOwnerWorld;

	
};