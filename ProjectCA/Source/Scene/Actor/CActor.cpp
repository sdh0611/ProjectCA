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
	:m_ActorCurVerticalState(Types::VS_IDLE), m_ActorHorizonalState(Types::HS_IDLE)
{
}

CActor::~CActor()
{
	//Destroy();
}

//기본 Actor들의 속성 초기화 및 TranformComponent 생성, 기본PivotRatio은 0.5, 1.0으로
bool CActor::PostInit(const Types::ActorData & data, CGameScene *pScene)
{
	//기본 Actor의 속성 초기화
	m_ActorType = data.m_ActorType;
	m_ActorCurState = data.m_ActorState;
	m_ActorCurVerticalState = data.m_VerticalState;
	m_ActorHorizonalState = data.m_HorizonalState;
	m_Direction = data.m_Direction;
	m_ActorID = data.m_iActorID;
	
	if (!CObject::PostInit(data, pScene))
		return false;	
	GetTransform().lock()->SetPivotRatio(0.5f, 1.f);		//Actor들의 기본 PivotRatio는 0.5, 1.0으로 설정

	return true;
}

void CActor::Init()
{
	CObject::Init();
}

void CActor::Update(double dDeltaTime)
{
	if (m_bActive) {
		std::weak_ptr<InputComponent> pInput =STATIC_POINTER_CAST(InputComponent, (GetComponent(TEXT("InputComponent")).lock()));
		if (!pInput.expired())
		{
			pInput.lock()->Update(dDeltaTime);
		}

		ActorBehavior(dDeltaTime);

		for (auto& component : m_ComponentTable)
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
	for (auto& it : m_ComponentTable)
	{
		it.second->LateUpdate(dDeltaTime);
	}

}

//void CActor::Destroy()
//{
//	//if (!m_ComponentTable.empty())
//	//	for (auto& it : m_ComponentTable)
//	//		SAFE_DELETE(it.second)
//
//	m_ComponentTable.clear();
//
//}

void CActor::SetActorState(Types::ActorState state)
{
	m_ActorCurState = state;
}

void CActor::SetActorDirection(Types::Direction dir)
{
	m_Direction = dir;
}


void CActor::SetActorVerticalState(Types::VerticalState vertical)
{
	m_ActorCurVerticalState = vertical;;
}

void CActor::SetActorHorizonalState(Types::HorizonalState horizonal)
{
	m_ActorHorizonalState = horizonal;
}

//CWorld* const CActor::GetOwnerWorld() const { return m_pOwnerWorld; }
//
//void CActor::SetOwnerWorld(CWorld* pWorld) { m_pOwnerWorld = pWorld; }

Types::ActorType CActor::GetActorType() const
{
	return m_ActorType;
}

Types::ActorState CActor::GetActorState() const
{
	return m_ActorCurState;
}

Types::Direction CActor::GetActorDirection() const
{
	return m_Direction;
}

UINT CActor::GetActorWidth() const
{
	return m_iObjectWidth;
}

UINT CActor::GetActorHeight() const
{
	return m_iObjectHeight;
}

Types::ActorID CActor::GetActorID() const
{
	return m_ActorID;
}

Types::VerticalState CActor::GetActorVerticalState() const
{
	return m_ActorCurVerticalState;
}

Types::HorizonalState CActor::GetActorHorizonalState() const
{
	return m_ActorHorizonalState;
}

void CActor::FlipActorDirection()
{
	if (m_Direction == Types::DIR_LEFT)
		m_Direction = Types::DIR_RIGHT;
	else if (m_Direction == Types::DIR_RIGHT)
		m_Direction = Types::DIR_LEFT;
	else if (m_Direction == Types::DIR_UP)
		m_Direction = Types::DIR_DOWN;
	else if (m_Direction == Types::DIR_DOWN)
		m_Direction = Types::DIR_UP;

}
