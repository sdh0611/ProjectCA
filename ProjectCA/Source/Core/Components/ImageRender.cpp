#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"


ImageRender::ImageRender()
	:RenderComponent()
{
}

ImageRender::~ImageRender()
{
}

bool ImageRender::PostInit(CEntity * pOwner, const Types::tstring & strTag)
{
	if (!RenderComponent::PostInit(pOwner, strTag))
		return false;

	m_ColorRef			= RGB(248, 7, 220);

	return true;
}

void ImageRender::Init()
{
	RenderComponent::Init();
}

void ImageRender::Draw(const HDC & hDC)
{
	if (m_bActive)
	{
		if (!m_pWeakSprite.expired())
		{
			HBITMAP hOldBit = (HBITMAP)SelectObject(m_hRenderDC, m_pWeakSprite.lock()->GetBitmap());

			if (m_RenderMode == RenderMode::RENDER_DEFAULT)
			{
				TransparentBlt(hDC, m_DrawPivot.x, m_DrawPivot.y,
					m_iDrawWidth, m_iDrawHeight, m_hRenderDC, 0, 0,
					m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight(),
					m_ColorRef);
				}
			else
			{
				HBITMAP hTempBit = CreateCompatibleBitmap(hDC, MAX_WIDTH, MAX_HEIGHT);
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_hBlendingDC, hTempBit);

				BitBlt(m_hBlendingDC, 0, 0, MAX_WIDTH, MAX_HEIGHT, hDC, 0, 0, SRCCOPY);

				TransparentBlt(m_hBlendingDC, m_DrawPivot.x, m_DrawPivot.y,
					m_iDrawWidth, m_iDrawHeight, m_hRenderDC, 0, 0,
					m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight(), m_ColorRef);

				AlphaBlend(hDC, m_DrawPivot.x, m_DrawPivot.y,
					m_iDrawWidth, m_iDrawHeight, m_hBlendingDC, 0, 0,
					m_iDrawWidth, m_iDrawHeight, m_BlendFunction);

				DeleteObject(SelectObject(m_hBlendingDC, hOldBitmap));
				DeleteObject(hTempBit);
			}
			SelectObject(m_hRenderDC, hOldBit);
		}
	}

}

void ImageRender::Draw(const HDC& hDC, const POSITION& position)
{
	if (m_bActive)
	{
		if (!m_pWeakSprite.expired())
		{
			HBITMAP hOldBit = (HBITMAP)SelectObject(m_hRenderDC, m_pWeakSprite.lock()->GetBitmap());

			if (m_RenderMode == RenderMode::RENDER_DEFAULT)
			{
				TransparentBlt(hDC, position.x, position.y,
					m_iDrawWidth, m_iDrawHeight, m_hRenderDC, 0, 0,
					m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight(),
					m_ColorRef);
			}
			else
			{
				HBITMAP hTempBit = CreateCompatibleBitmap(hDC, MAX_WIDTH, MAX_HEIGHT);
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_hBlendingDC, hTempBit);

				BitBlt(m_hBlendingDC, 0, 0, MAX_WIDTH, MAX_HEIGHT, hDC, 0, 0, SRCCOPY);

				TransparentBlt(m_hBlendingDC, position.x, position.y,
					m_iDrawWidth, m_iDrawHeight, m_hRenderDC, 0, 0,
					m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight(), m_ColorRef);

				AlphaBlend(hDC, position.x, position.y,
					m_iDrawWidth, m_iDrawHeight, m_hBlendingDC, 0, 0,
					m_iDrawWidth, m_iDrawHeight, m_BlendFunction);

				DeleteObject(SelectObject(m_hBlendingDC, hOldBitmap));
				DeleteObject(hTempBit);
			}
			SelectObject(m_hRenderDC, hOldBit);
		}
	}

}

void ImageRender::Draw(const HDC & hDC, const POSITION & position, std::weak_ptr<CSprite> pSprite)
{
	if (m_bActive)
	{
		HBITMAP hOldBit = (HBITMAP)SelectObject(m_hRenderDC, pSprite.lock()->GetBitmap());

		if (m_RenderMode == RenderMode::RENDER_DEFAULT)
		{
			TransparentBlt(hDC, position.x, position.y,
				m_iDrawWidth, m_iDrawHeight, m_hRenderDC, 0, 0,
				pSprite.lock()->GetBitWidth(), pSprite.lock()->GetBitHeight(),
				m_ColorRef);

		}
		else
		{
			HBITMAP hTempBit = CreateCompatibleBitmap(hDC, MAX_WIDTH, MAX_HEIGHT);
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_hBlendingDC, hTempBit);

			BitBlt(m_hBlendingDC, 0, 0, MAX_WIDTH, MAX_HEIGHT, hDC, 0, 0, SRCCOPY);

			TransparentBlt(m_hBlendingDC, position.x, position.y,
				m_iDrawWidth, m_iDrawHeight, m_hRenderDC, 0, 0,
				pSprite.lock()->GetBitWidth(), pSprite.lock()->GetBitHeight(), m_ColorRef);

			AlphaBlend(hDC, position.x, position.y,
				m_iDrawWidth, m_iDrawHeight, m_hBlendingDC, 0, 0,
				m_iDrawWidth, m_iDrawHeight, m_BlendFunction);

			DeleteObject(SelectObject(m_hBlendingDC, hOldBitmap));
			DeleteObject(hTempBit);
		}
		SelectObject(m_hRenderDC, hOldBit);
	}

}


bool ImageRender::SetSprite(const TSTRING & strImageTag)
{
	m_pWeakSprite = CResourceManager::GetInstance()->GetWeakSprtiePtr(strImageTag);
	
	if(m_pWeakSprite.expired())
		return false;

	return true;
}

bool ImageRender::SetSprite(std::shared_ptr<CSprite> pSprite)
{
	m_pWeakSprite = pSprite;

	if (m_pWeakSprite.expired())
		return false;

	return true;
}
