#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
//#include "..\..\..\Include\Scene\CWorld.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CLayer.h"
//#include "..\..\..\Include\Scene\Actor\CActorFactory.h"
#include "..\..\..\Include\Core\Components\InputComponent.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"


CActor::CActor() 
	:m_actorCurVerticalState(Types::VS_IDLE), m_actorHorizonalState(Types::HS_IDLE)
{
}

CActor::~CActor()
{
	Destroy();
}


void CActor::Update(double dDeltaTime)
{
	if (m_bActive) {
		std::weak_ptr<InputComponent> pInput = std::tr1::static_pointer_cast<InputComponent>(GetComponent(TEXT("InputComponent")).lock());
		if (!pInput.expired())
		{
			pInput.lock()->Update(dDeltaTime);
		}

		ActorBehavior(dDeltaTime);

		for (auto& component : m_componentTable)
		{
			if (component.first == TEXT("InputComponent"))
			{
				continue;
			}
			component.second->Update(dDeltaTime);
		}
	}

}

void CActor::LateUpdate(double dDeltaTime)
{
	for (auto& it : m_componentTable)
	{
		it.second->LateUpdate(dDeltaTime);
	}

}

void CActor::Destroy()
{
	//if (!m_componentTable.empty())
	//	for (auto& it : m_componentTable)
	//		SAFE_DELETE(it.second)

	m_componentTable.clear();

}

std::weak_ptr<ComponentBase> CActor::GetComponent(const TSTRING& strTag)
{
	auto it = m_componentTable.find(strTag);

	if (it == m_componentTable.end())
		return std::weak_ptr<ComponentBase>();

	return it->second;
}

bool CActor::AddComponent(std::shared_ptr<ComponentBase> pComponent, const TSTRING& strTag)
{
	return m_componentTable.insert(std::make_pair(strTag, pComponent)).second;
}

bool CActor::DeleteComponent(const TSTRING & strTag)
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
	GetComponent<TransformComponent>().lock()->SetPosition(fx, fy);
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

Types::Direction CActor::GetActorDirection() const
{
	return m_direction;
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
	return GetComponent<TransformComponent>().lock()->GetPosition();
}

const Types::Point CActor::GetActorPivot() const
{
	return GetComponent<TransformComponent>().lock()->GetPivot();
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

Types::HorizonalState CActor::GetActorHorizonalState() const
{
	return m_actorHorizonalState;
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
