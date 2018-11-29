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
	//virtual void LateUpdate(double dDeltaTime);
	//virtual void Destroy();


public:
	void SetActorDirection(DIRECTION dir);
	void SetActorVerticalState(VER_STATE vertical);
	void SetActorHorizonalState(HOR_STATE horizonal);
	//void SetOwnerWorld(CWorld* pWorld);


public:
	VER_STATE				GetActorVerticalState() const;
	HOR_STATE				GetActorHorizonalState() const;
	DIRECTION				GetActorDirection() const;
	UINT						GetActorWidth() const;
	UINT						GetActorHeight() const;
	//CWorld* const			GetOwnerWorld() const;


public:
	void FlipActorDirection();


private:
	virtual void ActorBehavior(double dDeltaTime) = 0;


protected:
	VER_STATE					m_ActorCurVerticalState;
	HOR_STATE					m_ActorHorizonalState;
	DIRECTION					m_Direction;
	//CWorld*					m_pOwnerWorld;

	
};