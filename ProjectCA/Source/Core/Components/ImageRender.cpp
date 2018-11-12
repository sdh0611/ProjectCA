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
	RenderComponent::PostInit(pOwner, strTag);

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
	POSITION point = m_pOwner->GetComponent<TransformComponent>()->GetScreenPivot();
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(memDC, m_pWeakSprite.lock()->GetBitmap());

	TransparentBlt(hDC, point.x, point.y,
		m_iDrawWidth, m_iDrawHeight, memDC, 0, 0,
		m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight(),
		m_colorRef);

	SelectObject(memDC, hOldBit);
	DeleteDC(memDC);

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
