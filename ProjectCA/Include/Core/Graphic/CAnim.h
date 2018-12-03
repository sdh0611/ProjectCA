#pragma once

#include "..\..\..\stdafx.h"

class CSprite;
class CActor;

typedef std::vector<std::weak_ptr<CSprite>> WeakSpriteTable;

class CAnim {

public:
	enum AnimMode {
		ANIM_DEFAULT, ANIM_BLEND
	};

public:
	CAnim();
	~CAnim();


public:
	//bool Init(const TSTRING& strSpriteName, UINT iWidth, UINT iHeight,
	//	double dPlayTime, bool bLoop, const TSTRING& strAnimTag = TEXT("Default"), bool bInterrupt = true);
	bool Init(const TSTRING& strSpriteName, double dPlayTime, bool bLoop, 
		const TSTRING& strAnimTag = TEXT("Default"), bool bInterrupt = true);
	void Update(double dDeltaTIme);
	void Draw(const HDC& hDC, const HDC& hMemDC, const POSITION& point, UINT iWidth, UINT iHeight);


public:
	void SetCanInterrupt(bool bInterrupt);
	void SetReadyToChange(bool bReady);
	void SetPauseAnimation(bool bPause);
	bool SetSprite(const Types::tstring& strSpriteName);
	bool SetTotalPlayTime(double dTime);
	bool SetPlaySpeed(double dSpeed);
	//void SetDrawingWidth(UINT iWidth);
	//void SetDrawingHeight(UINT iHeight);
	void SetAnimMode(AnimMode mode);


public:
	bool					IsCanInterrupt() const;
	bool					IsReadyToChange() const;
	bool					IsPauseAnimation() const;
	//UINT				GetDrawWidth() const;
	//UINT				GetDrawHeight() const;
	const TSTRING&	GetAnimTag() const;
	AnimMode			GetAnimMode() const;


public:
	void ClearEleapsedTime();


private:
	void DrawAnimation(const HDC& hDC, const HDC& hMemDC, const POSITION& point, UINT iWidth, UINT iHeight);


private:
	bool							m_bInterrupt;
	bool							m_bReadyToChange;
	bool							m_bLoop;
	bool							m_bPause;
	AnimMode					m_AnimMode;
	UINT							m_iCurFrame;
	UINT							m_iMaxFrame;
	//UINT							m_iDrawWidth;
	//UINT							m_iDrawHeight;
	COLORREF					m_ColorRef;
	BLENDFUNCTION			m_BlendFunction;
	double						m_dPlaySectionLength;
	double						m_dPlayTime;		//초 단위로 계산
	double						m_dPlaySpeed;
	double						m_dTimeElapsed;
	TSTRING						m_strAnimTag;
	WeakSpriteTable			m_weakSpriteTable;
	std::weak_ptr<CSprite>	m_pWeakSprite;

};