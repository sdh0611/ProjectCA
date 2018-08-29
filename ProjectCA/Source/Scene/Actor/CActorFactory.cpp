#include "..\..\..\Include\Scene\Actor\CActorFactory.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"


CActorFactory::CActorFactory()
{
}


CActorFactory::~CActorFactory()
{
}

ActorPtr CActorFactory::CreateActor(Types::ObjectData& data)
{
	ActorPtr pActor;
	
	if (!pActor->Init(data))
		return ActorPtr();

	return pActor;
}
