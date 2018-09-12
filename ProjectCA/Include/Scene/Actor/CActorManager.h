#pragma once

/*
	NOTE:
		Actor의 생성, 상호작용 관련 기능을 담당하는 Manager Class
*/

#include "..\..\..\stdafx.h"
#include "..\..\Singleton.hpp"

class CActor;
class CWorld;
class CGameScene;


typedef std::weak_ptr<CActor> WeakActorPtr;
typedef std::shared_ptr<CActor> StrongActorPtr;

using GenerateActor = std::function<StrongActorPtr(Types::ObjectType)>;

class CActorManager : public Singleton<CActorManager> {

	DECLARE_SINGLETON(CActorManager)

public:
	bool Init();
	void Update();



public:
	StrongActorPtr CreateActor(const Types::ObjectData& data);
	StrongActorPtr CreateActor(Types::ObjectType type, Types::Point point, Types::Direction dir, 
		UINT iWidth, UINT iHeight, CWorld* pWorld, CGameScene* pScene, const Types::tstring& strTag,
		Types::ObjectState state = Types::OS_IDLE);
	WeakActorPtr GetTarget();



private:



private:
	GenerateActor					m_genrateActor;
	std::list<StrongActorPtr>		m_strongActorPtrList;
	static Types::ActorID					m_lastActorID;
	


};