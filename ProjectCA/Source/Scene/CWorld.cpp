#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CWorld.h"
#include "..\..\Include\Scene\CGameScene.h"
#include "..\..\Include\Scene\Actor\CActor.h"
#include "..\..\Include\Scene\CLayer.h"


bool CWorld::Init()
{

	m_lastActorID = 0;

	return true;
}

void CWorld::Update(float fDeltaTime)
{
	for (auto& it : m_layerList) {
		it->Update(fDeltaTime);
	}
}

void CWorld::Render(const HDC & hDC)
{
	for (auto& it : m_layerList) {
		it->Render(hDC);
	}


}

bool CWorld::CreateActor(const Types::tstring& layerTag, Types::ObjectData data)
{
	CActor* pActor = new CActor;
	pActor->Init(data);

	CLayer* pLayer = FindLayer(layerTag);

	if (pLayer == nullptr)
		return false;

	pLayer->AddActor(pActor);
	IncreaseLastActorID();

	return true;
}

CActor * CWorld::GetActor(ActorID actorID)
{
	CActor* pActor = nullptr;

	for (auto& it : m_layerList) {
		pActor = it->FindActor(actorID);
		if (pActor != nullptr)
			break;
	}

	return pActor;
}

CActor * CWorld::GetActor(const Types::tstring & layerName, ActorID actorID)
{
	CActor* pActor = FindLayer(layerName)->FindActor(actorID);

	return pActor;
}

bool CWorld::DeleteActor(ActorID actorID)
{


	return false;
}

bool CWorld::DeleteActor(CActor * pActor)
{


	return false;
}

bool CWorld::DeleteActor(const Types::tstring & layerName, ActorID actorID)
{


	return false;
}


bool CWorld::CreateLayer(const Types::tstring & layerTag)
{
	if (FindLayer(layerTag) != nullptr)
		return false;

	m_layerList.insert(new CLayer);

	return true;
}

bool CWorld::DeleteLayer(const Types::tstring & layerTag)
{
	return false;
}

CLayer * CWorld::FindLayer(const Types::tstring & layerTag)
{
	for (auto& it : m_layerList) {
		if ( !(it->GetLayerTag().compare(layerTag)) ) 
			return it;

	}


	return nullptr;
}

