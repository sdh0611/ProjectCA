#pragma once

#include "..\..\..\stdafx.h"

class CSprite;
class CActor;

typedef std::vector<std::weak_ptr<CSprite>> WeakSpriteTable;

class CAnim {

public:
	CAnim();
	~CAnim();


public:
	bool Init(std::shared_ptr<CActor> pOwner, const Types::tstring& strSpriteName,
		double dPlayTime, bool bAnimate, const Types::tstring& strAnimTag = TEXT("Default"));
	void Update(double fDeltaTIme);
	void Draw(const HDC& hDC);


public:
	bool SetSprite(const Types::tstring& strSpriteName);
	bool SetTotalPlayTime(double dTime);
	const Types::tstring GetAnimTag() const { return m_strAnimTag; }


public:
	void ClearEleapsedTime() { m_dTimeElapsed = 0.f; m_iCurFrame = 0; }


private:
	void DrawAnimation(const HDC& hDC);
	void DrawImage(const HDC& hDC);


private:
	bool					m_bAnimate;
	UINT					m_iCurFrame;
	UINT					m_iMaxFrame;
	double				m_dPlaySectionLength;
	double				m_dPlayTime;		//초 단위로 계산
	double				m_dTimeElapsed;
	Types::tstring		m_strAnimTag;
	WeakSpriteTable	m_weakSpriteTable;
	std::weak_ptr<CSprite> m_pWeakSprite;
	std::weak_ptr<CActor> m_pOwner;

};