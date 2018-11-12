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
	bool Init(std::shared_ptr<CActor> pOwner, const TSTRING& strSpriteName, UINT iWidth, UINT iHeight,
		double dPlayTime, bool bLoop, const TSTRING& strAnimTag = TEXT("Default"));
	void Update(double dDeltaTIme);
	void Draw(const HDC& hDC, const Types::Point& point);


public:
	bool SetSprite(const Types::tstring& strSpriteName);
	bool SetTotalPlayTime(double dTime);
	bool SetPlaySpeed(double dSpeed);
	void SetDrawingWidth(UINT iWidth);
	void SetDrawingHeight(UINT iHeight);


public:
	UINT GetDrawWidth() const;
	UINT GetDrawHeight() const;


public:
	const Types::tstring	GetAnimTag() const;
	void						ClearEleapsedTime();


private:
	void DrawAnimation(const HDC& hDC, const Types::Point& point);
	void DrawImage(const HDC& hDC, const Types::Point& point);


private:
	bool							m_bLoop;
	UINT							m_iCurFrame;
	UINT							m_iMaxFrame;
	UINT							m_iDrawWidth;
	UINT							m_iDrawHeight;
	COLORREF					m_colorRef;
	double						m_dPlaySectionLength;
	double						m_dPlayTime;		//초 단위로 계산
	double						m_dPlaySpeed;
	double						m_dTimeElapsed;
	Types::tstring				m_strAnimTag;
	WeakSpriteTable			m_weakSpriteTable;
	std::weak_ptr<CSprite>	m_pWeakSprite;
	std::weak_ptr<CActor>	m_pOwner;

};