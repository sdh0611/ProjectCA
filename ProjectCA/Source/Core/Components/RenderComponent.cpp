#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\Collider.h"
#include	"..\..\..\Include\Core\CResourceManager.h"
#include	"..\..\..\Include\Core\Window\BackBuffer.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Core\Graphic\CAnim.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"



RenderComponent::RenderComponent()
	:m_bVisible(true)
{
}

RenderComponent::~RenderComponent()
{
	if(m_hRenderDC)
		DeleteObject(m_hRenderDC);
}

bool RenderComponent::PostInit(CObject * pOwner, const Types::tstring & strTag)
{
	m_pOwner					= pOwner;

	m_strComponentTag		= strTag;
	m_hRenderDC = BackBuffer::GetInstance()->AllocationCompatibleDC();
	if (m_hRenderDC == NULL)
		return false;

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

