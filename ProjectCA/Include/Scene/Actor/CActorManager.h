#pragma once

/*
	NOTE:
		Actor의 생성, 상호작용 관련 기능을 담당하는 Manager Class
*/

#include "..\..\..\stdafx.h"
#include "..\..\Singleton.hpp"
#include "CActor.h"

typedef std::weak_ptr<CActor> WeakActorPtr;
typedef std::shared_ptr<CActor> StrongActorPtr;


class CActorManager : public Singleton<CActorManager> {

	DECLARE_SINGLETON(CActorManager)

public:
	bool Init();
	void Update();



public:
	StrongActorPtr CreateActor(Types::ObjectData data);
	StrongActorPtr CreateActor(Types::ObjectType type, Types::Point point, Types::Direction dir, 
		Types::ObjectState state = Types::OS_IDLE);
	WeakActorPtr GetTarget();



private:



private:
	std::list<StrongActorPtr>		m_strongActorPtrList;
	static ActorID					m_lastActorID;
	


};