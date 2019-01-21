#pragma once

#include "..\..\..\stdafx.h"

//NOTE: AnimationRender가 애니메이션을 그리기 위해 필요한 객체.

class CSprite;
class CActor;

typedef std::vector<std::weak_ptr<CSprite>> WeakSpriteTable;

class CAnim {

public:
	CAnim();
	~CAnim();


public:
	bool Init(const TSTRING& strSpriteName, double dPlayTime, bool bLoop, 
		const TSTRING& strAnimTag = TEXT("Default"), bool bInterrupt = true);
	void Update(double dDeltaTIme);
	void DrawAnimation(const HDC& hDC, const HDC& hMemDC, const HDC& hBlendingDC,
		const BLENDFUNCTION& blendFunction, const POSITION& point, UINT iWidth, UINT iHeight);
	void DrawAnimation(const HDC& hDC, const HDC& hMemDC, const POSITION& point, UINT iWidth, UINT iHeight);


public:
	void SetCanInterrupt(bool bInterrupt);
	void SetReadyToChange(bool bReady);
	void SetPauseAnimation(bool bPause);
	bool SetSprite(const Types::tstring& strSpriteName);
	bool SetTotalPlayTime(double dTime);
	bool SetPlaySpeed(double dSpeed);

public:
	bool					IsCanInterrupt() const;
	bool					IsReadyToChange() const;
	bool					IsPauseAnimation() const;
	const TSTRING&	GetAnimTag() const;


public:
	void ClearEleapsedTime();


private:
	bool							m_bInterrupt;
	bool							m_bReadyToChange;
	bool							m_bLoop;
	bool							m_bPause;
	UINT							m_iCurFrame;
	UINT							m_iMaxFrame;
	COLORREF					m_ColorRef;
	double						m_dPlaySectionLength;
	double						m_dPlayTime;		//초 단위로 계산
	double						m_dPlaySpeed;
	double						m_dTimeElapsed;
	TSTRING						m_strAnimTag;
	WeakSpriteTable			m_weakSpriteTable;
	std::weak_ptr<CSprite>	m_pWeakSprite;

};