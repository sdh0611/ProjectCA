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
{
}

RenderComponent::~RenderComponent()
{
	if(m_hRenderDC)
		DeleteObject(m_hRenderDC);
}

bool RenderComponent::PostInit(CEntity * pOwner, const TSTRING & strTag)
{
	m_pOwner						= pOwner;

	m_strComponentTag			= strTag;
	m_hRenderDC					= BackBuffer::GetInstance()->AllocationCompatibleDC();
	if (m_hRenderDC == NULL)
		return false;

	m_bActive						= true;
	m_iDrawWidth					= SPRITE_WIDTH;
	m_iDrawHeight					= SPRITE_HEIGHT;
	m_fWidthExpansionRatio		= 1.f;
	m_fHeightExpansionRatio	= 1.f;
	SetPivotRatio(0.f, 0.f);
	LateUpdate();

	return true;
}

void RenderComponent::Init() 
{
	m_bActive = true;
}

void RenderComponent::Update(double dDeltaTime)
{
}

void RenderComponent::LateUpdate()
{
	POSITION position = m_pOwner->GetTransform().lock()->GetScreenPosition();

	m_DrawPivot.x = position.x - m_iDrawWidth * m_fWidthPivotRatio;
	m_DrawPivot.y = position.y - m_iDrawHeight * m_fHeightPivotRatio;
}

void RenderComponent::SetDrawWidth(UINT iWidth)
{
	m_iDrawWidth = iWidth;
}

void RenderComponent::SetDrawHeight(UINT iHeight)
{
	m_iDrawHeight = iHeight;
}

void RenderComponent::SetDrawWidthRatio(float fRatio)
{
	if (fRatio < 0.f)
		return;

	m_fWidthExpansionRatio = fRatio;
	m_iDrawWidth				*= fRatio;

}

void RenderComponent::SetDrawHeightRatio(float fRatio)
{
	if (fRatio < 0.f)
		return;

	m_fHeightExpansionRatio = fRatio;
	m_iDrawHeight				 *= fRatio;
}

void RenderComponent::SetDrawSize(UINT iWidth, UINT iHeight)
{
	m_iDrawWidth = iWidth;
	m_iDrawHeight = iHeight;
}

void RenderComponent::SetExpansionRatio(float fRatio)
{
	if (fRatio < 0.f)
		return;

	m_fWidthExpansionRatio		= fRatio;
	m_fHeightExpansionRatio	= fRatio;

	m_iDrawWidth					*= fRatio;
	m_iDrawHeight					*= fRatio;
}

void RenderComponent::SetWidthPivotRatio(float fRatio)
{
	if (fRatio < 0.f || fRatio > 1.f)
		return;

	m_fWidthPivotRatio = fRatio;
	m_DrawPivot.x -= (m_iDrawWidth * fRatio);

}

void RenderComponent::SetHeightPivotRatio(float fRatio)
{
	if (fRatio < 0.f || fRatio > 1.f)
		return;

	m_fHeightPivotRatio = fRatio;
	m_DrawPivot.y -= (m_iDrawHeight * fRatio);

}

void RenderComponent::SetPivotRatio(float fWidthRatio, float fHeightRatio)
{
	SetWidthPivotRatio(fWidthRatio);
	SetHeightPivotRatio(fHeightRatio);
}

void RenderComponent::MovePivot(float fx, float fy)
{
	m_DrawPivot.x += fx;
	m_DrawPivot.y -= fy;
}

const POSITION & RenderComponent::GetDrawPivot() const
{
	return m_DrawPivot;
}

