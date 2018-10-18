#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Graphic\CAnim.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"


CAnim::CAnim()
	:m_bAnimate(false), m_bLoop(false), m_iCurFrame(0), m_iMaxFrame(0),
	m_iDrawWidth(0), m_iDrawHeight(0),	m_dPlayTime(0.f), m_dPlaySpeed(0.f), 
	m_dPlaySectionLength(0.f), m_dTimeElapsed(0.f)
{
}

CAnim::~CAnim()
{
}

bool CAnim::Init(std::shared_ptr<CActor> pOwner, const Types::tstring& strSpriteName, 
	UINT iWidth, UINT iHeight, double dPlayTime, bool bLoop, bool bAnimate, 
	const Types::tstring& strAnimTag)
{
	m_pWeakSprite = CResourceManager::GetInstance()->GetWeakSprtiePtr(strSpriteName);
	if (m_pWeakSprite.expired())
		return false;

	m_pOwner					= pOwner;

	m_iMaxFrame				= m_pWeakSprite.lock()->GetBitWidth()/SPRITE_WIDTH;
	m_iDrawWidth				= iWidth;
	m_iDrawHeight				= iHeight;

	m_colorRef					= RGB(248, 7, 220);

	m_dPlayTime				= dPlayTime;
	m_dPlaySpeed				= 1.f;
	m_dPlaySectionLength	= m_dPlayTime / m_iMaxFrame;
	
	m_strAnimTag				= strAnimTag;

	m_bLoop						= bLoop;
	m_bAnimate				= bAnimate;

	return true;
}


void CAnim::Update(double fDeltaTIme)
{
	m_dTimeElapsed += fDeltaTIme;
}

void CAnim::Draw(const HDC & hDC, const Types::Point& point)
{
	if (!m_pWeakSprite.expired()) 
	{
		//printf("Image : (%d, %d)\n", m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight());

		if (m_bAnimate) 
		{
			DrawAnimation(hDC, point);
			//Rectangle(hDC, m_pOwner.lock()->GetActorPoint().x, m_pOwner.lock()->GetActorPoint().y,
			//	m_pOwner.lock()->GetActorPoint().x + (m_pWeakSprite.lock()->GetBitWidth())/SPRITE_WIDTH,
			//	m_pOwner.lock()->GetActorPoint().y + (m_pWeakSprite.lock()->GetBitHeight())/SPRITE_HEIGHT);
		}
		else 
		{
			DrawImage(hDC, point);
			//Rectangle(hDC, m_pOwner.lock()->GetActorPoint().x, m_pOwner.lock()->GetActorPoint().y,
			//	m_pOwner.lock()->GetActorPoint().x + m_pWeakSprite.lock()->GetBitWidth(),
			//	m_pOwner.lock()->GetActorPoint().y + m_pWeakSprite.lock()->GetBitHeight());
		}
	}
}

bool CAnim::SetSprite(const Types::tstring & strSpriteName)
{
	m_pWeakSprite = CResourceManager::GetInstance()->GetWeakSprtiePtr(strSpriteName);
	if (m_pWeakSprite.expired())
		return false;

	return true;
}

bool CAnim::SetTotalPlayTime(double dTime)
{
	if (dTime < 0.f)
		return false;
	m_dPlayTime = dTime;

	return true;
}

bool CAnim::SetPlaySpeed(double dSpeed)
{
	if(dSpeed < 0.f)
		return false;
	
	m_dPlaySpeed = dSpeed;

	return true;
}

void CAnim::SetDrawingWidth(UINT iWidth)
{
	if (iWidth < 0.f || iWidth >MAX_WIDTH)
		return;

	m_iDrawWidth = iWidth;
}

void CAnim::SetDrawingHeight(UINT iHeight)
{
	if (iHeight < 0.f || iHeight >MAX_HEIGHT)
		return;

	m_iDrawHeight = iHeight;
}

void CAnim::DrawAnimation(const HDC & hDC, const Types::Point& point)
{

	if (m_dTimeElapsed >= (m_dPlaySectionLength / m_dPlaySpeed)) {
		m_dTimeElapsed = 0.f;
		if (m_iCurFrame < m_iMaxFrame-1) 
		{
			++m_iCurFrame;
		}
		else
		{
			if (m_bLoop)
			{
				++m_iCurFrame %= m_iMaxFrame;
			}
		}


	}
	//printf("Frame : %d, %d\n", m_iCurFrame, m_iMaxFrame);

	HDC memDC			= CreateCompatibleDC(hDC);
	HBITMAP hOldBit		= (HBITMAP)SelectObject(memDC, m_pWeakSprite.lock()->GetBitmap());

	TransparentBlt(hDC, point.x, point.y,
		m_iDrawWidth, m_iDrawHeight, memDC, SPRITE_WIDTH * m_iCurFrame, 0, SPRITE_WIDTH,
		SPRITE_HEIGHT, m_colorRef);

	SelectObject(memDC, hOldBit);
	DeleteDC(memDC);
}

void CAnim::DrawImage(const HDC & hDC, const Types::Point& point)
{
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(memDC, m_pWeakSprite.lock()->GetBitmap());

	TransparentBlt(hDC, point.x, point.y,
		m_iDrawWidth, m_iDrawHeight, memDC, 0, 0, 
		m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight(),
		m_colorRef);

	SelectObject(memDC, hOldBit);
	DeleteDC(memDC);


}



