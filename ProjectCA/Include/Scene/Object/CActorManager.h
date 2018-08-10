#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"
#include "..\..\Core\ActorComponent\CActorComponent.h"


class CActorManager {

public:
	CActorManager();
	~CActorManager();


public:
	StrongActorPtr	CreateActor();
	


private:
	ActorID			m_lLastID;
	

};