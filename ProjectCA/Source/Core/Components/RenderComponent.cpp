#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\Collider.h"
#include	"..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Core\Graphic\CAnim.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"



RenderComponent::RenderComponent()
	:m_bVisible(true), m_bChangeAnim(false)
{
}

RenderComponent::~RenderComponent()
{
}

bool RenderComponent::PostInit(CActor * pOwner, const Types::tstring & strTag)
{
	std::shared_ptr<CActor> pActor = std::shared_ptr<CActor>(pOwner);
	m_pOwner					= pActor;

	m_strComponentTag		= strTag;
	

	return true;
}

void RenderComponent::Init() 
{
}

void RenderComponent::Update(double dDeltaTime)
{
}

void RenderComponent::LateUpdate(double dDeltaTime)
{
}

void RenderComponent::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

bool RenderComponent::IsVisible() const
{
	return m_bVisible;
}

