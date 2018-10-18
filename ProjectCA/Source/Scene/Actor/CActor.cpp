#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
//#include "..\..\..\Include\Scene\CWorld.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CLayer.h"
//#include "..\..\..\Include\Scene\Actor\CActorFactory.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"


CActor::CActor()
	:m_actorVector(0, 0), m_actorCurVerticalState(Types::VS_IDLE), m_actorHorizonalState(Types::HS_IDLE)
{
}

CActor::~CActor()
{
	Destroy();
}


//bool CActor::Init(const Types::ActorData& data)
//{
//	m_fActorPoint = data.fObjectPoint;
//	m_actorType = data.objectType;
//	m_actorState = data.objectState;
//	m_direction = data.objectDirection;
//	m_iActorWidth = data.iWidth;
//	m_iActorHeight = data.iHeight;
//	m_actorID = data.actorID;
//	m_strActorTag = data.strActorName;
//	//m_pOwnerScene = reinterpret_cast<CGameScene*>(data.pOwnerScene);
//
//	m_bActive = true;
//
//	return true;
//}

void CActor::Update(double fDeltaTime)
{
	if (m_bActive) {
		//m_componentTable[TEXT("PhysicsComponent")]->Update(fDeltaTime);
		for (auto& it : m_componentTable)
			it.second->Update(fDeltaTime);

		m_actorPreState = m_actorCurState;
		m_actorPreVerticalState = m_actorCurVerticalState;
	}

}

//void CActor::Render(const HDC & hDC)
//{
//	if (m_bActive) {
//		Rectangle(hDC, m_fActorPoint.x, m_fActorPoint.y, m_fActorPoint.x + m_iActorWidth, m_fActorPoint.y + m_iActorHeight);
//	}
//
//}

//bool						m_bActive;
//UINT						m_iActorWidth;
//UINT						m_iActorHeight;
//Types::Point				m_fActorPoint;
//Types::ObjectType		m_actorType;
//Types::ObjectState		m_actorState;
//Types::Direction		m_direction;
//Types::ActorID			m_actorID;
//Types::tstring			m_strActorTag;
////CWorld*					m_pOwnerWorld;
//CGameScene*			m_pOwnerScene;

//void CActor::LateUpdate(float fDeltaTime)
//{
//}

void CActor::Destroy()
{
	if (!m_componentTable.empty())
		for (auto& it : m_componentTable)
			SAFE_DELETE(it.second)

	m_componentTable.clear();

}

ComponentBase * CActor::GetComponent(const Types::tstring & strTag)
{
	auto it = m_componentTable.find(strTag);

	if(it == m_componentTable.end())
		return nullptr;

	return it->second;
}

bool CActor::AddComponent(ComponentBase * pComponent, const Types::tstring & strTag)
{
	return m_componentTable.insert(std::make_pair(strTag, pComponent)).second;
}

bool CActor::DeleteComponent(const Types::tstring & strTag)
{
	ComponentTable::iterator it = m_componentTable.find(strTag);

	if (it == m_componentTable.end())
		return false;

	m_componentTable.erase(strTag);

	return true;
}

//CWorld* const CActor::GetOwnerWorld() const { return m_pOwnerWorld; }
//
//void CActor::SetOwnerWorld(CWorld* pWorld) { m_pOwnerWorld = pWorld; }

CGameScene* const CActor::GetOwnerScene() const { return m_pOwnerScene; }

void CActor::SetOwnerScene(CGameScene* pScene) { m_pOwnerScene = pScene; }

bool CActor::SetLayer(const Types::tstring & strLayerTag)
{
	//CLayer* pLayer = m_pOwnerScene->FindLayer(strLayerTag);

	//if (pLayer == nullptr)
	//	return false;

	//pLayer->AddActor(this);

	return true;
}

void CActor::FlipVector()
{
	m_actorVector.x *= -1.f;
	m_actorVector.y *= -1.f;

	//Debug::MessageInfo(TEXT("FlipVector"));
}

void CActor::FlipActorDirection()
{
	if (m_direction == Types::DIR_LEFT)
		m_direction = Types::DIR_RIGHT;
	else if (m_direction == Types::DIR_RIGHT)
		m_direction = Types::DIR_LEFT;
	else if (m_direction == Types::DIR_UP)
		m_direction = Types::DIR_DOWN;
	else if (m_direction == Types::DIR_DOWN)
		m_direction = Types::DIR_UP;

}
