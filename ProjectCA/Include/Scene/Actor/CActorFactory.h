#pragma once

/*
	NOTE:
		Actor를 생성하기 위한 ActorFactory 메소드
		Actor를 생성하기 위해서는 반드시 ActorFactory Class를 사용해야함
		Actor의 Component들 생성도 담당
*/

#include "..\..\..\stdafx.h"
#include "CActor.h"
#include "..\..\Core\Components\ComponentBase.h"

typedef std::shared_ptr<CActor>				ActorPtr;
typedef std::shared_ptr<ComponentBase>	ComponentPtr;


class CActorFactory
{
	friend CActor;

public:
	CActorFactory();
	~CActorFactory();


public:
	static ActorPtr	CreateActor(Types::ObjectData&);
	template<typename ComponentType>
	ComponentPtr CreateComponent(const Types::tstring& strTag) {
		ComponentPtr pComponent = ComponentPtr();
		ComponentType pTemp = std::tr1::static_pointer_cast<ComponentType>
		

	}

private:
	static ActorID		m_iLastActorID;



};
