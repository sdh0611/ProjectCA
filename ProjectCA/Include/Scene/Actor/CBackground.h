#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"

class CSprite;

class CBackground : public CObject {

public:
	CBackground();
	virtual ~CBackground();


public:
	virtual bool PostInit(const Types::ObjectData& data, CGameScene* pScene);
	virtual void Init() override;
	virtual void Update(double dDeltaTIme) override;
	virtual void Render(const HDC& hDC) override;


public:
	bool SetBackgroundImage(const TSTRING& strImageName);


private:
	float							m_fScrollSpeed;
	//COLORREF					m_ColorRef;
	//std::weak_ptr<CSprite>	m_pBackgroundImage;
	UINT							m_iBackgroundWidth;
	UINT							m_iBackgroundHeight;


};