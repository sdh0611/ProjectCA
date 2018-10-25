#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
//#include "..\..\..\Include\Scene\CWorld.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CLayer.h"
//#include "..\..\..\Include\Scene\Actor\CActorFactory.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"


CActor::CActor()
	:m_actorVector(0, 0), m_actorCurVerticalState(Types::VS_IDLE), m_actorHorizonalState(Types::HS_IDLE)
{
}

CActor::~CActor()
{
	Destroy();
}


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

void CActor::SetActive(bool bActive)
{
	m_bActive = bActive;
}

void CActor::SetActorState(Types::ActorState state)
{
	m_actorCurState = state;
}

void CActor::SetActorVector(float fx, float fy)
{
	m_actorVector.x = fx;
	m_actorVector = fy;
}

void CActor::SetActorDirection(Types::Direction dir)
{
	m_direction = dir;
}

void CActor::SetActorWidth(UINT iWidth)
{
	m_iActorWidth = iWidth;
}

void CActor::SetActorHeight(UINT iHeight)
{
	m_iActorHeight = iHeight;
}

void CActor::SetActorPoint(float fx, float fy)
{
	GetComponent<TransformComponent>()->SetPosition(fx, fy);
}

void CActor::SetActorTag(const Types::tstring & strTag)
{
	m_strActorTag = strTag;
}

void CActor::SetActorVerticalState(Types::VerticalState vertical)
{
	m_actorCurVerticalState = vertical;;
}

void CActor::SetActorHorizonalState(Types::HorizonalState horizonal)
{
	m_actorHorizonalState = horizonal;
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

bool CActor::IsActive() const
{
	return m_bActive;
}

Types::ActorType CActor::GetActorType() const
{
	return m_actorType;
}

Types::ActorState CActor::GetActorState() const
{
	return m_actorCurState;
}

Types::ActorState CActor::GetActorPreState() const
{
	return m_actorPreState;
}

Types::Direction CActor::GetActorDirection() const
{
	return m_direction;
}

Types::Point CActor::GetActorVector() const
{
	return m_actorVector;
}

UINT CActor::GetActorWidth() const
{
	return m_iActorWidth;
}

UINT CActor::GetActorHeight() const
{
	return m_iActorHeight;
}

Types::Point CActor::GetActorPoint() const
{
	return GetComponent<TransformComponent>()->GetPosition();
}

const Types::Point CActor::GetActorPivot() const
{
	return GetComponent<TransformComponent>()->GetPivot();
}

const Types::tstring & CActor::GetActorTag() const
{
	return m_strActorTag;
}

Types::ActorID CActor::GetActorID() const
{
	return m_actorID;
}

Types::VerticalState CActor::GetActorVerticalState() const
{
	return m_actorCurVerticalState;
}

Types::VerticalState CActor::GetActorPreVerticalState() const
{
	return m_actorPreVerticalState;
}

Types::HorizonalState CActor::GetActorHorizonalState() const
{
	return m_actorHorizonalState;
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
