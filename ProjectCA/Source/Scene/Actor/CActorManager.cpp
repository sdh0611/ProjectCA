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

//
//WeakActorPtr CActorManager::GetTarget(Types::ActorID id)
//{
//	for (auto& it : m_strongActorPtrList) {
//		if (it->GetActorID() == id)
//			return WeakActorPtr(it);
//	}
//
//	return WeakActorPtr();	//return nullptr
//}
//
//WeakActorPtr CActorManager::GetTarger(const Types::tstring & strTag)
//{
//	for (auto& it : m_strongActorPtrList) {
//		if (it->GetObjectTag() == strTag)
//			return WeakActorPtr(it);
//	}
//
//	return WeakActorPtr();
//}
//
//void CActorManager::Clear()
//{
//	m_strongActorPtrList.clear();
//}
