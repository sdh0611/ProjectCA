#pragma once

/*
	NOTE:
		Actor의 생성, 상호작용 관련 기능을 담당하는 Manager Class
*/

#include "..\..\..\stdafx.h"
#include "..\..\Singleton.hpp"

class CObject;
class CWorld;
class CScene;
class CGameScene;

typedef std::weak_ptr<CObject> WeakObjectPtr;
typedef std::shared_ptr<CObject> StrongObjectPtr;


class CObjectManager : public Singleton<CObjectManager> {

	DECLARE_SINGLETON(CObjectManager)

public:
	bool Init();


public:
//	WeakObjectPtr GetTarget(Types::ActorID id);
//	WeakObjectPtr GetTarger(const Types::tstring& strTag);
//	
//
//public:
//	void Clear();
//
//
public:
	template<typename T>
	std::shared_ptr<T> CreateActor(const Types::ActorData& data) {

		std::shared_ptr<T> pActor = std::make_shared<T>();
		
		if (!pActor->PostInit(data))
			return std::shared_ptr<T>();
		

		return pActor;
	}

	//오버로딩버전
	template<typename T>
	std::shared_ptr<T> CreateActor(UINT iWidth, UINT iHeight, float fx, float fy, OBJECT_TYPE type, 
		OBJECT_STATE state, VER_STATE vertical, HOR_STATE horizonal, DIRECTION dir,
		const TSTRING& strTag, CGameScene* pScene) {

		std::shared_ptr<T> pActor = std::make_shared<T>();

		Types::ActorData data(iWidth, iHeight, POSITION(fx, fy), type, state, 
			vertical, horizonal, dir, m_lastActorID++,strTag);

		if (!pActor->PostInit(data, pScene))
			return std::shared_ptr<T>();


		return pActor;
	}

	//오버로딩버전
	template<typename T>
	std::shared_ptr<T> CreateActor(UINT iWidth, UINT iHeight, float fx, float fy, OBJECT_TYPE type,
		DIRECTION dir, const TSTRING& strTag, CGameScene* pScene) {

		std::shared_ptr<T> pActor = std::make_shared<T>();

		Types::ActorData data(iWidth, iHeight, POSITION(fx, fy), type, Types::OS_IDLE,
			Types::VS_IDLE, Types::HS_IDLE, dir, m_lastActorID++, strTag);

		if (!pActor->PostInit(data, pScene))
			return std::shared_ptr<T>();


		return pActor;
	}

	template<typename T>
	std::shared_ptr<T> CreateObject(UINT iWidth, UINT iHeight, float fx, float fy, OBJECT_TYPE type,
		const TSTRING& strTag, OBJECT_STATE state, CScene* pScene) {

		std::shared_ptr<T> pObject = std::make_shared<T>();

		Types::ObjectData data(iWidth, iHeight, POSITION(fx, fy), strTag, type, state);

		if (!pObject->PostInit(data, pScene))
			return std::shared_ptr<T>();


		return pObject;
	}

	template<typename T>
	std::shared_ptr<T> CreateObject(UINT iWidth, UINT iHeight, float fx, float fy, OBJECT_TYPE type,
		const TSTRING& strTag, CScene* pScene) {

		std::shared_ptr<T> pObject = std::make_shared<T>();

		Types::ObjectData data(iWidth, iHeight, POSITION(fx, fy), strTag, type, Types::OS_IDLE);

		if (!pObject->PostInit(data, pScene))
			return std::shared_ptr<T>();


		return pObject;
	}



private:



private:
	//std::list<StrongObjectPtr>		m_strongActorPtrList;
	static Types::ActorID				m_lastActorID;
	
};

