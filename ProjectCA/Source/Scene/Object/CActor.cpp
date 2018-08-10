#include "..\..\..\Include\Scene\Object\CActor.h"

CActor::CActor(ActorID id)
	:m_actorID(id)
{
}

CActor::~CActor()
{
}

void CActor::AddComponent(StrongActorComponentPtr pComponent)
{
	m_componentTable.insert(pComponent);
}

