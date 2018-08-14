#pragma once

/*
	NOTE:
		GameScene이 진행되는 동안 플레이하게 될 Stage의 역할을 수행하는 Class.
		GameScene에서만 생성 가능하게 설정
		World내 Actor들을 Layer객체들의 list를 이용하여 관리함.
	->해당 클래스가 Actor들을 관리하는 역할까지 맡게 할 것인지, 아니면 World상의 Actor간 상호작용에 대해서만
		역할을 하게 할 것인지 재고해볼 필요가 있음.(08.14)
*/

#include "..\..\stdafx.h"
#include "Actor\CActor.h"

class CLayer;

class CWorld {
	friend class CGameScene;

private:
	CWorld();
	virtual ~CWorld() { };


public:
	bool Init();
	void Update(float fDeltaTime);
	void Render(const HDC& hDC);


public:
	bool CreateLayer(const Types::tstring& layerTag);
	bool DeleteLayer(const Types::tstring& layerTag);
	CLayer * FindLayer(const Types::tstring& layerTag);


public:
	bool CreateActor(const Types::tstring& layerTag, Types::ObjectData data);
	CActor* GetActor(ActorID actorID);
	CActor* GetActor(const Types::tstring& layerName, ActorID actorID);
	bool DeleteActor(ActorID actorID);
	bool DeleteActor(CActor* pActor);
	bool DeleteActor(const Types::tstring& layerName, ActorID actorID);
	ActorID GetLastActorID() const { return m_lastActorID; }


private:
	inline void IncreaseLastActorID() { ++m_lastActorID; }
	inline bool DecreaseLastActorID() { if (m_lastActorID < 1) return false; --m_lastActorID; return true; }

	
private:
	ActorID						m_lastActorID;
	std::list<class CLayer*>	m_layerList;
	


};