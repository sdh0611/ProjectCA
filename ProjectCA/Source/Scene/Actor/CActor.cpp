#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\CLayer.h"
#include "..\..\..\Include\Core\Components\InputComponent.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"


CActor::CActor() 
{
}

CActor::~CActor()
{
	//Destroy();
}

//기본 Actor들의 속성 초기화 및 TranformComponent 생성, 기본PivotRatio은 0.5, 1.0으로
bool CActor::PostInit(const ACTOR_DATA& data, CGameScene *pScene)
{
	if (!CObject::PostInit(data, pScene))
		return false;

	GetTransform().lock()->SetPivotRatio(0.5f, 1.f);		//Actor들의 기본 PivotRatio는 0.5, 1.0으로 설정

	//기본 Actor의 속성 초기화
	m_ActType					= data.m_ActType;
	m_Direction					= data.m_Direction;
	
	return true;
}

void CActor::Init()
{
	CObject::Init();
	m_ActType = Types::ACT_IDLE;
}

void CActor::Update(double dDeltaTime)
{
	if (m_bActive) 
	{
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

void CActor::SetActorAct(ACT act)
{
	m_ActType = act;
}

void CActor::SetActorDirection(DIRECTION dir)
{
	m_Direction = dir;
}

ACT CActor::GetActorAct() const
{
	return m_ActType;
}

Types::Direction CActor::GetActorDirection() const
{
	return m_Direction;
}

UINT CActor::GetActorWidth() const
{
	return m_iEntityWidth;
}

UINT CActor::GetActorHeight() const
{
	return m_iEntityHeight;
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
