#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScene.h"
#include "..\..\..\Include\Scene\CLayer.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"


ENTITY_ID CObjectManager::m_LastEntityID = 0;

CObjectManager::CObjectManager()
{

}

CObjectManager::~CObjectManager() {

	
}

bool CObjectManager::Init() {

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
void CObjectManager::Clear()
{
	m_LastEntityID = 0;
}
