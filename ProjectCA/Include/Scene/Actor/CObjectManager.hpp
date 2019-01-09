#pragma once

/*
	NOTE: Entity, Object, Actor를 상속받는 객체들을 생성하기 위한 Class.
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
	void Clear();


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
		DIRECTION dir, const TSTRING& strTag, CGameScene* pScene) {

		std::shared_ptr<T> pActor = std::make_shared<T>();

		Types::ActorData data(iWidth, iHeight, POSITION(fx, fy), type, Types::OS_IDLE, Types::ACT_IDLE, dir, m_LastEntityID++, strTag);

		if (!pActor->PostInit(data, pScene))
			return std::shared_ptr<T>();


		return pActor;
	}

	//오버로딩버전
	template<typename T>
	std::shared_ptr<T> CreateActor(UINT iWidth, UINT iHeight, float fx, float fy, OBJECT_TYPE type, 
		ACT act, DIRECTION dir, const TSTRING& strTag, CGameScene* pScene) {

		std::shared_ptr<T> pActor = std::make_shared<T>();

		Types::ActorData data(iWidth, iHeight, POSITION(fx, fy), type, Types::OS_IDLE, act, dir, m_LastEntityID++, strTag);

		if (!pActor->PostInit(data, pScene))
			return std::shared_ptr<T>();


		return pActor;
	}

	template<typename T>
	std::shared_ptr<T> CreateObject(UINT iWidth, UINT iHeight, float fx, float fy, OBJECT_TYPE type,
		const TSTRING& strTag, OBJECT_STATE state, CScene* pScene) {

		std::shared_ptr<T> pObject = std::make_shared<T>();

		Types::ObjectData data(iWidth, iHeight, POSITION(fx, fy), strTag, m_LastEntityID++,type, state);

		if (!pObject->PostInit(data, pScene))
			return std::shared_ptr<T>();


		return pObject;
	}

	template<typename T>
	std::shared_ptr<T> CreateObject(UINT iWidth, UINT iHeight, float fx, float fy, OBJECT_TYPE type,
		const TSTRING& strTag, CScene* pScene) {

		std::shared_ptr<T> pObject = std::make_shared<T>();

		Types::ObjectData data(iWidth, iHeight, POSITION(fx, fy), strTag, m_LastEntityID++, type, Types::OS_IDLE);

		if (!pObject->PostInit(data, pScene))
			return std::shared_ptr<T>();


		return pObject;
	}

	template<typename T>
	std::shared_ptr<T> CreateEntity(UINT iWidth, UINT iHeight, float fx, float fy, const TSTRING& strName, CScene* pScene) {

		std::shared_ptr<T> pEntity = std::make_shared<T>();

		Types::EntityData data(iWidth, iHeight, POSITION(fx, fy), strName, m_LastEntityID++);

		if (!pEntity->PostInit(data, pScene))
			return std::shared_ptr<T>();


		return pEntity;
	}

private:



private:
	//std::list<StrongObjectPtr>		m_strongActorPtrList;
	ENTITY_ID			m_LastEntityID;
	
};

