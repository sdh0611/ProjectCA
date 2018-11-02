#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CActorManager.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScene.h"
#include "..\..\..\Include\Scene\CLayer.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"


Types::ActorID CActorManager::m_lastActorID = 0;

CActorManager::CActorManager()
{

}

CActorManager::~CActorManager() {

	
}

bool CActorManager::Init() {

	//m_lastActorID = 0;

	return true;
}

//StrongActorPtr CActorManager::CreateActor(const Types::ActorData& data)
//{
//	StrongActorPtr pActor = std::make_shared<CActor>();
//
//	if(!pActor->Init(data))
//		return StrongActorPtr();		//return nullptr
//
//	m_strongActorPtrList.emplace_back(pActor);
//
//	pActor->GetOwnerScene()->FindLayer(TEXT("default"))->AddActor(pActor);
//
//	return pActor;
//
//}
//
//StrongActorPtr CActorManager::CreateActor(Types::ObjectType type, Types::Point point, 
//	Types::Direction dir, UINT iWidth, UINT iHeight, CGameScene* pScene, 
//	const Types::tstring& strTag, Types::ObjectState state)
//{
//	StrongActorPtr pActor = std::make_shared<CActor>();
//
//	if (!pActor->Init(Types::ActorData(type, point, dir, state, iWidth, iHeight, m_lastActorID++, strTag)))
//		return StrongActorPtr();		//return nullptr
//
//	if (type == Types::OT_PLAYER) {
//		PlayerInputComponent* pInput = new PlayerInputComponent;
//		pInput->Init(pActor);
//		pActor->AddComponent(pInput, pInput->GetComponentTag());
//	}
//
//	PhysicsComponent* pPhysics = new PhysicsComponent;
//	pPhysics->Init(pActor, 500.f, 1300.f, -600.f);
//
//	ColliderBox* pBox = new ColliderBox;
//	pBox->Init(pActor);
//
//	pActor->AddComponent(pPhysics, pPhysics->GetComponentTag());
//	pActor->AddComponent(pBox, pBox->GetComponentTag());
//
//	pActor->SetOwnerScene(pScene);
//	m_strongActorPtrList.emplace_back(pActor);
//
//	//pActor->GetOwnerScene()->FindLayer(TEXT("default"))->AddActor(pActor);
//
//	return pActor;
//
//}


WeakActorPtr CActorManager::GetTarget(Types::ActorID id)
{
	for (auto& it : m_strongActorPtrList) {
		if (it->GetActorID() == id)
			return WeakActorPtr(it);
	}

	return WeakActorPtr();	//return nullptr
}

WeakActorPtr CActorManager::GetTarger(const Types::tstring & strTag)
{
	for (auto& it : m_strongActorPtrList) {
		if (it->GetActorTag() == strTag)
			return WeakActorPtr(it);
	}

	return WeakActorPtr();
}

void CActorManager::Clear()
{
	m_strongActorPtrList.clear();
}
