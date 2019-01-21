#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Graphic\CAnim.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"


CAnim::CAnim()
	:m_bLoop(false), m_iCurFrame(0), m_iMaxFrame(0),
	m_dPlayTime(0.f), m_dPlaySpeed(0.f), 
	m_dPlaySectionLength(0.f), m_dTimeElapsed(0.f)
{
}

CAnim::~CAnim()
{
}

bool CAnim::Init(const TSTRING& strSpriteName, double dPlayTime, bool bLoop,const TSTRING& strAnimTag, bool bInterrupt)
{
	m_pWeakSprite = CResourceManager::GetInstance()->GetWeakSprtiePtr(strSpriteName);
	if (m_pWeakSprite.expired())
		return false;

	m_bInterrupt									= bInterrupt;
	if (m_bInterrupt)
	{
		m_bReadyToChange	= true;
	}
	else
	{
		m_bReadyToChange	= false;
	}
	m_iMaxFrame									= m_pWeakSprite.lock()->GetBitWidth() / SPRITE_WIDTH;
	m_ColorRef										= RGB(248, 7, 220);

	m_dPlayTime									= dPlayTime;
	m_dPlaySpeed									= 1.f;

	if (m_dPlayTime == 0.f)
	{
		m_dPlaySectionLength					= 0.f;
	}
	else
	{
		m_dPlaySectionLength					= m_dPlayTime / m_iMaxFrame;
	}

	m_strAnimTag									= strAnimTag;

	m_bLoop											= bLoop;
	m_bPause										= false;

	return true;
}


void CAnim::Update(double fDeltaTIme)
{
	m_dTimeElapsed += fDeltaTIme;
}


void CAnim::DrawAnimation(const HDC & hDC, const HDC& hMemDC, const HDC& hBlendingDC,
	const BLENDFUNCTION& blendFunction, const POSITION& point, UINT iWidth, UINT iHeight)
{
	if (!m_bPause)
	{
		if (m_dPlayTime != 0.f)
		{
			if (m_dTimeElapsed >= (m_dPlaySectionLength / m_dPlaySpeed)) {
				m_dTimeElapsed = 0.f;
				if (m_iCurFrame < m_iMaxFrame - 1)
				{
					++m_iCurFrame;
				}
				else
				{
					if (m_bLoop)
					{
						++m_iCurFrame %= m_iMaxFrame;
					}
					m_bReadyToChange = true;

				}

			}

		}
	}
	HBITMAP hTempBit = CreateCompatibleBitmap(hDC, MAX_WIDTH, MAX_HEIGHT);
	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, m_pWeakSprite.lock()->GetBitmap());
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBlendingDC, hTempBit);

	BitBlt(hBlendingDC, 0, 0, MAX_WIDTH, MAX_HEIGHT, hDC, 0, 0, SRCCOPY);

	TransparentBlt(hBlendingDC, point.x, point.y,
		iWidth, iHeight, hMemDC, SPRITE_WIDTH * m_iCurFrame, 0, SPRITE_WIDTH,
		SPRITE_HEIGHT, m_ColorRef);

	AlphaBlend(hDC, point.x, point.y, iWidth, iHeight, hBlendingDC, point.x, point.y, iWidth,
		iHeight, blendFunction);

	DeleteObject(SelectObject(hBlendingDC, hOldBitmap));
	SelectObject(hMemDC, hOldBit);
	DeleteObject(hTempBit);
}

void CAnim::DrawAnimation(const HDC & hDC, const HDC & hMemDC, const POSITION & point, UINT iWidth, UINT iHeight)
{
	if (!m_bPause)
	{
		if (m_dPlayTime != 0.f)
		{
			if (m_dTimeElapsed >= (m_dPlaySectionLength / m_dPlaySpeed)) {
				m_dTimeElapsed = 0.f;
				if (m_iCurFrame < m_iMaxFrame - 1)
				{
					++m_iCurFrame;
				}
				else
				{
					if (m_bLoop)
					{
						++m_iCurFrame %= m_iMaxFrame;
					}
					m_bReadyToChange = true;

				}

			}

		}
	}

	HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, m_pWeakSprite.lock()->GetBitmap());
	TransparentBlt(hDC, point.x, point.y,
		iWidth, iHeight, hMemDC, SPRITE_WIDTH * m_iCurFrame, 0, SPRITE_WIDTH,
		SPRITE_HEIGHT, m_ColorRef);

	SelectObject(hMemDC, hOldBit);
}



void CAnim::SetCanInterrupt(bool bInterrupt)
{
	m_bInterrupt = bInterrupt;
}

void CAnim::SetReadyToChange(bool bReady)
{
	m_bReadyToChange = bReady;
}

void CAnim::SetPauseAnimation(bool bPause)
{
	m_bPause = bPause;
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

bool CAnim::IsCanInterrupt() const
{
	return m_bInterrupt;
}

bool CAnim::IsReadyToChange() const
{
	return m_bReadyToChange;
}

bool CAnim::IsPauseAnimation() const
{
	return m_bPause;
}

const TSTRING& CAnim::GetAnimTag() const
{
	return m_strAnimTag;
}


void CAnim::ClearEleapsedTime()
{
	m_dTimeElapsed = 0.f;
	m_iCurFrame = 0;
	if (!m_bInterrupt)
	{
		m_bReadyToChange = false;
	}
	m_bPause = false;

}
