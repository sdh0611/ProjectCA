#pragma once

/*
	NOTE:
		GameScene이 진행되는 동안 플레이하게 될 Stage의 역할을 수행하는 Class.
		GameScene에서만 생성 가능하게 설정
		World내 Actor들을 Layer객체들의 list를 이용하여 관리함.
	->해당 클래스가 Actor들을 관리하는 역할까지 맡게 할 것인지, 아니면 World상의 Actor간 상호작용에 대해서만
		역할을 하게 할 것인지 재고해볼 필요가 있음.(08.14)
	-> World내의 모든 Actor들의 포인터에 접근하고, 물리관련 상호작용에 대해서만 기능을 수행하게끔 변경.(08.16)
*/

#include "..\..\stdafx.h"
#include "Actor\CActor.h"



class CWorld {
	friend class CGameScene;

private:
	CWorld();
	virtual ~CWorld();


public:
	bool Init();
	void Update(float fDeltaTime);


public:
	void AddActor(std::shared_ptr<CActor> pActor);
	std::weak_ptr<CActor> GetActor(ActorID actorID);
	bool DeleteActor(ActorID actorID);
	bool DeleteActor(std::weak_ptr<CActor> pActor);
	bool CollisionUpdate();


public:
	std::weak_ptr<CActor> GetTarget(ActorID id);




private:
	void ResolveCollision();
	

	
private:
	std::list<std::weak_ptr<CActor>>				m_actorList;
	std::unique_ptr<class CollisionDetector>		m_pCollisionDetector;

};