#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Scene\CWorld.h"
#include "..\..\..\Include\Scene\CGameScene.h"
//#include "..\..\..\Include\Scene\CLayer.h"
//#include "..\..\..\Include\Scene\Actor\CActorFactory.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"


CActor::~CActor()
{
	Destroy();
}

bool CActor::Init(const Types::ObjectData& data)
{
	m_fActorPoint = data.fObjectPoint;
	m_actorType = data.objectType;
	m_actorState = data.objectState;
	m_direction = data.objectDirection;
	m_iActorWidth = data.iWidth;
	m_iActorHeight = data.iHeight;
	m_actorID = data.actorID;
	m_pOwnerWorld = data.pOwnerWorld;
	m_pOwnerScene = data.pOwnerScene;


	return true;
}

void CActor::Update(float fDeltaTime)
{
	if (m_bActive) {

	}

}

void CActor::Render(const HDC & hDC)
{
	if (m_bActive) {
		Rectangle(hDC, m_fActorPoint.x, m_fActorPoint.y, m_fActorPoint.x + m_iActorWidth, m_fActorPoint.y + m_iActorHeight);
	}

}

void CActor::LateUpdate(float fDeltaTime)
{
}

void CActor::Destroy()
{
	if (!m_componentTable.empty())
		for (auto& it : m_componentTable)
			SAFE_DELETE(it.second)

	m_componentTable.clear();

}

ComponentBase * CActor::GetComponent(const Types::tstring & strTag)
{
	ComponentTable::iterator it = m_componentTable.find(strTag);

	if(it == m_componentTable.end())
		return nullptr;

	return it->second;
}

bool CActor::DeleteComponent(const Types::tstring & strTag)
{
	ComponentTable::iterator it = m_componentTable.find(strTag);

	if (it == m_componentTable.end())
		return false;

	m_componentTable.erase(strTag);

	return true;
}

CWorld* const CActor::GetOwnerWorld() const { return m_pOwnerWorld; }

void CActor::SetOwnerWorld(CWorld* pWorld) { m_pOwnerWorld = pWorld; }

CGameScene* const CActor::GetOwnerScene() const { return m_pOwnerScene; }

void CActor::SetOwnerScene(CGameScene* pScene) { m_pOwnerScene = pScene; }