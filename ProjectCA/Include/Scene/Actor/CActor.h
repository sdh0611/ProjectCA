#pragma once

/*
	NOTE:게임 내의 움직이는 물체들이 상속받게 될 기본 Class 정의.
			CObject를 상속받아서 작성.
			수평, 수직상의 상태, 방향, ActionType 값들을 내부적으로 가짐.
*/

#include "..\..\..\stdafx.h"
#include "..\CObject.h"

class CGameScene;

typedef std::shared_ptr<ComponentBase> StrongComponentPtr;

class CActor : public CObject
{	
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
	

public:
	void SetActorAct(ACT act);
	void SetActorDirection(DIRECTION dir);


public:
	ACT						GetActorAct() const;
	DIRECTION				GetActorDirection() const;
	UINT						GetActorWidth() const;
	UINT						GetActorHeight() const;


public:
	void FlipActorDirection();


private:
	virtual void ActorBehavior(double dDeltaTime) = 0;


protected:
	ACT							m_ActType;
	DIRECTION					m_Direction;

	
};