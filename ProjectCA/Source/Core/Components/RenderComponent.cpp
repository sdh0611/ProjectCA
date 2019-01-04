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
	//더블 버퍼링에서 BackDC에 그려넣기 위한 BackDC에 대한 CompatibleDC를 BackBuffer로부터 할당받음.
	m_hRenderDC					= BackBuffer::GetInstance()->AllocationCompatibleDC();
	if (m_hRenderDC == NULL)
		return false;
	//Blending을 위한 DC. BackBuffer로부터 BackDC에 대한 CompatibleDC를 할당받는다.
	m_hBlendingDC				= BackBuffer::GetInstance()->AllocationCompatibleDC();
	if (m_hBlendingDC == NULL)
		return false;

	m_BlendFunction.BlendOp					= AC_SRC_OVER;
	m_BlendFunction.BlendFlags				= 0;
	m_BlendFunction.AlphaFormat				= 0;
	m_BlendFunction.SourceConstantAlpha	= 127;

	m_bActive						= true;
	//처음 초기화할 때는 기본적인 이미지의 크기값인 SPRITE_WIDTH, SPRITE_HEGITH로 각각 초기화
	m_iDrawWidth					= SPRITE_WIDTH;
	m_iDrawHeight					= SPRITE_HEIGHT;
	//그림을 그릴 때 너비, 높이 각각의 확대비율. 기본값은 1.0
	m_fWidthExpansionRatio		= 1.f;
	m_fHeightExpansionRatio	= 1.f;
	//Pivot 비율은 기본값으로 각각 0으로 초기화
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
			//FADE_OUT일 경우 Alpha값을 일정 비율만큼 계속 줄임.
		case RenderMode::RENDER_FADE_OUT:
			if (m_BlendFunction.SourceConstantAlpha > 0)
			{
				m_BlendFunction.SourceConstantAlpha -= (10.f*dDeltaTime);
			}
			else
			{
				//Alpha값이 0이면 보이지 않는 경우이므로 비활성화 시킴.
				m_BlendFunction.SourceConstantAlpha = 0;
				SetActive(false);
			}
			break;
			//FADE_IN일 경우 Alpha값을 일정 비율만큼 계속 늘림.
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
	//Pivot의 값은 0~1사이여야만 한다.
	if (fRatio < 0.f || fRatio > 1.f)
		return;

	m_fWidthPivotRatio = fRatio;
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

const POSITION & RenderComponent::GetDrawPivot() const
{
	return m_DrawPivot;
}

void RenderComponent::SwitchBlending()
{
	if (m_RenderMode == RenderMode::RENDER_DEFAULT)
	{
		m_RenderMode = RenderMode::RENDER_BLEND;
	}
	else if (m_RenderMode == RenderMode::RENDER_BLEND)
	{
		m_RenderMode = RenderMode::RENDER_DEFAULT;
	}
}

