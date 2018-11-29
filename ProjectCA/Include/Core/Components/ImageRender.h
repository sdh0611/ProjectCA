#pragma once

#include "..\..\..\stdafx.h"
#include "RenderComponent.h"

class CSprite;

class ImageRender : public RenderComponent {

public:
	ImageRender();
	virtual ~ImageRender();


public:
	virtual bool	PostInit(CObject* pOwner, const Types::tstring& strTag = TEXT("RenderComponent")) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTIme) override;
	virtual void Draw(const HDC& hDC) override;
	virtual void Draw(const HDC& hDC, const POSITION& position);
	virtual void Draw(const HDC& hDC, const POSITION& position, std::weak_ptr<CSprite> pSprite);


public:
	void SetDrawWidth(UINT iWidth);
	void SetDrawHeight(UINT iHeight);
	void SetDrawSize(UINT iWidth, UINT iHeight);
	bool SetSprite(const TSTRING& strImageTag);
	bool SetSprite(std::shared_ptr<CSprite> pSprite);


private:



private:
	UINT							m_iDrawWidth;
	UINT							m_iDrawHeight;
	COLORREF					m_ColorRef;
	TSTRING						m_strImageTag;
	std::weak_ptr<CSprite>	m_pWeakSprite;

};