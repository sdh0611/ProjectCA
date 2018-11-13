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

bool ImageRender::PostInit(CActor * pOwner, const Types::tstring & strTag)
{
	if (!RenderComponent::PostInit(pOwner, strTag))
		return false;

	m_iDrawWidth = pOwner->GetActorWidth();
	m_iDrawHeight = pOwner->GetActorHeight();
	m_ColorRef = RGB(248, 7, 220);

	return true;
}

void ImageRender::Init()
{
}

void ImageRender::Update(double dDeltaTIme)
{
}

void ImageRender::Draw(const HDC & hDC)
{
	POSITION point = m_pOwner->GetComponent<TransformComponent>().lock()->GetScreenPivot();
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_hRenderDC, m_pWeakSprite.lock()->GetBitmap());

	TransparentBlt(hDC, point.x, point.y,
		m_iDrawWidth, m_iDrawHeight, m_hRenderDC, 0, 0,
		m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight(),
		m_ColorRef);

	SelectObject(m_hRenderDC, hOldBit);
}

void ImageRender::Draw(const HDC& hDC, const POSITION& position)
{
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_hRenderDC, m_pWeakSprite.lock()->GetBitmap());

	TransparentBlt(hDC, position.x, position.y,
		m_iDrawWidth, m_iDrawHeight, m_hRenderDC, 0, 0,
		m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight(),
		m_ColorRef);

	SelectObject(m_hRenderDC, hOldBit);
}

void ImageRender::Draw(const HDC & hDC, const POSITION & position, std::weak_ptr<CSprite> pSprite)
{
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_hRenderDC, pSprite.lock()->GetBitmap());

	TransparentBlt(hDC, position.x, position.y,
		m_iDrawWidth, m_iDrawHeight, m_hRenderDC, 0, 0,
		pSprite.lock()->GetBitWidth(), pSprite.lock()->GetBitHeight(),
		m_ColorRef);

	SelectObject(m_hRenderDC, hOldBit);

}

void ImageRender::SetDrawWidth(UINT iWidth)
{
	m_iDrawWidth = iWidth;
}

void ImageRender::SetDrawHeight(UINT iHeight)
{
	m_iDrawHeight = iHeight;
}

void ImageRender::SetDrawSize(UINT iWidth, UINT iHeight)
{
	m_iDrawWidth = iWidth;
	m_iDrawHeight = iHeight;
}

bool ImageRender::SetSprite(const TSTRING & strImageTag)
{
	m_pWeakSprite = CResourceManager::GetInstance()->GetWeakSprtiePtr(strImageTag);
	
	if(m_pWeakSprite.expired())
		return false;

	return true;
}
