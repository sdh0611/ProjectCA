#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CRandomBlock.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.hpp"



CRandomBlock::CRandomBlock()
{
}

CRandomBlock::~CRandomBlock()
{
}

bool CRandomBlock::PostInit(const OBJECT_DATA & objectData, CScene * pScene)
{
	if (!CProb::PostInit(objectData, pScene))
		return false;
	GetTransform().lock()->SetPivotRatio(0.5f, 1.f);

	m_bHiding	= false;
	m_Type		= BT_DEFAULT;

	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->AddAnimation(0.5f, TEXT("Default"), TEXT("RandomBlockAnimation"),   true, TEXT("Idle")))
		return false;
	if (!pRender->AddAnimation(0.f, TEXT("Default"), TEXT("RandomBlockDead"),   false, TEXT("Dead")))
		return false;
	if (!pRender->AddAnimation(0.1f, TEXT("Default"), TEXT("RandomBlockHit"),   false, TEXT("Hit")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	return true;
}

void CRandomBlock::Init()
{
	CObject::Init();
	if (m_Type == BT_HIDE)
	{
		m_bHiding = true;
	}
	GetComponent<AnimationRender>().lock()->ChangeAnimation(TEXT("Idle"));
	m_bActive					= true;
	m_ObjectState				= Types::OS_IDLE;
	
}

void CRandomBlock::Render(const HDC & hDC)
{
	if (!m_bHiding)
	{
		if (m_bActive)
		{
			auto pRender = GetComponent<AnimationRender>().lock();
			if (pRender->IsActive())
			{
				pRender->Draw(hDC);
			}

		}
	}
}

void CRandomBlock::LateUpdate()
{
	if (m_bActive)
	{
		CObject::LateUpdate();
	}

}

void CRandomBlock::SetHide()
{
	m_bHiding	= true;
	m_Type		= BT_HIDE;

}

bool CRandomBlock::IsHiding() const
{
	return m_bHiding;
}

