#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CWorld.h"
#include "..\..\Include\Scene\CGameScene.h"
#include "..\..\Include\Scene\Actor\CActor.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Core\CollisionDetector.h"



CWorld::CWorld()
	:m_pCollisionDetector(nullptr)
{

}



CWorld::~CWorld()
{
	if (!m_actorList.empty())
		m_actorList.clear();

}

bool CWorld::Init()
{
	m_pCollisionDetector = std::make_unique<CollisionDetector>();

	//m_lastActorID = 0;

	return true;
}

//CWorld Class에서 관리하는 weak_ptr List중 가리키는 포인터가 소멸됬는지 확인.
void CWorld::Update(float fDeltaTime)
{
	for (auto it = m_actorList.begin(); it != m_actorList.end(); ) {
		if ((*it).expired()) {
			it = m_actorList.erase(it);
		}
		else {
			++it;
		}

	}
	

}
void CWorld::AddActor(std::shared_ptr<CActor> pActor)
{
	m_actorList.emplace_back(std::weak_ptr<CActor>(pActor));

}

//해당 Actor의 weak_ptr를 찾은 경우 weak_ptr값을 반환하고, 못찾은 경우 default생성자값을 반환
// -> lock()메소드 사용시 nullptr 반환
std::weak_ptr<CActor>  CWorld::GetActor(ActorID actorID)
{
	for (auto& it : m_actorList)
		if (it.lock()->GetActorID() == actorID)
			return it;

	return std::weak_ptr<CActor>();
}

bool CWorld::DeleteActor(ActorID actorID)
{
	std::weak_ptr<CActor> pActor = GetActor(actorID);
	if (pActor.lock() == nullptr)	//
		return false;
	
	for (auto it = m_actorList.begin(); it != m_actorList.end();) {
		if ((*it).lock() == pActor.lock()) {
			m_actorList.erase(it);
			return true;
		}
		else
			++it;
	}

	return false;
}

bool CWorld::DeleteActor(std::weak_ptr<CActor> pActor)
{
	for (auto it = m_actorList.begin(); it != m_actorList.end();) {
		if ((*it).lock() == pActor.lock()) {
			m_actorList.erase(it);
			return true;
		}
		else
			++it;
	}
	
	return false;
}

bool CWorld::CollisionUpdate()
{



	return true;
}

void CWorld::ResolveCollision()
{



}

void CWorld::SimulWorld()
{
}

