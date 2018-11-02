#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"

class CSprite;

class CBackground : public CActor {

public:
	CBackground();
	virtual ~CBackground();


public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual bool Init() override;
	virtual void Update(double dDeltaTIme) override;
	virtual void Render(const HDC& hDC) override;
	virtual void ActorBehavior(double dDeltaTime) override;


public:
	bool SetBackgroundImage(const TSTRING& strImageName);


private:
	float							m_fScrollSpeed;
	COLORREF					m_ColorRef;
	std::weak_ptr<CSprite>	m_pBackgroundImage;
	UINT							m_iBackgroundWidth;
	UINT							m_iBackgroundHeight;


};