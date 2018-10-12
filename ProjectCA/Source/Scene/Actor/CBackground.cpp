#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CBackground.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"


CBackground::CBackground()
{
}

CBackground::~CBackground()
{
}

bool CBackground::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	if (data.iActorWidth > MAX_ACTOR_SIZE || data.iActorHeight > MAX_ACTOR_SIZE)
		return false;

	if (data.actorPoint.x < 0 || data.actorPoint.x > MAX_WIDTH ||
		data.actorPoint.y < 0 || data.actorPoint.y > MAX_HEIGHT)
		return false;

	m_iActorWidth = data.iActorWidth;
	m_iActorHeight = data.iActorHeight;
	m_actorPoint = m_spawnPoint = data.actorPoint;
	m_actorType = data.actorType;
	m_actorCurState = m_actorPreState = data.actorState;
	m_actorJumpState = m_actorPreJumpState = Types::JS_IDLE;
	m_direction = data.direction;
	m_actorVector = data.vector;
	m_actorID = data.actorID;
	m_strActorTag = data.strActorTag;
	m_pOwnerScene = pScene;
	m_bActive = data.bActive;

	RenderComponent* pRender = new RenderComponent;
	if (!pRender->Init(this))
		return false;

	if (!pRender->AddAnim(100.f, TEXT("BackgroundMountain2"), MAX_WIDTH, MAX_HEIGHT, false, false))
		return false;

	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	return true;;
}

bool CBackground::Init()
{
	return true;
}

void CBackground::Update(double dDeltaTIme)
{
	for (auto& it : m_componentTable)
		it.second->Update(dDeltaTIme);

}

void CBackground::Render(const HDC & hDC)
{
	auto it = m_componentTable.find(TEXT("RenderComponent"));

	if (it != m_componentTable.end())
		static_cast<RenderComponent*>((*it).second)->Draw(hDC);


}

