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
		DeleteDC(m_hRenderDC);

	if (m_hBlendingDC)
		DeleteDC(m_hBlendingDC);
}

bool RenderComponent::PostInit(CEntity * pOwner, const TSTRING & strTag)
{
	m_pOwner						= pOwner;

	//m_bBlending					= false;
	m_RenderMode				= RENDER_DEFAULT;
	m_strComponentTag			= strTag;
	m_hRenderDC					= BackBuffer::GetInstance()->AllocationCompatibleDC();
	if (m_hRenderDC == NULL)
		return false;
	m_hBlendingDC				= BackBuffer::GetInstance()->AllocationCompatibleDC();
	if (m_hBlendingDC == NULL)
		return false;

	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.AlphaFormat = 0;
	m_BlendFunction.SourceConstantAlpha = 127;

	m_bActive						= true;
	m_iDrawWidth					= SPRITE_WIDTH;
	m_iDrawHeight					= SPRITE_HEIGHT;
	m_fWidthExpansionRatio		= 1.f;
	m_fHeightExpansionRatio	= 1.f;
	SetPivotRatio(0.f, 0.f);
	//LateUpdate();

	return true;
}

void RenderComponent::Init() 
{
	m_bActive										= true;
	m_RenderMode								= RenderMode::RENDER_DEFAULT;
	m_BlendFunction.SourceConstantAlpha	= 127;
}

void RenderComponent::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		switch (m_RenderMode)
		{
		case RenderMode::RENDER_FADE_OUT:
			if (m_BlendFunction.SourceConstantAlpha > 0)
			{
				m_BlendFunction.SourceConstantAlpha -= (10.f*dDeltaTime);
			}
			else
			{
				m_BlendFunction.SourceConstantAlpha = 0;
				SetActive(false);
			}
			break;
		case RenderMode::RENDER_FADE_IN:
			if (m_BlendFunction.SourceConstantAlpha < 255)
			{
				m_BlendFunction.SourceConstantAlpha += (10.f * dDeltaTime);
			}
			else
			{
				m_RenderMode = RenderMode::RENDER_DEFAULT;
			}
			break;
		}

	}

}

void RenderComponent::LateUpdate()
{
	POSITION position = m_pOwner->GetTransform().lock()->GetScreenPosition();

	m_DrawPivot.x = position.x - m_iDrawWidth * m_fWidthPivotRatio;
	m_DrawPivot.y = position.y - m_iDrawHeight * m_fHeightPivotRatio;
}

//void RenderComponent::SetBlending(bool bBlending)
//{
//	m_bBlending = bBlending;
//}

void RenderComponent::SetRenderMode(RenderMode mode)
{
	m_RenderMode = mode;
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
	//m_DrawPivot.x -= (m_iDrawWidth * fRatio);
	m_DrawPivot.x = m_pOwner->GetEntityPosition().x - m_iDrawWidth * fRatio;

}

void RenderComponent::SetHeightPivotRatio(float fRatio)
{
	if (fRatio < 0.f || fRatio > 1.f)
		return;

	m_fHeightPivotRatio = fRatio;
	//m_DrawPivot.y -= (m_iDrawHeight * fRatio);
	m_DrawPivot.x = m_pOwner->GetEntityPosition().y - m_iDrawHeight * fRatio;
}

void RenderComponent::SetPivotRatio(float fWidthRatio, float fHeightRatio)
{
	SetWidthPivotRatio(fWidthRatio);
	SetHeightPivotRatio(fHeightRatio);
}

void RenderComponent::SetAlpha(int iAlpha)
{
	if (iAlpha < 0 || iAlpha > 255)
		return;

	m_BlendFunction.SourceConstantAlpha = iAlpha;
}

void RenderComponent::MovePivot(float fx, float fy)
{
	m_DrawPivot.x += fx;
	m_DrawPivot.y -= fy;
}

//bool RenderComponent::IsBlending() const
//{
//	return m_bBlending;
//}

const POSITION & RenderComponent::GetDrawPivot() const
{
	return m_DrawPivot;
}

void RenderComponent::SwitchBlending()
{
	//m_bBlending = !m_bBlending;
	if (m_RenderMode == RenderMode::RENDER_DEFAULT)
	{
		m_RenderMode = RenderMode::RENDER_BLEND;
	}
	else if (m_RenderMode == RenderMode::RENDER_BLEND)
	{
		m_RenderMode = RenderMode::RENDER_DEFAULT;
	}
}

