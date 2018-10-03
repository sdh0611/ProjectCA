#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Graphic\CAnim.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"


CAnim::CAnim()
	:m_iCurFrame(0), m_iMaxFrame(0), m_dPlayTime(0.f), 
	m_dPlaySectionLength(0.f), m_dTimeElapsed(0.f)
{
}

CAnim::~CAnim()
{
}

bool CAnim::Init(std::shared_ptr<CActor> pOwner, const Types::tstring& strSpriteName, 
	double dPlayTime, bool bAnimate, const Types::tstring& strAnimTag)
{
	m_pWeakSprite = CResourceManager::GetInstance()->GetWeakSprtiePtr(strSpriteName);
	if (m_pWeakSprite.expired())
		return false;

	m_pOwner = pOwner;

	m_iMaxFrame = m_pWeakSprite.lock()->GetBitWidth()/SPRITE_WIDTH;
	m_dPlayTime = dPlayTime;
	m_dPlaySectionLength = m_dPlayTime / m_iMaxFrame;
	m_strAnimTag = strAnimTag;

	m_bAnimate = bAnimate;

	return true;
}


void CAnim::Update(double fDeltaTIme)
{
	m_dTimeElapsed += fDeltaTIme;
}

void CAnim::Draw(const HDC & hDC)
{
	if (!m_pWeakSprite.expired()) {
		if (m_bAnimate)
			DrawAnimation(hDC);
		else
			DrawImage(hDC);
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

void CAnim::DrawAnimation(const HDC & hDC)
{
	if (m_dTimeElapsed >= m_dPlaySectionLength) {
		m_dTimeElapsed = 0.f;
		++m_iCurFrame;
		m_iCurFrame %= m_iMaxFrame;
	}
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(memDC, m_pWeakSprite.lock()->GetBitmap());

	TransparentBlt(hDC, m_pOwner.lock()->GetActorPoint().x, m_pOwner.lock()->GetActorPoint().y,
		SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, memDC, SPRITE_WIDTH * m_iCurFrame, 0, SPRITE_WIDTH,
		SPRITE_HEIGHT, RGB(248, 7, 220));

	//BitBlt(hDC, m_pOwner.lock()->GetActorPoint().x, m_pOwner.lock()->GetActorPoint().y,
	//	SPRITE_WIDTH, SPRITE_HEIGHT, memDC, SPRITE_WIDTH * m_iCurFrame, 0,  SRCCOPY);

	SelectObject(memDC, hOldBit);
	DeleteDC(memDC);

}

void CAnim::DrawImage(const HDC & hDC)
{
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(memDC, m_pWeakSprite.lock()->GetBitmap());

	TransparentBlt(hDC, m_pOwner.lock()->GetActorPoint().x, m_pOwner.lock()->GetActorPoint().y,
		MAX_WIDTH, MAX_HEIGHT, 
		memDC, 0, 0, m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight(),
		RGB(255, 255, 255));

	//BitBlt(hDC, m_pOwner.lock()->GetActorPoint().x, m_pOwner.lock()->GetActorPoint().y,
	//	m_pWeakSprite.lock()->GetBitWidth(), m_pWeakSprite.lock()->GetBitHeight(), memDC, 
	//	0, 0,  SRCCOPY);

	SelectObject(memDC, hOldBit);
	DeleteDC(memDC);


}



