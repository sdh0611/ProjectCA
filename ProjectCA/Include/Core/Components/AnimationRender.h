#pragma once

#include "..\..\..\stdafx.h"
#include "RenderComponent.h"

class CSprite;
class CActor;

typedef std::vector<std::weak_ptr<CSprite>> WeakSpriteTable;


class AnimationRender : public RenderComponent {

public:
	AnimationRender();
	virtual ~AnimationRender();


public:
	bool Init(std::shared_ptr<CActor> pOwner, const Types::tstring& strSpriteName, UINT iWidth, UINT iHeight,
		double dPlayTime, bool bLoop, const Types::tstring& strAnimTag = TEXT("Default"));
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
	void						ClearEleapsedTime();
	const Types::tstring	GetAnimTag() const;



private:
	void DrawAnimation(const HDC& hDC, const Types::Point& point);
	void DrawImage(const HDC& hDC, const Types::Point& point);
	

private:
	bool								m_bLoop;
	UINT								m_iCurFrame;
	UINT								m_iMaxFrame;
	UINT								m_iDrawWidth;
	UINT								m_iDrawHeight;
	COLORREF						m_ColorRef;
	double							m_dPlaySectionLength;
	double							m_dPlayTime;		//초 단위로 계산
	double							m_dPlaySpeed;
	double							m_dTimeElapsed;
	Types::tstring					m_strAnimTag;
	WeakSpriteTable				m_WeakSpriteTable;
	std::weak_ptr<CSprite>		m_pWeakSprite;
	std::weak_ptr<CActor>		m_pOwner;
	std::vector<CAnim>			m_AnimationList;
	Types::AnimationMotion		m_AnimationState;

};