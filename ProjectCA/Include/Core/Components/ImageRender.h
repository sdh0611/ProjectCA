#pragma once

#include "..\..\..\stdafx.h"
#include "RenderComponent.h"

//NOTE : RenderComponent를 상속받은 Component.
//			애니메이션이 없는 객체들의 Rendering을 위한 기능들을 정의함.

class CSprite;

class ImageRender : public RenderComponent {

public:
	ImageRender();
	virtual ~ImageRender();


public:
	virtual bool	PostInit(CEntity* pOwner, const Types::tstring& strTag = TEXT("RenderComponent")) override;
	virtual void Init() override;
	virtual void Draw(const HDC& hDC) override;
	virtual void Draw(const HDC& hDC, const POSITION& position);
	virtual void Draw(const HDC& hDC, const POSITION& position, std::weak_ptr<CSprite> pSprite);


public:
	bool SetSprite(const TSTRING& strImageTag);
	bool SetSprite(std::shared_ptr<CSprite> pSprite);


private:



private:
	COLORREF					m_ColorRef;
	TSTRING						m_strImageTag;
	std::weak_ptr<CSprite>	m_pWeakSprite;

};