#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CActorManager.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScene.h"
#include "..\..\..\Include\Scene\CLayer.h"


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
	Types::Direction dir, UINT iWidth, UINT iHeight, CWorld* pWorld, CGameScene* pScene, 
	const Types::tstring& strTag, Types::ObjectState state)
{
	StrongActorPtr pActor = std::make_shared<CActor>();

	if (pActor->Init(Types::ObjectData(type, point, dir, state, iWidth, iHeight, m_lastActorID++, pWorld, pScene, strTag)))
		return StrongActorPtr();		//return nullptr

	m_strongActorPtrList.push_back(pActor);
	
	if (!pActor->GetOwnerScene()->FindLayer(strTag)) {
		pActor->GetOwnerScene()->CreateLayer(strTag, 0);
	}

	return pActor;

}

WeakActorPtr CActorManager::GetTarget()
{



	return WeakActorPtr();	//return nullptr
}
