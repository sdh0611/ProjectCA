#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Scene\CWorld.h"
#include "..\..\..\Include\Scene\CLayer.h"
#include "..\..\..\Include\Scene\Actor\CActorFactory.h"



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
	m_actorID = data.actorID;

	return true;
}

void CActor::Update(float fDeltaTime)
{


}

void CActor::Render(const HDC & hDC)
{


}

void CActor::Destroy()
{
	if (!m_componentTable.empty())
		for (auto& it : m_componentTable)
			SAFE_DELETE(it.second)

	m_componentTable.clear();

}
