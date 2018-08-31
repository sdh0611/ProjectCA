#include "..\..\..\Include\Scene\Actor\CActorManager.h"
#include "..\..\..\stdafx.h"


CActorManager::CActorManager()
{

}

CActorManager::~CActorManager() {

	
}

bool CActorManager::Init() {

	m_lastActorID = 0;

	return true;
}

StrongActorPtr CActorManager::CreateActor(const Types::ObjectData& data)
{
	StrongActorPtr pActor = std::make_shared<CActor>();

	if(pActor->Init(data))
		return StrongActorPtr();		//return nullptr

	m_strongActorPtrList.push_back(pActor);

	return pActor;

}

StrongActorPtr CActorManager::CreateActor(Types::ObjectType type, Types::Point point, 
	Types::Direction dir, UINT iWidth, UINT iHeight, Types::ObjectState state)
{
	StrongActorPtr pActor = std::make_shared<CActor>();

	if (pActor->Init(Types::ObjectData(type, point, dir, state, iWidth, iHeight, m_lastActorID++)))
		return StrongActorPtr();		//return nullptr

	m_strongActorPtrList.push_back(pActor);

	return pActor;

}

WeakActorPtr CActorManager::GetTarget()
{



	return WeakActorPtr();	//return nullptr
}
